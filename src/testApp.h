#pragma once

#include "ofMain.h"
#include "ofxBoids.h"
#include "ofxTuio.h"
#include "trxHarvester.h"
#include "trxConverter.h"
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
    void drawBoid(trxVehicle * _tmpBoid);
    drawBoidReturn getVertexData(trxVehicle * tmpBoid);
    bool isIDsame(trxFlock * flock);
	ofCamera camera;
    //ofEasyCam camera;
    
    ofRectangle viewMain;
    ofVbo vbo2;

    
    vector<trxFlock> myFlocks;
    vector<trxConverter> myConverters;
    vector<trxObject> myObjects;
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
    void catchBoid(trxHarvester * _myHarverster);
    
    vector<trxVehicle *> getAllBoidsFromFlocks(vector<trxFlock> * _myFlocks);
    
    int startTime;
    int timeStamp;
    
    ofxTuioClient   tuioClient;
    
    
    vector<trxHarvester> harvesters;    
    vector<ofTexture> textures;
    vector<ofImage> icons;
    vector<int> activeFlocks;
    vector<trxVehicle *> allMyBoids;
    
    bool debug;
    
    string imagePaths[6] = {"futter.png","Thunfisch.png","Shark.png","Turtle.png","Shrimps.png","Qualle.png"};
    
    
    vector<ofVec3f> allVertex;
    vector<ofIndexType> allIndex;
    vector<ofFloatColor> allColors;
    
    trxXML xml;
    
    ofxBlur blur;
    
    void updateMouseRay();
    ofVec2f getCorrectedPosition(float _x, float _y);
    ofVec3f ray[2];
    
    ofVec3f screenPosition(ofVec3f _position, ofCamera * cam);
    
};
