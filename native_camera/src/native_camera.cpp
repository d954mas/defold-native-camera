#include "native_camera.h"

#define M_PI 3.14159265358979323846

namespace NativeCamera {

Camera::Camera()
    : orthographic(true),
      orthoScale(1.0f),
      orthoScaleMode(CameraOrthoScaleMode::FIXEDAREA),
      viewAreaWidth(1),
      viewAreaHeight(1),

      dirtyView(true),
      dirtyProj(true),

      screenWidth(1),
      screenHeight(1),
      screenAspectRatio(1),

      pos(dmVMath::Vector3(0, 0, 0)),
      rot(dmVMath::Quat::rotationZ(0)),

      fov((60.0 * M_PI) / 180.0),
      nearZ(-1.0f),
      farZ(1.0f),
      view(dmVMath::Matrix4::identity()),
      proj(dmVMath::Matrix4::identity()) {
}

Camera::~Camera() {
}

void Camera::setOrthographic(bool orthographic) {
    this->orthographic = orthographic;
    dirtyProj = true;
}

void Camera::setOrthoScale(float orthoScale) {
    this->orthoScale = orthoScale;
    dirtyProj = true;
}

void Camera::setOrthoScaleMode(CameraOrthoScaleMode orthoScaleMode) {
    this->orthoScaleMode = orthoScaleMode;
    dirtyProj = true;
}

void Camera::setViewArea(float viewAreaWidth, float viewAreaHeight) {
    this->viewAreaWidth = viewAreaWidth;
    this->viewAreaHeight = viewAreaHeight;
    dirtyProj = true;
}

void Camera::setScreenSize(float screenWidth, float screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->screenAspectRatio = screenWidth / screenHeight;
    dirtyProj = true;
}

void Camera::setFov(float fov) {
    this->fov = fov;
    dirtyProj = true;
}

void Camera::setNearZ(float nearZ) {
    this->nearZ = nearZ;
    dirtyProj = true;
}

void Camera::setFarZ(float farZ) {
    this->farZ = farZ;
    dirtyProj = true;
}

void Camera::setPosition(dmVMath::Vector3 pos) {
    this->pos = pos;
    dirtyView = true;
}

void Camera::setRotation(dmVMath::Quat rot) {
    this->rot = rot;
    dirtyView = true;
}

dmVMath::Matrix4 Camera::getView() {
    if (dirtyView) {
        dmVMath::Vector3 vForward = Vectormath::Aos::rotate(rot, dmVMath::Vector3(0, 0, -1));
        view = dmVMath::Matrix4::lookAt(dmVMath::Point3(pos), dmVMath::Point3(pos + vForward), dmVMath::Vector3(0, 1, 0));
        dirtyView = false;
    };
    return view;
}

dmVMath::Matrix4 Camera::getProj() {
    if (dirtyProj) {
        if (orthographic) {
            // calculate viewArea based on screen size and scaleMode
            float viewAreaCalculatedWidth = viewAreaWidth;
            float viewAreaCalculatedHeight = viewAreaHeight;

            switch (orthoScaleMode) {
            case CameraOrthoScaleMode::EXPANDVIEW: {
                viewAreaCalculatedWidth = screenWidth;
                viewAreaCalculatedHeight = screenHeight;
                break;
            }
            case CameraOrthoScaleMode::FIXEDAREA: {
                float minScale = fmax(viewAreaWidth / screenWidth, viewAreaHeight / screenHeight);
                viewAreaCalculatedWidth = screenWidth * minScale;
                viewAreaCalculatedHeight = screenHeight * minScale;
                break;
            }
            case CameraOrthoScaleMode::FIXEDWIDTH: {
                viewAreaCalculatedWidth = viewAreaWidth;
                viewAreaCalculatedHeight = viewAreaWidth / screenAspectRatio;
                break;
            }
            case CameraOrthoScaleMode::FIXEDHEIGHT: {
                viewAreaCalculatedWidth = viewAreaHeight * screenAspectRatio;
                viewAreaCalculatedHeight = viewAreaHeight;
                break;
            }
            default: {
                // Handle unexpected scale mode, possibly fallback to a default behavior or error
                assert(false);
            }
            }

            float x = viewAreaCalculatedWidth * 0.5f * orthoScale;
            float y = viewAreaCalculatedHeight * 0.5f * orthoScale;
            proj = dmVMath::Matrix4::orthographic(-x, x, -y, y, nearZ, farZ);
        } else {
            proj = dmVMath::Matrix4::perspective(fov, screenAspectRatio, nearZ, farZ);
        }
        dirtyProj = false;
    }
    return proj;
}

dmVMath::Matrix4 Camera::getFrustum() {
    return getProj() * getView();
}

// Returns start and end points for a ray from the camera through the supplied screen coordinates
// Start point is on the camera near plane, end point is on the far plane.
void Camera::screenToWorldRay(float screenX, float screenY, dmVMath::Vector3 *start, dmVMath::Vector3 *end) {
    // Remap coordinates to range -1 to 1
    screenX = (screenX - screenWidth * 0.5) / screenWidth * 2;
    screenY = (screenY - screenHeight * 0.5) / screenHeight * 2;

    dmVMath::Matrix4 inv = Vectormath::Aos::inverse(getProj() * getView());

    dmVMath::Vector4 nv(screenX, screenY, -1, 1);
    dmVMath::Vector4 fv(screenX, screenY, 1, 1);

    dmVMath::Vector4 np = inv * nv;
    dmVMath::Vector4 fp = inv * fv;
    np = np * (1 / np.getW());
    fp = fp * (1 / fp.getW());

    *start = dmVMath::Vector3(np.getX(), np.getY(), np.getZ());
    *end = dmVMath::Vector3(fp.getX(), fp.getY(), fp.getZ());
}

void Camera::screenToWorld2D(float screenX, float screenY, float *outX, float *outY) {
    // Remap coordinates to range -1 to 1
    screenX = (screenX - screenWidth * 0.5) / screenWidth * 2;
    screenY = (screenY - screenHeight * 0.5) / screenHeight * 2;

    dmVMath::Matrix4 inv = Vectormath::Aos::inverse(getProj() * getView());

    dmVMath::Vector4 nv(screenX, screenY, -1, 1);
    dmVMath::Vector4 fv(screenX, screenY, 1, 1);

    dmVMath::Vector4 np = inv * nv;
    dmVMath::Vector4 fp = inv * fv;
    np = np * (1 / np.getW());
    fp = fp * (1 / fp.getW());

    *outX = np.getX();
    *outY = np.getY();
}

bool Camera::screenToWorldPlane(float screenX, float screenY, dmVMath::Vector3 planeNormal, dmVMath::Vector3 pointOnPlane, dmVMath::Vector3 *out) {
    dmVMath::Vector3 np, fp;
    screenToWorldRay(screenX, screenY, &np, &fp);
    float denom = dmVMath::Dot(planeNormal, fp - np);
    if (denom == 0) {
        // ray is perpendicular to plane normal, so there are either 0 or infinite intersections
        return false;
    }
    float numer = dmVMath::Dot(planeNormal, pointOnPlane - np);
    *out = dmVMath::Lerp(numer / denom, np, fp);
    return true;
}

void Camera::worldToScreen(dmVMath::Vector3 worldPos, float *outX, float *outY){
    dmVMath::Matrix4 m = getProj() * getView();
    dmVMath::Vector4 pv = dmVMath::Vector4(worldPos.getX(), worldPos.getY(), worldPos.getZ(), 1);
    pv = m * pv;
    pv = pv * (1/pv.getW());
    *outX = (pv.getX() / 2 + 0.5) * screenWidth;
    *outY = (pv.getY() / 2 + 0.5) * screenHeight;
}

// Implementation of the updated toString method
void Camera::toString(char *buffer, size_t size) {
    snprintf(buffer, size,
             "NativeCamera[%p]{ orthographic: %d orthoScale: %.2f scaleMode: %d "
             "viewArea: (%.2f, %.2f) screenSize: (%.2f, %.2f) "
             "screenAspectRatio: %.2f position: (%.2f, %.2f, %.2f) "
             "rotation: (%.2f, %.2f, %.2f, %.2f) "
             "fov: %.2f nearZ: %.2f farZ: %.2f dirtyView: %d dirtyProj: %d }",
             this,
             orthographic, orthoScale, orthoScaleMode,
             viewAreaWidth, viewAreaHeight,
             screenWidth, screenHeight,
             screenAspectRatio,
             pos.getX(), pos.getY(), pos.getZ(),
             rot.getX(), rot.getY(), rot.getZ(), rot.getW(),
             fov, nearZ, farZ,
             dirtyView, dirtyProj);
}

} // namespace NativeCamera
