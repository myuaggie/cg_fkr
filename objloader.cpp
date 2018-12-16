//
//  objloader.cpp
//  try1
//
//  Created by myu on 2018/10/3.
//  Copyright © 2018年 myu. All rights reserved.
//

#include "objloader.hpp"
#include <fstream>
#include <iostream>
#include <math.h>

using namespace std;


ObjLoader::ObjLoader(){
}

void ObjLoader::load(string filename, string rootpath, int prog)
{
    cout << "load " << filename << endl;
    program = prog;
    string line;
    fstream f;
    rootPath = rootpath;
    f.open(rootpath + filename, ios::in);
    if (!f){
        cout << "file not exists" << endl;
        return;
    }
    while (!f.eof()){
        getline(f, line);
        vector<string> params;
        string temp_param = "";
        line = line.substr(0, line.length());
        line += " ";
        for (int i = 0; i < line.length(); i++){
            if (line[i] == ' '){
                params.push_back(temp_param);
                temp_param = "";
            }
            else {
                temp_param += line[i];
            }
        }
        if (params[0] == "v"){
            if (params.size() != 4){
                cout << "incorrect obj format" << endl;
            }
            vector<GLfloat> vertex;
            vertex.push_back(atof(params[1].c_str()));
            vertex.push_back(atof(params[2].c_str()));
            vertex.push_back(atof(params[3].c_str()));
            vSets.push_back(vertex);
        }
        else if (params[0] == "f"){
            vector<GLint> vertex;
            vector<GLint> texture;
            vector<GLint> normal;
            for (int i = 1; i < params.size(); i++){
                string param = params[i];
                int idx = 0;
                param += "/";
                temp_param = "";
                for (int j = 0; j < param.length(); j++){
                    if (param[j] == '/'){
                        if (idx == 0){
                            vertex.push_back(atoi(temp_param.c_str()));
                        }
                        else if (idx == 1){
                            texture.push_back(atoi(temp_param.c_str()));
                        }
                        else if (idx == 2){
                            normal.push_back(atoi(temp_param.c_str()));
                        }
                        idx ++;
                        temp_param = "";
                    }
                    else {
                        temp_param += param[j];
                    }
                }
            }
            // not support texture and normal yet.
            vector<vector<GLint> > fVector;
            fVector.push_back(vertex);
            if (texture.size() > 0) fVector.push_back(texture);
            if (normal.size() > 0)  fVector.push_back(normal);
            fSets[fSets.size()-1].push_back(fVector);
        }
        else if (params[0] == "vt"){
            if (params.size() != 3){
                cout << "incorrect obj format" << endl;
            }
            vector<GLfloat> vertex;
            vertex.push_back(atof(params[1].c_str()));
            vertex.push_back(atof(params[2].c_str()));
            vtSets.push_back(vertex);
        }
        else if (params[0] == "vn"){
            if (params.size() != 4){
                cout << "incorrect obj format" << endl;
            }
            vector<GLfloat> vertex;
            vertex.push_back(atof(params[1].c_str()));
            vertex.push_back(atof(params[2].c_str()));
            vertex.push_back(atof(params[3].c_str()));
            vnSets.push_back(vertex);
        }
        else if (params[0] == "s"){

        }
        else if (params[0] == "g"){

        }
        else if (params[0] == "usemtl"){
            vector<vector<vector<GLint> > > v;
            fSets.push_back(v);
            mtlSets.push_back(params[1]);
        }
        else if (params[0] == "mtllib"){
            MtlParser mtlParser = MtlParser(rootPath+params[1], rootPath);
            mtlmaps = mtlParser.mtls;
            textures = mtlParser.textures;
        }
        else {
            //cout << "not implemented: " << params[0] << endl;
        }
    }
    f.close();
}

void ObjLoader::Draw(){
    for (int i = 0; i < fSets.size(); i++) {
        Mtl tmp = mtlmaps[mtlSets[i]];
        if (tmp.Ka.size() == 3) {
            float a[4];
            a[0] = tmp.Ka[0];
            a[1] = tmp.Ka[1];
            a[2] = tmp.Ka[2];
            a[3] = 1;
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, a);
        }
        else if (tmp.Kd.size() == 3) {
            float d[4];
            d[0] = tmp.Kd[0];
            d[1] = tmp.Kd[1];
            d[2] = tmp.Kd[2];
            d[3] = 1;
            glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, d);
        }
        else if (tmp.Ks.size() == 3) {
            float s[4];
            s[0] = tmp.Ks[0];
            s[1] = tmp.Ks[1];
            s[2] = tmp.Ks[2];
            s[3] = 1;
            glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, s);
        }
        float shininess[1];
        shininess[0] = 100;
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
//        float emission[4];
//        shininess[0] = 1;shininess[1] = 1;shininess[2] = 1;shininess[3] = 1;
//        glMaterialfv(GL_FRONT,GL_EMISSION,emission);
        glUseProgram(program);
        if (tmp.map_Kd != "") {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[tmp.map_Kd]);
            glUniform1i(glGetUniformLocation(program, "kd"), 0);
        }
        if (tmp.map_Ka != ""){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textures[tmp.map_Ka]);
            glUniform1i(glGetUniformLocation(program, "ka"), 1);
        }
        if (tmp.map_Ks != ""){
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, textures[tmp.map_Ks]);
            glUniform1i(glGetUniformLocation(program, "ks"), 2);
        }

        for (int k = 0; k < fSets[i].size() ; k++) {
            glBegin(GL_POLYGON);
            for (int j = 0; j < fSets[i][k][0].size(); j++) {
                if (fSets[i][k][2][j] > 0) { //normal
                    vector<GLfloat> n = vnSets[fSets[i][k][2][j] - 1];
                    glNormal3f(n[0], n[1], n[2]);
                }
                if (fSets[i][k][1][j] > 0) { //texture
                    vector<GLfloat> t = vtSets[fSets[i][k][1][j] - 1];
                    glTexCoord2f(t[0], t[1]);
                }
                vector<GLfloat> v = vSets[fSets[i][k][0][j] - 1];
                glVertex3f(v[0], v[1], v[2]);
            }
            glEnd();
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void ObjLoader::DrawWithBias(float bias) {
    for (int i = 0; i < fSets.size(); i++) {
        Mtl tmp = mtlmaps[mtlSets[i]];
        if (tmp.Ka.size() == 3) {
            float a[4];
            a[0] = tmp.Ka[0];
            a[1] = tmp.Ka[1];
            a[2] = tmp.Ka[2];
            a[3] = 1;
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, a);
        }
        else if (tmp.Kd.size() == 3) {
            float d[4];
            d[0] = tmp.Kd[0];
            d[1] = tmp.Kd[1];
            d[2] = tmp.Kd[2];
            d[3] = 1;
            glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, d);
        }
        else if (tmp.Ks.size() == 3) {
            float s[4];
            s[0] = tmp.Ks[0];
            s[1] = tmp.Ks[1];
            s[2] = tmp.Ks[2];
            s[3] = 1;
            glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, s);
        }
        float shininess[1];
        shininess[0] = 100;
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
//        float emission[4];
//        shininess[0] = 1;shininess[1] = 1;shininess[2] = 1;shininess[3] = 1;
//        glMaterialfv(GL_FRONT,GL_EMISSION,emission);
        glUseProgram(program);
        if (tmp.map_Kd != "") {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[tmp.map_Kd]);
            glUniform1i(glGetUniformLocation(program, "kd"), 0);
        }
        if (tmp.map_Ka != ""){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textures[tmp.map_Ka]);
            glUniform1i(glGetUniformLocation(program, "ka"), 1);
        }
        if (tmp.map_Ks != ""){
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, textures[tmp.map_Ks]);
            glUniform1i(glGetUniformLocation(program, "ks"), 2);
        }

        for (int k = 0; k < fSets[i].size() ; k++) {
            glPushMatrix();
            vector<GLfloat> face_n = vnSets[fSets[i][k][2][0] - 1];
           // if (bias > 3) glScalef(1.0/bias, 1.0/bias, 1.0/bias);
            glTranslatef(bias*face_n[0]*2, bias*face_n[1]*2, bias*face_n[2]*2);
            glBegin(GL_POLYGON);
            for (int j = 0; j < fSets[i][k][0].size(); j++) {
                if (fSets[i][k][2][j] > 0) { //normal
                    vector<GLfloat> n = vnSets[fSets[i][k][2][j] - 1];
                    glNormal3f(n[0], n[1], n[2]);
                }
                if (fSets[i][k][1][j] > 0) { //texture
                    vector<GLfloat> t = vtSets[fSets[i][k][1][j] - 1];
                    glTexCoord2f(t[0], t[1]);
                }
                vector<GLfloat> v = vSets[fSets[i][k][0][j] - 1];
                glVertex3f(v[0], v[1], v[2]);
            }
            glEnd();
            glPopMatrix();
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}