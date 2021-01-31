#include "camera.h"
#include "matrix.h"

camera_t camera = {
    .position = { 0, 0, 0 },
    .direction = { 0, 0, 1 },
    .forwardVelocity = { 0, 0, 0 },
    .yaw = 0.0,
    .pitch = 0.0
};


vec3_t get_camera_target(void)
{
    vec3_t target = { 0, 0, 1 };
    mat4_t camera_yaw_rotation = Mat4MakeRotationY(camera.yaw);
    mat4_t camera_pitch_rotation = Mat4MakeRotationX(camera.pitch);

    mat4_t camera_rotation = Mat4Identity();
    camera_rotation = Mat4MultMat4(camera_pitch_rotation, camera_rotation); 
    camera_rotation = Mat4MultMat4(camera_yaw_rotation, camera_rotation);

    camera.direction = Vec3FromVec4(Mat4MultVect4(camera_rotation, Vec4FromVec3(target))); 
    target = Vector3Add(camera.position, camera.direction);

    return target;
}


