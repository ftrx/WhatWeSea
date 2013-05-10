//
//  trxOscController.cpp
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 06.05.13.
//
//

#include "trxOscController.h"

trxOscController::trxOscController(){
    mySender.setup("localhost", 12000);
    
}

void trxOscController::setup(){
    
    
}



void trxOscController::sendOscTopic(int _topicNumber){
    ofxOscMessage m;
    m.setAddress("/topic");
    m.addIntArg(_topicNumber);
    mySender.sendMessage(m);
    cout<< "OSC sendMessage: /topic" + ofToString(_topicNumber)<<endl;
}

void trxOscController::sendOscAction(int _actionNumber){
    ofxOscMessage m;
    m.setAddress("/action");
    m.addIntArg(_actionNumber);
    mySender.sendMessage(m);
    cout<< "OSC sendMessage: /action" + ofToString(_actionNumber)<<endl;
}