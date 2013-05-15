//
//  trxFloatingMessage.h
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 07.05.13.
//
//

#pragma once

#include "ofMain.h"
#include "general.h"


class trxFloatingMessage {
    
public:
    string text;
    bool active = false;
    ofVec3f position = ofVec3f(0,0,0);
    ofVec3f velocity = ofVec3f(0,0,2.0);

    
    int textSize = 36;
    float targetHight = DEPTH-400.0;
    float endHight = DEPTH-200.0;
    ofColor color = ofColor(255,255,255,0);
    ofTrueTypeFont font;
    
    ofRectangle bounds;
    
    trxFloatingMessage();
    void update();

    void draw();
    void setPosition(ofVec3f _position);
    void setText(string _text);
    
    string wrapString(string text, int width);
    
};
