//
// Created by myu on 2018/12/2.
//

#ifndef OPENGL_ANDROID_H
#define OPENGL_ANDROID_H


class Android {
public:
    float x;
    float y;
    float z;

    float angle;

    float angleLeftHand;
    float angleRightHand;
    float angleLeftFoot;
    float angleRightFoot;
    float angleHead;

    int modeLeftHand;
    int modeRightHand;
    int modeLeftFoot;
    int modeRightFoot;

    float c;
    float v;
    float w;
    float maxD;
    int mode;  //0: manual 1: auto
    float bias;
    Android(float dx, float dy, float dz);

    void changeDirection(float degree);

    void setForward();
    void setStill();

    void swing();
    void forward();

    void captured();
};


#endif //OPENGL_ANDROID_H
