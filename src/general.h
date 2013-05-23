//
//  general.h
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 18.04.13.
//
//


#pragma once
#include "ofMain.h"

// Harvester (finger)
const float HARVESTER_RADIUS = 100.0f;
const float LONGLINE_RADIUS = 20.0f;

const int VERTEXNUMBER = 6;
const int DEPTH = 600; // Depth of the virtual water

const int IDLETIME = 120*1000; // Time after which the Story is resetting

const float DISTANCE = 200; // distance between to tuio.points until its handled as a new event
const float TIMER = 0.200; // time in seconds until the cursor is removed
const float ADDTIMER = 0.100;


const ofColor activColor = ofColor(255,255,255,255);
const ofColor standardColor = ofColor(6,43,104,255);

#define trx_HOST "172.31.7.147"
#define trx_PORT 12000


// OFFSET for the touchtable
/*
const float SCREEN_X_MIN = -0.05;
const float SCREEN_X_MAX = 0.00;
const float SCREEN_Y_MIN = 0.12;
const float SCREEN_Y_MAX = 0.05;
*/
const float SCREEN_X_MIN = -0.01;
const float SCREEN_X_MAX = -0.03;
const float SCREEN_Y_MIN = 0.03;
const float SCREEN_Y_MAX = 0.13;


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

