//
//  trxObjectHandler.h
//  WhatWeSea
//
//  Created by Fabian Troxler on 24.04.13.
//
//

#pragma once

#include "ofMain.h"
#include "general.h"
#include "trxFlock.h"
#include "trxConverter.h"
#include "trxHarvester.h"
#include "trxConnectionSlot.h"
#include "trxXML.h"
#include "ofxTuio.h"
#include "trxStoryHandler.h"
#include "ofxOsc.h"

class trxObjectHandler {
    
public:
    
    bool threadStarted=false;
    
    trxObjectHandler();
    void setup();
    void update();
    void draw();
    void draw3D();
    void drawInfo();
    
    //XML
    void initXML();
    void generateObjects();
    
    
    vector<trxVehicle *> getAllBoidsFromFlocks(const vector<trxFlock * >& _myFlocks);
    void drawAllBoids();

    void catchBoid(trxHarvester * _myHarverster);
    
    void catchLongLineBoid(trxHarvester * _myHarverster);
    
    
    void addObject(ofxTuioObject & tuioObject);
    void updateObject(ofxTuioObject & tuioObject);
    void removeObject(ofxTuioObject & tuioObject);
    
    void addCursor(ofxTuioCursor & tuioCursor);
    void updateCursor(ofxTuioCursor & tuioCursor);
    void removeCursor(ofxTuioCursor & tuioCursor);
    
    
    void updateSessionID(long _oldSessionID,long _sessionID);
    
    //Variables
    
    trxXML xml;
    
    bool debug = false;
    
    int startTime;
    int timeStamp;
    int maxID = 11;
    
    bool firstCatch;
    
    vector<ofTexture> textures;
    vector<ofImage> icons;
    
    
    vector<trxFlock *> myFlocks;
    vector<trxConverter *> myConverters;
    
    vector<trxHarvester* > harvesters;
    vector<trxFlock *> activeFlocks;
    vector<trxConverter *> activeConverters;
    
    vector<trxVehicle *> allMyBoids;
    vector<trxVehicle *> myPredators;
    vector<trxVehicle *> myPrey;
    
    vector<trxConnectionSlot*> myConnections;
    trxConnectionSlot * myActiveConnection = NULL;
    trxConnectionSlot * myLastActiveConnection = NULL;
    
    //Grafix
    ofVbo vbo;
    ofCamera * myCamera;
    
    
    void updateAllVertexes();
    
    void drawAllVertexes();
    
    
    bool checkActiveSlots = false;
    bool checkStillActiveSlots= false;
    
    void checkIfActiveSlot();
    void checkIfStillActiveSlot();
    
    ofVec2f getCorrectedPosition(float _x, float _y);
    ofVec3f screenPosition(ofVec3f _position, ofCamera * cam);
    
    trxFlock* getFlockWithID(int ID);
    trxConverter* getConverterWithID(int ID);
    
    // StoryHandler
    trxStoryHandler myStoryHandler;
    bool closeMessage = false;
    
    bool isIdAnCatch(int _id, vector<int> * _catchIDs);
    
    int newPredatorCounter;
    void generatePredators();
    
    int newPreyCounter;
    void randomPrey();
};