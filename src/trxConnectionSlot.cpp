//
//  trxConnectionSlot.cpp
//  WhatWeSea
//
//  Created by Fabian Troxler on 25.04.13.
//
//

#include "trxConnectionSlot.h"
int trxConnectionSlot::number = 0;
trxConnectionSlot::trxConnectionSlot()
{
    id = number++;
}



bool trxConnectionSlot::isConnectionActive(){
    return state;
}

void trxConnectionSlot::activate(){
    state = true;
    myConverter->isActiveConnection = true;
    myFlock->isActiveConnection = true;
}

void trxConnectionSlot::deactivate(){
    state = false;
    myConverter->isActiveConnection = false;
    myFlock->isActiveConnection = false;
}

void trxConnectionSlot::connect(trxConverter * _myConverter){
    
    if(!state)
    {
        myConverter = _myConverter;
        //myFlock->hasConnection = true;
        state = true;
        myConverter->isActiveConnection = true;
        myFlock->isActiveConnection = true;
    }
    
}

void trxConnectionSlot::deletConnection(){
    if(state){
        myConverter->isActiveConnection = false;
        myFlock->isActiveConnection = false;
        myConverter = NULL;
        state = false;
    }
}


void trxConnectionSlot::draw()
{
    ofPushStyle();
    ofSetLineWidth(2.0);
    ofSetColor(255, 255, 255,255);
    
    ofVec2f p1 = ofVec2f(myFlock->position.x,myFlock->position.y);
    ofVec2f p2 = ofVec2f(myConverter->position.x,myConverter->position.y);
    
    //float angle = ofDegToRad(p1.angle(p2));
    float angle = atan2(p1.y - p2.y, p1.x - p2.x);
    float offsset1x = cos(angle)*myFlock->radius;
    float offsset1y = sin(angle)*myFlock->radius;
    float offsset2x = cos(angle)*myConverter->radius;
    float offsset2y = sin(angle)*myConverter->radius;

    drawWobbleLine(myFlock->position.x-offsset1x, myFlock->position.y-offsset1y, myConverter->position.x+offsset2x, myConverter->position.y+offsset2y);
    //ofLine(myFlock->position.x, myFlock->position.y, myConverter->position.x, myConverter->position.y);
    ofPopStyle();
}

void trxConnectionSlot::drawPossibleConnection(string _type, int _position)
{
    ofPushStyle();

    float dist = 200.00;
    

    float maxAmplitude = 5.0;  

    if(_type == "flock")
    {
        
        ofVec3f randomWiggle = ofVec3f(ofSignedNoise(ofGetElapsedTimef()+id),ofSignedNoise(ofGetElapsedTimef()+2+2*id),0)*maxAmplitude;
        ofPushMatrix();
        ofTranslate(myFlock->position.x,myFlock->position.y,0);
        ofTranslate(randomWiggle);
        ofRotate(myFlock->rotation+90.0*_position);
        //ofLine(0,0, dist, 0);
        drawWobbleLine(myFlock->radius,0, dist-40, 0);
        ofTranslate(dist, 0);
        ofRotate(-90.0*_position);
        myConverter->myIcon->draw(-40, -40, 80,80);
        ofPopMatrix();
    }
    if(_type == "converter")
    {
        ofVec3f randomWiggle = ofVec3f(ofSignedNoise(ofGetElapsedTimef()+id),ofSignedNoise(ofGetElapsedTimef()+2+2*id),0)*maxAmplitude;
        ofPushMatrix();
        ofTranslate(myConverter->position.x,myConverter->position.y,0);
        ofTranslate(randomWiggle);
        ofRotate(myConverter->rotation+90.0*_position);
        drawWobbleLine(myConverter->radius,0, dist-40, 0);
        ofTranslate(dist, 0);
        ofRotate(-90.0*_position);
        myFlock->myIcon->draw(-40, -40, 80,80);
        
        ofPopMatrix();
    }

    ofPopStyle();
}

void trxConnectionSlot::drawWobbleLine(float _x1, float _y1,float _x2, float _y2){
    float dist = ofDist(_x1, _y1, _x2, _y2);
    float amplitude = 20.0;
    //float rotate = ofVec2f(_x1,_y1).angle(ofVec2f(_x2,_y2));
    ofVec2f middle = ofVec2f(_x1,_y1).middle(ofVec2f(_x2,_y2));
    ofVec2f lineVector = ofVec3f(_x2-_x1,_y2-_y1);
    ofVec3f normal = lineVector.getPerpendicular();
    
    ofVec2f bezierPoint = middle+normal*ofSignedNoise(ofGetElapsedTimef()+id)*amplitude;
    
    ofPolyline wigglyPolyLine;

    wigglyPolyLine.quadBezierTo(_x1, _y1, bezierPoint.x, bezierPoint.y, _x2, _y2);
    
    
    ofPushStyle();
    //ofSetColor(255, 255, 255);
    ofEnableSmoothing();
    ofNoFill();
    ofPushMatrix();
	wigglyPolyLine.draw();
	ofPopMatrix();
    ofPopStyle();
    
}

