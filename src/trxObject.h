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
    bool isActiveConnection = false;
    string title;
    int topicNumber = NULL;
    float radius= 40.0;
    ofVec3f position;
    ofVec3f unprojectedPosition;
    float rotation;
    int id;
    ofImage * myIcon;
    ofImage glow;
    ofColor myColor;
    

    float randomWiggleInit = ofRandom(2000);
    
    ofTrueTypeFont	HelveticaNeueRoman18;
    
    vector<int> myConnections;
    
    bool hasConnection = false;
    
    trxObject(float _x, float _y, float _z, int _id);
    
    void update();
    void draw();
    void drawAsConnection();
    void drawInfo();
    void removeDeadBoids();
    
    
    ofVec3f screenPosition(ofVec3f _position,ofCamera * cam);

    int returnID();
};