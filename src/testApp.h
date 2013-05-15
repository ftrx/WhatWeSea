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
    bool lightning = false;
    
    
    //float runningTime = 0.0f;
    
	ofCamera camera;
    //ofEasyCam camera;
    
    ofRectangle viewMain;

    struct activeTuioObject{
        bool active = false;
        float firstAdded = 0.0f;
        float lastTimeSeen = 0.0f;
        ofxTuioObject * tuioObject = NULL;
    };
    
    struct activeTuioCursor{
        bool active = false;
        float firstAdded = 0.0f;
        float lastTimeSeen = 0.0f;
        ofxTuioCursor * tuioCursor = NULL;
        ofxTuioCursor lastTuioCursor = ofxTuioCursor(0, 0, 0, 0);
    };
    
    
    vector<activeTuioCursor> activeCursors;
    vector<activeTuioCursor> onRemoveCursors;
    
    void addNewCursor();
    void removeNewCursor();
    
    bool isCursorStillThere(ofxTuioCursor & tuioCursor);
    void addCursorToRemovable(ofxTuioCursor & tuioCursor);
    void removeDeadCursors();
    
    trxObjectHandler myObjectHandler;
    
    ofImage background;
	ofTexture texture1;
    ofShader shader1;
    
    ofLight light;
    
    
    void	tuioCursorAdded(ofxTuioCursor & tuioCursor);
	void	tuioCursorRemoved(ofxTuioCursor & tuioCursor);
	void	tuioCursorUpdated(ofxTuioCursor & tuioCursor);
    
    void	tuioObjectAdded(ofxTuioObject & tuioObject);
	void	tuioObjectRemoved(ofxTuioObject & tuioObject);
	void	tuioObjectUpdated(ofxTuioObject & tuioObject);
    

    ofxTuioClient   tuioClient;
    

    
};
