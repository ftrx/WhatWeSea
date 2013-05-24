//
//  trxStoryButton.cpp
//  WhatWeSea
//
//  Created by Fabian Troxler on 01.05.13.
//
//

#include "trxStoryButton.h"

trxStoryButton::trxStoryButton(){
    
}

trxStoryButton::trxStoryButton(ofVec2f _pos = ofVec2f(0,0), float _w = 0, float _h = 0, string _text = "schliessen"){
    ofTrueTypeFont::setGlobalDpi(72);
	HelveticaNeueRoman18.loadFont("fonts/NewsGot-Reg.otf", 24, true, true);
	HelveticaNeueRoman18.setLineHeight(28.0f);
	HelveticaNeueRoman18.setLetterSpacing(1.037);
    
    
    width = _w;
    height = _h;
    text = _text;
    
    bounds = HelveticaNeueRoman18.getStringBoundingBox(text, 0, 0);
    width= bounds.width;
    height= bounds.height;
    position = ofVec2f(_pos.x,_pos.y);
    
}

void trxStoryButton::draw(){
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(position-ofVec2f(width/2.0+border,height/2.0+border));
    ofSetColor(255, 255, 255);
    ofNoFill();
    ofSetLineWidth(1);
    //ofRect(0,0,width+2*border,height+2*border);
    
    helper.roundedRect(0.0f, 0.0f, width+2*border, height+2*border, 10.0f);
    
    ofFill();
    
    HelveticaNeueRoman18.drawString(text, border, height+border);
    ofPopMatrix();
    ofPopStyle();
    
}
bool trxStoryButton::clickOverButton(ofVec2f cursorPos){
    if (isOverButton(cursorPos)) {
        state = true;
    }
    else {
        state = false;
    }
    return state;
}

void trxStoryButton::setPosition(ofVec2f _pos)
{
    position = _pos;
}

bool trxStoryButton::isOverButton(ofVec2f cursorPos){
    float tolerance = 50.0;
    
    if (cursorPos.x >= position.x-tolerance && cursorPos.x <= position.x+width+2*border+tolerance && cursorPos.y >= position.y-tolerance && cursorPos.y <= position.y+height+2*border+tolerance) {
        return true;
    }
    return false;
}