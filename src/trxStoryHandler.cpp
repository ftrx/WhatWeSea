//
//  trxStoryHandler.cpp
//  BoidsAndTUIO
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
}

void trxStoryHandler::setup(vector<trxFlock> *_allFLocks,vector<trxConverter> * _allConverters, vector<trxConnectionSlot> * _allConnections) {

    
    xml = trxXML("stories.xml");
    xml.setup("stories", "story");
    myActiveTask = NULL;
    myActiveStory = NULL;
    allFlocks = _allFLocks;
    allConverters = _allConverters;
    allConnectionSlots = _allConnections;
    generateStories();
    
    myFloatingMessageController.setup();
    //myOsc.setup();
}

void trxStoryHandler::startStory(trxConnectionSlot * _activeConnection){
    
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
            activeFlock = getFlockWithID(myActiveTask->catchID);
            
        }
        
        activeConverter = myActiveStory->myStoryConverter;
        cout << "start new Story:" << myActiveStory->description << endl;
        catchedQuantity = 0;
        myActiveStory->finished = false;
        
        idleTimer = ofGetElapsedTimeMillis(); // Timer for resetting the Story (or end it)
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
        catchedQuantity = 0;
        activeFlock->removeDeadBoids();
        showMessage = false;
        myActiveStory = NULL;
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
                catchedQuantity = activeFlock->countDead();
                onWayQuantity = activeFlock->countOnWay();
                if (catchedQuantity >= myActiveTask->quantity) {
                    finishTask();
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
        myFloatingMessageController.newRandomFact();
        messageTimer = ofGetElapsedTimeMillis();
    }
    
    if (activeFlock || activeConverter){
        
        int flockID = NULL;
        int converterID = NULL;
        
        if (activeFlock) {
            flockID = activeFlock->id;
        }
        if (activeConverter) {
            converterID = activeConverter->id;
        }
        
       // myFloatingMessageController.changeTopic(flockID, converterID);
    }
    
    myFloatingMessageController.update();
}




trxStoryHandler::task * trxStoryHandler::nextTask(){
    catchedQuantity = 0;
    activeFlock->removeDeadBoids();
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
     messageButton = trxStoryButton(ofVec2f(ofGetWidth()/2.0-50.0, ofGetHeight()/2.0+50.0),100, 20, "close");
     showMessage = true;
    
}

void trxStoryHandler::draw(){
    if (myActiveStory) {
        if (myActiveTask) {
            ofPushMatrix();
            ofTranslate(activeConverter->position.x,activeConverter->position.y);
            ofRotate(activeConnection->myConverter->rotation);
            activeConnection->drawWobbleLine(0, 0, myActiveTask->targetPosition.x,myActiveTask->targetPosition.y);
            ofTranslate(myActiveTask->targetPosition);
            //drawTarget();
            drawProgressCircle(myActiveTask->targetSize, 10.0, catchedQuantity, 12);
            ofTranslate(0, -80);
            drawTaskMessage(myActiveTask->taskMessage);
            
            //drawProgressBar(catchedQuantity);
            
            ofPopMatrix();
            if (myActiveTask->finished) {
                //drawMessage("Task Finished Message");
            }
        }
        if (myActiveStory->finished) {
           drawMessage("Story Finished Message");
        }
    }
    myFloatingMessageController.draw();
   
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
    
    messageButton.draw();
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
    
    float radius = _radius;
    float barHeight = _barHeight;
    float sectorSpace = 5.0;
    float sectorSize = (360.0/_numberOfSectors);
    
    for (int i=0; i<_numberOfSectors; i++) {
        int progress = int(ofMap(_currentQuantity, 0, myActiveTask->quantity, 0, _numberOfSectors)+0.5);
        ofColor color= ofColor(0, 82, 144);
        if (i<progress) {
            color= ofColor(255, 255, 255);
        }
        
        
        ofPath progressCircle;
        
        progressCircle.setColor(color);
        progressCircle.setArcResolution(40);
        progressCircle.arc(ofPoint(0,0), radius, radius, sectorSpace/2.0+i*sectorSize, (i+1)*sectorSize-sectorSpace/2.0, true);
        progressCircle.arc(ofPoint(0,0), radius-barHeight, radius-barHeight, (i+1)*sectorSize-sectorSpace/2.0, sectorSpace/2.0+i*sectorSize,false);
        progressCircle.close();
        progressCircle.draw();
    }

    
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

        int numberOfTasks = xml.XML.getNumTags("task");
        for (int taskN=0; taskN < numberOfTasks; taskN++) {
            xml.XML.pushTag("task",taskN);
            task thisTask;
            thisTask.no = xml.getIntValue(NULL, "no");
            thisTask.catchID = xml.getIntValue(NULL, "catchID");
            thisTask.catchSize = xml.getIntValue(10, "catchSize");
            thisTask.taskMessage = xml.getString("No Task Message", "taskMessage");
            thisTask.quantity = xml.getIntValue(1, "quantity");
            thisTask.targetSize = xml.getFloatValue(40.0, "targetSize");
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
                activeFlock = getFlockWithID(myActiveTask->catchID);
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
    myScreenTargetMovement = myScreenTargetPosition- myLastScreenTargetPosition;
    myLastScreenTargetPosition = myScreenTargetPosition;
}

void trxStoryHandler::changeAction(int _actionNumber){
    int currentAction = myActiveTask->no*2+_actionNumber;
    myOsc.sendOscAction(currentAction);
}

void trxStoryHandler::changeTopic(int _topicNumber){

    myOsc.sendOscTopic(_topicNumber);
    myFloatingMessageController.changeTopic(_topicNumber);
}


