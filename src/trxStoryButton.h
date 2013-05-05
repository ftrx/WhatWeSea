//
//  trxStoryButton.h
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 01.05.13.
//
//

#pragma once

#include <iostream>
#include "ofMain.h"


class trxStoryButton {
public:
    trxStoryButton();
    trxStoryButton(ofVec2f pos, float w, float h, string text);
    
    bool state = false;
    
    ofVec2f position;
    float height;
    float width;
    float border = 10.0;
    string text;
    
    ofTrueTypeFont HelveticaNeueRoman18;
    
    void draw();
    bool isOverButton(ofVec2f cursorPos);
    bool clickOverButton(ofVec2f cursorPos);
    
};