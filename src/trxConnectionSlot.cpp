//
//  trxConnectionSlot.cpp
//  BoidsAndTUIO
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

void trxConnectionSlot::connect(trxConverter * _myConverter){
    
    if(!state)
    {
        myConverter = _myConverter;
        //myFlock->hasConnection = true;
        state = true;
    }
    
}

void trxConnectionSlot::deletConnection(){
    if(state){
        myConverter = NULL;
        state = false;
    }
}


void trxConnectionSlot::draw()
{
    ofPushStyle();
    ofSetLineWidth(2.0);
    ofSetColor(255, 255, 255,255);
    drawWobbleLine(myFlock->position.x, myFlock->position.y, myConverter->position.x, myConverter->position.y);
    //ofLine(myFlock->position.x, myFlock->position.y, myConverter->position.x, myConverter->position.y);
    ofPopStyle();
}

void trxConnectionSlot::drawPossibleConnection(int flockPosition, int converterPosition)
{
    ofPushStyle();

    float dist = 200.00;
    

    float maxAmplitude = 5.0;  

    if(myFlock->isActive)
    {
        
        ofVec3f randomWiggle = ofVec3f(ofSignedNoise(ofGetElapsedTimef()+id),ofSignedNoise(ofGetElapsedTimef()+2+2*id),0)*maxAmplitude;
        ofPushMatrix();
        ofTranslate(myFlock->position.x,myFlock->position.y,0);
        ofTranslate(randomWiggle);
        ofRotate(myFlock->rotation+90.0*flockPosition);
        //ofLine(0,0, dist, 0);
        drawWobbleLine(0,0, dist, 0);
        myConverter->myIcon->draw(dist-40, -40, 80,80);
        ofPopMatrix();
    }
    if(myConverter->isActive)
    {
        ofVec3f randomWiggle = ofVec3f(ofSignedNoise(ofGetElapsedTimef()+id),ofSignedNoise(ofGetElapsedTimef()+2+2*id),0)*maxAmplitude;
        ofPushMatrix();
        ofTranslate(myConverter->position.x,myConverter->position.y,0);
        ofTranslate(randomWiggle);
        ofRotate(myConverter->rotation+90.0*converterPosition);
        drawWobbleLine(0,0, dist, 0);
        myFlock->myIcon->draw(dist-40, -40, 80,80);
        
        ofPopMatrix();
    }

    ofPopStyle();
}

void trxConnectionSlot::drawWobbleLine(float _x1, float _y1,float _x2, float _y2){
    float dist = ofDist(_x1, _y1, _x2, _y2);
    float amplitude = 20.0;
    float rotate = ofVec2f(_x1,_y1).angle(ofVec2f(_x2,_y2));
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

