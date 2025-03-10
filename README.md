
# Defold-Native-Camera
Camera module for Defold

A simple camera native extension. Inspired by [Rendercam](https://github.com/rgrams/rendercam) but with some changes.

Defold Native Camera is a minimalistic camera module for Defold, designed to be as independent as possible from the game engine itself. It provides a simple, low-level API while avoiding unnecessary dependencies on Defold's game object system. The core logic and mathematics are implemented in C++ to improve performance and reduce garbage collection overhead.


## Features
- Minimalistic API with only essential functions.
- Fully independent from game objects and Defold’s update loop.
- Garbage Collection-friendly API

## Limitations
- Focused on manual control rather than high-level convenience features.(no camera shake, no automatic following of game objects, ...)
- Not change viewport. If you need black bars at viewport, you should manual set viewport size, and set camera screen size to match.
- You need to make custom render script to use this camera.



If you like the extension you can support me on patreon.
It will help me make more items for defold.

[![](https://c5.patreon.com/external/logo/become_a_patron_button.png)](https://www.patreon.com/d954mas)
[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/d954mas)

---



## Installation

Install NativeCamera in your project by adding it as a [library dependency](https://www.defold.com/manuals/libraries/). Open your game.project file and in the "Dependencies" field under "Project", add:

```
https://github.com/d954mas/defold-native-camera/archive/1.0.1.zip
```

Then open the "Project" menu of the editor and click "Fetch Libraries". You should see the "native_camera" folder appear in your assets panel after a few moments.

## API

```lua
---@meta
native_camera = {
    SCALE_MODE = {
        EXPANDVIEW = 0,
        FIXEDAREA = 1,
        FIXEDWIDTH = 2,
        FIXEDHEIGHT = 3,
    }
}

---@class NativeCameraConfig
---@field orthographic boolean|nil
---@field ortho_scale number|nil
---@field ortho_scale_mode integer|nil
---@field view_area_width number|nil
---@field view_area_height number|nil
---@field position vector3|nil
---@field rotation quaternion|nil
---@field fov number|nil
---@field near_z number|nil
---@field far_z number|nil


---@return NativeCamera
---@param config NativeCameraConfig|nil
function native_camera.new(config) end

---@class NativeCamera
local NativeCamera = {}

---@param orthographic boolean
function NativeCamera:set_orthographic(orthographic) end

---@param orthoScale number
function NativeCamera:set_ortho_scale(orthoScale) end

---@param scaleMode integer
function NativeCamera:set_scale_mode(scaleMode) end

---@param viewAreaWidth number
---@param viewAreaHeight number
function NativeCamera:set_view_area(viewAreaWidth, viewAreaHeight) end

---@param screenWidth number
---@param screenHeight number
function NativeCamera:set_screen_size(screenWidth, screenHeight) end

---@param fov number
function NativeCamera:set_fov(fov) end

---@param nearZ number
function NativeCamera:set_near_z(nearZ) end

---@param farZ number
function NativeCamera:set_far_z(farZ) end

---@param position vector3
function NativeCamera:set_position(position) end

---@param x number
---@param y number
---@param z number
function NativeCamera:set_position_raw(x, y, z) end

---@param rotation quaternion
function NativeCamera:set_rotation(rotation) end

---@param x number
---@param y number
---@param z number
---@param w number
function NativeCamera:set_rotation_raw(x, y, z, w) end

---@return quaternion
function NativeCamera:get_rotation() end

---@param result quaternion
function NativeCamera:get_rotation_to_quat(result) end

---@return number, number, number, number
function NativeCamera:get_rotation_raw() end

---@return matrix4
function NativeCamera:get_view() end

---@param result matrix4
function NativeCamera:get_view_to_matrix(result) end

---@return matrix4
function NativeCamera:get_proj() end

---@param result matrix4
function NativeCamera:get_proj_to_matrix(result) end

---@return matrix4
function NativeCamera:get_frustum() end

---@param result matrix4
function NativeCamera:get_frustum_to_matrix(result) end

---@return number
function NativeCamera:get_fov() end

---@return number
function NativeCamera:get_near_z() end

---@return number
function NativeCamera:get_far_z() end

---@return vector3
function NativeCamera:get_position() end

---@param result vector3
function NativeCamera:get_position_to_vector3(result) end

---@return number, number, number
function NativeCamera:get_position_raw() end

---@param screenX number
---@param screenY number
---@return vector3, vector3
function NativeCamera:screen_to_world_ray(screenX, screenY) end

---@param screenX number
---@param screenY number
---@param near_point vector3
---@param far_point vector3
function NativeCamera:screen_to_world_ray_to_vector3(screenX, screenY, near_point, far_point) end

---@param screenX number
---@param screenY number
---@return number, number
function NativeCamera:screen_to_world_2d(screenX, screenY) end

---@param worldPos vector3
---@return number, number
function NativeCamera:world_to_screen(worldPos) end

---@param screenX number
---@param screenY number
---@param planeNormal vector3
---@param pointOnPlane vector3
---@return boolean, vector3|nil
function NativeCamera:screen_to_world_plane(screenX, screenY, planeNormal, pointOnPlane) end

---@param screenX number
---@param screenY number
---@param planeNormal vector3
---@param pointOnPlane vector3
---@param out vector3
---@return boolean
function NativeCamera:screen_to_world_plane_to_vector3(screenX, screenY, planeNormal, pointOnPlane, out) end

---@param screenX number
---@param screenY number
---@param planeNormal vector3
---@param pointOnPlane vector3
---@return boolean, number|nil, number|nil, number|nil
function NativeCamera:screen_to_world_plane_raw(screenX, screenY, planeNormal, pointOnPlane) end

```
