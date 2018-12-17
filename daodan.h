//
// Created by myu on 2018/12/17.
//

#ifndef OPENGL_DAODAN_H
#define OPENGL_DAODAN_H
#include <Eigen/Dense>
using namespace Eigen;

class Daodan {
private:
    float v = 8;
    float c;
public:
    float x;
    float y;
    float z;
    float angleX;
    float angleY;
    int state;
    int alive_time;
    float bias;

    Daodan();
    Daodan(float pos_x, float pos_y, float pos_z);

    void trace(float target_x, float target_y, float target_z);
};


#endif //OPENGL_DAODAN_H
