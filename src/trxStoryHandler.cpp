//
//  trxStoryHandler.cpp
//  WhatWeSea
//
//  Created by Fabian Troxler on 29.04.13.
//
//

#include "trxStoryHandler.h"

trxStoryHandler::trxStoryHandler() {
    ofTrueTypeFont::setGlobalDpi(72);
	HelveticaNeueRoman18.loadFont("fonts/NewsGot-Reg.otf", 24, true, true);
	HelveticaNeueRoman18.setLineHeight(28.0f);
	HelveticaNeueRoman18.setLetterSpacing(1.037);
    HelveticaNeueRoman36.loadFont("fonts/NewsGot-Reg.otf", 36, true, true);
	HelveticaNeueRoman36.setLineHeight(40.0f);
	HelveticaNeueRoman36.setLetterSpacing(1.037);
    
    message_edge_topright.loadImage("message_background/edge_topright.png");
    message_edge_topleft.loadImage("message_background/edge_topleft.png");
    message_edge_bottomright.loadImage("message_background/edge_bottomright.png");
    message_edge_bottomleft.loadImage("message_background/edge_bottomleft.png");
    message_line_left.loadImage("message_background/line_left.png");
    message_line_top.loadImage("message_background/line_top.png");
    message_line_right.loadImage("message_background/line_right.png");
    message_line_bottom.loadImage("message_background/line_bottom.png");

    messageButton = trxStoryButton(ofVec2f(ofGetWidth()/2.0, ofGetHeight()/2.0+40.0),100, 20, "schliessen");
    
    ship.loadImage("schiff.png");
    
    testbild.loadImage("winscreen/Test_Winscreen.png");
}

void trxStoryHandler::setup(vector<trxFlock*>& _allFLocks,vector<trxConverter*>& _allConverters, vector<trxConnectionSlot*>& _allConnections) {

    
    xml = trxXML("stories.xml");
    xml.setup("stories", "story");
    myActiveTask = NULL;
    myActiveStory = NULL;
    allFlocks = _allFLocks;
    allConverters = _allConverters;
    allConnectionSlots = _allConnections;
    generateStories();
    
    myFloatingMessageController.setup();
    
    fingerHint.setPosition(ofVec2f(ofGetWidth()/2,ofGetHeight()/2));
    //myOsc.setup();
}

void trxStoryHandler::startStory(trxConnectionSlot* _activeConnection){
    
    stopStory(); // stop any current running Story
    
    if (getStoryWithConnection(_activeConnection)) {
        myActiveStory = getStoryWithConnection(_activeConnection);
        changeTopic(myActiveStory->topicNumber);
        myOsc.sendOscAction(2);
        activeConnection = _activeConnection;
        //myActiveStory = getStoryWithConnection(activeConnection);
        if(myActiveStory->myTasks.size() > 0)
        {
            myActiveTask = &myActiveStory->myTasks.at(0);
            for (int i=0; i<myActiveTask->catchID.size(); i++) {
                activeFlock.push_back(getFlockWithID(myActiveTask->catchID.at(i)));
            }
            if(myActiveTask->bycatchID.size()>0){
                
                for (int i=0; i<myActiveTask->bycatchID.size(); i++) {
                    activeBycatchFlock.push_back(getFlockWithID(myActiveTask->bycatchID.at(i)));
                }
                
                
            }
            else{
                activeBycatchFlock.clear();
            }
        }
        activeConverter = myActiveStory->myStoryConverter;
        cout << "start new Story:" << myActiveStory->description << endl;
        catchedQuantity = 0;
        bycatchQuantity = 0;
        myActiveStory->finished = false;
        
        idleTimer = ofGetElapsedTimeMillis(); // Timer for resetting the Story (or end it)
        
        showFingerHint = true;
    }
        
}

void trxStoryHandler::stopStory(){
    if (myActiveStory) {
        cout << "stop Story:" << myActiveStory->description << endl;
        if(myActiveTask)
        {
            myActiveTask = NULL;
        }
        for (int i=0; i<myActiveStory->myTasks.size(); i++) {
            myActiveStory->myTasks.at(i).finished = false;
            
        }
        
        for (int i=0; i<finishedFlocks.size(); i++) {
            finishedFlocks[i]->removeDeadBoids();
            finishedFlocks[i]->freeCatchedBoids();

        }
        
        catchedQuantity = 0;
        bycatchQuantity = 0;
        
        tempBycatchQuantity = 0;
        tempCatchedQuantity = 0;
        
        showMessage = false;
        myActiveStory = NULL;
    
        showFingerHint = false;
        
        activeBycatchFlock.clear();
        activeFlock.clear();
    }
    
}

void trxStoryHandler::update()
{
    int onWayQuantity = 0;
    updateMyLastTargetScreenPosition();
    if (myActiveStory) {
        if (myActiveTask) {
            updateTargetPosition();
            if (!showMessage) {
                if (myActiveTask->type == "standard") {
                    
                    if(myActiveTask->dieAfterCatch) {
                        for (int i=0; i<activeFlock.size(); i++) {
                            catchedQuantity += activeFlock.at(i)->countDead();
                            activeFlock.at(i)->removeDeadBoids();
                        }
                    }
                    else{
                        for (int i=0; i<activeFlock.size(); i++) {
                            catchedQuantity = activeFlock.at(i)->countDead();
                        }
                    }
                    if(activeBycatchFlock.size()>0){
                        for (int i=0; i<activeBycatchFlock.size(); i++) {
                            bycatchQuantity += activeBycatchFlock.at(i)->countDead();
                            activeBycatchFlock.at(i)->removeDeadBoids();
                        }
                    }
                    for (int i=0; i<activeFlock.size(); i++) {
                        onWayQuantity += activeFlock.at(i)->countOnWay();
                    }
                    if (catchedQuantity >= myActiveTask->quantity) {
                        finishTask();
                    }
                }
                else if (myActiveTask->type == "fraction"){
                    if(myActiveTask->dieAfterCatch) {
                        int lastCatchedQuantity = catchedQuantity;
                        for (int i=0; i<activeFlock.size(); i++) {
                            tempCatchedQuantity += activeFlock.at(i)->countDead();
                        }
                        catchedQuantity = int((tempCatchedQuantity/100.0 * myActiveTask->percent) +0.5);
                        bycatchQuantity = tempCatchedQuantity-catchedQuantity;
                        
                        for (int i=0; i<activeFlock.size(); i++) {
                            activeFlock.at(i)->removeDeadBoids();
                        }
                        
                        if (lastCatchedQuantity != catchedQuantity) {
                            myOsc.sendOscAction(6);
                        }
                        
                    }
                    else{
                        catchedQuantity = 0;
                        for (int i=0; i<activeFlock.size(); i++) {
                            catchedQuantity += activeFlock.at(i)->countDead();
                        }
                    }
                    if(activeBycatchFlock.size()>0){
                        for (int i=0; i<activeBycatchFlock.size(); i++) {
                            bycatchQuantity += activeBycatchFlock.at(i)->countDead();
                            activeBycatchFlock.at(i)->removeDeadBoids();
                        }
                    }
                    for (int i=0; i<activeFlock.size(); i++) {
                        onWayQuantity += activeFlock.at(i)->countOnWay();
                    }
                    if (catchedQuantity >= myActiveTask->quantity) {
                        finishTask();
                    }
                }
            }
        }
        else {
            if (!showMessage) {
                finishStory();
            }
        }
    }
    if (onWayQuantity == catchedQuantity && catchedQuantity > 0 && !runningAction) {
        changeAction(2);
        cout<< "retour zu boot"<<endl;
        runningAction = true;
    }
    if(myActiveStory){ resetStoryAfterTimeout(IDLETIME);}
    
    if (ofGetElapsedTimeMillis() > messageTimer + 10000 ) {
      //  myFloatingMessageController.newRandomFact();
        messageTimer = ofGetElapsedTimeMillis();
    }
    if (showFingerHint) {
        fingerHint.update();
    }
    
    myFloatingMessageController.update();
}


trxStoryHandler::task * trxStoryHandler::nextTask(){
    catchedQuantity = 0;
    tempCatchedQuantity = 0;
    bycatchQuantity = 0;
    tempBycatchQuantity = 0;
    //activeFlock->removeDeadBoids();
    for (int i=0; i<activeFlock.size(); i++) {
        activeFlock.at(i)->freeCatchedBoids();
        finishedFlocks.push_back(activeFlock.at(i));
    }
    if(activeBycatchFlock.size()>(0))
    {
        for (int i=0; i<activeBycatchFlock.size(); i++) {
            finishedFlocks.push_back(activeBycatchFlock.at(i));
            //activeBycatchFlock->removeDeadBoids();
            activeBycatchFlock.at(i)->freeCatchedBoids();
        }
    }
    if(myActiveTask->no < myActiveStory->myTasks.size()-1){
        int newNo = myActiveTask->no + 1;
        return &myActiveStory->myTasks.at(newNo);
    }
    else return NULL;
}


void trxStoryHandler::finishTask(){
    if (myActiveTask)
    {
        cout << "Task Finished" << endl;
        myActiveTask->finished = true;
        //activeFlock->removeDeadBoids();
        //messageButton = trxStoryButton(ofVec2f(ofGetWidth()/2.0-50.0, ofGetHeight()/2.0+50.0),100, 20, "close");
        //showMessage = true;
        closeMessage();
    }
}


void trxStoryHandler::finishStory(){
    
    myActiveStory->finished = true;
   
    showMessage = true;
    myOsc.sendOscAction(7);
}


void trxStoryHandler::draw(){
    if (myActiveStory) {
        if (myActiveTask) {
            ofPushMatrix();
            ofPushStyle();
            ofTranslate(activeConverter->position.x,activeConverter->position.y);
            ofRotate(activeConnection->myConverter->rotation);
            ofSetColor(255, 255, 255);
            float maxAmplitude = 5.0;
            ofVec3f randomWiggle = ofVec3f(ofSignedNoise(ofGetElapsedTimef()),ofSignedNoise(ofGetElapsedTimef()),0)*maxAmplitude;
            
            ofVec3f target = myActiveTask->targetPosition+randomWiggle;
            activeConnection->drawWobbleLine(0+activeConverter->radius, 0, target.x-myActiveTask->targetSize,target.y);
            ofTranslate(target);
            
            //drawTarget();
            //ofTranslate(randomWiggle);
            if (activeConverter->id == 11 || activeConverter->id == 10) {
                
                if (activeConverter->id == 10) {
                    ofEnableAlphaBlending();
                    ofSetColor(0,30,80,255);
                    ship.draw(-10, -24, 160, 48);
                    ofDisableAlphaBlending();
                }
                
                ofSetLineWidth(2);
                ofSetColor(255, 255, 255);
                ofSetCircleResolution(40.0);
                ofNoFill();
                ofCircle(0,0, myActiveTask->targetSize-1.0);
                
                if (myActiveTask->progress == "circle") {

                    drawProgressCircle(myActiveTask->targetSize, 10.0, catchedQuantity, myActiveTask->quantity);
                }
                else if (myActiveTask->progress == "amount"){
                    drawProgressAmount(myActiveTask->targetSize, 100.0, catchedQuantity);
                }
                if (myActiveTask->bycatchID.size()>0 || myActiveTask->type == "fraction") {
                    drawProgressAmount(myActiveTask->targetSize, 100.0, bycatchQuantity);
                }

            }            
            ofTranslate(0, -80);
            drawTaskMessage(myActiveTask->taskMessage);
            ofPopMatrix();
            ofPopStyle();
            if (myActiveTask->finished) {
                //drawMessage("Task Finished Message");
            }
        }
        if (myActiveStory->finished) {
           //drawMessage(myActiveStory->finalMessage);
            drawMessage(randomFact);
        }
    }
    myFloatingMessageController.draw();
    
    if (showFingerHint) {
        fingerHint.draw();
    }
    
    /*
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255,255);
    testbild.draw(0,0);
    ofDisableAlphaBlending();
     */
   
}


void trxStoryHandler::draw3D(){
  //  myFloatingMessageController.draw();
}


void trxStoryHandler::drawDebug()
{
    if (myActiveTask) {

        
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate(myScreenTargetPosition);
        ofSetColor(255, 0, 0);
        ofCircle(0, 0, 20);
        ofPopStyle();
        
        ofPopMatrix();
    }
}


void trxStoryHandler::drawTaskMessage(string _message){
    ofPushStyle();
    ofSetColor(255, 255, 255);
    HelveticaNeueRoman18.drawString(_message, 0, 0);
    ofPopStyle();
}


void trxStoryHandler::drawMessage(string _message){
    
    int frameWidth = 840;
    int margin = 40;
    int outerMargin = 10;
    int alpha = 234;
    int border = 40;
    frameWidth = frameWidth -(2+2*margin);
    string message = wrapString(_message, frameWidth-2*border);
    
    ofPushStyle();
    ofFill();
    ofEnableAlphaBlending();
    ofSetColor(0, 0, 0,100);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    ofPushMatrix();
    
    ofRectangle bounds = HelveticaNeueRoman36.getStringBoundingBox(message, 0, 0);
    bounds.height +=2*margin;
    bounds.width +=2*margin;
    
    int imageHeight = myActiveStory->finalFactImage.height;
    int imageWidth = myActiveStory->finalFactImage.width;
    int totalHeigth = (bounds.height+imageHeight+2*border+outerMargin);
    ofTranslate(ofGetWidth()/2.0-frameWidth/2.0, ofGetHeight()/2.0-totalHeigth/2.0);
    
    ofSetColor(255, 255, 255,255);
    myActiveStory->finalFactImage.draw(-border,0,imageWidth,imageHeight);
    
    ofTranslate(0, imageHeight+outerMargin+border);
    
    
    
    
    //draw rounded box
    ofSetColor(0, 0, 0,alpha);
    ofRect(0, 0, frameWidth, bounds.height);
    
    ofSetColor(255, 255, 255,alpha);
    ofPushMatrix();
    ofTranslate(-border, -border);
    message_edge_topleft.draw(0,0,0);
    message_line_top.draw(border,0,frameWidth,border);
    ofTranslate(frameWidth+border, 0);
    message_edge_topright.draw(0,0);
    message_line_right.draw(0,border,border,bounds.height);
    ofTranslate(0, bounds.height+border);
    message_edge_bottomright.draw(0,0);
    message_line_bottom.draw(0, 0, -frameWidth, border);
    ofTranslate(-frameWidth-border, 0);
    message_edge_bottomleft.draw(0,0);
    message_line_left.draw(0,0, border, -bounds.height);
    ofPopMatrix();
    
    //draw text
    ofSetColor(255, 255, 255,255);
    HelveticaNeueRoman36.drawString(message, margin, bounds.height/2);
    ofPopMatrix();
    
    //draw Button
    messageButton.setPosition(ofVec3f(ofGetWidth()/2,ofGetHeight()/2+totalHeigth/2+border+outerMargin));
    ofDisableAlphaBlending();
    messageButton.draw();
    ofPopStyle();
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
void trxStoryHandler::drawProgressCircle(float _radius, float _barHeight,int _currentQuantity, int _numberOfSectors){

    ofPushStyle();
    ofSetLineWidth(0);
    
    ofFill();
    ofEnableSmoothing();
    float radius = _radius;
    float barHeight = _barHeight;
    float sectorSpace = 5.0;
    float sectorSize = (360.0/_numberOfSectors);
    
    for (int i=0; i<_numberOfSectors; i++) {
        int progress = int(ofMap(_currentQuantity, 0, myActiveTask->quantity, 0, _numberOfSectors)+0.5);
        ofColor color= PROGRESSBARCOLOR;
        if (i<progress) {
            color= ofColor(255, 255, 255);
        }
        ofPath progressCircle;
        progressCircle.setColor(color);
        progressCircle.setArcResolution(40);
        progressCircle.arc(ofPoint(0,0), radius, radius, sectorSpace/2.0+i*sectorSize, (i+1)*sectorSize-sectorSpace/2.0, true);
        progressCircle.arc(ofPoint(0,0), radius+barHeight, radius+barHeight, (i+1)*sectorSize-sectorSpace/2.0, sectorSpace/2.0+i*sectorSize,false);
        progressCircle.close();

        progressCircle.draw();
        
        
    
    
    }

    ofDisableSmoothing();
    ofPopStyle();

}

void trxStoryHandler::drawProgressAmount(float _radius, float _barHeight, int _currentQuantity){
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetLineWidth(0);
    
    ofFill();
    
    float radius = _radius;
    float barHeight = _barHeight;
    float progressHeight = int(ofMap(_currentQuantity, 0, myActiveTask->quantity, 0, _barHeight));
    ofColor color= PROGRESSBARAMOUNTCOLOR;
    color.a = 100;
    
    
    ofPath progressCircle;
    
    progressCircle.setColor(color);
    progressCircle.setArcResolution(40);
    progressCircle.arc(ofPoint(0,0), radius, radius, 0, 360, true);
    progressCircle.arc(ofPoint(0,0), radius+progressHeight, radius+progressHeight, 0, 360,true);
    progressCircle.close();
    progressCircle.draw();
    
    ofDisableAlphaBlending();
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
        thisStory.myStoryFlock = getFlockWithID(xml.getIntValue(NULL, "flock"));
        thisStory.myStoryConverter = getConverterWithID(xml.getIntValue(NULL, "converter"));
        thisStory.myConnection = getConnectionSlotWithID(thisStory.myStoryFlock, thisStory.myStoryConverter);
        thisStory.finalMessage = xml.getString("", "finalMessage");
        thisStory.description = xml.getString("no Description found", "description");
        thisStory.topicNumber = xml.getIntValue(i, "topicNumber");
        thisStory.finalFactImage.loadImage("facts/"+ xml.getString("", "finalFact"));
        int numberOfTasks = xml.XML.getNumTags("task");
        
        for (int taskN=0; taskN < numberOfTasks; taskN++) {
            xml.XML.pushTag("task",taskN);
            task thisTask;
            thisTask.no = xml.getIntValue(NULL, "no");
            //thisTask.catchID = xml.getIntValue(NULL, "catchID");
            xml.XML.pushTag("catchIDs");
            int numberOfCatchIDs = xml.XML.getNumTags("catchID");
            for (int catchID = 0; catchID<numberOfCatchIDs; catchID++) {
                xml.XML.pushTag("catchID",catchID);
                int id = xml.getIntValue(99, "ID");
                if (id != 99) {
                    thisTask.catchID.push_back(id);
                }
                xml.XML.popTag();
            }
            xml.XML.popTag();
            xml.XML.pushTag("bycatchIDs");
            int numberOfbycatchIDs = xml.XML.getNumTags("bycatchID");
            for (int bycatch = 0; bycatch<numberOfbycatchIDs; bycatch++) {
                xml.XML.pushTag("bycatchID",bycatch);
                int id = xml.getIntValue(99, "ID");
                if (id != 99) {
                    thisTask.bycatchID.push_back(id);
                }
                xml.XML.popTag();
            }
            xml.XML.popTag();
            thisTask.catchSize = xml.getIntValue(10, "catchSize");
            thisTask.taskMessage = xml.getString("No Task Message", "taskMessage");
            thisTask.quantity = xml.getIntValue(1, "quantity");
            thisTask.targetSize = xml.getFloatValue(40.0, "targetSize");
            int intToBool = xml.getIntValue(0, "dieAfterCatch");
            thisTask.dieAfterCatch = ( intToBool !=0);
            thisTask.progress = xml.getString("circle", "progress");
            thisTask.type = xml.getString("standard", "type");
            thisTask.percent = xml.getIntValue(100, "percent");
            thisTask.harvester = xml.getString("net", "harvester");
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
    for (int i=0; i< allFlocks.size(); i++) {
        trxFlock * thisFlock = allFlocks.at(i);
        if (thisFlock->id == _id) {
            return thisFlock;
        }
    }
    return NULL;
}

trxConverter* trxStoryHandler::getConverterWithID(int _id){
    for (int i=0; i< allConverters.size(); i++) {
        trxConverter * thisConverter = allConverters.at(i);
        if (thisConverter->id == _id) {
            return thisConverter;
        }
    }
    return NULL;
}

trxConnectionSlot* trxStoryHandler::getConnectionSlotWithID(trxFlock * _flock, trxConverter * _converter){
    for (int i=0; i< allConnectionSlots.size(); i++) {
        trxConnectionSlot * thisConnectionSlot = allConnectionSlots.at(i);
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


void trxStoryHandler::resetStoryAfterTimeout (int _idletime = IDLETIME){
    if (ofGetElapsedTimeMillis()-idleTimer > _idletime) {
        if(myActiveStory){
            cout << "Timeout! StoppedStory" << endl;
            startStory(activeConnection);
        }
        idleTimer = ofGetElapsedTimeMillis();
        
    }
}

void trxStoryHandler::closeMessage(){
    showMessage = false;
    
    if (myActiveTask)
    {
        if (myActiveTask->finished) {
            myActiveTask = nextTask();
            if (myActiveTask) {
                activeFlock.clear();
                for (int i=0; i<myActiveTask->catchID.size(); i++) {
                    activeFlock.push_back( getFlockWithID(myActiveTask->catchID.at(i)));
                }
                
                if (myActiveTask->bycatchID.size()>0) {
                    activeBycatchFlock.clear();
                    for (int i =0; i<myActiveTask->bycatchID.size(); i++) {
                        activeBycatchFlock.push_back(getFlockWithID(myActiveTask->bycatchID.at(i)));
                    }
                    
                }
                else{
                    activeBycatchFlock.clear();
                }
            }
            
        }
    }
    if (myActiveStory->finished) {
        startStory(activeConnection);
    }
}
void trxStoryHandler::updateTargetPosition(){
    
    ofVec3f rotPos = myActiveTask->targetPosition;
    rotPos.rotate(activeConverter->rotation, ofVec3f(0, 0, 1));
    rotPos +=activeConverter->position;
    myTargetPosition = rotPos;
    
    
}

void trxStoryHandler::updateMyLastTargetScreenPosition()
{
    myScreenTargetMovement = myScreenTargetPosition-myLastScreenTargetPosition;
    myLastScreenTargetPosition = myScreenTargetPosition;
}

void trxStoryHandler::changeAction(int _actionNumber){
    int currentAction = myActiveTask->no*2+_actionNumber;
    myOsc.sendOscAction(currentAction);
}

void trxStoryHandler::changeTopic(int _topicNumber){

    if (_topicNumber != 0) {
        myOsc.sendOscTopic(_topicNumber);
        myFloatingMessageController.changeTopic(_topicNumber);
        
        randomFact = myFloatingMessageController.getRandomFact();
        
        activeTopicNumber = _topicNumber;
    }
    
}


string trxStoryHandler::wrapString(string text, int width) {
	
	string typeWrapped = "";
	string tempString = "";
	vector <string> words = ofSplitString(text, " ");
	
	for(int i=0; i<words.size(); i++) {
		
		string wrd = words[i];
		
		// if we aren't on the first word, add a space
		if (i > 0) {
			tempString += " ";
		}
		tempString += wrd;
		
		int stringwidth = HelveticaNeueRoman36.stringWidth(tempString);
        
		if(stringwidth >= width) {
			typeWrapped += "\n";
			tempString = wrd;		// make sure we're including the extra word on the next line
		} else if (i > 0) {
			// if we aren't on the first word, add a space
			typeWrapped += " ";
		}
		
		typeWrapped += wrd;
	}
	
	return typeWrapped;
	
}
