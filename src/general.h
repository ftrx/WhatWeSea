//
//  general.h
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 18.04.13.
//
//


#pragma once
#include "ofMain.h"
const int VERTEXNUMBER = 6;
const int DEPTH = 600; // Depth of the virtual water

const int IDLETIME = 90*1000; // Time after which the Story is resetting

const float DISTANCE = 200; // distance between to tuio.points until its handled as a new event
const float TIMER = 0.200; // time in seconds until the cursor is removed
const float ADDTIMER = 0.100;


const ofColor activColor = ofColor(255,255,255,255);
const ofColor standardColor = ofColor(0,80,160,255);

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



