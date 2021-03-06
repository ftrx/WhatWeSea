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
    title = "";
    id = _id;
    
    
    
    
    position = ofVec3f(_x,_y,_z);
    unprojectedPosition = ofVec3f(_x,_y,_z);
    rotation = 0.0;
    myColor = ofColor(255,255,255,255);
    
    ofTrueTypeFont::setGlobalDpi(72);
    
    glow.loadImage("glow.png");
    
	HelveticaNeueRoman18.loadFont("fonts/NewsGot-Med.otf", 24, true, true);
	HelveticaNeueRoman18.setLineHeight(28.0f);
	HelveticaNeueRoman18.setLetterSpacing(1.037);
}


void trxObject::update(){

}

void trxObject::draw(){
    ofPushStyle();
    ofSetCircleResolution(100);
    ofEnableAlphaBlending();
    if(isActive){
        /*
        ofSetColor(255, 255, 255, 50);
        ofCircle(0,0,radius);
         */
        ofSetColor(255, 255, 255,255);
        //ofNoFill();
        
        float pulsingRadius = radius + abs(ofSignedNoise(ofGetElapsedTimef()*.4+randomWiggleInit)* (radius*0.3));
        
        if (isActiveConnection) {
            glow.draw(-pulsingRadius*3,-pulsingRadius*3,pulsingRadius*6,pulsingRadius*6);
        }
        else {
            ofSetColor(255, 255, 255,130);
            glow.draw(-pulsingRadius*3,-pulsingRadius*3,pulsingRadius*6,pulsingRadius*6);
        }
        
        ofSetColor(255, 255, 255,50);
        myIcon->draw(-radius/2.0, -radius/2.0, 0, radius, radius);
        
		//ofRectangle bounds = HelveticaNeueRoman18.getStringBoundingBox(title, 0, 0);
		//HelveticaNeueRoman18.drawString(title, -bounds.width/2.0, 110);
    }
    ofDisableAlphaBlending();
    ofPopStyle();
}



void trxObject::drawInfo(){
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(unprojectedPosition);
	ofSetColor(myColor);
    ofCircle(0, 0, 10);
    
    string info = "Object: "+ title +" "+ ofToString(position)+"\n";
    ofDrawBitmapString(info, 0,0);
    ofPopMatrix();
    ofPopStyle();
}

void trxObject::drawAsConnection(){
    myIcon->draw(-40, -40, 0, 80, 80);

}
int trxObject::returnID(){
    
    return id;
}

ofVec3f trxObject::screenPosition(ofVec3f _position, ofCamera * cam){
    ofVec3f pos= ofVec3f (_position.x,_position.y,0.666666);
    pos = cam->screenToWorld(pos, ofRectangle(0,0,ofGetWidth(),ofGetHeight()));
    return pos;
}



