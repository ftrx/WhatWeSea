//
//  trxStoryHandler.h
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 29.04.13.
//
//

#pragma once

#include <iostream>
#include "ofMain.h"
#include "general.h"

#include "trxConnectionSlot.h"
#include "trxFlock.h"
#include "trxConverter.h"
#include "trxXML.h"


class trxStoryHandler {
    
    public:
    ofTrueTypeFont	HelveticaNeueRoman18;
    ofTrueTypeFont	HelveticaNeueRoman36;
    
    trxConnectionSlot * activeConnection = NULL;
    trxFlock * activeFlock;
    
    vector<trxFlock> * allFlocks;
    vector<trxConverter> * allConverters;
    vector<trxConnectionSlot> * allConnectionSlots;
    
    int catchedQuantity;
    
    trxStoryHandler();
    trxStoryHandler(vector<trxFlock> * _allFLocks,vector<trxConverter> * _allConverters, vector<trxConnectionSlot> * _allConnections);
    void startStory(trxConnectionSlot * _activeConnection);
    void stopStory();
    void draw();
    void update();
    
    void generateStories();
    
    
    
    void drawProgressBar(int _currentQuantity);
    void drawTaskMessage(string _message);
    void drawTarget();
    
    void drawMessage(string _message);
    
    struct task {
        bool finished = false;
        string taskMessage = "Fange Thunfische";
        int catchID;
        float targetSize;
        int    quantity;
        
    };
    
    struct story {
        vector<task> myTasks;
        trxConnectionSlot * myConnection;
        trxFlock * myStoryFlock;
        trxConverter * myStoryConverter;
        string finalMessage;
    };
    
    vector<story> myStories;
    
    trxXML xml;
    
    story * myActiveStory;
    task * myActiveTask;
    
    
    trxFlock* getFlockWithID(int ID);
    trxConverter* getConverterWithID(int ID);
    
    trxConnectionSlot* getConnectionSlotWithID(trxFlock * _flock, trxConverter * _converter);
    story * getStoryWithConnection(trxConnectionSlot * _connection);
    
};