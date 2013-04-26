//
//  trxHarvester.cpp
//  ofxBoid_test2
//
//  Created by Fabian Troxler on 01.04.13.
//
//

#include "trxHarvester.h"



trxHarvester::trxHarvester(float _x, float _y, float _z,int _id){
    id = _id;
    position.set(ofVec3f(_x,_y,_z));
    
}

void trxHarvester::update(){
    
       
}

void trxHarvester::draw(){
    
    ofPushMatrix();
    ofPushStyle();
    ofSetCircleResolution(100);
    ofTranslate(position.x,ofGetHeight()-position.y,0);
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255, 100);
    ofCircle(0,0,radius);
    ofSetColor(255, 255, 255);
    ofNoFill();
    ofSetLineWidth(1.0);
    
    ofCircle(0,0,radius);
    ofFill();
    ofDisableAlphaBlending();
    ofPopStyle();
    ofPopMatrix();
    
    }

void trxHarvester::drawInfo(){
    ofPushStyle();
    //ofEnableAlphaBlending();
    ofPushMatrix();
    ofTranslate(unprojectedPosition);
    ofColor(200,200,200,100);
    ofSetLineWidth(2.0);
    ofLine(0, 0, DEPTH-position.z, 0, 0, -DEPTH);
   
    
    ofSetColor(255, 255, 255,50);
    ofSphere(0, 0, 0, radius);
     
    
	ofSetColor(color);
    
    string info = "Harvesterposition:"+ofToString(position)+"\n";
    ofDrawBitmapString(info, 0,0);
    ofPopMatrix();
    //ofDisableAlphaBlending();
    ofPopStyle();
}



void trxHarvester::createNewBoid(){
    
}

void trxHarvester::clearCatch(){
    for (int i=0; i<myCatch.size(); i++) {
        trxVehicle * boid = myCatch.at(i);
        boid->caught = false;
        boid->clearTargets(); /// !!! fast target eraser, but delets all targets
    }
    myCatch.clear();
}

ofVec3f trxHarvester::screenPosition(ofCamera * cam){
    ofVec3f hPos= ofVec3f (position.x,ofGetHeight()-position.y,0.370);
    hPos = cam->screenToWorld(hPos, ofRectangle(0,0,ofGetWidth(),ofGetHeight()));
    return hPos;
}
