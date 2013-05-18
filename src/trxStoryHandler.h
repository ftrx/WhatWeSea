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
#include "trxOscController.h"
#include "trxFloatingMessageController.h"


class trxStoryHandler {
    
    public:
    struct task {
        bool finished = false;
        int no = NULL;
        string taskMessage = "Story Comming Soon";
        string finalMessage = "Task Finished Message";
        string type = "standard";
        int percent = 100;
        int catchID = NULL;
        int bycatchID = NULL;
        int catchSize = 10.0;
        float targetSize = 10.0;
        ofVec3f targetPosition = ofVec3f(200,0,0);
        int    quantity = NULL;
        bool dieAfterCatch = false;
        string progress = "circle";
        
    };
    
    struct story {
        bool finished = false;
        int topicNumber = NULL;
        vector<task> myTasks;
        trxConnectionSlot * myConnection;
        trxFlock * myStoryFlock;
        trxConverter * myStoryConverter;
        string finalMessage = "Story Finished Message";
        string description = "Story Comming soon";
        vector<string> messages;
    };
    
    
    vector<trxFlock *> finishedFlocks;

    ofTrueTypeFont	HelveticaNeueRoman18;
    ofTrueTypeFont	HelveticaNeueRoman36;
    
    trxConnectionSlot * activeConnection = NULL;
    trxFlock * activeFlock;
    trxFlock * activeBycatchFlock;
    trxConverter * activeConverter;
    
    vector<trxFlock> * allFlocks;
    vector<trxConverter> * allConverters;
    vector<trxConnectionSlot> * allConnectionSlots;
    
    ofVec3f myTargetPosition;
    ofVec3f myScreenTargetPosition;
    ofVec3f myLastScreenTargetPosition;
    ofVec3f myScreenTargetMovement;
    
    int catchedQuantity;
    int tempCatchedQuantity;
    int bycatchQuantity;
    int tempBycatchQuantity;
    
    bool showMessage = false;
    trxStoryButton messageButton;
    
    trxStoryHandler();
    void setup(vector<trxFlock> * _allFLocks,vector<trxConverter> * _allConverters, vector<trxConnectionSlot> * _allConnections);
    void generateStories();
    
    void startStory(trxConnectionSlot * _activeConnection);
    void stopStory();
    void draw();
    void draw3D();
    void drawDebug();
    void update();
    
    void updateMyLastTargetScreenPosition();
    
    task * nextTask();
    void finishTask();
    
    void finishStory();
    
    void resetStoryAfterTimeout (int _time);
    
    int idleTimer;
    
    void drawProgressBar(int _currentQuantity);
    void drawProgressCircle(float _radius, float _barHeight,int _currentQuantity, int _numberOfSectors);
    
    void drawProgressAmount(float _radius, float _barHeight,int _currentQuantity);
    void drawTaskMessage(string _message);
    void drawTarget();
    
    
    // Task and Story Messages
    ofImage message_edge_topright;
    ofImage message_edge_topleft;
    ofImage message_edge_bottomright;
    ofImage message_edge_bottomleft;
    ofImage message_line_left;
    ofImage message_line_top;
    ofImage message_line_right;
    ofImage message_line_bottom;
    void drawMessage(string _message);
    void closeMessage();
    
    string wrapString(string text, int width);

    
    void updateTargetPosition ();
    
    void changeAction(int _actionNumber);
    void changeTopic(int _topicNumber);
    int activeTopicNumber;
        
    vector<story> myStories;
    
    trxXML xml;
    
    story * myActiveStory;
    task * myActiveTask;
    
    bool runningAction = false;
    
    
    trxFlock* getFlockWithID(int ID);
    trxConverter* getConverterWithID(int ID);
    
    trxConnectionSlot* getConnectionSlotWithID(trxFlock * _flock, trxConverter * _converter);
    story * getStoryWithConnection(trxConnectionSlot * _connection);
    
    trxOscController myOsc;
    
    
    ofImage ship;
    
    int messageTimer =0;
    trxFloatingMessageController myFloatingMessageController;
    
    string randomFact;


};