//
// Created by myu on 2018/12/17.
//

#include "daodan.h"

#include <math.h>


Daodan::Daodan() {
    state = -1;
    bias = 30;
}

Daodan::Daodan(float pos_x, float pos_y, float pos_z) {
    x = pos_x;
    y = pos_y;
    z = pos_z;

    angleX = 0;
    angleY = 0;
    state = 0;
    alive_time = 0;
    bias = 0;
    c =  3.1415926 / 180.0f;
}

void Daodan::trace(float target_x, float target_y, float target_z) {
    float dx = target_x - x;
    float dy = target_y - y;
    float dz = target_z - z;
    float d = sqrt(dx*dx+dy*dy+dz*dz);
    if (d < v){
        state = 1;  //anti done
        return;
    }
    if (alive_time >= 50){
        state = -1;
        return;
    }
    if (dz == 0){
        if (dx > 0){
            angleY = 90;
        }
        else if (dx < 0){
            angleY = -90;
        }
        else {
            angleY = 0;
        }
    }
    else {
        angleY = atan(dx / dz)/c;
        if (dz < 0){
            angleY += 180;
        }
    }
    float dxz = sqrt(dx*dx+dz*dz);
    if (dxz == 0){
        if (dy > 0){
            angleX = 90;
        }
        else if (dy < 0){
            angleX = -90;
        }
        else {
            angleX = 0;
        }
    }
    else {
        angleX = -atan(dy / dxz)/c;
    }
    Vector3d vt(dx, dy, dz);
    vt.normalize();

    x += vt.x()*v;
    y += vt.y()*v;
    z += vt.z()*v;
    alive_time+=1;

    if (x <= -380/sqrt(2)){x = -379/sqrt(2);}
    else if (x >= 380/sqrt(2)) {x = 379/sqrt(2);}
    if (z <= -380/sqrt(2)) {z = -379/sqrt(2);}
    else if (z >= 380/sqrt(2)) {z = 379/sqrt(2);}
}