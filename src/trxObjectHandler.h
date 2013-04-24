//
//  trxObjectHandler.h
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 24.04.13.
//
//

#pragma once

#include "ofMain.h"
#include "general.h"
#include "trxFlock.h"
#include "trxConverter.h"
#include "ofxXmlSettings.h"

class trxObjectHandler {
    
public:
    
    trxObjectHandler();
    void update();
    void draw();
    void drawInfo();
    
    ofxXmlSettings xml;
    
    
    
};