//
//  trxConnectionSlot.h
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 25.04.13.
//
//
#pragma once

#include "ofMain.h"
#include "general.h"
#include "trxConnectionSlot.h"
#include "trxConverter.h"
#include "trxFlock.h"


class trxConnectionSlot {
    
public:
    trxConnectionSlot();
    trxConnectionSlot(trxFlock * _myFlock);
    
    trxFlock  * myFlock;
    trxConverter * myConverter;
    
    bool state = false;
    
    bool isConnectionActive();
    
    void connect(trxConverter * _myConverter);
    void deletConnection();
    
    void draw();
    
    
    
    
};