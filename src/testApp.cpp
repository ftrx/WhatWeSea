#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{
    //myObjectHandler = trxObjectHandler();
    myObjectHandler.myCamera = &camera;
    
	ofBackground(20,20,40);
    //ofSetBackgroundAuto(true);
    background.loadImage("background.jpg");
    
	//ofSetFrameRate(90);
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
     
//    light.disable();
//    ofEnableLighting();
 //   light.setPointLight();
  //  light.setPosition(ofGetWidth(),ofGetHeight(),900);
}

//--------------------------------------------------------------
void testApp::update()
{
    //updateMouseRay();
    
    tuioClient.getMessage();
    myObjectHandler.debug = debug;
    myObjectHandler.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofSetColor(255, 255, 255);
    
    background.draw(0,0, ofGetWidth(), ofGetHeight());
    
    //drawOld();
    
    camera.begin();
    
    shader1.begin();

    myObjectHandler.drawAllBoids();
//    if(lightning) light.draw();
    shader1.end();

    myObjectHandler.draw3D();
    camera.end();
    
    
    int health = ofMap(myObjectHandler.myFlocks.at(4).boids.size(), 0, myObjectHandler.allMyBoids.size(), 0, 100);
    
    ofEnableAlphaBlending();
    ofSetColor(25, 0, 0, health);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofDisableAlphaBlending();
    
    myObjectHandler.draw();
    
    if(debug)
    {

        ofPushStyle();
        
        camera.begin();
        ofDrawGrid(1920, 10, true, true, true, true);
        camera.end();
        
        ofSetColor(255,255,255, 100);
        ofRect(0, 0, 250, 90);
        ofSetColor(0);
        string info = "FPS "+ofToString(ofGetFrameRate(), 0) + "\n";
        info += "Total Points "+ofToString(myObjectHandler.allMyBoids.size())+"\n";
        info += "ofGetElapsedTimef() "+ofToString(ofGetElapsedTimef())+"\n";
        info += "Press 'd' to debug\n";
        ofDrawBitmapString(info, 20, 20);
        ofPopStyle();
        
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
        case 'l':{
            lightning = !lightning;
            if (lightning){
                light.enable();
            }
            else{
                light.disable();
            }
            
            break;
        }


        default:
            break;
    }
 
}

//----- Eventlistener

void testApp::tuioObjectAdded(ofxTuioObject & tuioObject)
{
    myObjectHandler.addObject( tuioObject);
}

void testApp::tuioObjectUpdated(ofxTuioObject & tuioObject)
{
    myObjectHandler.updateObject(tuioObject);
}

void testApp::tuioObjectRemoved(ofxTuioObject & tuioObject){
    myObjectHandler.removeObject(tuioObject);
    //cout << "Object n" << tuioObject.getSessionId() << " remove at " << loc << endl;
}

void testApp::tuioCursorAdded(ofxTuioCursor &tuioCursor){
	myObjectHandler.addCursor(tuioCursor);
}

void testApp::tuioCursorUpdated(ofxTuioCursor &tuioCursor){
    myObjectHandler.updateCursor(tuioCursor);
	}

void testApp::tuioCursorRemoved(ofxTuioCursor &tuioCursor){
    myObjectHandler.removeCursor(tuioCursor);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    light.setPosition(600,ofGetMouseY(),ofGetMouseX());
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






