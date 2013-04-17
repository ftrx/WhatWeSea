#include "testApp.h"




//--------------------------------------------------------------
void testApp::setup()
{
    xml = trxXML();
    xml.setup();
    debug = false;
	ofBackground(20,20,40);
    //ofSetBackgroundAuto(true);
    background.loadImage("background.jpg");
    
	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	//ofEnableNormalizedTexCoords();
	//ofHideCursor();
	
    ofDisableArbTex();
	
    camera.setPosition(ofVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,1000));
    camera.lookAt(ofVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,0));
    //camera.setDistance(500);
    //camera.disableMouseInput();
    
    /*
    int nFlocks=2;
    vector<ofColor> colors;
    colors.push_back(ofColor(255,0,0));
    colors.push_back(ofColor(255,255,0));
    colors.push_back(ofColor(255,0,255));
    
    
    for (int i=0; i<colors.size(); i++) {
        trxFlock thisFlock = trxFlock(ofRandom(-200.0f, 200.0f),ofRandom(-200.0f, 200.0f),ofRandom(-200.0f, 200.0f));
        thisFlock.color = colors[i];
        myFlocks.push_back(thisFlock);
        
    }
    */
    if(shader1.load("shader")) {
		printf("Shader is loaded\n");
	}
    cout << xml.flockNumber <<endl;
    for (int i=0; i < xml.flockNumber; i++)
    {
        if(xml.flockNumber > 0){
            ofTexture img;
            string tmpString = "icons/" + xml.getTexture(i);
            cout << tmpString << endl;
            ofLoadImage(img,tmpString);
            textures.push_back(img);
        }
    }
    
    for (int i=0; i<xml.flockNumber; i++) {
        trxFlock thisFlock = trxFlock(ofGetWidth()/2.0,ofGetHeight()/2.0,300.0,xml.getIntValue(i, "ID"),&harvesters);
        //thisFlock.color = colors[i];
        thisFlock.boidNum = xml.getIntValue(i, "START_BOIDS");
        thisFlock.maxSpeed = xml.getFloatValue(i, "MAX_SPEED");
        thisFlock.texture = &textures.at(i);
        myFlocks.push_back(thisFlock);
        
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

        
    harvesters = *new vector<trxHarvester>();
    
    blur.setup(ofGetWidth(), ofGetHeight(), 4, .2, 4);
    
    
    timeStamp = ofGetElapsedTimeMillis();

}

//--------------------------------------------------------------
void testApp::update()
{
    tuioClient.getMessage();
    float timer = ofGetElapsedTimeMillis() - timeStamp;
    
    
    // 16.66f is abaout 1000/60 so about 60FPS
    if(timer>15.0f){
        for (int i = 0; i<myFlocks.size(); i++) {
            myFlocks[i].update();
        }
        timeStamp = ofGetElapsedTimeMillis();
    }
    
    
    //blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 1, 4));
   }

//--------------------------------------------------------------
void testApp::draw()
{
    ofSetColor(255, 255, 255);

    background.draw(0,0, ofGetWidth(), ofGetHeight());
    int totalPoints = 0;
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableAlphaBlending();
    
	ofEnablePointSprites();
    //ofSetColor(255, 0, 0);
    
    //ofCircle(50,50, 20);
    camera.begin();
    shader1.begin();
    
    ofPushMatrix();
    shader1.setUniform1f("zPosition", ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 600));
    for (int i = 0; i<myFlocks.size(); i++) {
        
       
        myFlocks[i].draw();
        //myFlocks[i].drawCircles();

                totalPoints += myFlocks[i].points.size();
        
        
        
    }
    ofPopMatrix();
    
    shader1.end();
    camera.end();
	ofDisablePointSprites();
	//ofDisableBlendMode();
    
    
    for (int i=0; i<harvesters.size(); i++) {
        harvesters.at(i).draw();
    }

    
    if(debug)
    {
        camera.begin();
        ofDrawGrid(1920, 10, true, true, true, true);
        for (int i = 0; i<myFlocks.size(); i++) {
            
            
            myFlocks[i].drawInfo();
            
        }
        
        camera.end();
        ofSetColor(255, 100);
        ofRect(0, 0, 250, 90);
        ofSetColor(0);
        string info = "FPS "+ofToString(ofGetFrameRate(), 0) + "\n";
        info += "Total Points "+ofToString(totalPoints)+"\n";
        info += "Press 'd' to debug\n";
        
        ofDrawBitmapString(info, 20, 20);
        tuioClient.drawCursors();
        tuioClient.drawObjects();

    }
        
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

        default:
            break;
    }
 
}

//----- Eventlistener

void testApp::tuioObjectAdded(ofxTuioObject & tuioObject){
    ofPoint loc = ofPoint(tuioObject.getX()*ofGetWidth(),tuioObject.getY()*ofGetHeight());
    
    trxFlock thisFlock = trxFlock(tuioObject.getX()*ofGetWidth(),tuioObject.getY()*ofGetHeight(),ofRandom(0, 600.0f),tuioObject.getFiducialId(),&harvesters);
    thisFlock.texture = &textures.at(myFlocks.size());
    myFlocks.push_back(thisFlock);
    
    
    cout << "Object n" << tuioObject.getSessionId() << " add at " << loc << endl;
}

void testApp::tuioObjectUpdated(ofxTuioObject & tuioObject){
    
    for (int i = 0; i < myFlocks.size(); i++) {
        trxFlock *thisFlock = &myFlocks[i];
        if (thisFlock->id == tuioObject.getFiducialId()) {
            thisFlock->position = ofVec3f(tuioObject.getX()*ofGetWidth(),tuioObject.getY()*ofGetHeight(),thisFlock->position.z);
            ofPoint loc = ofPoint(tuioObject.getX()*ofGetWidth(),tuioObject.getY()*ofGetHeight());
            
        }
    }
    ofPoint loc = ofPoint(tuioObject.getX()*ofGetWidth(),tuioObject.getY()*ofGetHeight());
    cout << "Object n" << tuioObject.getSessionId() << " updated at " << loc << endl;
   
}

void testApp::tuioObjectRemoved(ofxTuioObject & tuioObject){
    for (int i = 0; i < myFlocks.size(); i++) {
        trxFlock thisFlock = myFlocks[i];
        if (thisFlock.id == tuioObject.getFiducialId()) {
            myFlocks.erase(myFlocks.begin()+i);
        }
    }

    ofPoint loc = ofPoint(tuioObject.getX()*ofGetWidth(),tuioObject.getY()*ofGetHeight());
    cout << "Object n" << tuioObject.getSessionId() << " remove at " << loc << endl;
}


void testApp::tuioCursorAdded(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
    
    harvesters.push_back(trxHarvester(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight(),0,tuioCursor.getSessionId()));
	cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
}

void testApp::tuioCursorUpdated(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
    
    
    for (int i = 0; i < harvesters.size(); i++) {
        trxHarvester *thisHarvester = &harvesters[i];
        if (thisHarvester->id == tuioCursor.getSessionId()) {
            thisHarvester->position = ofVec3f(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight(),thisHarvester->position.z);
            ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
        }
    }
    
	cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
}

void testApp::tuioCursorRemoved(ofxTuioCursor &tuioCursor){
    for (int i = 0; i < harvesters.size(); i++) {
        trxHarvester thisHarvester = harvesters[i];
        if (thisHarvester.id == tuioCursor.getSessionId()) {
            harvesters.erase(harvesters.begin()+i);
        }
    }
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

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

bool isIDsame(trxFlock &flock,ofxTuioObject &tuio){
    return (flock.id == tuio.getFiducialId());
}

void testApp::drawGrid(){
    ofPushStyle();
    ofSetColor(255, 255, 255);
    ofSetLineWidth(20);
    ofLine(0, 0, 0, 1920, 0, 0);
    ofPopStyle();
    
}
