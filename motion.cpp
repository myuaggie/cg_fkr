//
// Created by myu on 2018/10/5.
//

#include "motion.h"
#include <iostream>
#include <math.h>
using namespace std;

Motion::Motion() {
    angleX = 0;
    angleZ = 0;
    angleY = 0;
    x = -50;
    y = -50;
    z = -50;
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
    state = -1;
    count = 0;
    mode = false;
    c =  3.1415926 / 180.0f;

    meet_times = 0;
}

void Motion::clockwiseRotation() {
//    if (vX != 0) {
//        vX = vX / cos(c*angleY);
//    }
//    if (vZ != 0) {
//        vZ = vZ / sin(c*angleY);
//    }
    angleY += wY;
    float tmpX, tmpZ;
    tmpX = x * cos(c*wY) - z * sin(c*wY);
    tmpZ = x * sin(c*wY) + z * cos(c*wY);
    x = tmpX;
    z = tmpZ;
    float tmpVX, tmpVZ;
    tmpVX = vX * cos(c*wY) - vZ * sin(c*wY);
    tmpVZ = vX * sin(c*wY) + vZ * cos(c*wY);
    vX = tmpVX;
    vZ = tmpVZ;
    wingAngle -= wY;
//
//    vX = vX * cos(c*angleY);
//    vZ = vZ * sin(c*angleY);
 //   cout << "angleY: " << angleY << endl;
//    cout << "vX:" << vX << endl;
}

void Motion::antiClockwiseRotation() {
//    if (vX != 0) {
//        vX = vX / cos(c*angleY);
//    }
//    if (vZ != 0) {
//        vZ = vZ / sin(c*angleY);
//    }
    angleY -= wY;
    float tmpX, tmpZ;
    tmpX = x * cos(c*wY) + z * sin(c*wY);
    tmpZ = z * cos(c*wY) - x * sin(c*wY);
    x = tmpX;
    z = tmpZ;
    float tmpVX, tmpVZ;
    tmpVX = vX * cos(c*wY) + vZ * sin(c*wY);
    tmpVZ = vZ * cos(c*wY) - vX * sin(c*wY);
    vX = tmpVX;
    vZ = tmpVZ;
    wingAngle += wY;
//    vX = vX * cos(c*angleY);
//    vZ = vZ * sin(c*angleY);
//    cout << "angleY: " << angleY << endl;
//    cout << "vX:" << vX << endl;
}

void Motion::xPositiveLift() {
    if (angleZ < maxAngleXZ){
        angleZ += wXZ;
    }
//    if (vX / cos(c*angleY)> 0 ) {
//        vX -= 2 * aXZ * sin(c * angleZ) * cos(c * angleY);
//    }
//    else {
//        vX -= aXZ * sin(c * angleZ) * cos(c * angleY);
//    }
//    if (vZ / sin(c*angleY)< 0 ) {
//        vZ += 2 * aXZ * sin(c * angleZ) * sin(c * angleY);
//    }
//    else {
//        vZ += aXZ * sin(c * angleZ) * sin(c * angleY);
//    }
    if (vX > 0){vX -= 2 * aXZ * sin(c * angleZ);}
    else {vX -= aXZ * sin(c * angleZ);}
    if (vX > maxVXZ) vX = maxVXZ;
    else if (vX < -maxVXZ) vX = -maxVXZ;
//    if (vZ > maxVXZ) vZ = maxVXZ;
//    else if (vZ < -maxVXZ) vZ = -maxVXZ;
//    cout << "angleZ: " << angleZ << endl;
//    cout << "vX:" << vX << endl;
}

void Motion::xNegativeLift() {
    if (angleZ > -maxAngleXZ){
        angleZ -= wXZ;
    }
//    if (vX / cos(c*angleY)< 0 ) {
//        vX -= 2 * aXZ * sin(c * angleZ) * cos(c * angleY);
//    }
//    else {
//        vX -= aXZ * sin(c * angleZ) * cos(c * angleY);
//    }
//    if (vZ / sin(c*angleY)> 0 ) {
//        vZ += 2 * aXZ * sin(c * angleZ) * sin(c * angleY);
//    }
//    else {
//        vZ += aXZ * sin(c * angleZ) * sin(c * angleY);
//    }
//    vX -= aXZ * sin(c*angleZ) * cos(c*angleY);
//    vX -= aXZ * sin(c*angleZ) * sin(c*angleY);
    if (vX < 0){vX -= 2 * aXZ * sin(c * angleZ);}
    else {vX -= aXZ * sin(c * angleZ);}
    if (vX > maxVXZ) vX = maxVXZ;
    else if (vX < -maxVXZ) vX = -maxVXZ;
//    if (vZ > maxVXZ) vZ = maxVXZ;
//    else if (vZ < -maxVXZ) vZ = -maxVXZ;
 //   cout << "angleZ: " << angleZ << endl;
   // cout << "vX:" << vX << endl;
}

void Motion::zPositiveLift() {
    if (angleX > -maxAngleXZ){
        angleX -= wXZ;
    }
//    if (vZ / cos(c*angleY) > 0) {
//        vZ -= 2 * aXZ * sin(c * angleX) * sin(c * (angleY-90));
//    }
//    else {
//        vZ -= aXZ * sin(c * angleX) * sin(c * (angleY-90));
//    }
//    if (vX / sin(c*angleY) > 0){
//        vX -= 2 * aXZ * sin(c*angleX) * cos(c*(angleY-90));
//    }
//    else {
//        vX -= aXZ * sin(c * angleX) * cos(c * (angleY-90));
//    }
    if (vZ > 0){vZ += 2 * aXZ * sin(c * angleX);}
    else {vZ += aXZ * sin(c * angleX);}
    if (vZ > maxVXZ) vZ = maxVXZ;
    else if (vZ < -maxVXZ) vZ = -maxVXZ;
//    if (vX > maxVXZ) vX = maxVXZ;
//    else if (vX < -maxVXZ) vX = -maxVXZ;
//    cout << "angleX: " << angleX << endl;
 //   cout << "vZ:" << vZ << endl;
}

void Motion::zNegativeLift() {
    if (angleX < maxAngleXZ){
        angleX += wXZ;
    }
//    if (vZ / cos(c*angleY) > 0 ) {
//        vZ -= 2 * aXZ * sin(c * angleX) * sin(c * (angleY-90));
//    }
//    else {
//        vZ -= aXZ * sin(c * angleX) * sin(c * (angleY-90));
//    }
//    if (vX / sin(c*angleY) > 0){
//        vX -= 2 * aXZ * sin(c*angleX) * cos(c*(angleY-90));
//    }
//    else {
//        vX -= aXZ * sin(c * angleX) * cos(c * (angleY-90));
//    }
//    vZ += aXZ * sin(c*angleX) * cos(c*angleY);
//    vX += aXZ * sin(c*angleX) * sin(c*angleY);
    if (vZ < 0){vZ += 2 * aXZ * sin(c * angleX);}
    else {vZ += aXZ * sin(c * angleX);}
    if (vZ > maxVXZ) vZ = maxVXZ;
    else if (vZ < -maxVXZ) vZ = -maxVXZ;
//    if (vX > maxVXZ) vX = maxVXZ;
//    else if (vX < -maxVXZ) vX = -maxVXZ;
//    cout << "angleX: " << angleX << endl;
  //  cout << "vZ:" << vZ << endl;
}

void Motion::up() {
    if (vY < maxVY){
        vY += aY;
    }
 //   cout << "vY:" << vY << endl;
}

void Motion::down() {
    if (vY > -maxVY){
        vY -= aY;
    }
 //   cout << "vY:" << vY << endl;
}

void Motion::move() {
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
}

void Motion::pending() {
    if (vX == 0 && vZ == 0 && vY == 0){
        state = -1;
     //   cout << "pending done" << endl;
        return;
    }
    state = 0;
    if (vY > 0){
        if (vY < 1){
            vY = 0;
        }
        else {
            vY -= aY;
        }
    }
    else if (vY < 0){
        if (vY > -1){
            vY = 0;
        }
        else{
            vY += aY;
        }
    }
    if (vX > 0){
        xPositiveLift();
        if (vX < aXZ || angleZ > -wXZ){
            angleZ = 0;
            vX = 0;
        }
        if (vZ < aXZ && vZ > -aXZ){
            angleX = 0;
            vZ = 0;
        }
    }
    else if (vX < 0){
        xNegativeLift();
        if (vX > -aXZ || angleZ < wXZ){
            angleZ = 0;
            vX = 0;
        }
        if (vZ < aXZ && vZ > -aXZ){
            angleX = 0;
            vZ = 0;
        }
    }
    if (vZ > 0){
        zPositiveLift();
        if (vZ < aXZ ||angleX > -wXZ){
            angleX = 0;
            vZ = 0;
        }
        if (vX < aXZ && vX > -aXZ){
            angleZ = 0;
            vX = 0;
        }
    }
    else if (vZ < 0){
        zNegativeLift();
        if (vZ > -aXZ ||angleX < wXZ){
            angleX = 0;
            vZ = 0;
        }
        if (vX < aXZ && vX > -aXZ){
            angleZ = 0;
            vX = 0;
        }
    }
}

void Motion::recover() {
    if (angleZ < 0){
        angleZ += wXZ/2;
    }
    else if (angleZ > 0){
        angleZ -= wXZ/2;
    }
//    vX -= aXZ * sin(c*angleZ) * cos(c*angleY);
    vX -= aXZ * sin(c*angleZ);
    if (vX > maxVXZ) vX = maxVXZ;
    if (vX < -maxVXZ) vX = -maxVXZ;
    if (angleX < 0){
        angleX += wXZ/2;
    }
    else if (angleX > 0){
        angleX -= wXZ/2;
    }
//    vZ -= aXZ * sin(c*angleX) * cos(c*angleY);
    vZ -= aXZ * sin(c*angleX);
    if (vZ > maxVXZ) vZ = maxVXZ;
    if (vZ < -maxVXZ) vZ = -maxVXZ;
}

void Motion::chase(float targetX, float targetY, float targetZ) {
    float tmpX, tmpZ;
    tmpX = targetX * cos(angleY * c) - targetZ * sin(angleY * c);
    tmpZ = targetX * sin(c*angleY) + targetZ * cos(c*angleY);
    targetX = tmpX;
    targetZ = tmpZ;
    if (abs(x - targetX) < abs(vX)){
        x = targetX;
        vX = 0;
        angleZ = 0;
    }
    if (abs(z - targetZ) < abs(vZ)){
        z = targetZ;
        vZ = 0;
        angleX = 0;
    }
    if (abs(y - targetY) < abs(vY)){
        y = targetY;
        vY = 0;
    }
    if (x == targetX & z == targetZ & y == targetY){
        state = 0; //pending
        return;
    }
    if (y < targetY){
        up();
    }
    else if (y > targetY){
        down();
    }
    float tan;
    if (x == targetX){
        tan = -9999;
    }
    else {
        tan = abs((z - targetZ) / (x - targetX));
    }
    float aX, aZ;
    if (x == targetX){
        aX = 0;
        aZ = aXZ;
    }
    else {
        aX = aXZ;
        aZ = aXZ * tan;
    }
    if (x < targetX){
        if (angleZ > -maxAngleXZ){
            angleZ -= wXZ;
        }
        if (vX < 0){vX += aX;}
        if (vX < maxVXZ) vX += aX;
    }
    else if (x > targetX){
        if (angleZ > -maxAngleXZ){
            angleZ -= wXZ;
        }
        if (vX > 0){vX -= aX;}
        if (vX > -maxVXZ) vX -= aX;
    }
    if (z < targetZ){
        if (angleX > -maxAngleXZ){
            angleX -= wXZ;
        }
        if (vZ < 0){vZ += aZ;}
        if (vZ < maxVXZ) vZ += aZ;
    }
    else if (z > targetZ){
        if (angleX < maxAngleXZ){
            angleX += wXZ;
        }
        if (vZ > 0){vZ -= aZ;}
        if (vZ > -maxVXZ) vZ -= aZ;
    }
}

void Motion::dodge(float targetX, float targetY, float targetZ) {
    if (rand()%2 == 1){
        down();
    }
    else {
        up();
    }
    float tan;
//    if (x == targetX){
//        tan = 0;
//    }
//    else {
//        tan = abs((z - targetZ) / (x - targetX));
//    }
    if (z == targetZ){
        tan = 0;
    }
    else {
        tan = abs( (x - targetX) / (z - targetZ) );
    }
    float aX, aZ;
    if (x == targetX){
        aX = 0;
        aZ = aXZ;
    }
    else {
        aX = aXZ;
        aZ = aXZ * tan;
    }
    if (x <= targetX){
        if (angleZ < maxAngleXZ){
            angleZ += wXZ;
        }
        if (vX > 0) vX -= aX;
        if (vX > -maxVXZ) vX -= aX;
    }
    else if (x > targetX){
        if (angleZ > -maxAngleXZ){
            angleZ -= wXZ;
        }
        if (vX < 0) vX += aX;
        if (vX < maxVXZ) vX += aX;
    }
    if (z <= targetZ){
        if (angleX < maxAngleXZ){
            angleX += wXZ;
        }
        if (vZ > 0) vZ -= aZ;
        if (vZ > -maxVXZ) vZ -= aZ;
    }
    else if (z > targetZ){
        if (angleX > -maxAngleXZ){
            angleX -= wXZ;
        }
        if (vZ < 0) vZ += aZ;
        if (vZ < maxVXZ) vZ += aZ;
    }
}

bool Motion::forward(float targetX, float targetY, float targetZ, float dodgeX, float dodgeY, float dodgeZ,
        vector<Vector3d> dodges){
//    float d = sqrt(pow(x-dodgeX, 2) + pow(y-dodgeY, 2) + pow(z-dodgeZ, 2));
//    float v = sqrt(pow(vX, 2) + pow(vY, 2) + pow(vZ, 2));
//    if (d < v){
//        cout << "dodge" << endl;
//        dodge(dodgeX, dodgeY, dodgeZ);
//    }
//    else {
//        cout << "chase" << endl;
//        chase(targetX, targetY, targetZ);
//    }
//    if (abs(x - targetX) < abs(maxVXZ)){
//        cout << "xdone" << endl;
//        x = targetX;
//        vX = 0;
//        angleZ = 0;
//    }
//    if (abs(z - targetZ) < abs(maxVXZ)){
//        cout << "zdone" << endl;
//        z = targetZ;
//        vZ = 0;
//        angleX = 0;
//    }
//cout << "target: " << targetX << "," << targetY << "," <<targetZ << "," <<endl;
//cout << "self:" << x << "," << y << "," <<z << "," <<endl;
    if (abs(y - targetY) < abs(vY)){
    //    cout << "ydone" << endl;
        y = targetY;
        vY = 0;
    }

    float d = sqrt(pow(x-targetX, 2) + pow(y-targetY, 2) + pow(z-targetZ, 2));
    float v = sqrt(pow(vX, 2) + pow(vY, 2) + pow(vZ, 2));
//    if (x == targetX && z == targetZ && y == targetY){
//        cout << "done" << endl;
//        state = 0; //pending
//        return;
//    }
    if (d <= maxVXZ){
      //  cout << "done" << endl;
        x = targetX;
        vX = 0;
        angleZ = 0;
        z = targetZ;
        vZ = 0;
        angleX = 0;
        state = 0;
        return true;
    }

    for (int i =0; i< dodges.size();i++){
        Vector3d tmp_v = dodges[i];
        float tmp_d = sqrt((x-tmp_v.x())*(x-tmp_v.x())+(y-tmp_v.y())*(y-tmp_v.y())+(z-tmp_v.z())*(z-tmp_v.z()));
        if (tmp_d <= 80) {
            dodge(tmp_v.x(),tmp_v.y(),tmp_v.z());
            return false;
        }
    }
    if (y < targetY){
        up();
    }
    else if (y > targetY){
        down();
    }

    float dxz = sqrt(pow(x-targetX, 2) + pow(z-targetZ, 2));
    if (dxz <= 2 * maxVXZ && meet_times < 1){
        meet_times += 1;
    }
    if (meet_times != 1){
        float dd = sqrt(pow(x-dodgeX, 2) + pow(z-dodgeZ, 2));
        if (dd <= 2 * maxVXZ){
          //  cout << "to target" << endl;
            forwardTarget(targetX, targetZ);
            return false;
        }
        float x0 = targetX;  float y0 = targetZ;
        float x1 = x; float y1 = z;
        float r0 = 2*maxVXZ;
        if (-r0*r0+x0*x0-2*x0*x1+x1*x1 == 0){
           // cout << "to target" << endl;
            forwardTarget(targetX, targetZ);
            return false;
        }
        float k1=(y0*x0+y1*x1-y0*x1-y1*x0+sqrt(r0*r0*(-2*y0*y1-2*x0*x1+y1*y1+y0*y0+x0*x0-r0*r0+x1*x1)))/(-r0*r0+x0*x0-2*x0*x1+x1*x1);

      //  float k2= (y0*x0+y1*x1-y0*x1-y1*x0-sqrt(r0*r0*(-2*y0*y1-2*x0*x1+y1*y1+y0*y0+x0*x0-r0*r0+x1*x1)))/(-r0*r0+x0*x0-2*x0*x1+x1*x1);

        float meetX1=(-k1*y1+x0+k1*k1*x1+y0*k1)/(1+k1*k1);
        float meetY1 =-(-y1-k1*x0-y0*k1*k1+k1*x1)/(1+k1*k1);

     //   cout << "to meet:" << meetX2 << "," << meetY2 << endl;
        forwardTarget(meetX1, meetY1);

    }
    else{
     //   cout << "to target" << endl;
       forwardTarget(targetX, targetZ);
    }
    return false;
}



void Motion::forwardTarget(float targetX, float targetZ){
    float tan;
    if (x == targetX){
        tan = -9999;
    }
    else {
        tan = abs((z - targetZ) / (x - targetX));
    }
    float aX, aZ;
    if (x == targetX){
        aX = 0;
        aZ = aXZ;
    }
    else {
        aX = aXZ;
        aZ = aXZ * tan;
    }
    if (x < targetX){
        if (angleZ > -maxAngleXZ){
            angleZ -= wXZ;
        }
        if (vX < 0){vX += aX;}
        if (vX < maxVXZ) vX += aX;
    }
    else if (x > targetX){
        if (angleZ > -maxAngleXZ){
            angleZ -= wXZ;
        }
        if (vX > 0){vX -= aX;}
        if (vX > -maxVXZ) vX -= aX;
    }
    if (z < targetZ){
        if (angleX > -maxAngleXZ){
            angleX -= wXZ;
        }
        if (vZ < 0){vZ += aZ;}
        if (vZ < maxVXZ) vZ += aZ;
    }
    else if (z > targetZ){
        if (angleX < maxAngleXZ){
            angleX += wXZ;
        }
        if (vZ > 0){vZ -= aZ;}
        if (vZ > -maxVXZ) vZ -= aZ;
    }
}

void Motion::captured() {
    vX = 0;
    vY = 0;
    vZ = 0;
    angleX = 0;
    angleZ = 0;
    angleY = 0;
    state = -2;
    if (bias != 40) {
        bias += 0.5;
    }
}

void Motion::random() {
    count ++;
    chase(pow(-1, rand()) * (rand() % 1000) / 4.0, pow(-1, rand()) * (rand() % 1000) / 4.0, pow(-1, rand()) * (rand() % 1000) / 4.0);
}

void Motion::restart() {
    angleX = 0;
    angleZ = 0;
    angleY = 0;
    x = -50;
    y = -50;
    z = -50;
    vX = 0;
    vY = 0;
    vZ = 0;
    state = -1;
    meet_times = 0;
    bias = 0;
}