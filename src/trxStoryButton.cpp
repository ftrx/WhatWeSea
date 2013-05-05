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
    
    position = _pos;
    width = _w;
    height = _h;
    text = _text;
    
    ofRectangle bounds = HelveticaNeueRoman18.getStringBoundingBox(text, 0, 0);
    width= bounds.width;
    height= bounds.height;
    
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
    
    
    
}
bool trxStoryButton::clickOverButton(ofVec2f cursorPos){
    if (isOverButton(cursorPos)) {
        state = true;
    }
    return state;
}

bool trxStoryButton::isOverButton(ofVec2f cursorPos){
    
    if (cursorPos.x >= position.x && cursorPos.x <= position.x+width+2*border && cursorPos.y >= position.y && cursorPos.y <= position.y+height+2*border) {
        return true;
    }
    else return false;
}