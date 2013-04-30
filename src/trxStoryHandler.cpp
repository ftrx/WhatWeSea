//
//  trxStoryHandler.cpp
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 29.04.13.
//
//

#include "trxStoryHandler.h"

trxStoryHandler::trxStoryHandler() {
}

trxStoryHandler::trxStoryHandler(vector<trxFlock> *_allFLocks,vector<trxConverter> * _allConverters, vector<trxConnectionSlot> * _allConnections) {
    ofTrueTypeFont::setGlobalDpi(72);
	HelveticaNeueRoman18.loadFont("fonts/HelveticaRoman.ttf", 24, true, true);
	HelveticaNeueRoman18.setLineHeight(28.0f);
	HelveticaNeueRoman18.setLetterSpacing(1.037);
    HelveticaNeueRoman36.loadFont("fonts/HelveticaRoman.ttf", 36, true, true);
	HelveticaNeueRoman36.setLineHeight(40.0f);
	HelveticaNeueRoman36.setLetterSpacing(1.037);
    
    xml = trxXML("stories.xml");
    xml.setup("stories", "story");
    
    allFlocks = _allFLocks;
    allConverters = _allConverters;
    allConnectionSlots = _allConnections;
    generateStories();
}

void trxStoryHandler::startStory(trxConnectionSlot * _activeConnection){
    
    activeConnection = _activeConnection;
    myActiveStory = getStoryWithConnection(activeConnection);
    myActiveTask = &myActiveStory->myTasks.at(0);
    activeFlock = activeConnection->myFlock;
    cout << "start new Story:" << activeConnection->getID() << endl;
    catchedQuantity = 0;    
}

void trxStoryHandler::stopStory(){
    myActiveTask->finished = false;
    catchedQuantity = 0;
    activeFlock->removeDeadBoids();
    activeFlock= NULL;
    activeConnection = NULL;
}

void trxStoryHandler::update()
{
    if (activeConnection) {
        catchedQuantity = activeFlock->countDead();
        if (catchedQuantity >= myActiveTask->quantity) {
            myActiveTask->finished = true;
        }
    }
}


void trxStoryHandler::draw(){
    if (activeConnection == myActiveStory->myConnection) {
        ofPushMatrix();
        ofTranslate(activeConnection->myConverter->position.x,ofGetHeight()-activeConnection->myConverter->position.y);
        ofRotate(activeConnection->myConverter->rotation);
        
        activeConnection->drawWobbleLine(0, 0, 200.0, 200.0);
        ofTranslate(200.0, 200.0);
        drawTarget();
        
        ofTranslate(80, -80);
        drawTaskMessage(myActiveTask->taskMessage);
        ofTranslate(0, 10);
        drawProgressBar(catchedQuantity);
        ofPopMatrix();
    }
    if (myActiveTask->finished) {
        
        drawMessage("Finished");
        
    }
}


void trxStoryHandler::drawTaskMessage(string _message){
    HelveticaNeueRoman18.drawString(_message, 0, 0);
}

void trxStoryHandler::drawMessage(string _message){
    ofEnableAlphaBlending();
    ofSetColor(0, 0, 0,100);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255, 255, 255,255);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
    HelveticaNeueRoman36.drawString(_message, 0, 0);
    ofPopMatrix();
    ofDisableAlphaBlending();
}

void trxStoryHandler::drawProgressBar(int _currentQuantity){
    float length = 200.0;
    float height = 5.0;
    ofPushStyle();
    ofSetColor(0, 82, 144);
    ofRect(0, 0, length, height);
    ofSetColor(255, 255, 255);
    ofRect(0, 0, ofMap(_currentQuantity, 0, myActiveTask->quantity, 0, length), height);
    ofPopStyle();
}

void trxStoryHandler::drawTarget(){
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(4.0);
        ofCircle(0, 0, myActiveTask->targetSize);
    ofPopStyle();
}

void trxStoryHandler::generateStories(){
    
    for (int i=0; i < xml.objectNumber; i++)
    {
        story thisStory;
        xml.XML.pushTag("stories");
        xml.XML.pushTag("story",i);
        thisStory.myStoryFlock = getFlockWithID(xml.getIntValue(i, "flock"));
        thisStory.myStoryConverter = getConverterWithID(xml.getIntValue(i, "converter"));
        thisStory.myConnection = getConnectionSlotWithID(thisStory.myStoryFlock, thisStory.myStoryConverter);
        thisStory.finalMessage = xml.getString(i, "finalMessage");
        

        int numberOfTasks = xml.XML.getNumTags("task");
        for (int taskN=0; taskN < numberOfTasks; taskN++) {
            xml.XML.pushTag("task",taskN);
            task thisTask;
            thisTask.catchID = xml.getIntValue(taskN, "catchID");
            thisTask.taskMessage = xml.getString(taskN, "taskMessage");
            thisStory.myTasks.push_back(thisTask);
            xml.XML.popTag();
        }

        xml.XML.popTag();
        xml.XML.popTag();
        myStories.push_back(thisStory);
    }
}


// helpers
trxFlock* trxStoryHandler::getFlockWithID(int _id){
    for (int i=0; i< allFlocks->size(); i++) {
        trxFlock * thisFlock = & allFlocks->at(i);
        if (thisFlock->id == _id) {
            return thisFlock;
        }
    }
    return NULL;
}

trxConverter* trxStoryHandler::getConverterWithID(int _id){
    for (int i=0; i< allConverters->size(); i++) {
        trxConverter * thisConverter = & allConverters->at(i);
        if (thisConverter->id == _id) {
            return thisConverter;
        }
    }
    return NULL;
}

trxConnectionSlot* trxStoryHandler::getConnectionSlotWithID(trxFlock * _flock, trxConverter * _converter){
    for (int i=0; i< allConnectionSlots->size(); i++) {
        trxConnectionSlot * thisConnectionSlot = &allConnectionSlots->at(i);
        if (thisConnectionSlot->myFlock == _flock && thisConnectionSlot->myConverter == _converter) {
            return thisConnectionSlot;
        }
    }
    return NULL;
}

trxStoryHandler::story * trxStoryHandler::getStoryWithConnection(trxConnectionSlot * _connection){
    for (int i=0; i< myStories.size(); i++) {
        story * thisStory = &myStories.at(i);
        if (thisStory->myConnection == _connection) {
            return thisStory;
        }
    }
    return NULL;
}