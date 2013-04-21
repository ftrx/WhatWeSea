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
#include "trxVehicle.h"
#include "general.h"

class trxFlock : public ofVec3f {

    
public:
    int boidNum;
    int startBoidNum;
    int generateNewTime;
    float maxSpeed;
    ofVec3f target;
    ofVec3f position;
    int id;
    vector<trxVehicle> boids;
    
    vector <trxHarvester> * myHarvesters;
    ofCamera * myCamera;
    ofColor color;
    
    ofVbo vbo;
    ofShader shader;
    ofImage myImage;
    ofTexture  * texture;

    vector <ofVec3f> points;
    vector <ofVec3f> sizes;

    trxFlock(float _x, float _y, float _z, int _id,vector <trxHarvester>* _harvesters, int _startBoidNum);
    void update();
    void draw();
    void drawCircles();
    void removeDeathVehicle(trxVehicle* _v);
    void drawInfo();
    void createNewBoid();
    int returnID();
};