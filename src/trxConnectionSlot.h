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
private:
    static int number;
    int id;
    
public:
    trxConnectionSlot();
    trxConnectionSlot(trxFlock * _myFlock);
    
    trxFlock  * myFlock;
    trxConverter * myConverter;
    
    bool state = false;
    int inactiveFlockPosition = 0;
    int inactiveConverterPosition = 0;
    bool isConnectionActive();
    
    void connect(trxConverter * _myConverter);
    void deletConnection();
    
    void draw();
    void drawPossibleConnection(int flockPosition, int converterPosition);
    void drawWobbleLine(float _x1, float _y1,float _x2, float _y2);

    float	signedNoiseData[400];
    int		nSignedNoiseData = 400;
    int radialNoiseCursor = 0.0;
};