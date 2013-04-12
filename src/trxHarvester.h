//
//  trxHarvester.h
//  ofxBoid_test2
//
//  Created by Fabian Troxler on 01.04.13.
//
//

#pragma once

#include <iostream>
#include "ofMain.h"
//#include "BiologicalVehicle.h"


class trxHarvester: public ofVec3f {
    
    
public:
    int id;
    
    float radius = 100.0f;
    
    ofVec3f target;
    ofVec3f position;
    
    ofColor color;
    
    ofVbo vbo;
    ofShader shader;
    ofImage myImage;
    ofTexture texture;
    
    vector <ofVec3f> points;
    vector <ofVec3f> sizes;
    
    trxHarvester(float _x, float _y, float _z, int _id);
    void update();
    void draw();
    void drawInfo();
    void createNewBoid();
};