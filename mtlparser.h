//
// Created by myu on 2018/10/8.
//

#ifndef OPENGL_MTLPARSER_H
#define OPENGL_MTLPARSER_H

#endif //OPENGL_MTLPARSER_H

#include <GLUT/GLUT.h>
#include <vector>
#include <string>
#include <map>
using namespace std;

struct Mtl{
    vector<float> Ka;     //阴影色
    vector<float> Kd;     //固有色
    vector<float> Ks;     //高光色
//    vector<float> Ke;     //
//    vector<float> Ns;                    //shininess
    string map_Kd;               //固有纹理贴图
    string map_Ks;               //高光纹理贴图
    string map_Ka;               //阴影纹理贴图
    int illum;
};

class MtlParser{
public :
    map<string, Mtl> mtls;
    map<string, GLuint> textures;
    MtlParser(string filename, string rootpath);
};
