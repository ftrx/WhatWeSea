//
//  trxStoryButton.cpp
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 01.05.13.
//
//

#include "trxStoryButton.h"
trxStoryButton::trxStoryButton(){
    
}

trxStoryButton::trxStoryButton(ofVec2f _pos = ofVec2f(0,0), float _w = 0, float _h = 0, string _text = ""){
    ofTrueTypeFont::setGlobalDpi(72);
	HelveticaNeueRoman18.loadFont("fonts/HelveticaRoman.ttf", 24, true, true);
	HelveticaNeueRoman18.setLineHeight(28.0f);
	HelveticaNeueRoman18.setLetterSpacing(1.037);
    
    
    width = _w;
    height = _h;
    text = _text;
    
    ofRectangle bounds = HelveticaNeueRoman18.getStringBoundingBox(text, 0, 0);
    width= bounds.width;
    height= bounds.height;
    position = ofVec2f(_pos.x-width/2.0-border,_pos.y-height/2.0-border);
    
}

void trxStoryButton::draw(){
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(position);
    ofSetColor(255, 255, 255);
    ofRect(0,0,width+2*border,height+2*border);
    ofSetColor(0, 0, 0);
    HelveticaNeueRoman18.drawString(text, border, height+border);
    ofPopMatrix();
    ofPopStyle();
    
    ofPushStyle();
    //ofSetColor(255, 0, 0);
    //ofNoFill();
    //ofSetLineWidth(1.0);
    //float tolerance = 50.0;
    //ofRect(position.x-tolerance, position.y-tolerance, width+2*border+2*tolerance, height+2*border+2*tolerance);
    ofPopStyle();
    
}
bool trxStoryButton::clickOverButton(ofVec2f cursorPos){
    if (isOverButton(cursorPos)) {
        state = true;
    }
    return state;
}

bool trxStoryButton::isOverButton(ofVec2f cursorPos){
    float tolerance = 50.0;
    
    if (cursorPos.x >= position.x-tolerance && cursorPos.x <= position.x+width+2*border+tolerance && cursorPos.y >= position.y-tolerance && cursorPos.y <= position.y+height+2*border+tolerance) {
        return true;
    }
    else return false;
}