#pragma once

#include "ofMain.h"
#include "ofxBoids.h"
#include "ofxTuio.h"
#include "trxHarvester.h"
#include "ofxBlur.h"
#include "trxXML.h"
#include "general.h"


class testApp : public ofBaseApp{

	public:
        static const GLfloat boidVert[];
        static const GLubyte boidIndices[];
    
    
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
	
	void drawAllBoids();
    void drawOld();
    void drawBoid(BiologicalVehicle * _tmpBoid);
    drawBoidReturn getVertexData(BiologicalVehicle * tmpBoid);
    
	//ofCamera camera;
    ofEasyCam camera;
    ofVbo vbo2;

    
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
    
    
    vector<BiologicalVehicle *> getAllBoidsFromFlocks(vector<trxFlock> * _myFlocks);
    
    int startTime;
    int timeStamp;
    
    ofxTuioClient   tuioClient;
    
    
    vector<trxHarvester> harvesters;    
    vector<ofTexture> textures;
    
    vector<BiologicalVehicle *> allMyBoids;
    
    bool debug;
    
    string imagePaths[6] = {"futter.png","Thunfisch.png","Shark.png","Turtle.png","Shrimps.png","Qualle.png"};
    
    
    vector<ofVec3f> allVertex;
    vector<ofIndexType> allIndex;
    
    trxXML xml;
    
    ofxBlur blur;
    
    
};
