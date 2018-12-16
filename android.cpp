//
// Created by myu on 2018/12/2.
//

#include "android.h"
#include <iostream>
#include <math.h>

using namespace std;

Android::Android(float dx, float dy, float dz) {
    x = dx;
    y = dy;
    z = dz;

    c =  3.1415926 / 180.0f;
    v = 2;
    w = 5;
    maxD = 10;
    mode = 0;

    angle = 0;
    angleHead = 0;    //angleY
    angleLeftFoot = 0; // angleZ
    angleRightFoot = 0;
    angleLeftHand = 0;
    angleRightHand = 0;

    modeLeftFoot = -1;
    modeRightFoot = 1;
    modeLeftHand = 1;
    modeRightHand = -1;

    bias = 0;
}

void Android::changeDirection(float degree) {
    angle = degree;
}

void Android::swing(){
    if (angleLeftFoot == maxD || angleLeftFoot == -maxD){
        modeLeftFoot = 0-modeLeftFoot;
    }
    if (angleRightFoot == maxD || angleRightFoot == -maxD){
        modeRightFoot = 0-modeRightFoot;
    }
    if (angleLeftHand == maxD || angleLeftHand == -maxD){
        modeLeftHand = 0-modeLeftHand;
    }
    if (angleRightHand == maxD || angleRightHand == -maxD){
        modeRightHand = 0-modeRightHand;
    }
    angleLeftFoot += modeLeftFoot * w;
    angleRightFoot += modeRightFoot * w;
    angleLeftHand += modeLeftHand * w;
    angleRightHand += modeRightHand * w;
}

void Android::forward() {
    swing();

    z += v * cos(angle*c);
    x += v * sin(angle*c);
}

void Android::setForward(){
    mode = 1;
}

void Android::setStill() {
    angleLeftFoot = 0;
    angleRightFoot = 0;
    angleLeftHand = 0;
    angleRightHand = 0;

    modeLeftFoot = -1;
    modeRightFoot = 1;
    modeLeftHand = 1;
    modeRightHand = -1;
    mode = 0;
}

void Android::captured() {
    setStill();
    mode = -2;
    if (bias != 20) {
        bias += 0.5;
    }
}