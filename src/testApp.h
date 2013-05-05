#pragma once

#include "ofMain.h"
#include "ofxBoids.h"
#include "ofxTuio.h"
#include "ofxBlur.h"
#include "general.h"
#include "trxObjectHandler.h"


class testApp : public ofBaseApp{

	public:    
    
		void setup();
		void update();
		void draw();
    
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    bool debug = false;
    
	ofCamera camera;
    //ofEasyCam camera;
    
    ofRectangle viewMain;

    
    trxObjectHandler myObjectHandler;
    
    ofImage background;
	ofTexture texture1;
    ofShader shader1;
    
    
    
    void	tuioCursorAdded(ofxTuioCursor & tuioCursor);
	void	tuioCursorRemoved(ofxTuioCursor & tuioCursor);
	void	tuioCursorUpdated(ofxTuioCursor & tuioCursor);
    
    void	tuioObjectAdded(ofxTuioObject & tuioObject);
	void	tuioObjectRemoved(ofxTuioObject & tuioObject);
	void	tuioObjectUpdated(ofxTuioObject & tuioObject);
    

    ofxTuioClient   tuioClient;
    

    
};
