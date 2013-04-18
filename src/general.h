//
//  general.h
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 18.04.13.
//
//


#pragma once
const int VERTEXNUMBER = 6;

struct drawBoidReturn {
    ofVec3f verts[VERTEXNUMBER];
    ofIndexType faces[9];
};

