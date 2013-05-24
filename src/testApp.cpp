#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{
   // ofSetLogLevel(OF_LOG_VERBOSE);
    ofHideCursor();

    myObjectHandler = new trxObjectHandler();
    myObjectHandler->myCamera = &camera;
    
	ofBackground(20,20,40);
    //ofSetBackgroundAuto(true);
    background.loadImage("background.png");
    
	//ofSetFrameRate(20);
	//ofSetVerticalSync(true);
    
	ofEnableSmoothing();
        
    viewMain.x = 0;
	viewMain.y = 0;
	viewMain.width = ofGetWidth();
	viewMain.height = ofGetHeight();
	
    camera.setPosition(ofVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,1000));
    camera.lookAt(ofVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,0));
    camera.setFov(90);
    camera.setNearClip(100.0);
    camera.setFarClip(1000.0);
    
    
    
    if(shader1.load("shader")) {
		printf("Shader is loaded\n");
	}
    
    
    tuioClient.start(3333);
    //----- Eventlistener
    //finger
    ofAddListener(tuioClient.cursorAdded,this,&testApp::tuioCursorAdded);
	ofAddListener(tuioClient.cursorRemoved,this,&testApp::tuioCursorRemoved);
	ofAddListener(tuioClient.cursorUpdated,this,&testApp::tuioCursorUpdated);
    //fiducials
    ofAddListener(tuioClient.objectAdded,this,&testApp::tuioObjectAdded);
	ofAddListener(tuioClient.objectRemoved,this,&testApp::tuioObjectRemoved);
	ofAddListener(tuioClient.objectUpdated,this,&testApp::tuioObjectUpdated);
    
    
   
    
    myObjectHandler->setup();

}

//--------------------------------------------------------------
void testApp::update()
{
    
    /*
    tuioClient.getMessage();
     
    mutex.lock();
    addNewCursor();
    removeDeadCursors();
    
    myObjectHandler->debug = debug;
    myObjectHandler->update();
    mutex.unlock();
*/
}

//--------------------------------------------------------------
void testApp::draw()
{
    
    tuioClient.getMessage();
    
    mutex.lock();
   
    addNewCursor();
    removeDeadCursors();
    
    myObjectHandler->debug = debug;
    myObjectHandler->update();
    


    ofSetColor(255, 255, 255);
    
    background.draw(0,0, ofGetWidth(), ofGetHeight());
    
    //drawOld();
    
    camera.begin();
    
    shader1.begin();

    myObjectHandler->drawAllBoids();

    shader1.end();

    myObjectHandler->draw3D();
    camera.end();
    
    
    int health = ofMap(myObjectHandler->myFlocks.at(4)->boids.size(), 0, myObjectHandler->allMyBoids.size(), 0, 100);
    
    ofEnableAlphaBlending();
    ofSetColor(25, 0, 0, health);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofDisableAlphaBlending();
    
    myObjectHandler->draw();
    
    
    
    
    if(debug || boidCounter)
    {

        ofPushStyle();
        
        camera.begin();
        if(debug) ofDrawGrid(1920, 10, true, true, true, true);
        camera.end();
        
        ofEnableAlphaBlending();
        ofSetColor(255,255,255, 100);
        ofRect(0, 0, 250, 180);
        ofSetColor(0);
        string info = "FPS "+ofToString(ofGetFrameRate(), 1) + "\n";
        info += "Total Boids "+ofToString(myObjectHandler->allMyBoids.size())+"\n";
        for (int i=0; i<myObjectHandler->myFlocks.size(); i++) {
            trxFlock* flock = myObjectHandler->myFlocks[i];
            info += ofToString(flock->title) + ": \t"+ofToString(flock->boids.size())+"\n";
        }
        info += "Time elapsed: "+ofToString(ofGetElapsedTimef(),2)+" seconds \n";
        info += "Press 'd' to debug\n";
        ofDrawBitmapString(info, 20, 20);
        ofDisableAlphaBlending();
        ofPopStyle();
        
        if (debug) {
            tuioClient.drawCursors();
            tuioClient.drawObjects();
        }
        
        
    }

    mutex.unlock();
    /*
    try{
    mutex.unlock();
    }
    catch(Poco::SystemException e)
    {}
     */
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

    switch (key) {
        case 'd':{
            debug = !debug;
            break;
        }
        case 'f':{
            ofToggleFullscreen();

            break;
        }
        case 'b':{
            boidCounter = !boidCounter;
            break;
        }


        default:
            break;
    }
 
}

//----- Eventlistener

void testApp::tuioObjectAdded(ofxTuioObject & tuioObject)
{
    mutex.lock();
    myObjectHandler->addObject( tuioObject);
    mutex.unlock();
}

void testApp::tuioObjectUpdated(ofxTuioObject & tuioObject)
{
    mutex.lock();
    myObjectHandler->updateObject(tuioObject);
    mutex.unlock();
}

void testApp::tuioObjectRemoved(ofxTuioObject & tuioObject){
    mutex.lock();
    myObjectHandler->removeObject(tuioObject);
    mutex.unlock();
}

void testApp::tuioCursorAdded(ofxTuioCursor &tuioCursor){
    mutex.lock();
    //myObjectHandler.addCursor(tuioCursor);
    
    if (!isCursorStillThere(tuioCursor) && activeCursors.size()< 10) {

        testApp::activeTuioCursor * tmpCursor = new testApp::activeTuioCursor();
        tmpCursor->tuioCursor = tuioCursor;
        tmpCursor->lastTuioCursor = tuioCursor;
        tmpCursor->firstAdded = ofGetElapsedTimef();
        tmpCursor->lastTimeSeen = ofGetElapsedTimef();
        activeCursors.push_back(tmpCursor);

    }
    else{
        
    //cout<<"cursorAllreadyThere"<<endl;
    }
    mutex.unlock();
	
}

void testApp::tuioCursorUpdated(ofxTuioCursor &tuioCursor){
    if (mutex.tryLock() == false) {
        cout<<"mutex locked"<<endl;
    }
    mutex.lock();
    myObjectHandler->updateCursor(tuioCursor);
    mutex.unlock();
}

void testApp::tuioCursorRemoved(ofxTuioCursor &tuioCursor){
    mutex.lock();
    //myObjectHandler.removeCursor(tuioCursor);
    addCursorToRemovable(tuioCursor);
    mutex.unlock();
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    //light.setPosition(600,ofGetMouseY(),ofGetMouseX());
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}


bool testApp::isCursorStillThere(ofxTuioCursor & tuioCursor){
    
    ofVec2f tuioPosition = ofVec2f(tuioCursor.getX(),tuioCursor.getY());
    for (int i=0; i < onRemoveCursors.size(); i++) {
        testApp::activeTuioCursor *  tmpCursor = onRemoveCursors[i];
        ofVec2f tmpPosition = ofVec2f(tmpCursor->lastTuioCursor.getX(),tmpCursor->lastTuioCursor.getY());
        if (tuioPosition.distance(tmpPosition) < DISTANCE/ofGetWidth()) {
            if (ofGetElapsedTimef() - tmpCursor->lastTimeSeen <= TIMER) {
                long sessionID = tmpCursor->lastTuioCursor.getSessionId();
                myObjectHandler->updateSessionID(tmpCursor->lastTuioCursor.getSessionId(), tuioCursor.getSessionId());
                tmpCursor->tuioCursor = &tuioCursor;
                tmpCursor->lastTuioCursor = tuioCursor;
                tmpCursor->lastTimeSeen = ofGetElapsedTimef();
                activeCursors.push_back(tmpCursor);
                onRemoveCursors.erase(onRemoveCursors.begin()+i);
                
                return true;
            }
        }
    }
    return false;
    
}


void testApp::addNewCursor(){
    for (int i=0; i<activeCursors.size(); i++) {
         testApp::activeTuioCursor * tmpCursor = activeCursors[i];
        if (!tmpCursor->active) {
            if (ofGetElapsedTimef() - tmpCursor->firstAdded > ADDTIMER) {
                myObjectHandler->addCursor(tmpCursor->tuioCursor);
                tmpCursor->active = true;
            }
        }
    }
}


void testApp::addCursorToRemovable(ofxTuioCursor &tuioCursor){
    for (int i=0; i<activeCursors.size(); i++) {
        if (activeCursors[i]->tuioCursor.getSessionId() == tuioCursor.getSessionId()) {
            testApp::activeTuioCursor * tmpCursor = activeCursors[i];
            tmpCursor->lastTuioCursor = tuioCursor;
            tmpCursor->lastTimeSeen = ofGetElapsedTimef();
            onRemoveCursors.push_back(tmpCursor);
            activeCursors.erase(activeCursors.begin()+i);
        }
    }
}

void testApp::removeDeadCursors(){
    for (int i=0; i<onRemoveCursors.size(); i++) {
        testApp::activeTuioCursor * tmpCursor = onRemoveCursors[i];
        if (ofGetElapsedTimef() - tmpCursor->lastTimeSeen > TIMER) {
            myObjectHandler->removeCursor(tmpCursor->lastTuioCursor);
            tmpCursor = NULL;
            onRemoveCursors.erase(onRemoveCursors.begin()+i);
        }
    }
    
}



