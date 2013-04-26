//
//  trxConnectionSlot.cpp
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 25.04.13.
//
//

#include "trxConnectionSlot.h"
trxConnectionSlot::trxConnectionSlot()
{
    
}

trxConnectionSlot::trxConnectionSlot(trxFlock * _myFlock)
{
    myFlock = _myFlock;
    
}

bool trxConnectionSlot::isConnectionActive(){
    return state;
}

void trxConnectionSlot::connect(trxConverter * _myConverter){
    
    if(!_myConverter->myActiveConnection && !myFlock->myActiveConnection)
    {
        myConverter = _myConverter;
        myConverter->myActiveConnection = this;
        myFlock->hasConnection = true;
        state = true;
    }
    
}

void trxConnectionSlot::deletConnection(){
    if(state){
        myFlock->myActiveConnection = NULL;
        myConverter->myActiveConnection = NULL;
        myConverter = NULL;
        state = false;
    }
}


void trxConnectionSlot::draw()
{
    if (state) {
        ofPushStyle();
        ofSetColor(255, 255, 255,100);
        ofLine(myFlock->position.x, myFlock->position.y, myConverter->position.x, myConverter->position.y);
        ofPopStyle();
    }
    
    
}