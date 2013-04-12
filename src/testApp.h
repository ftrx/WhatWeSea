#pragma once

#include "ofMain.h"
#include "ofxBoids.h"
#include "ofxTuio.h"
#include "trxHarvester.h"
#include "ofxBlur.h"


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        void    drawGrid();
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
	
	ofCamera camera;
    ofVbo vbo;
    vector<trxFlock> myFlocks;
    ofImage background;
	ofTexture texture1;
    ofShader shader1;
    
    void	tuioCursorAdded(ofxTuioCursor & tuioCursor);
	void	tuioCursorRemoved(ofxTuioCursor & tuioCursor);
	void	tuioCursorUpdated(ofxTuioCursor & tuioCursor);
    
    void	tuioObjectAdded(ofxTuioObject & tuioObject);
	void	tuioObjectRemoved(ofxTuioObject & tuioObject);
	void	tuioObjectUpdated(ofxTuioObject & tuioObject);
    
    
    
    bool    isIDsame(trxFlock &flock,ofxTuioObject &tuio);
    
    int startTime;
    int timeStamp;
    
    ofxTuioClient   tuioClient;
    
    
    vector<trxHarvester> harvesters;
    
    vector<ofTexture> textures;
    
    bool debug;
    
    string imagePaths[6] = {"futter.png","Thunfisch.png","Shark.png","Turtle.png","Shrimps.png","Qualle.png"};
    
    ofxBlur blur;
};
