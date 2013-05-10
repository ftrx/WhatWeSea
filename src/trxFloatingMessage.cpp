//
//  trxFloatingMessage.cpp
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 07.05.13.
//
//

#include "trxFloatingMessage.h"


trxFloatingMessage::trxFloatingMessage(){
    ofTrueTypeFont::setGlobalDpi(72);
	font.loadFont("fonts/NewsGot-Reg.otf", 48, true, true);
	font.setLineHeight(28.0f);
	font.setLetterSpacing(1.037);
    text = "";
}


void trxFloatingMessage::update(){
    if (position.z < endHight) {
        position+=velocity;
        
        if (position.z < targetHight) {
            color.a = ofMap(position.z, 0, targetHight, 0, 200);
        }
        else{
            color.a = ofMap(position.z, targetHight, endHight, 200, 0);
        }
    }
    
    if (position.z >= endHight) {
        color.a = 0;
        active = false;
    }
}

void trxFloatingMessage::draw(){
    ofPushMatrix();
    ofTranslate(position);
    ofPushStyle();
    ofSetColor(color);
    font.drawString(text, -bounds.width/2.0, -bounds.height/2.0);
    ofPopStyle();
    ofPopMatrix();
}


void trxFloatingMessage::setText(string _text){
    text = _text;
    bounds = font.getStringBoundingBox(text, 0, 0);
}

void trxFloatingMessage::setPosition(ofVec3f _position){
    position = _position;
}


