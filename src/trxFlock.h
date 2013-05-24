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
    
    int maxBoidNum;
    int startBoidNum;
    int generateNewTime;
    
    
    float maxSpeed;
    float sightDistance;
    float tooCloseDistance;
    float length;
    int numberOfBones;
    
    bool fleeFromHarvester = true;
    float fleeSpeed = 8.0;
    
    ofVec3f target;

    vector<trxVehicle*> boids;
    vector<trxVehicle*> deadBoids;
    
    //growBack Settings
    bool growBack = false;
    int growBackTime = 0;
    int growBackTimestamp = 0;
    
    // Jellfish Settings
    bool isJellyFish = false;
    float contractScale = 0.5;
    int contractSpeed = 200;
    int expandSpeed = 100;
    
    
    //trxFlockUpdater * flockUpdater;
    
    
    ofVbo vbo;
    ofShader shader;
    ofTexture  * texture;


    int timeStamp = 0;
    
    trxFlock(float _x, float _y, float _z, int _id, int _startBoidNum);
    void update();
    void draw();
    void drawCircles();
    void drawInfo();
    
    void removeDeadBoids();
    void removeFirstVehicle();
    
    void freeCatchedBoids();
    void sortDeadsOut();
    void removeVehicles(vector<trxVehicle*> _v);
    
    int countDead();
    int countOnWay();
    
    void generateBoids();

    void createNewBoid(float _x,float _y, float _z);
    
    int returnID();

};