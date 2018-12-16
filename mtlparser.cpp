//
// Created by myu on 2018/10/8.
//

#include "mtlparser.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include "tgaparser.h"

using namespace std;

MtlParser::MtlParser(string filename, string rootpath) {
    string line;
    fstream f;
    f.open(filename, ios::in);
    if (!f){
        cout << "file not exists" << endl;
        return;
    }
    string mtlNow;
    while (!f.eof()) {
        getline(f, line);
        vector<string> params;
        string temp_param = "";
        line = line.substr(0, line.length());
        line += " ";
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ' ') {
                params.push_back(temp_param);
                temp_param = "";
            } else {
                temp_param += line[i];
            }
        }
        if (params[0] == "newmtl"){
            Mtl mtl;
            mtls[params[1]] = mtl;
            mtlNow = params[1];
        }
        else if (params[0] == "illum"){
            mtls[mtlNow].illum = atoi(params[1].c_str());
        }
        else if (params[0] == "Kd"){
            vector<float> v;
            v.push_back(atof(params[1].c_str()));
            v.push_back(atof(params[2].c_str()));
            v.push_back(atof(params[3].c_str()));
            mtls[mtlNow].Kd = v;
        }
        else if (params[0] == "Ka"){
            vector<float> v;
            v.push_back(atof(params[1].c_str()));
            v.push_back(atof(params[2].c_str()));
            v.push_back(atof(params[3].c_str()));
            mtls[mtlNow].Ka = v;
        }
        else if (params[0] == "Ks"){
            vector<float> v;
            v.push_back(atof(params[1].c_str()));
            v.push_back(atof(params[2].c_str()));
            v.push_back(atof(params[3].c_str()));
            mtls[mtlNow].Ks = v;
        }
        else if (params[0] == "map_Kd"){
            mtls[mtlNow].map_Kd = params[1];
            if (textures.find(params[1]) == textures.end()){
                TextureImage ti;
                loadTGA(&ti, (char*)(rootpath+params[1]).c_str(), 0);
                glBindTexture(GL_TEXTURE_2D, 0);
                glGenerateMipmap(GL_TEXTURE_2D);
                textures[params[1]] = ti.texID;
            }
        }
        else if (params[0] == "map_Ka"){
            mtls[mtlNow].map_Ka = params[1];
            if (textures.find(params[1]) == textures.end()){
                TextureImage ti;
                loadTGA(&ti, (char*)(rootpath+params[1]).c_str(), 1);
                glBindTexture(GL_TEXTURE_2D, 0);
                glGenerateMipmap(GL_TEXTURE_2D);
                textures[params[1]] = ti.texID;
            }
        }
        else if (params[0] == "map_Ks"){
            mtls[mtlNow].map_Ks = params[1];
            if (textures.find(params[1]) == textures.end()){
                TextureImage ti;
                loadTGA(&ti, (char*)(rootpath+params[1]).c_str(), 2);
                glBindTexture(GL_TEXTURE_2D, 0);
                glGenerateMipmap(GL_TEXTURE_2D);
                textures[params[1]] = ti.texID;
            }
        }
        else{
            //cout << "Not implemented: " << params[0] << endl;
        }
    }
}