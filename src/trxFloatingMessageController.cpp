//
//  trxFloatingMessageController.cpp
//  WhatWeSea
//
//  Created by Fabian Troxler on 09.05.13.
//
//

#include "trxFloatingMessageController.h"


trxFloatingMessageController::trxFloatingMessageController(){
    
}


void trxFloatingMessageController::setup(){
    xml = trxXML("messages.xml");
    xml.setup("messages", "topic");
    generateAllMessages();
}

void trxFloatingMessageController::update(){
    if (myActiveMessages.size() > 0) {
        removeInactiveMessages();
        
        for (int i=0; i<myActiveMessages.size(); i++) {
            myActiveMessages.at(i).update();
        }
    }
}

void trxFloatingMessageController::draw(){
    for (int i=0; i<myActiveMessages.size(); i++) {
        myActiveMessages.at(i).draw();
    }
}


void trxFloatingMessageController::changeTopic(int _topicNumber){
    myActiveTopicMessages = NULL;
    
    for (int i=0; i<myTopics.size(); i++) {
        topic * thisTopic = &myTopics[i];
        if (thisTopic->topicNumber == _topicNumber) {
            myActiveTopicMessages = &thisTopic->messages;
        }
    }
}

void trxFloatingMessageController::newRandomFact(){
    if (myActiveTopicMessages) {
        int numberOfFacts = myActiveTopicMessages->size();
        int factNumber = int(ofRandom(numberOfFacts-1)+0.5);
        
        trxFloatingMessage thisMessage = myActiveTopicMessages->at(factNumber);
        thisMessage.setPosition(ofVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,0));
        thisMessage.active = true;
        myActiveMessages.push_back(thisMessage);

        
    }
}
string trxFloatingMessageController::getRandomFact(){
    if (myActiveTopicMessages) {
        int numberOfFacts = myActiveTopicMessages->size();
        int factNumber = int(ofRandom(numberOfFacts-1)+0.5);
        
        trxFloatingMessage thisMessage = myActiveTopicMessages->at(factNumber);
        thisMessage.setPosition(ofVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,0));
        thisMessage.active = true;
        //myActiveMessages.push_back(thisMessage);
        return thisMessage.text;
        
    }
}


void trxFloatingMessageController::generateAllMessages(){
    
    for (int i=0; i < xml.objectNumber; i++)
    {
        topic thisTopic;
        xml.XML.pushTag("messages");
        xml.XML.pushTag("topic",i);
        thisTopic.description = xml.getString("no Description", "description");
        thisTopic.type = xml.getString("none", "type");
        thisTopic.flock = xml.getIntValue(NULL, "flock");
        thisTopic.converter = xml.getIntValue(NULL, "converter");
        thisTopic.topicNumber = xml.getIntValue(NULL, "topicNumber");
        xml.XML.pushTag("facts");
        int numberOfFacts = xml.XML.getNumTags("fact");
        
        for (int facts=0; facts < numberOfFacts; facts++) {
            trxFloatingMessage thisFact;
            xml.XML.pushTag("fact",facts);
            thisFact.setText(xml.getString("Fact comming soon", "text"));
            xml.XML.popTag();
            thisTopic.messages.push_back(thisFact);
        }
        xml.XML.popTag();
        xml.XML.popTag();
        xml.XML.popTag();
        myTopics.push_back(thisTopic);
    }

}

bool checkInactive( trxFloatingMessage  &m ){return !m.active;}

void trxFloatingMessageController::removeInactiveMessages(){
    ofRemove(myActiveMessages, checkInactive);
}



