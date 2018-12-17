//
// Created by myu on 2018/11/6.
//

#include "antimotion.h"
#include <iostream>
#include <math.h>
using namespace std;

AntiMotion::AntiMotion(float dx, float dy, float dz) {
    angleX = 0;
    angleZ = 0;
    angleY = 0;
    x = dx;
    y = dy;
    z = dz;
    wingAngle = 0;
    // move
    vX = 0;
    vY = 0;
    vZ = 0;
    // rotate
    wXZ = 3.33f;
    wY = 5.0f;
    wWing = 60;
    // accelerate
    aXZ = 4.0f;
    aY = 4.0f;
    maxAngleXZ = 10;
    maxVXZ = 10.0f;
    maxVY = 10.0f;
    c =  3.1415926 / 180.0f;

    colorX = 1;
    colorY = 1;
    colorZ = 1;

    flag = false;
}

bool AntiMotion::move(float targetX, float targetY, float targetZ, float protectX, float protectY, float protectZ, float protectV, float offsetX, float offsetY, float offsetZ) {
    if ((targetX == x)&&(targetY == y)&&(targetZ == z)&&!flag){
     //   cout << "anti done\n" << endl;
        flag = true;
    }
    if (flag) return true;
    float dTarget = sqrt((x - targetX)*(x - targetX)+(z - targetZ)*(z - targetZ)+(y - targetY)*(y - targetY));
    float dProtect = sqrt((x - protectX)*(x - protectX)+(z - protectZ)*(z - protectZ)+(y - protectY)*(y - protectY));
   // if (offsetX == 0 && offsetY == 0 && offsetZ == 0){
        if (dTarget < 60) flag=chase(targetX, targetY, targetZ, offsetX, offsetY, offsetZ, true);
        else if (dProtect > 60) chase(protectX, protectY, protectZ, 0, 0, 0, false);
  //  }
//    if (dTarget > 60){
//        float dProtect = sqrt((x - protectX)*(x - protectX)+(z - protectZ)*(z - protectZ)+(y - protectY)*(y - protectY));
//        if (dProtect <= 2*protectV){
//            x = protectX;
//            y = protectY;
//            z = protectZ;
//            wingAngle += wWing;
//            return;
//        }
//        chase(protectX, protectY, protectZ, 0, 0, 0, false);      //chase owner
//    }
//    else {
  //  else  flag=chase(targetX, targetY, targetZ, offsetX, offsetY, offsetZ, true);        //chase FKR
    //}
    y += vY;
    x += vX;
    z += vZ;
    if (x <= -380/sqrt(2)){x = -379/sqrt(2);}
    else if (x >= 380/sqrt(2)) {x = 379/sqrt(2);}
    if (z <= -380/sqrt(2)) {z = -379/sqrt(2);}
    else if (z >= 380/sqrt(2)) {z = 379/sqrt(2);}
    if (y <= -100){y = -99;}
    else if (y >= 100) {y = 99;}
    wingAngle += wWing;
    return flag;
}

bool AntiMotion::chase(float targetX, float targetY, float targetZ, float offsetX, float offsetY, float offsetZ, bool mode) {
    float tmpX, tmpZ;
//    tmpX = (targetX+offsetX) * cos(angleY * c) - (targetZ+offsetZ) * sin(angleY * c);
//    tmpZ = (targetX+offsetX) * sin(c*angleY) + (targetZ+offsetZ) * cos(c*angleY);
    tmpX = targetX+offsetX;
    tmpZ = targetZ+offsetZ;
    float chaseX = tmpX;
    float chaseZ = tmpZ;
    float chaseY = targetY + offsetY;
    tmpX = targetX * cos(angleY * c) - targetZ * sin(angleY * c);
    tmpZ = targetX * sin(c*angleY) + targetZ * cos(c*angleY);
    targetX = tmpX;
    targetZ = tmpZ;
    if (abs(x - targetX) < abs(maxVXZ)){
        x = targetX;
        vX = 0;
        angleZ = 0;
    }
    if (abs(z - targetZ) < abs(maxVXZ)){
        z = targetZ;
        vZ = 0;
        angleX = 0;
    }
    if (abs(y - targetY) < abs(maxVY)){
        y = targetY;
        vY = 0;
    }
    if (x == targetX & z == targetZ & y == targetY){
        if (mode) {
            flag = true;
//            colorX = 0;
//            colorY = 1;
//            colorZ = 0;
          //  cout << "Anti done" << endl;
        }
        return flag;
    }
    float d = sqrt(pow(x-chaseX, 2) + pow(y-chaseY, 2) + pow(z-chaseZ, 2));
    if (d < 30){
        chaseY = targetY;
        chaseX = targetX;
        chaseZ = targetZ;
    }
    if (y != targetY) {
        if (y < chaseY) {   //up
            if (vY < maxVY) {
                vY += aY;
            }
        } else if (y > chaseY) {  //down
            if (vY > -maxVY) {
                vY -= aY;
            }
        }
    }
    float tan;
    if (x == chaseX){
        tan = -9999;
    }
    else {
        tan = abs((z - chaseZ) / (x - chaseX));
    }
    float aX, aZ;
    if (x == chaseX){
        aX = 0;
        aZ = aXZ;
    }
    else {
        aX = aXZ;
        aZ = aXZ * tan;
    }
    if (x != targetX) {
        if (x < chaseX) {
            if (angleZ > -maxAngleXZ) {
                angleZ -= wXZ;
            }
            if (vX < 0) { vX += aX; }
            if (vX < maxVXZ) vX += aX;
        } else if (x > chaseX) {
            if (angleZ > -maxAngleXZ) {
                angleZ -= wXZ;
            }
            if (vX > 0) { vX -= aX; }
            if (vX > -maxVXZ) vX -= aX;
        }
    }
    if (z != targetZ) {
        if (z < chaseZ) {
            if (angleX > -maxAngleXZ) {
                angleX -= wXZ;
            }
            if (vZ < 0) { vZ += aZ; }
            if (vZ < maxVXZ) vZ += aZ;
        } else if (z > chaseZ) {
            if (angleX < maxAngleXZ) {
                angleX += wXZ;
            }
            if (vZ > 0) { vZ -= aZ; }
            if (vZ > -maxVXZ) vZ -= aZ;
        }
    }
    return false;
}

void AntiMotion::restart(float dx, float dy, float dz) {
    x = dx;
    y = dy;
    z = dz;
    vX = 0;
    vY = 0;
    vZ = 0;
    angleX = 0;
    angleZ = 0;
    angleY = 0;
    wingAngle = 0;
    colorX = 1;
    colorY = 1;
    colorZ = 1;
    flag = false;
}

void AntiMotion::still(float protectX, float protectY, float protectZ) {
    x = protectX;
    y = protectY;
    z = protectZ;
}

void AntiMotion::done() {
    vX = 0;
    vY = 0;
    vZ = 0;
    angleX = 0;
    angleZ = 0;
    angleY = 0;
}