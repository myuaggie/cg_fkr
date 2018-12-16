//
//  objloader.hpp
//  try1
//
//  Created by myu on 2018/10/3.
//  Copyright © 2018年 myu. All rights reserved.
//

#ifndef objloader_hpp
#define objloader_hpp

#include <stdio.h>

#endif /* objloader_hpp */

#include "mtlparser.h"
#include <GLUT/GLUT.h>
#include <vector>
#include <string>
#include <map>

#include <SOIL.h>
using namespace std;

class ObjLoader{
public:
    ObjLoader();
    void load(string filename, string rootpath, int prog);
    void Draw();
    void DrawWithBias(float bias);
private:
    int program;
    string rootPath;
    vector<vector<GLfloat> > vSets;
    vector<vector<vector<vector<GLint> > > > fSets;
    vector<vector<GLfloat> > vtSets;
    vector<vector<GLfloat> > vnSets;
    vector<string> mtlSets;
    map<string, Mtl> mtlmaps;
    map<string, GLuint> textures;
};


