//
//  trxFloatingMessageController.h
//  WhatWeSea
//
//  Created by Fabian Troxler on 09.05.13.
//
//

#pragma once

#include "ofMain.h"
#include "trxFloatingMessage.h"
#include "trxXML.h"

class trxFloatingMessageController {
public:
    
    struct topic {
        string description;
        string type;
        int flock;
        int converter;
        int topicNumber;
        vector<trxFloatingMessage> messages;
    };
    
    vector<topic> myTopics;
    
    vector<trxFloatingMessage>* myActiveTopicMessages;
    vector<trxFloatingMessage> myActiveMessages;
    
    trxXML xml;
    
    trxFloatingMessageController();
    
    void setup();
    void update();
    void draw();
    
    void changeTopic(int _topicNumber);
    void newRandomFact();
    string getRandomFact();
    
    void generateAllMessages();
    
    void createNewMessage();
    void deleteMessage();
    
    void removeInactiveMessages();
    
    
    
};
