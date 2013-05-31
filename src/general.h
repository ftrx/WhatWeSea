//
//  general.h
//  WhatWeSea
//
//  Created by Fabian Troxler on 18.04.13.
//
//


#pragma once
#include "ofMain.h"

// Harvester (finger)
// LONGLINE

const float LONGLINEDISTANCE = 300;
const float HARVESTER_RADIUS = 100.0f;
const float LONGLINE_RADIUS = 20.0f;

const int VERTEXNUMBER = 6;
const int DEPTH = 600; // Depth of the virtual water

const int IDLETIME = 360*1000; // Time after which the Story resets.

const float DISTANCE = 400; // distance between to tuio.points until its handled as a new event
const float TIMER = 0.400; // time in seconds until the cursor is removed
const float ADDTIMER = 0.150;


const float DRAWTASKHINTHEIGHT = 150;


// fish settings
const int MAXFISHES = 1700;

const int BORDERMARGIN = 100;


//fish colors
const ofColor activColor = ofColor(250,255,255,255);
const ofColor standardColor = ofColor(6,43,104,255);

const ofColor catchedColor = ofColor(30,185,129,255);
const ofColor deadColor = ofColor(1,145,87,255);
// progress bar color
const ofColor PROGRESSBARCOLOR = ofColor(6,60,144,255);

const ofColor PROGRESSBARAMOUNTCOLOR = ofColor(2,40,104,255);

const ofColor WINSCREENTEXTCOLOR = ofColor(45,127,219,255);

#define trx_HOST "192.168.1.130"
#define trx_PORT 12000


// OFFSET for the touchtable
/*
const float SCREEN_X_MIN = -0.05;
const float SCREEN_X_MAX = 0.00;
const float SCREEN_Y_MIN = 0.12;
const float SCREEN_Y_MAX = 0.05;
*/
const float SCREEN_X_MIN = -0.00;
const float SCREEN_X_MAX = -0.0;
const float SCREEN_Y_MIN = 0.0;
const float SCREEN_Y_MAX = 0.0;


struct drawBoidReturn {
    ofVec3f verts[VERTEXNUMBER];
    ofIndexType faces[9];
};

class helpers{

    public:
    void quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY) {
        float cp1x = prevX + 2.0/3.0*(cpx - prevX);
        float cp1y = prevY + 2.0/3.0*(cpy - prevY);
        float cp2x = cp1x + (x - prevX)/3.0;
        float cp2y = cp1y + (y - prevY)/3.0;
        
        // finally call cubic Bezier curve function
        ofBezierVertex(cp1x, cp1y, cp2x, cp2y, x, y);
    };

    void roundedRect(float x, float y, float w, float h, float r) {
        ofBeginShape();
        ofVertex(x+r, y);
        ofVertex(x+w-r, y);
        quadraticBezierVertex(x+w, y, x+w, y+r, x+w-r, y);
        ofVertex(x+w, y+h-r);
        quadraticBezierVertex(x+w, y+h, x+w-r, y+h, x+w, y+h-r);
        ofVertex(x+r, y+h);
        quadraticBezierVertex(x, y+h, x, y+h-r, x+r, y+h);
        ofVertex(x, y+r);
        quadraticBezierVertex(x, y, x+r, y, x, y+r);
        ofEndShape();
    };


};

