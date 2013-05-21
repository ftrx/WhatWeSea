//
//  trxStoryHint.cpp
//  WhatWeSea
//
//  Created by Fabian Troxler on 21.05.13.
//
//

#include "trxStoryHint.h"


trxStoryHint::trxStoryHint(){
    timeStamp = ofGetElapsedTimef();
    radius = startRadius;
    color = ofColor(255,255,255,255);
    tapIcon.loadImage("gestures/tap.png");
}

void trxStoryHint::update(){
    if (radius < maxRadius) {
        radius += speed;
    }
    else {
        radius = startRadius;
    }
}

void trxStoryHint::draw(){
    float width = startRadius;
    float height = (width/tapIcon.width)*tapIcon.height;
    
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(position.x, position.y);
    
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255,150);
    ofSetLineWidth(lineWidth);
    ofSetCircleResolution(40);
    //tapIcon.draw(-width*(3.2/8.0), -height*(1.0/8.0),width,height);
    //tapIcon.draw(-width/2, -height/2,width,height);
    ofFill();
    ofCircle(0, 0, 4);
    
    ofNoFill();
     //ofCircle(0, 0, 8);
    if (radius > maxRadius) {
        radius = maxRadius;
    }
    if (radius <= startRadius+fadeInDistance) {
        color.a = ofMap(radius, startRadius, startRadius+fadeInDistance, 0, 200);
    }
    else{
        color.a = ofMap(radius, startRadius+fadeInDistance, maxRadius, 200, 0);
    }
    
    ofSetColor(color);
   
    
    ofCircle(0, 0, radius);
    
    ofDisableAlphaBlending();
    ofPopMatrix();
    ofPopStyle();
    
}

void trxStoryHint::setPosition(ofVec2f _position){
    position = _position;
}