//
//  trxFlock.h
//  ofxBoid_test2
//
//  Created by Fabian Troxler on 01.04.13.
//
//

#pragma once

#include <iostream>
#include "ofMain.h"
#include "trxHarvester.h"
#include "BiologicalVehicle.h"

class trxFlock : public ofVec3f {

    
public:
    int boidNum;
    int generateNewTime;
    ofVec3f target;
    ofVec3f position;
    int id;
    vector<BiologicalVehicle> boids;
    
    vector <trxHarvester> * myHarvesters;
    
    ofColor color;
    
    ofVbo vbo;
    ofShader shader;
    ofImage myImage;
    ofTexture  * texture;

    vector <ofVec3f> points;
    vector <ofVec3f> sizes;

    trxFlock(float _x, float _y, float _z, int _id,vector <trxHarvester>* _harvesters);
    void update();
    void draw();
    void drawCircles();
    void removeDeathVehicle(BiologicalVehicle* _v);
    void drawInfo();
    void createNewBoid();
    int returnID();
};