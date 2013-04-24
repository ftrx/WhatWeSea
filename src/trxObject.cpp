//
//  trxObject
//  
//
//  Created by Fabian Troxler on 01.04.13.
//
//

#include "trxObject.h"


trxObject::trxObject(float _x, float _y, float _z, int _id){
    isActive = false;
    myActiveConnection = NULL;
    title = "";
    id = _id;
    position = ofVec3f(_x,_y,_z);
    unprojectedPosition = ofVec3f(_x,_y,_z);
    rotation = 0.0;
    myColor = ofColor(255,255,255,255);
}


void trxObject::update(){
    
}

void trxObject::draw(){
    ofPushStyle();
    ofSetCircleResolution(100);
    
    
    ofEnableAlphaBlending();
    
    if(isActive){
        ofSetColor(255, 255, 255, 50);
        ofCircle(0,0,80);
        ofSetColor(255, 255, 255);
        ofNoFill();
        ofSetLineWidth(3.0);
        ofCircle(0,0,80);
        ofFill();
        myIcon->draw(-40, -40, 0, 80, 80);
        ofDrawBitmapString(title, 80,80);
    }
    ofDisableAlphaBlending();
    ofPopStyle();
}



void trxObject::drawInfo(){
    ofPushMatrix();
    ofTranslate(position.x,position.y,position.z);
	ofSetColor(myColor);
    ofCircle(0, 0, 10);
    
    string info = "Object: "+ title +" "+ ofToString(position)+"\n";
    ofDrawBitmapString(info, 0,0);
    ofPopMatrix();
}

void trxObject::drawAsConnection(){
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255, 50);
    myIcon->draw(-40, -40, 0, 80, 80);
    ofDisableAlphaBlending();
    ofPopStyle();
}
int trxObject::returnID(){
    
    return id;
}