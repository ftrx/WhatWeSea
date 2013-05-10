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
#include "trxObject.h"
#include "general.h"
class trxFlockUpdater;

class trxFlock : public trxObject {

    
public:
    
    int boidNum;
    int startBoidNum;
    int generateNewTime;
    
    
    float maxSpeed;
    float sightDistance;
    float tooCloseDistance;
    float length;
    int numberOfBones;
    
    
    ofVec3f target;

    vector<trxVehicle> boids;
    vector<trxVehicle> deadBoids;
    
    
    vector <trxHarvester> * myHarvesters;
    
    trxFlockUpdater * flockUpdater;
    
    
    ofVbo vbo;
    ofShader shader;
    ofTexture  * texture;

    vector <ofVec3f> points;
    vector <ofVec3f> sizes;

    trxFlock(float _x, float _y, float _z, int _id,vector <trxHarvester>* _harvesters, int _startBoidNum);
    void update();
    void draw();
    void drawCircles();
    void removeDeadBoids();
    void sortDeadsOut();
    void removeVehicles(vector<trxVehicle*> *_v);
    void drawInfo();
    
    void generateBoids();
    void createNewBoid();
    int returnID();
    int countDead();
    int countOnWay();
};