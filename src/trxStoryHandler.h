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
#include "trxStoryButton.h"


class trxStoryHandler {
    
    public:
    struct task {
        bool finished = false;
        int no = NULL;
        string taskMessage = "Story Comming Soon";
        string finalMessage = "Task Finished Message";
        int catchID = NULL;
        int catchSize = 10.0;
        float targetSize = 10.0;
        ofVec3f targetPosition = ofVec3f(200,0,0);
        int    quantity = NULL;
        
    };
    
    struct story {
        bool finished = false;
        vector<task> myTasks;
        trxConnectionSlot * myConnection;
        trxFlock * myStoryFlock;
        trxConverter * myStoryConverter;
        string finalMessage = "Story Finished Message";
        string description = "Story Comming soon";
    };

    
    
    ofTrueTypeFont	HelveticaNeueRoman18;
    ofTrueTypeFont	HelveticaNeueRoman36;
    
    trxConnectionSlot * activeConnection = NULL;
    trxFlock * activeFlock;
    trxConverter * activeConverter;
    
    vector<trxFlock> * allFlocks;
    vector<trxConverter> * allConverters;
    vector<trxConnectionSlot> * allConnectionSlots;
    
    int catchedQuantity;
    
    bool showMessage = false;
    trxStoryButton messageButton;
    
    trxStoryHandler();
    trxStoryHandler(vector<trxFlock> * _allFLocks,vector<trxConverter> * _allConverters, vector<trxConnectionSlot> * _allConnections);
    void generateStories();
    
    void startStory(trxConnectionSlot * _activeConnection);
    void stopStory();
    void draw();
    void update();
    
    
    task * nextTask();
    void finishTask();
    
    void finishStory();
    
    void resetStoryAfterTimeout (int _time);
   
    
    int idleTimer;
    
    
    void drawProgressBar(int _currentQuantity);
    void drawTaskMessage(string _message);
    void drawTarget();
    
    void drawMessage(string _message);
    void closeMessage();
    
    
    
    
        
    vector<story> myStories;
    
    trxXML xml;
    
    story * myActiveStory;
    task * myActiveTask;
    
    
    trxFlock* getFlockWithID(int ID);
    trxConverter* getConverterWithID(int ID);
    
    trxConnectionSlot* getConnectionSlotWithID(trxFlock * _flock, trxConverter * _converter);
    story * getStoryWithConnection(trxConnectionSlot * _connection);
    
};