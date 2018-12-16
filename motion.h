//
// Created by myu on 2018/10/5.
//

#ifndef OPENGL_MOTION_H
#define OPENGL_MOTION_H
#include <GLUT/GLUT.h>
#include <vector>
#include <string>
using namespace std;
#endif //OPENGL_MOTION_H

class Motion{
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
    int count;  // random seed

    // constant
    float c;

    // chase temp data
    int meet_times;

public:
    int state;
    bool mode;  // random mode
    float angleX;
    float angleZ;
    float angleY;
    float wingAngle;
    float x;
    float y;
    float z;
    // explode bias
    float bias;
    Motion();
    void restart();
    // manual operation
    void clockwiseRotation();
    void antiClockwiseRotation();
    void up();
    void down();
    void xPositiveLift();
    void xNegativeLift();
    void zPositiveLift();
    void zNegativeLift();
    void recover();
    void pending();
    void move();
    void captured();

    // flight path
    bool forward(float targetX, float targetY, float targetZ, float dodgeX, float dodgeY, float dodgeZ);
    void dodge(float targetX, float targetY, float targetZ);
    void chase(float targetX, float targetY, float targetZ);
    void random();


    // forward
    void forwardEdge(float targetX, float targetY, float targetZ);
    void forwardTarget(float targetX, float targetY);
};
