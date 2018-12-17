//
// Created by myu on 2018/12/13.
//

#ifndef OPENGL_CAMERA_H
#define OPENGL_CAMERA_H

#include <GLUT/GLUT.h>
#include <vector>
#include <string>

#include <Eigen/Dense>
using namespace Eigen;
using namespace std;

class Camera {
public:
    Camera();
    Camera(const Vector3d& pos, const Vector3d& target, const Vector3d& up);
    void setModelViewMatrix();
    void setShape(float viewAngle,float aspect,float Near,float Far);
    void slide(float du, float dv, float dn);
    void roll(float angle);
    void yaw(float angle);
    void pitch(float angle);
    float getDist();

    void chaseTarget(float x, float z, float angle);
    void backChaseTarget(float x, float z, float angle);
    void reverseChaseTarget(float target_x, float target_y, float target_z, float x, float y, float z);
    void chaseTargetWithBias(float target_x, float target_y, float target_z, float x, float y, float z, float bias);
    float camera_close = 120;
    float camera_far = 400;
private:
    Vector3d m_pos;
    Vector3d m_target;
    Vector3d m_up;
    Vector3d u,v,n;

    float c;
};


#endif //OPENGL_CAMERA_H
