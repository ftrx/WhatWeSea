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
#include "general.h"

class trxObject : public ofVec3f {
    
    
public:
    bool isActive;
    int myActiveConnection;
    string title;
    ofVec3f position;
    ofVec3f unprojectedPosition;
    float rotation;
    int id;
    ofImage * myIcon;
    ofColor myColor;
    
    vector<int> myConnections;

    
    trxObject(float _x, float _y, float _z, int _id);
    
    void update();
    void draw();
    void drawAsConnection();
    void drawInfo();

    int returnID();
};