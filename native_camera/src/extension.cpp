#define EXTENSION_NAME NativeCameraNE
#define LIB_NAME "NativeCameraNE"
#define MODULE_NAME "native_camera"
#define USERDATA_METATABLE "NATIVE_CAMERA_USERDATA"

#include "native_camera.h"
#include "utils.h"

// lua bindings
static NativeCamera::Camera *CameraCheck(lua_State *L, int index) {
    return *(NativeCamera::Camera **)luaL_checkudata(L, index, USERDATA_METATABLE);
}

static int LuaCameraDestroy(lua_State *L) {
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    delete userdata;
    return 0;
}

static int LuaCameraSetOrthographic(lua_State *L) {
    check_arg_count(L, 2);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    userdata->setOrthographic(lua_toboolean(L, 2));
    return 0;
}

static int LuaCameraSetOrthoScale(lua_State *L) {
    check_arg_count(L, 2);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    userdata->setOrthoScale(luaL_checknumber(L, 2));
    return 0;
}

static int LuaCameraSetOrthoScaleMode(lua_State *L) {
    check_arg_count(L, 2);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    userdata->setOrthoScaleMode((NativeCamera::CameraOrthoScaleMode)luaL_checkinteger(L, 2));
    return 0;
}

static int LuaCameraSetViewArea(lua_State *L) {
    check_arg_count(L, 3);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    userdata->setViewArea(luaL_checknumber(L, 2), luaL_checknumber(L, 3));
    return 0;
}

static int LuaCameraSetScreenSize(lua_State *L) {
    check_arg_count(L, 3);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    userdata->setScreenSize(luaL_checknumber(L, 2), luaL_checknumber(L, 3));
    return 0;
}

static int LuaCameraSetFov(lua_State *L) {
    check_arg_count(L, 2);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    userdata->setFov(luaL_checknumber(L, 2));
    return 0;
}

static int LuaCameraSetNearZ(lua_State *L) {
    check_arg_count(L, 2);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    userdata->setNearZ(luaL_checknumber(L, 2));
    return 0;
}

static int LuaCameraSetFarZ(lua_State *L) {
    check_arg_count(L, 2);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    userdata->setFarZ(luaL_checknumber(L, 2));
    return 0;
}

static int LuaCameraSetPosition(lua_State *L) {
    check_arg_count(L, 2);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    userdata->setPosition(*dmScript::CheckVector3(L, 2));
    return 0;
}

static int LuaCameraSetPositionRaw(lua_State *L) {
    check_arg_count(L, 4);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    userdata->setPosition(dmVMath::Vector3(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
    return 0;
}

static int LuaCameraSetRotation(lua_State *L) {
    check_arg_count(L, 2);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    userdata->setRotation(*dmScript::CheckQuat(L, 2));
    return 0;
}

static int LuaCameraSetRotationRaw(lua_State *L) {
    check_arg_count(L, 5);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    userdata->setRotation(dmVMath::Quat(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5)));
    return 0;
}

static int LuaCameraGetRotation(lua_State *L) {
    check_arg_count(L, 1);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    dmScript::PushQuat(L, userdata->rot);
    return 1;
}

static int LuaCameraGetRotationToQuat(lua_State *L) {
    check_arg_count(L, 2);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    dmVMath::Quat *out = dmScript::CheckQuat(L, 2);
    *out = userdata->rot;
    return 0;
}

static int LuaCameraGetRotationRaw(lua_State *L) {
    check_arg_count(L, 1);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    lua_pushnumber(L, userdata->rot.getX());
    lua_pushnumber(L, userdata->rot.getY());
    lua_pushnumber(L, userdata->rot.getZ());
    lua_pushnumber(L, userdata->rot.getW());
    return 4;
}

static int LuaCameraGetView(lua_State *L) {
    check_arg_count(L, 1);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    dmVMath::Matrix4 view = userdata->getView();
    dmScript::PushMatrix4(L, view);
    return 1;
}

static int LuaCameraGetViewToMatrix(lua_State *L) {
    check_arg_count(L, 2);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    dmVMath::Matrix4 *out = dmScript::CheckMatrix4(L, 2);
    *out = userdata->getView();
    return 0;
}

static int LuaCameraGetProj(lua_State *L) {
    check_arg_count(L, 1);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    dmVMath::Matrix4 proj = userdata->getProj();
    dmScript::PushMatrix4(L, proj);
    return 1;
}

static int LuaCameraGetProjToMatrix(lua_State *L) {
    check_arg_count(L, 2);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    dmVMath::Matrix4 *out = dmScript::CheckMatrix4(L, 2);
    *out = userdata->getProj();
    return 0;
}

static int LuaCameraGetFrustum(lua_State *L) {
    check_arg_count(L, 1);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    dmVMath::Matrix4 frustum = userdata->getFrustum();
    dmScript::PushMatrix4(L, frustum);
    return 1;
}

static int LuaCameraGetFrustumToMatrix(lua_State *L) {
    check_arg_count(L, 2);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    dmVMath::Matrix4 *out = dmScript::CheckMatrix4(L, 2);
    *out = userdata->getFrustum();
    return 0;
}

static int LuaCameraGetFov(lua_State *L) {
    check_arg_count(L, 1);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    lua_pushnumber(L, userdata->fov);
    return 1;
}

static int LuaCameraGetNearZ(lua_State *L) {
    check_arg_count(L, 1);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    lua_pushnumber(L, userdata->nearZ);
    return 1;
}

static int LuaCameraGetFarZ(lua_State *L) {
    check_arg_count(L, 1);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    lua_pushnumber(L, userdata->farZ);
    return 1;
}

static int LuaCameraGetPosition(lua_State *L) {
    check_arg_count(L, 1);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    dmScript::PushVector3(L, userdata->pos);
    return 1;
}

static int LuaCameraGetPositionToVector3(lua_State *L) {
    check_arg_count(L, 2);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    dmVMath::Vector3 *out = dmScript::CheckVector3(L, 2);
    *out = userdata->pos;
    return 0;
}

static int LuaCameraGetPositionRaw(lua_State *L) {
    check_arg_count(L, 1);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    lua_pushnumber(L, userdata->pos.getX());
    lua_pushnumber(L, userdata->pos.getY());
    lua_pushnumber(L, userdata->pos.getZ());
    return 3;
}

static int LuaCameraScreenToWorldRay(lua_State *L) {
    check_arg_count(L, 3);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    float screenX = luaL_checknumber(L, 2);
    float screenY = luaL_checknumber(L, 3);
    dmVMath::Vector3 nv;
    dmVMath::Vector3 fv;
    userdata->screenToWorldRay(screenX, screenY, &nv, &fv);
    dmScript::PushVector3(L, nv);
    dmScript::PushVector3(L, fv);
    return 2;
}

static int LuaCameraScreenToWorldRayToVector3(lua_State *L) {
    check_arg_count(L, 5);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    float screenX = luaL_checknumber(L, 2);
    float screenY = luaL_checknumber(L, 3);
    dmVMath::Vector3 *nv = dmScript::CheckVector3(L, 4);
    dmVMath::Vector3 *fv = dmScript::CheckVector3(L, 5);
    userdata->screenToWorldRay(screenX, screenY, nv, fv);
    return 0;
}

static int LuaCameraScreenToWorld2D(lua_State *L) {
    check_arg_count(L, 3);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    float screenX = luaL_checknumber(L, 2);
    float screenY = luaL_checknumber(L, 3);
    float x, y;
    userdata->screenToWorld2D(screenX, screenY, &x, &y);
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return 2;
}

static int LuaCameraWorldToScreen(lua_State *L) {
    check_arg_count(L, 2);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    dmVMath::Vector3 *worldPos = dmScript::CheckVector3(L, 2);
    float x,y;
    userdata->worldToScreen(*worldPos, &x, &y);
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return 2;
}

static int LuaCameraScreenToWorldPlane(lua_State *L) {
    check_arg_count(L, 5);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    float screenX = luaL_checknumber(L, 2);
    float screenY = luaL_checknumber(L, 3);
    dmVMath::Vector3 planeNormal = *dmScript::CheckVector3(L, 4);
    dmVMath::Vector3 pointOnPlane = *dmScript::CheckVector3(L, 5);
    dmVMath::Vector3 out;
    bool result = userdata->screenToWorldPlane(screenX, screenY, planeNormal, pointOnPlane, &out);
    lua_pushboolean(L, result);
    if (result) {
        dmScript::PushVector3(L, out);
        return 2;
    } else {
        return 1;
    }
}

static int LuaCameraScreenToWorldPlaneToVector3(lua_State *L) {
    check_arg_count(L, 6);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    float screenX = luaL_checknumber(L, 2);
    float screenY = luaL_checknumber(L, 3);
    dmVMath::Vector3 planeNormal = *dmScript::CheckVector3(L, 4);
    dmVMath::Vector3 pointOnPlane = *dmScript::CheckVector3(L, 5);
    dmVMath::Vector3 *out = dmScript::CheckVector3(L, 6);
    bool result = userdata->screenToWorldPlane(screenX, screenY, planeNormal, pointOnPlane, out);
    lua_pushboolean(L, result);
    return 1;
}

static int LuaCameraScreenToWorldPlaneRaw(lua_State *L) {
    check_arg_count(L, 5);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    float screenX = luaL_checknumber(L, 2);
    float screenY = luaL_checknumber(L, 3);
    dmVMath::Vector3 planeNormal = *dmScript::CheckVector3(L, 4);
    dmVMath::Vector3 pointOnPlane = *dmScript::CheckVector3(L, 5);
    dmVMath::Vector3 out;
    bool result = userdata->screenToWorldPlane(screenX, screenY, planeNormal, pointOnPlane, &out);
    lua_pushboolean(L, result);
    if (result) {
        lua_pushnumber(L, out.getX());
        lua_pushnumber(L, out.getY());
        lua_pushnumber(L, out.getZ());
        return 4;
    } else {
        return 1;
    }
}

static int LuaCameraToString(lua_State *L) {
    check_arg_count(L, 1);
    NativeCamera::Camera *userdata = CameraCheck(L, 1);
    const size_t bufferSize = 256;
    char buffer[bufferSize];
    userdata->toString(buffer, bufferSize);
    lua_pushstring(L, buffer);
    return 1; // Returning one result to Lua, the string
}

static const luaL_Reg LuaCameraMethods[] = {
    {"__gc", LuaCameraDestroy},
    {"set_orthographic", LuaCameraSetOrthographic},
    {"set_ortho_scale", LuaCameraSetOrthoScale},
    {"set_otrho_scale_mode", LuaCameraSetOrthoScaleMode},
    {"set_view_area", LuaCameraSetViewArea},
    {"set_screen_size", LuaCameraSetScreenSize},
    {"set_fov", LuaCameraSetFov},
    {"set_near_z", LuaCameraSetNearZ},
    {"set_far_z", LuaCameraSetFarZ},
    {"set_position", LuaCameraSetPosition},
    {"set_position_raw", LuaCameraSetPositionRaw},
    {"set_rotation", LuaCameraSetRotation},
    {"set_rotation_raw", LuaCameraSetRotationRaw},
    {"get_position", LuaCameraGetPosition},
    {"get_position_to_vector3", LuaCameraGetPositionToVector3},
    {"get_position_raw", LuaCameraGetPositionRaw},
    {"get_rotation", LuaCameraGetRotation},
    {"get_rotation_to_quat", LuaCameraGetRotationToQuat},
    {"get_rotation_raw", LuaCameraGetRotationRaw},
    {"get_view", LuaCameraGetView},
    {"get_view_to_matrix", LuaCameraGetViewToMatrix},
    {"get_proj", LuaCameraGetProj},
    {"get_proj_to_matrix", LuaCameraGetProjToMatrix},
    {"get_frustum", LuaCameraGetFrustum},
    {"get_frustum_to_matrix", LuaCameraGetFrustumToMatrix},
    {"get_fov", LuaCameraGetFov},
    {"get_near_z", LuaCameraGetNearZ},
    {"get_far_z", LuaCameraGetFarZ},
    {"screen_to_world_ray", LuaCameraScreenToWorldRay},
    {"screen_to_world_ray_to_vector3", LuaCameraScreenToWorldRayToVector3},
    {"screen_to_world_2d", LuaCameraScreenToWorld2D},
    {"world_to_screen", LuaCameraWorldToScreen},
    {"screen_to_world_plane", LuaCameraScreenToWorldPlane},
    {"screen_to_world_plane_to_vector3", LuaCameraScreenToWorldPlaneToVector3},
    {"screen_to_world_plane_raw", LuaCameraScreenToWorldPlaneRaw},
    {"__tostring", LuaCameraToString},
    {0, 0}
};

static void RegisterCameraScaleModeEnum(lua_State *L) {
    lua_newtable(L);
    lua_pushinteger(L, NativeCamera::CameraOrthoScaleMode::EXPANDVIEW);
    lua_setfield(L, -2, "EXPANDVIEW");
    lua_pushinteger(L, NativeCamera::CameraOrthoScaleMode::FIXEDAREA);
    lua_setfield(L, -2, "FIXEDAREA");
    lua_pushinteger(L, NativeCamera::CameraOrthoScaleMode::FIXEDWIDTH);
    lua_setfield(L, -2, "FIXEDWIDTH");
    lua_pushinteger(L, NativeCamera::CameraOrthoScaleMode::FIXEDHEIGHT);
    lua_setfield(L, -2, "FIXEDHEIGHT");
    lua_setfield(L, -2, "SCALE_MODE");
}

static const dmhash_t HASH_ORTHOGRAPHIC = dmHashString64("orthographic");
static const dmhash_t HASH_ORTHO_SCALE = dmHashString64("ortho_scale");
static const dmhash_t HASH_ORTHO_SCALE_MODE = dmHashString64("ortho_scale_mode");
static const dmhash_t HASH_VIEW_AREA_WIDTH = dmHashString64("view_area_width");
static const dmhash_t HASH_VIEW_AREA_HEIGHT = dmHashString64("view_area_height");
static const dmhash_t HASH_POSITION = dmHashString64("position");
static const dmhash_t HASH_ROTATION = dmHashString64("rotation");
static const dmhash_t HASH_FOV = dmHashString64("fov");
static const dmhash_t HASH_NEAR_Z = dmHashString64("near_z");
static const dmhash_t HASH_FAR_Z = dmHashString64("far_z");
static int LuaCreateCamera(lua_State *L) {
    check_arg_count(L, 0, 1);
    int top = lua_gettop(L);
    NativeCamera::Camera *userdata = new NativeCamera::Camera();
    NativeCamera::Camera **ud = (NativeCamera::Camera **)lua_newuserdata(L, sizeof(NativeCamera::Camera *));
    *ud = userdata;
    if (luaL_newmetatable(L, USERDATA_METATABLE)) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        luaL_register(L, 0, LuaCameraMethods);
    }
    lua_setmetatable(L, -2);
   // dmLogInfo("NativeCamera %d",top);
    if (top == 1) {
        if (lua_istable(L, 1)) {
            /* table is in the stack at index 't' */
            lua_pushnil(L);  /* first key */
           // dmLogInfo("Starting to iterate table");
            while (lua_next(L, 1) != 0) {
                const char* key = lua_tostring(L, -2);
                dmhash_t hash_value = dmHashString64(key);
               // dmLogInfo("key %s",key);
                if (hash_value == HASH_POSITION) {
                    userdata->pos = *dmScript::CheckVector3(L, -1);
                } else if (hash_value == HASH_ROTATION) {
                    userdata->rot = *dmScript::CheckQuat(L, -1);
                } else if (hash_value == HASH_ORTHO_SCALE) {
                    userdata->orthoScale = luaL_checknumber(L, -1);
                } else if (hash_value == HASH_ORTHO_SCALE_MODE) {
                    userdata->orthoScaleMode = (NativeCamera::CameraOrthoScaleMode)luaL_checkinteger(L, -1);
                } else if (hash_value == HASH_VIEW_AREA_WIDTH) {
                    userdata->viewAreaWidth = luaL_checknumber(L, -1);
                } else if (hash_value == HASH_VIEW_AREA_HEIGHT) {
                    userdata->viewAreaHeight = luaL_checknumber(L, -1);
                } else if (hash_value == HASH_FOV) {
                    userdata->fov = luaL_checknumber(L, -1);
                } else if (hash_value == HASH_NEAR_Z) {
                    userdata->nearZ = luaL_checknumber(L, -1);
                } else if (hash_value == HASH_FAR_Z) {
                    userdata->farZ = luaL_checknumber(L, -1);
                } else if (hash_value == HASH_ORTHOGRAPHIC) {
                    userdata->orthographic = lua_toboolean(L, -1);
                } else {
                   // dmLogError("unknown key: %s", key);
                }
                /* removes 'value'; keeps 'key' for next iteration */
                lua_pop(L, 1);
            }
            userdata->dirtyProj = true;
            userdata->dirtyView = true;
        }else{
            luaL_error(L, "1 arg should be nil or table");
        }
    }
    return 1;
}

static const luaL_Reg Module_methods[] = {
    {"new", LuaCreateCamera},
    {0, 0}
};

static void LuaInit(lua_State *L) {
    int top = lua_gettop(L);
    luaL_register(L, MODULE_NAME, Module_methods);
    RegisterCameraScaleModeEnum(L);
    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

static dmExtension::Result InitializeMyExtension(dmExtension::Params *params) {
    // Init Lua
    LuaInit(params->m_L);
    printf("Registered %s Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, 0, 0, InitializeMyExtension, 0, 0, 0)
