//
//  trxFlockUpdater.h
//  WhatWeSea
//
//  Created by Fabian Troxler on 08.05.13.
//
//

#pragma once

#include "ofMain.h"
#include "trxFlock.h"


class trxFlockUpdater : public ofThread {
    
public:
    trxFlock* flock;
    
    // the thread function
    void threadedFunction() {
        
        // start
        
        while(isThreadRunning()) {
            
            //flock->update();
        }
        
        // done
    }
    

};