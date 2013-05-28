//
//  trxStoryHint.h
//  WhatWeSea
//
//  Created by Fabian Troxler on 21.05.13.
//
//
#include "ofMain.h"


#pragma once

class trxStoryHint {
public:
    
    trxStoryHint();
    void update();
    void draw();
    void setPosition(ofVec2f _position);
    
    bool active = false;
    ofImage tapIcon;
    
    float radius;
    float startRadius = 20;
    float fadeInDistance = 5;
    float maxRadius = 30;
    int lineWidth = 2;
    float timeStamp = 0;
    float speed = .3f;
    ofColor color = ofColor(255,255,255);
    int alpha = 255;
    ofVec2f position;
    
    
    
};