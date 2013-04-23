//
//  general.h
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 18.04.13.
//
//


#pragma once
const int VERTEXNUMBER = 6;
const int DEPTH = 600;

const float SCREEN_X_MIN = 0.1;
const float SCREEN_X_MAX = 0.1;
const float SCREEN_Y_MIN = 0.1;
const float SCREEN_Y_MAX = 0.1;


struct drawBoidReturn {
    ofVec3f verts[VERTEXNUMBER];
    ofIndexType faces[9];
};

