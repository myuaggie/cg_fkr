//
// Created by myu on 2018/11/6.
//

#ifndef OPENGL_ANTIMOTION_H
#define OPENGL_ANTIMOTION_H
#include <GLUT/GLUT.h>
#include <vector>
#include <string>
using namespace std;
#endif //OPENGL_ANTIMOTION_H

class AntiMotion{
private:
    // rotate
    float wXZ;
    float wY;
    // move
    float vX;
    float vY;
    float vZ;

    float aXZ;  // 90degree
    float aY;  // up&down
    float maxAngleXZ;
    float maxVXZ;
    float maxVY;

    float wWing;
    float c;


public:
    float angleX;
    float angleZ;
    float angleY;
    float wingAngle;
    float x;
    float y;
    float z;
    float colorX;
    float colorY;
    float colorZ;
    bool flag;    // explode or not
    AntiMotion(float dx, float dy, float dz);
    bool move(float targetX, float targetY, float targetZ, float protectX, float protectY, float protectZ, float protectV, float offsetX, float offsetY, float offsetZ);
    bool chase(float targetX, float targetY, float targetZ, float offsetX, float offsetY, float offsetZ, bool mode);
    void restart(float dx, float dy, float dz);
    void still(float protectX, float protectY, float protectZ);
    void done();
};