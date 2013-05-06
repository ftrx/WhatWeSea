//
//  trxOscController.h
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 06.05.13.
//
//

#pragma once

#include "ofxOsc.h"
#include "ofMain.h"
#include "general.h"

class trxOscController {
    public:
    ofxOscSender mySender;
    
    trxOscController();
    void setup();

    
    
    
    void sendOscTopic(int _topicNumber);
    void sendOscAction(int _actionNumber);

    
    


};