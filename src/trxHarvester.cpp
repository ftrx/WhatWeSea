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
    ofSetColor(255, 255, 255, 100);
    ofCircle(position,radius);
    
    ofSetColor(255, 255, 255);
    ofNoFill();
    ofSetLineWidth(1.0);
    ofSetCircleResolution(100);
    ofCircle(position,radius);
    ofFill();
    ofPopMatrix();
    
    }

void trxHarvester::drawInfo(){
    ofPushMatrix();
    ofTranslate(position);
    
	ofSetColor(color);
    
    string info = "Harvesterposition:"+ofToString(position)+"\n";
    ofDrawBitmapString(info, 0,0);
    ofPopMatrix();
}



void trxHarvester::createNewBoid(){
    
}