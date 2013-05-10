//
//  general.h
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 18.04.13.
//
//


#pragma once
const int VERTEXNUMBER = 6;
const int DEPTH = 600; // Depth of the virtual water

const int IDLETIME = 90*1000; // Time after which the Story is resetting


#define trx_HOST "localhost"
#define trx_PORT 12000


// OFFSET for the touchtable
/*
const float SCREEN_X_MIN = -0.05;
const float SCREEN_X_MAX = 0.00;
const float SCREEN_Y_MIN = 0.12;
const float SCREEN_Y_MAX = 0.05;
*/
const float SCREEN_X_MIN = -0.05;
const float SCREEN_X_MAX = -0.01;
const float SCREEN_Y_MIN = 0.00;
const float SCREEN_Y_MAX = 0.15;


struct drawBoidReturn {
    ofVec3f verts[VERTEXNUMBER];
    ofIndexType faces[9];
};



