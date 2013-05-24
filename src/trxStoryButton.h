//
//  trxStoryButton.h
//  WhatWeSea
//
//  Created by Fabian Troxler on 01.05.13.
//
//

#pragma once

#include <iostream>
#include "ofMain.h"
#include "general.h"


class trxStoryButton {
public:
    trxStoryButton();
    trxStoryButton(ofVec2f pos, float w, float h, string text);
    
    bool state = false;
    
    void setPosition(ofVec2f _pos);
    
    ofVec2f position;
    ofRectangle bounds;
    float height;
    float width;
    float border = 15.0;
    string text;
    
    ofTrueTypeFont HelveticaNeueRoman18;
    
    void draw();
    bool isOverButton(ofVec2f cursorPos);
    bool clickOverButton(ofVec2f cursorPos);
    
    
    helpers helper;
};