#include "testApp.h"

const GLfloat testApp::boidVert[] = {0.0, 0.0, 0.0,
    -2.5, 0.0, 0.0,
    2.5, 0.0, 0.0,
    0.0, -2.5, 0.0,
    0.0, 2.5, 0.0,
    0.0, 0.0, 10.0};

const GLubyte testApp::boidIndices[] = {4,1,0,
    3,1,0,
    4,2,0,
    3,2,0,
    4,5,2,
    4,5,1,
    3,5,2,
    3,5,1};


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
	//ofEnableAlphaBlending();
	//ofEnableNormalizedTexCoords();
	//ofHideCursor();
	
    //ofDisableArbTex();
	
    camera.setPosition(ofVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,1000));
    camera.lookAt(ofVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,0));
    camera.setFov(60);
    camera.setNearClip(100.0);
    camera.setFarClip(-1000.0);
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
        trxFlock thisFlock = trxFlock(ofGetWidth()/2.0,ofGetHeight()/2.0,300.0,xml.getIntValue(i, "ID"),&harvesters,xml.getIntValue(i, "START_BOIDS"));
        //thisFlock.color = colors[i];
        thisFlock.boidNum = xml.getIntValue(i, "MAX_BOIDS");
        thisFlock.startBoidNum = xml.getIntValue(i, "START_BOIDS");
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
    allMyBoids = getAllBoidsFromFlocks(&myFlocks);
}

//--------------------------------------------------------------
void testApp::update()
{
    allVertex.clear();
    allIndex.clear();
    tuioClient.getMessage();
    float timer = ofGetElapsedTimeMillis() - timeStamp;
    
    allMyBoids = getAllBoidsFromFlocks(&myFlocks);
    
    // 16.66f is abaout 1000/60 so about 60FPS
    if(timer>15.0f){
        for (int i = 0; i<myFlocks.size(); i++) {
            myFlocks[i].update();
        }
        timeStamp = ofGetElapsedTimeMillis();
    }
    
    
    //blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 1, 4));
    for (int i = 0; i<allMyBoids.size(); i++) {
        
        
        BiologicalVehicle * tmpBoid = allMyBoids.at(i);
        /*
        if(tmpBoid->trails.size()> 3){
            //drawBoid(tmpBoid);
            drawBoidReturn temp = getVertexData(tmpBoid);
            int size = sizeof(temp.verts);
            for (int j=0; j<3; j++) {
                ofVec3f tmp = temp.verts[j];
                allVertex.push_back(tmp);
                int tempVector = allVertex.size();
                
            }
            for (int j=0; j<3; j++) {
                ofIndexType tmp = temp.faces[j]+3*j;
                allIndex.push_back(tmp);
            }
        }
         */
        if(tmpBoid->trails.size()> 7){
            ofVec3f vecs[] = {
                ofVec3f((float)tmpBoid->position.x,  (float)tmpBoid->position.y,  (float)tmpBoid->position.z),
                ofVec3f((float)tmpBoid->trails.at(6).x-2.0f,  (float)tmpBoid->trails.at(6).y,  (float)tmpBoid->trails.at(6).z),
                ofVec3f((float)tmpBoid->trails.at(6).x+2.0f,  (float)tmpBoid->trails.at(6).y,  (float)tmpBoid->trails.at(6).z),
                ofVec3f((float)tmpBoid->trails.at(3).x,  (float)tmpBoid->trails.at(3).y,  (float)tmpBoid->trails.at(3).z),
                ofVec3f((float)tmpBoid->trails.at(0).x-2.0f,  (float)tmpBoid->trails.at(0).y,  (float)tmpBoid->trails.at(0).z),
                ofVec3f((float)tmpBoid->trails.at(0).x+2.0f,  (float)tmpBoid->trails.at(0).y,  (float)tmpBoid->trails.at(0).z)};
            
            ofIndexType faces[] = {
                0+i*VERTEXNUMBER, 1+i*VERTEXNUMBER, 2+i*VERTEXNUMBER,
                1+i*VERTEXNUMBER, 2+i*VERTEXNUMBER, 3+i*VERTEXNUMBER,
                3+i*VERTEXNUMBER, 4+i*VERTEXNUMBER, 5+i*VERTEXNUMBER };
            
            for (int j=0; j<VERTEXNUMBER; j++) {
                allVertex.push_back(vecs[j]);

            }
            for (int j=0; j<9; j++) {
                allIndex.push_back(faces[j]);
                
            }
            
        }
    }

    if (allVertex.size()>0) {
        //cout << "allVertex.size()" << allVertex.size() << endl;
        vbo2.setVertexData( &allVertex[0], allVertex.size(), GL_DYNAMIC_DRAW );
        
        vbo2.setIndexData( &allIndex[0], allIndex.size(), GL_DYNAMIC_DRAW );
        
        //vbo2.drawElements( GL_TRIANGLES, allIndex.size());
       
    }
    blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 1, 4));
	blur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofSetColor(255, 255, 255);

    background.draw(0,0, ofGetWidth(), ofGetHeight());
    
    //drawOld();

    drawAllBoids();

    for (int i=0; i<harvesters.size(); i++) {
        harvesters.at(i).draw();
    }

    
    if(debug)
    {
        ofPushStyle();
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
        info += "Total Points "+ofToString(allMyBoids.size())+"\n";
        info += "Press 'd' to debug\n";
        
        ofDrawBitmapString(info, 20, 20);
        tuioClient.drawCursors();
        tuioClient.drawObjects();
        ofPopStyle();
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
    
    trxFlock thisFlock = trxFlock(tuioObject.getX()*ofGetWidth(),tuioObject.getY()*ofGetHeight(),ofRandom(0, 600.0f),tuioObject.getFiducialId(),&harvesters,xml.getIntValue(tuioObject.getFiducialId(), "START_BOIDS"));
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
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    ofSetLineWidth(20);
    ofLine(0, 0, 0, 1920, 0, 0);
    ofDisableAlphaBlending();
    ofPopStyle();
    
}

vector<BiologicalVehicle *> testApp::getAllBoidsFromFlocks(vector<trxFlock> * _myFlocks){
    vector<BiologicalVehicle *> allBoids;
    for(int i=0; i< _myFlocks->size(); i++)
    {
        trxFlock * tmpFlock = &_myFlocks->at(i);
        for (int j=0; j<tmpFlock->boids.size();j++)
        {
            allBoids.push_back(&tmpFlock->boids.at(j));
        }
        
    }
    return allBoids;
    
    
    
}
void testApp::drawAllBoids(){
    camera.begin();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    //shader1.begin();
    //shader1.setUniform1f("zPosition", ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 600));
    //textures[0].bind();
    vbo2.drawElements( GL_TRIANGLES, allIndex.size());
    //textures[0].unbind();
    //shader1.end();
    camera.end();
}
/*
void testApp::drawAllBoids(){
    //ofEnableAlphaBlending();
    //ofSetColor(255, 255, 0,255);
	//ofEnablePointSprites();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    
    //ofCircle(50,50, 20);
    //camera.begin();
    //shader1.begin();
    
    //ofPushMatrix();
    //shader1.setUniform1f("zPosition", ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 600));
    
    allVertex.clear();
    allIndex.clear();
    
    float h = 4.0;
    float w = 4.0;
    for (int i = 0; i<allMyBoids.size(); i++) {

        
        BiologicalVehicle * tmpBoid = allMyBoids.at(i);
        
        if(tmpBoid->trails.size()> 3){
        //drawBoid(tmpBoid);
            drawBoidReturn temp = getVertexData(tmpBoid);
            for (int j=0; j<sizeof(temp.verts); j++) {
                allVertex.push_back(temp.verts[j]);

            }
            for (int j=0; j<sizeof(temp.faces); j++) {
                allIndex.push_back(temp.faces[j]+3*j);
            }
        }
        
        //textures.at(allMyBoids.at(i)->myTypeID).bind();
        
        ofPushMatrix();
            glTranslatef(tmpBoid->position.x, tmpBoid->position.y, tmpBoid->position.z);

            
            
            glColor4f(1.0, 1.0, 1.0, 1.0);
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, boidVert);
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, boidIndices);
            glDisableClientState(GL_VERTEX_ARRAY);
             
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);  glVertex2f(0, 0);
            glTexCoord2f(0, h);  glVertex2f(0, h);
            glTexCoord2f(w, h);  glVertex2f(w, h);
            glTexCoord2f(w, 0);  glVertex2f(w, 0);
            glEnd();
             
        
        
        ofPopMatrix();
        
        //textures.at(allMyBoids.at(i)->myTypeID).unbind();
        
        
        //glEnable(GL_DEPTH_TEST);
     
    }
    if (allVertex.size()>0) {
        vbo2.setVertexData( &allVertex[0], allVertex.size(), GL_STATIC_DRAW );
        
        vbo2.setIndexData( &allIndex[0], allIndex.size(), GL_STATIC_DRAW );
        
        vbo2.drawElements( GL_TRIANGLES, allIndex.size());
    }
    
    //ofPopMatrix();
    
    //shader1.end();
    //camera.end();
	//ofDisablePointSprites();
    //ofDisableAlphaBlending();

}
*/
void testApp::drawOld(){
    
    //int totalPoints = 0;
    
    glEnable(GL_DEPTH_TEST);
    //glAlphaFunc(GL_GREATER, 0.1);
    glEnable(GL_ALPHA_TEST);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glAlphaFunc(GL_GREATER, 1);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255,255);
	ofEnablePointSprites();
    
    
    //ofCircle(50,50, 20);
    camera.begin();
    shader1.begin();
    
    ofPushMatrix();
    shader1.setUniform1f("zPosition", ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 600));
    for (int i = 0; i<myFlocks.size(); i++) {
        
        
        myFlocks[i].draw();
        //myFlocks[i].drawCircles();
        
        //totalPoints += myFlocks[i].points.size();
        
        
        
    }
    ofPopMatrix();
    
    shader1.end();
    camera.end();
	ofDisablePointSprites();
	//ofDisableBlendMode();
    ofDisableAlphaBlending();

}


void testApp::drawBoid(BiologicalVehicle * tmpBoid){

    
    ofVec3f verts[] = {
        ofVec3f((float)tmpBoid->position.x-1.0f,  (float)tmpBoid->position.y-1.0f,  (float)tmpBoid->position.z-1.0f),
        ofVec3f((float)tmpBoid->position.x+1.0f,  (float)tmpBoid->position.y+1.0f,  (float)tmpBoid->position.z+1.0f),
        ofVec3f((float)tmpBoid->trails.at(0).x,  (float)tmpBoid->trails.at(0).y,  (float)tmpBoid->trails.at(0).z) };
    
    ofIndexType faces[] = {
        0, 1, 2 };
    
    
    vbo2.setVertexData( &verts[0], 3, GL_STATIC_DRAW );

    vbo2.setIndexData( &faces[0], 3, GL_STATIC_DRAW );
    
    vbo2.drawElements( GL_TRIANGLES, 3);

}



drawBoidReturn testApp::getVertexData(BiologicalVehicle * tmpBoid){
    
    drawBoidReturn ret;
    ofVec3f vecs[] = {
        ofVec3f((float)tmpBoid->position.x-1.0f,  (float)tmpBoid->position.y-1.0f,  (float)tmpBoid->position.z-1.0f),
        ofVec3f((float)tmpBoid->position.x+1.0f,  (float)tmpBoid->position.y+1.0f,  (float)tmpBoid->position.z+1.0f),
        ofVec3f((float)tmpBoid->trails.at(0).x,  (float)tmpBoid->trails.at(0).y,  (float)tmpBoid->trails.at(0).z) };
    
    ofIndexType faces[] = {
        0, 1, 2 };
    
    for (int i=0;i<sizeof(vecs);i++){
        ret.verts[i] = vecs[i];
    }
    
    for (int i=0;i<sizeof(faces);i++){
        ret.faces[i] = faces[i];
    }

    
    return ret;
        
}

