//
//  main.cpp
//  try1
//
//  Created by myu on 2018/9/18.
//  Copyright © 2018年 myu. All rights reserved.
//

#include "objloader.hpp"
#include "tgaparser.h"
#include "motion.h"
#include "antimotion.h"
#include "camera.h"
#include "android.h"
#include "daodan.h"
#include <stdio.h>
#include <GLUT/GLUT.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <thread>

#include <unistd.h>

#include <sys/select.h>

using namespace std;

const int MAXPATH=250;

string root;
string root_path = "/FKR/";
string root_path2 = "/anti-FKR/";
string root_path3 = "/station/";
string root_path4 = "/android/";
string root_path5 = "/daodan/";
string file = "wurenjismall.obj";
string file1 = "wurenjiyi1.obj";
string file2 = "wurenjiyi2.obj";
string file3 = "wurenjiyi3.obj";
string file4 = "wurenjiyi4.obj";
string file5 = "anti.obj";
string file6 = "station.obj";
string file7 = "androidbody.obj";
string file8 = "androidleftfoot.obj";
string file9 = "androidrightfoot.obj";
string file10 = "androidlefthand.obj";
string file11 = "androidrighthand.obj";
string file12 = "androidrhead.obj";
string file13 = "daodan.obj";
string floor_texture = "/scene/floor2.tga";
string wall_texture = "/scene/wall2.tga";
string station_texture = "/scene/station.tga";
GLuint floor_tid, wall_tid, station_tid;
int program;
ObjLoader objModel, objModel1, objModel2, objModel3, objModel4,  //fkr
    objModel5, //anti-fkr
    objModel6, //bg
    objModelAndroidBody, objModelAndroidHead, objModelAndroidLeftFoot, objModelAndroidRightFoot,
    objModelAndroidLeftHand, objModelAndroidRightHand,
    objModelDaoDan;
static float c = 3.1415926 / 180.0f;
static float r = 1.0f;
static int oldPosY = -1;
static int oldPosX = -1;

static float dx = 150;
static float dy = -98;
static float dz = 150;
Android android(dx, dy, dz);


static float vTarget = 2.0f;
float degree = 0;
vector<AntiMotion *> v_anti;
vector<vector<float> > v_offset;
AntiMotion anti0 = AntiMotion(dx, dy+40, dz);
AntiMotion anti1 = AntiMotion(dx, dy+40, dz);
AntiMotion anti2 = AntiMotion(dx, dy+40, dz);
AntiMotion anti3 = AntiMotion(dx, dy+40, dz);
AntiMotion anti4 = AntiMotion(dx, dy+40, dz);

Motion motion = Motion();

Daodan* daodan = new Daodan();

Vector3d pos(50.0, -40.0, 120.0);
Vector3d target(82.0, -40.0, -400.0);
Vector3d up(0.0, 1.0, 0.0);
Vector3d pos2(-82.0, -40.0, -245.0);
Vector3d target2(-82.0, -40.0, 400.0);
Vector3d up2(0.0, 1.0, 0.0);
Camera* camera;
Camera* camera2;
Camera* camera3;
Camera* camera4;

int window_width, window_height;

int window_mode, window_change;
clock_t start, now;

//安置光源
void setLightRes() {
    GLfloat lightPosition[] = { 0,0,0,1 };
    GLfloat lightAmbient [] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat lightAmbient0 [] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat lightDiffuse [] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT , lightAmbient );
    glLightfv(GL_LIGHT0, GL_DIFFUSE , lightDiffuse );
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightModelfv(GL_AMBIENT, lightAmbient);
    glLightModelfv(GL_DIFFUSE , lightDiffuse );
    glLightModelfv(GL_SPECULAR, lightSpecular);
       glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
   // glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    GLfloat lightPosition2[] = { -400,0, 0,0 };
    GLfloat lightPosition3[] = { 400,0, 400,0 };
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
    glLightfv(GL_LIGHT3, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT3, GL_DIFFUSE , lightDiffuse );
    glLightfv(GL_LIGHT3, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT3, GL_AMBIENT, lightAmbient0);
    glEnable(GL_LIGHTING); //启用光源
    glEnable(GL_LIGHT0);   //使用指定灯光
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_COLOR_MATERIAL);
    glLightModeli( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

}

void drawFloor() {
    glUseProgram(program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floor_tid);
    glUniform1i(glGetUniformLocation(program, "kd"), 0);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    int zLen = 800;
    int xLen = 800;
    float precision = 1.0f;
    float zMax = zLen * precision/2;
    float xMax = xLen * precision/2;
    for (int z = 0; z < zLen; z++) {
        float zStart = zMax - z*precision;
        for (int x = 0; x < xLen; x++) {
            float xStart = x*precision - xMax;
            glVertex3f(xStart, -100.0f, zStart);
            glVertex3f(xStart + precision, -100.0f, zStart);
            glVertex3f(xStart + precision, -100.0f, zStart - precision);
            glVertex3f(xStart, -100.0f, zStart - precision);
            glTexCoord2f(x*1.0f/xLen, z*1.0f/zLen);
        }
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawWall(){
    glUseProgram(program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wall_tid);
    glUniform1i(glGetUniformLocation(program, "kd"), 0);
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    int yLen = 500;
    int zLen = 800;
    float precision = 1.0f;
    float zMax = zLen * precision/2;
    float yMax = yLen * precision/2;
    for (int z = 0; z < zLen; z++) {
        float zStart = zMax - z*precision;
        for (int y = 0; y < yLen; y++) {
            float yStart = y*precision - 100;
            glVertex3f(-zMax, yStart, zStart);
            glVertex3f(-zMax, yStart + precision, zStart);
            glVertex3f(-zMax, yStart + precision, zStart - precision);
            glVertex3f(-zMax, yStart, zStart - precision);
            glTexCoord2f(z*1.0f/zLen, y*1.0f/yLen);
        }
    }
    for (int z = 0; z < zLen; z++) {
        float zStart = zMax - z*precision;
        for (int y = 0; y < yLen; y++) {
            float yStart = y*precision - 100;
            glVertex3f(zMax, yStart, zStart);
            glVertex3f(zMax, yStart + precision, zStart);
            glVertex3f(zMax, yStart + precision, zStart - precision);
            glVertex3f(zMax, yStart, zStart - precision);
            glTexCoord2f(z*1.0f/zLen, y*1.0f/yLen);
        }
    }
    for (int z = 0; z < zLen; z++) {
        float zStart = zMax - z*precision;
        for (int y = 0; y < yLen; y++) {
            float yStart = y*precision - 100;
            glVertex3f(zStart, yStart, -zMax);
            glVertex3f(zStart, yStart + precision, -zMax);
            glVertex3f(zStart - precision, yStart + precision, -zMax);
            glVertex3f(zStart - precision, yStart, -zMax);
            glTexCoord2f(z*1.0f/zLen, y*1.0f/yLen);
        }
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

class Point
{
public:
    Point(){};
    Point(double a,double b,double c):x(a),y(b),z(c){};
public:
    double x;
    double y;
    double z;
};

Point getNormal(Point p1, Point p2, Point p3){
    double a = ( (p2.y-p1.y)*(p3.z-p1.z)-(p2.z-p1.z)*(p3.y-p1.y) );

    double b = ( (p2.z-p1.z)*(p3.x-p1.x)-(p2.x-p1.x)*(p3.z-p1.z) );

    double c = ( (p2.x-p1.x)*(p3.y-p1.y)-(p2.y-p1.y)*(p3.x-p1.x) );

    return Point(a,b,c);
}

void drawReferenceBias(float xP, float xN, float zP, float zN){
    glPushMatrix();
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    for (int x = -30; x < 0; x++){
        glColor4ub(200, 200, 200, 255);
        glVertex3f(x, -5.0f, -xN);
        glVertex3f(x+1, -5.0f, -xN);
        glVertex3f(x+1, -5.0f, xN);
        glVertex3f(x, -5.0f, xN);
    }
    for (int x = 0; x < 30; x++){
        glColor4ub(200, 200, 200, 255);
        glVertex3f(x, -5.0f, -xP);
        glVertex3f(x+1, -5.0f,  -xP);
        glVertex3f(x+1, -5.0f, xP);
        glVertex3f(x, -5.0f, xP);
    }
    for (int z = -30; z < 0; z++){
        glColor4ub(0,0,0, 255);
        glVertex3f(-zN, -5.0f, z);
        glVertex3f(-zN, -5.0f, z+1);
        glVertex3f(zN, -5.0f, z+1);
        glVertex3f(zN, -5.0f, z);
    }
    for (int z = 0; z < 30; z++){
        glColor4ub(0,0,0, 255);
        glVertex3f(-zP, -5.0f, z);
        glVertex3f(-zP, -5.0f, z+1);
        glVertex3f(zP, -5.0f, z+1);
        glVertex3f(zP, -5.0f, z);
    }
    glEnd();
    glPopMatrix();
}

void drawReflection(float x, float y, float z, int size){
    glPushMatrix();
    glColor4ub(0,0,0, 155);
    glTranslatef(x, -90, z);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    for (int i = -size; i<size;i++){
        for (int j=-size; j<size;j++){
            glVertex3f(i, -9, j);
            glVertex3f(i+1, -9, j);
            glVertex3f(i+1, -9, j+1);
            glVertex3f(i, -9, j+1);
        }
    }
    glEnd();
    glPopMatrix();
}

void drawAttackTrack(float x1, float y1, float z1, float x2, float y2, float z2){
    glPushMatrix();
    glColor4f(178/256.0, 34/256.0, 34/256.0,0.5);
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glEnd();
    glPopMatrix();
}

void drawWing(){
    glPushMatrix();
    glRotated(motion.wingAngle, 0, 1, 0);
    glScalef(0.5f, 0.5f, 0.5f);
    objModel1.Draw();
    objModel2.Draw();
    objModel3.Draw();
    objModel4.Draw();
    glPopMatrix();

}

void drawTarget(bool mode){
    glPushMatrix();
    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT3);
    if (mode && android.mode == 1){android.forward();}

    glTranslatef(android.x, android.y, android.z);
    glRotated(android.angle, 0, 1, 0);
    glScalef(200, 200, 200);
    objModelAndroidBody.Draw();

    glPushMatrix();
    glRotated(android.angleLeftFoot, 1, 0, 0);
    objModelAndroidLeftFoot.Draw();
    glPopMatrix();

    glPushMatrix();
    glRotated(android.angleRightFoot, 1, 0, 0);
    objModelAndroidRightFoot.Draw();
    glPopMatrix();

    glPushMatrix();
    glRotated(android.angleLeftHand, 1, 0, 0);
    objModelAndroidLeftHand.Draw();
    glPopMatrix();

    glPushMatrix();
    glRotated(android.angleRightHand, 1, 0, 0);
    objModelAndroidRightHand.Draw();
    glPopMatrix();

    if (android.mode != -2) {
        objModelAndroidHead.Draw();
    }
    else {
        if (android.bias <= 10) {
            android.bias += 0.2;
        }
        objModelAndroidHead.DrawWithBias(android.bias*0.1);
    }
    glDisable(GL_LIGHT3);
    glEnable(GL_LIGHT0);
    glPopMatrix();

    drawReflection(android.x, android.y, android.z, 10);

//    if (mode) {
//        camera->chaseTarget(android.x, android.z, android.angle);
//        camera2->backChaseTarget(android.x, android.z, android.angle);
//    }
}

bool drawAnti(bool mode){
    bool flag = false;
    float tmp_d;
    glPushMatrix();
    if (mode) {
        if (motion.state == 6) {
            tmp_d = sqrt((anti0.x-motion.x)*(anti0.x-motion.x)+(anti0.y-motion.y)*(anti0.y-motion.y)
                    +(anti0.z-motion.z)*(anti0.z-motion.z));
            if (tmp_d <= 80) drawAttackTrack(anti0.x, anti0.y, anti0.z, motion.x, motion.y, motion.z);
            if (anti0.move(motion.x, motion.y, motion.z, android.x, android.y + 40, android.z, android.v, 0, 0, 0)) {
                motion.captured();
            }
        } else if (motion.state == -2) {
            anti0.done();
        } else {
            anti0.still(android.x, android.y + 40, android.z);
        }
    }
    drawReflection(anti0.x, anti0.y, anti0.z, 5);
    glTranslatef(anti0.x, anti0.y ,anti0.z);
    glColor3f(anti0.colorX, anti0.colorY, anti0.colorZ);
    objModel5.Draw();
    glPopMatrix();
    glPushMatrix();
    if (mode) {
        if (motion.state == 6) {
            tmp_d = sqrt((anti1.x-motion.x)*(anti1.x-motion.x)+(anti1.y-motion.y)*(anti1.y-motion.y)
                         +(anti1.z-motion.z)*(anti1.z-motion.z));
            if (tmp_d <= 80) drawAttackTrack(anti1.x, anti1.y, anti1.z, motion.x, motion.y, motion.z);
            if (anti1.move(motion.x, motion.y, motion.z, android.x, android.y + 40, android.z, android.v, 0, 0, 20)) {
                motion.captured();
            }
        } else if (motion.state == -2) {
            anti1.done();
        } else {
            anti1.still(android.x, android.y + 40, android.z);
        }
    }
    drawReflection(anti1.x, anti1.y, anti1.z, 5);
    glTranslatef(anti1.x, anti1.y ,anti1.z);
    glColor3f(anti1.colorX, anti1.colorY, anti1.colorZ);
    objModel5.Draw();
    glPopMatrix();
    glPushMatrix();
    if (mode) {
        if (motion.state == 6) {
            tmp_d = sqrt((anti2.x-motion.x)*(anti2.x-motion.x)+(anti2.y-motion.y)*(anti2.y-motion.y)
                         +(anti2.z-motion.z)*(anti2.z-motion.z));
            if (tmp_d <= 80) drawAttackTrack(anti2.x, anti2.y, anti2.z, motion.x, motion.y, motion.z);
            if (anti2.move(motion.x, motion.y, motion.z, android.x, android.y + 40, android.z, android.v, 0, 0, -20)) {
                motion.captured();
            }
        } else if (motion.state == -2) {
            anti2.done();
        } else {
            anti2.still(android.x, android.y + 40, android.z);
        }
    }
    drawReflection(anti2.x, anti2.y, anti2.z, 5);
    glTranslatef(anti2.x, anti2.y ,anti2.z);
    glColor3f(anti2.colorX, anti2.colorY, anti2.colorZ);
    objModel5.Draw();
    glPopMatrix();
    glPushMatrix();
    if (mode) {
        if (motion.state == 6) {
            tmp_d = sqrt((anti3.x-motion.x)*(anti3.x-motion.x)+(anti3.y-motion.y)*(anti3.y-motion.y)
                         +(anti3.z-motion.z)*(anti3.z-motion.z));
            if (tmp_d <= 80) drawAttackTrack(anti3.x, anti3.y, anti3.z, motion.x, motion.y, motion.z);
            if (anti3.move(motion.x, motion.y, motion.z, android.x, android.y + 40, android.z, android.v, 20, 0, 0)) {
                motion.captured();
            }
        } else if (motion.state == -2) {
            anti3.done();
        } else {
            anti3.still(android.x, android.y + 40, android.z);
        }
    }
    drawReflection(anti3.x, anti3.y, anti3.z, 5);
    glTranslatef(anti3.x, anti3.y ,anti3.z);
    glColor3f(anti3.colorX, anti3.colorY, anti3.colorZ);
    objModel5.Draw();
    glPopMatrix();
    glPushMatrix();
    if (mode) {
        if (motion.state == 6) {
            tmp_d = sqrt((anti4.x-motion.x)*(anti4.x-motion.x)+(anti4.y-motion.y)*(anti4.y-motion.y)
                         +(anti4.z-motion.z)*(anti4.z-motion.z));
            if (tmp_d <= 80) drawAttackTrack(anti4.x, anti4.y, anti4.z, motion.x, motion.y, motion.z);
            if (anti4.move(motion.x, motion.y, motion.z, android.x, android.y + 40, android.z, android.v, -20, 0, 0)) {
                motion.captured();
            }
        } else if (motion.state == -2) {
            anti4.done();
        } else {
            anti4.still(android.x, android.y + 40, android.z);
        }
    }
    drawReflection(anti4.x, anti4.y, anti4.z, 5);
    glTranslatef(anti4.x, anti4.y ,anti4.z);
    glColor3f(anti4.colorX, anti4.colorY, anti4.colorZ);
    objModel5.Draw();
    glPopMatrix();
    return flag;
}

void drawFKR(bool mode){
    glPushMatrix();
    if (mode) {
        if (motion.mode == true) {
            motion.random();
        }
        if (motion.state == 0) {
            motion.pending();
        } else if (motion.state == 1) {
            motion.xPositiveLift();
        } else if (motion.state == 2) {
            motion.xNegativeLift();
        } else if (motion.state == 3) {
            motion.zPositiveLift();
        } else if (motion.state == 4) {
            motion.zNegativeLift();
        } else if (motion.state == 5) {
            motion.clockwiseRotation();
        } else if (motion.state == 6) {
            drawAttackTrack(motion.x, motion.y, motion.z, android.x, android.y + 30, android.z);
            vector<Vector3d> vvd;
            if (daodan->state == 0){
                vvd.push_back(Vector3d(daodan->x, daodan->y, daodan->z));
            }
            if (motion.forward(android.x, android.y + 30, android.z, anti0.x, anti0.y, anti0.z, vvd)){
                android.captured();
            }
        } else {
            motion.recover();
        }
        motion.move();
    }
    drawReflection(motion.x, motion.y, motion.z, 10);
    //  glRotated(motion.angleY, 0, 1, 0);
    glTranslatef(motion.x, motion.y, motion.z);
    if (mode) {
        if (motion.state == 1) {
            drawReferenceBias(1, 3, 1, 1);
            motion.state = -1;
        } else if (motion.state == 2) {
            drawReferenceBias(3, 1, 1, 1);
            motion.state = -1;
        } else if (motion.state == 3) {
            drawReferenceBias(1, 1, 1, 3);
            motion.state = -1;
        } else if (motion.state == 4) {
            drawReferenceBias(1, 1, 3, 1);
            motion.state = -1;
        }
    }
    // glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    if (mode) {
        if (motion.state != -2){
            drawWing();
        }
    }
    glRotated(motion.angleX, 1, 0, 0);
    glRotated(motion.angleZ, 0, 0, 1);
    glPushMatrix();
    glScalef(0.5f, 0.5f, 0.5f);
    if (motion.state != -2) {
        objModel.Draw();
    }
    else {
        if (motion.bias<50){
            motion.bias += 0.5;
            objModel.DrawWithBias(motion.bias);
        }
    }
    glPopMatrix();
    glPopMatrix();
}

void drawDaoDan(bool mode){
    glPushMatrix();
    if (mode) {
        daodan->trace(motion.x, motion.y, motion.z);
        if (daodan->state == 1 && motion.state != -2){
            cout << daodan->state << endl;
            motion.captured();
        }
    }
    glTranslatef(daodan->x, daodan->y, daodan->z);
    glRotated(daodan->angleY, 0, 1, 0);
    glRotated(daodan->angleX, 1, 0, 0);
    if (daodan->state == 0){
        objModelDaoDan.Draw();
    }
    else if (daodan->state == 1){
        daodan->bias += 1;
        objModelDaoDan.DrawWithBias(daodan->bias);
    }
    else if (daodan->state == -1){
        daodan->bias += 1;
        objModelDaoDan.DrawWithBias(daodan->bias);
    }
    glPopMatrix();
}

void targetFrontView(bool mode){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera->setModelViewMatrix();

    glPushMatrix();
    glScalef(1.5f, 1.5f, 1.5f);
    objModel6.Draw();   //scene
    glPopMatrix();

    drawFKR(mode);
    drawTarget(mode);
    drawAnti(mode);
}

void targetBackView(bool mode){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera2->setModelViewMatrix();

    glPushMatrix();
    objModel6.Draw();   //scene
    glPopMatrix();

    drawTarget(mode);
    drawFKR(mode);
    drawAnti(mode);
}

void fkrFrontView(bool mode){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera3->setModelViewMatrix();

    glPushMatrix();
    objModel6.Draw();   //scene
    glPopMatrix();

    drawTarget(mode);
    drawFKR(mode);
    drawAnti(mode);
}

void daoDanFrontView(bool mode){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera4->setModelViewMatrix();

    glPushMatrix();
    objModel6.Draw();   //scene
    glPopMatrix();

    if (daodan->bias < 30){
        drawDaoDan(mode);
    }
    drawTarget(mode);
    drawFKR(mode);
    drawAnti(mode);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_TEXTURE_2D);

    int mode = 0;
    if (motion.state == 6){
        int d = sqrt((android.x-motion.x)*(android.x-motion.x)+
                (android.y-motion.y)*(android.y-motion.y)+(android.z-motion.z)*(android.z-motion.z));
        if (d < 120) mode = 2;
        else mode = 1;
        window_change = 0;
    }
    else {
        mode = 0;
    }

    if (window_change == 1){
        mode = 1;
    }

    glViewport(0, 0, window_width/3*2, window_height);
    if (daodan->bias < 5){
        daoDanFrontView(true);
    }
    else {
        if (mode == 2) {
            Vector3d v1(motion.x-android.x, motion.y-android.y+40, motion.z-android.z);
            Vector3d v2(sin(c*android.angle), 0, cos(c*android.angle));
            if (v1.dot(v2) >=0) targetFrontView(true);
            else targetBackView(true);
        }
        else if (mode == 0){
            targetFrontView(true);
        }else {
            fkrFrontView(true);
        }
    }

    camera->chaseTarget(android.x, android.z, android.angle);
    camera2->backChaseTarget(android.x, android.z, android.angle);
    camera3->reverseChaseTarget(android.x, android.y-10, android.z, motion.x, motion.y+30, motion.z);
    if (daodan->bias < 30){
        camera4->chaseTargetWithBias(motion.x, motion.y, motion.z, daodan->x, daodan->y, daodan->z, 75);
    }

    if (window_mode == 1) {
        glViewport(window_width / 3 * 2, window_height / 3 * 2, window_width / 3, window_height / 3);
        targetBackView(false);
    }

    glViewport(window_width/3*2, window_height/3, window_width/3, window_height/3);
    if (mode == 0 || mode == 2) {
        fkrFrontView(false);
    }
    else {
        targetFrontView(false);
    }

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width/3*2, height/3*2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    camera->setShape(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 3000.0f);
    glMatrixMode(GL_MODELVIEW);
    glViewport(width/3*2, height/3*2, width/3, height/3);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    camera2->setShape(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 3000.0f);
    glMatrixMode(GL_MODELVIEW);
    glViewport(width/3*2, height/3, width/3, height/3);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    camera3->setShape(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 3000.0f);
    glMatrixMode(GL_MODELVIEW);
    window_width = width;
    window_height = height;

}

void RotateX(float angle)
{
    float d=camera->getDist();
    int cnt=100;
    float theta=angle/cnt;
    float slide_d=-2*d*sin(theta*3.14159265/360);
    camera->yaw(theta/2);
    for(;cnt!=0;--cnt)
    {
        camera->slide(slide_d,0,0);
        camera->yaw(theta);
    }
    camera->yaw(-theta/2);
}

void RotateY(float angle)
{
    float d = camera->getDist();
    int cnt=100;
    float theta=angle/cnt;
    float slide_d=2*d*sin(theta*3.14159265/360);
    camera->pitch(theta/2);
    for(;cnt!=0;--cnt)
    {
        camera->slide(0,slide_d,0);
        camera->pitch(theta);
    }
    camera->pitch(-theta/2);
}

void mouseMove(int button, int state, int x, int y)
{
    now = clock();
    cout << (double)(now-start)/CLOCKS_PER_SEC << ":mousemove (" << x << "," << y << ")" << endl;
    start = now;
    if (state == GLUT_DOWN) {
        if (button==GLUT_LEFT_BUTTON){
          //  RotateX(x-oldPosX);
          //  RotateY(y-oldPosY);
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            //camera->roll(dx);
            //  camera->slide(-(x-oldPosX),y-oldPosY,0);
        }
        oldPosX = x; oldPosY = y;
    }
    else if (state == GLUT_UP){
        if (button == GLUT_MIDDLE_BUTTON){
            camera->slide(-(x-oldPosX),y-oldPosY,0);
        }
    }

}

void changeViewPoint(int x, int y)
{
    now = clock();
    cout << (double)(now-start)/CLOCKS_PER_SEC << ":changeviewpoint (" << x << "," << y << ")" << endl;
    start = now;
    int temp = x - oldPosX;
    degree -= temp;
    android.changeDirection(degree);
    //camera->yaw(temp);
   // camera->slide(5*cos(c*degree), 0, 5*sin(c*degree));
    oldPosX = x;
    oldPosY = y;
}

void myIdle()
{
    glutPostRedisplay();
}

void thread1(){
    sleep(5);
    mouseMove(GLUT_LEFT_BUTTON, GLUT_DOWN, 338, 609);
    sleep(2);
    changeViewPoint(371, 602);
    sleep(2);
    changeViewPoint(400, 602);
    sleep(2);
    changeViewPoint(415, 602);
    sleep(2);
    changeViewPoint(434, 601);
    sleep(2);
    changeViewPoint(445, 603);
    sleep(2);
    mouseMove(GLUT_LEFT_BUTTON, GLUT_DOWN, 445, 603);
    sleep(5);
    android.setForward();
    sleep(2);
    mouseMove(GLUT_LEFT_BUTTON, GLUT_DOWN, 445, 603);
    sleep(2);
    changeViewPoint(437, 603);
    sleep(2);
    changeViewPoint(421, 602);
    sleep(2);
    changeViewPoint(408, 601);
    sleep(2);
    mouseMove(GLUT_LEFT_BUTTON, GLUT_DOWN, 407, 600);
    sleep(10);
    motion.up();
    sleep(10);
    motion.down();
    sleep(2);
    changeViewPoint(435, 603);
    sleep(2);
    changeViewPoint(549, 609);
    sleep(2);
    changeViewPoint(563, 610);
    sleep(2);
    changeViewPoint(574, 612);
    sleep(2);
    changeViewPoint(566,613);
    sleep(2);
    motion.state = 6;
    sleep(2);
    mouseMove(GLUT_LEFT_BUTTON, GLUT_DOWN, 563, 613);
    sleep(10);
    android.setStill();
}

void keyPressed(unsigned char key, int x, int y) {
    if (key == 'a'){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":a" << endl;
        start = now;
        motion.state = 1;
    }
    if (key == 'd'){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":d" << endl;
        start = now;
        motion.state = 2;
    }
    if (key == 'w'){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":w" << endl;
        start = now;
        motion.state = 4;
    }
    if (key == 's'){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":s" << endl;
        start = now;
        motion.state = 3;
    }
    if (key == 'z'){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":z" << endl;
        start = now;
        motion.up();
    }
    if (key == 'x'){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":x" << endl;
        start = now;
        motion.down();
    }
    if (key == 'v'){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":c" << endl;
        start = now;
        motion.state = 0;
    }
    if (key == 'r'){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":r" << endl;
        start = now;
        motion.state = 5;
    }
    if (key == 'f'){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":f" << endl;
        start = now;
        motion.state = 6;
    }
    if (key == 'b'){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":b" << endl;
        start = now;
        motion.restart();
        android.setStill();
        anti0.restart(android.x, android.y+40, android.z);
        anti1.restart(android.x, android.y+40, android.z);
        anti2.restart(android.x, android.y+40, android.z);
        anti3.restart(android.x, android.y+40, android.z);
        anti4.restart(android.x, android.y+40, android.z);
        daodan = new Daodan();
    }
    if (key == 'i'){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":i" << endl;
        start = now;
        motion.mode = !motion.mode;
        if (motion.mode == false){
            motion.state = 0;
        }
    }
    if (key == '['){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":[" << endl;
        start = now;
        camera->slide(-5, 0, 0);
    }
    if (key == ']'){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":]" << endl;
        start = now;
        camera->slide(5, 0, 0);
    }
    if (key == '-'){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":-" << endl;
        start = now;
        camera->slide(0, 0, -5);
    }
    if (key == '='){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":=" << endl;
        start = now;
        camera->slide(0, 0, 5);
    }
    if (key == 'm'){
        thread task(thread1);
        task.join();
    }
    if (key == 'o'){
        window_mode = 1- window_mode;
    }
    if (key == 'c'){
        window_change = 1 - window_change;
    }
    if (key == 'e'){
        if (daodan->state != 0){
            int d = sqrt((android.x-motion.x)*(android.x-motion.x)+
                         (android.y-motion.y)*(android.y-motion.y)+(android.z-motion.z)*(android.z-motion.z));
            if (d > 80) daodan = new Daodan(android.x, android.y+40, android.z);
        }
    }
}

void specialKeyPressed(int key, int x, int y) {
    if (key == GLUT_KEY_UP){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":up" << endl;
        start = now;
        android.setForward();
    }
    else if (key == GLUT_KEY_DOWN){
        now = clock();
        cout << (double)(now-start)/CLOCKS_PER_SEC << ":down" << endl;
        start = now;
        android.setStill();
    }
}

void init() {
    start = clock();
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 2000);
    glutCreateWindow("FKR");
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    setLightRes();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    char buffer[MAXPATH];
    getcwd(buffer, MAXPATH);
    root = string(buffer);
    root_path = root + root_path;
    root_path2 = root + root_path2;
    root_path3 = root + root_path3;
    root_path4 = root + root_path4;
    root_path5 = root + root_path5;
    cout << root_path << endl;
    cout << root_path2 << endl;
    cout << root_path3 << endl;
    cout << root_path4 << endl;
    cout << root_path5 << endl;
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();
    glAttachShader(program, frag);
    objModel.load(file, root_path, program);
    objModel1.load(file1, root_path, program);
    objModel2.load(file2, root_path, program);
    objModel3.load(file3, root_path, program);
    objModel4.load(file4, root_path, program);
    objModel5.load(file5, root_path2, program);
    objModel6.load(file6, root_path3, program);
    objModelAndroidBody.load(file7, root_path4, program);
    objModelAndroidLeftFoot.load(file8, root_path4, program);
    objModelAndroidRightFoot.load(file9, root_path4, program);
    objModelAndroidLeftHand.load(file10, root_path4, program);
    objModelAndroidRightHand.load(file11, root_path4, program);
    objModelAndroidHead.load(file12, root_path4, program);
    objModelDaoDan.load(file13, root_path5, program);
    TextureImage ti,ti2,ti3;
    loadTGA(&ti, (char*)(root+floor_texture).c_str(), 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glGenerateMipmap(GL_TEXTURE_2D);
    loadTGA(&ti2, (char*)(root+wall_texture).c_str(), 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glGenerateMipmap(GL_TEXTURE_2D);
    loadTGA(&ti3, (char*)(root+station_texture).c_str(), 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glGenerateMipmap(GL_TEXTURE_2D);
    floor_tid = ti.texID;
    wall_tid = ti2.texID;
    station_tid = ti3.texID;
    camera = new Camera(pos, target, up);
    camera2 = new Camera(pos2, target2, up2);
    camera3 = new Camera(pos, target, up);
    camera3->camera_close = 60;
    camera4 = new Camera(pos, target, up);
    camera4->camera_close = 30;
    window_mode = 0;
    window_change = 0;
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseMove);
    glutMotionFunc(changeViewPoint);
    glutKeyboardFunc(keyPressed);
    glutSpecialFunc(specialKeyPressed);
    glutIdleFunc(myIdle);
    glutMainLoop();
    return 0;
}
