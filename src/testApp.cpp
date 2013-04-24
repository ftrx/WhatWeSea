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
    
    
    viewMain.x = 0;
	viewMain.y = 0;
	viewMain.width = ofGetWidth();
	viewMain.height = ofGetHeight();
	
    camera.setPosition(ofVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,1000));
    camera.lookAt(ofVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,0));
    camera.setFov(90);
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
    cout << xml.objectNumber <<endl;
    for (int i=0; i < xml.objectNumber; i++)
    {
        if(xml.objectNumber > 0){
            ofTexture tex;
            ofImage img;
            string texPath = "icons/" + xml.getString(i,"TEXTURE_PATH");
            string imgPath = "icon_figures/" + xml.getString(i,"ICON_PATH");
            cout << texPath << endl;
            cout << imgPath << endl;
            ofLoadImage(tex,texPath);
            textures.push_back(tex);
            ofLoadImage(img,imgPath);
            icons.push_back(img);
        }
    }
    
    for (int i=0; i<xml.objectNumber; i++) {
        string type = xml.getString(i, "Type");
        if ( type == "flock") {
            trxFlock thisFlock = trxFlock(ofGetWidth()/2.0,ofGetHeight()/2.0,0,xml.getIntValue(i, "ID"),&harvesters,xml.getIntValue(i, "START_BOIDS"));
            //thisFlock.color = colors[i];
            thisFlock.title = xml.getString(i, "NAME");
            thisFlock.boidNum = xml.getIntValue(i, "MAX_BOIDS");
            thisFlock.startBoidNum = xml.getIntValue(i, "START_BOIDS");
            thisFlock.maxSpeed = xml.getFloatValue(i, "MAX_SPEED");
            thisFlock.texture = &textures.at(i);
            thisFlock.myIcon = &icons.at(i);
            thisFlock.myCamera = &camera;
            thisFlock.myConnections = xml.getConnections(i);
            myFlocks.push_back(thisFlock);
        }
        else if(type == "converter")
        {
            trxConverter thisConverter = trxConverter(ofGetWidth()/2.0,ofGetHeight()/2.0,0,xml.getIntValue(i, "ID"));
            //thisFlock.color = colors[i];
            //thisConverter.title = xml.getString(i, "NAME");
            
            thisConverter.title = xml.getString(i, "NAME");
            thisConverter.myConnections = xml.getConnections(i);
            thisConverter.myIcon = &icons.at(i);
            myConverters.push_back(thisConverter);
        }
        
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
    //updateMouseRay();
    allVertex.clear();
    allIndex.clear();
    allColors.clear();
    tuioClient.getMessage();
    float timer = ofGetElapsedTimeMillis() - timeStamp;
    
    allMyBoids = getAllBoidsFromFlocks(&myFlocks);
    
    for (int i=0; i<harvesters.size();i++){
            catchBoid(&harvesters[i]);

        
    }
    
    
    // 16.66f is abaout 1000/60 so about 60FPS
    if(timer>15.0f){
        for (int i = 0; i<myFlocks.size(); i++) {
            myFlocks[i].update();
        }
        timeStamp = ofGetElapsedTimeMillis();
    }
    
    
    //blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 1, 4));
    for (int i = 0; i<allMyBoids.size(); i++) {
        
        
        trxVehicle * tmpBoid = allMyBoids.at(i);
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
        int maxTrail = tmpBoid->maxTrailSize;
        int firstBone = int(maxTrail/8*6);
        int secondBone = int(maxTrail/8*3);
        if(tmpBoid->trails.size()>= maxTrail){
            ofVec3f vecs[] = {
                ofVec3f((float)tmpBoid->position.x,  (float)tmpBoid->position.y,  (float)tmpBoid->position.z),
                ofVec3f((float)tmpBoid->trails.at(firstBone).x-5.0f,  (float)tmpBoid->trails.at(firstBone).y,  (float)tmpBoid->trails.at(firstBone).z),
                ofVec3f((float)tmpBoid->trails.at(firstBone).x+5.0f,  (float)tmpBoid->trails.at(firstBone).y,  (float)tmpBoid->trails.at(firstBone).z),
                ofVec3f((float)tmpBoid->trails.at(secondBone).x,  (float)tmpBoid->trails.at(secondBone).y,  (float)tmpBoid->trails.at(secondBone).z),
                ofVec3f((float)tmpBoid->trails.at(0).x-3.0f,  (float)tmpBoid->trails.at(0).y,  (float)tmpBoid->trails.at(0).z),
                ofVec3f((float)tmpBoid->trails.at(0).x+3.0f,  (float)tmpBoid->trails.at(0).y,  (float)tmpBoid->trails.at(0).z)};
            
            ofFloatColor vColor[6];
            float alpha = 1.0;
            if (activeFlocks.size() > 0) {
               alpha = 0.2;
                    for (int active=0; active<activeFlocks.size(); active++) {
                        if (activeFlocks.at(active) == tmpBoid->myTypeID) {
                            alpha = 1.0;
                        }
                        
                    }
                
            }
            
            for (int col=0; col<6; col++) {
                if (debug) {
                    if (tmpBoid->caught) {
                        
                        vColor[col].r = 1.0;
                        vColor[col].g = 0.0;
                        vColor[col].b = 0.0;
                    }
                    else {
                        vColor[col].r = 0.0;
                        vColor[col].g = 1.0;
                        vColor[col].b = 0.0;
                    }
                }
                else
                {
                    if (tmpBoid->caught) {
                        
                        vColor[col].r = 1.0;
                        vColor[col].g = 1.0;
                        vColor[col].b = 1.0;
                    }
                    else {
                        vColor[col].r = 1.0;
                        vColor[col].g = 1.0;
                        vColor[col].b = 1.0;
                    }
                }
                vColor[col].a = alpha;
            }
            
            ofIndexType faces[] = {
                0+i*VERTEXNUMBER, 1+i*VERTEXNUMBER, 2+i*VERTEXNUMBER,
                1+i*VERTEXNUMBER, 2+i*VERTEXNUMBER, 3+i*VERTEXNUMBER,
                3+i*VERTEXNUMBER, 4+i*VERTEXNUMBER, 5+i*VERTEXNUMBER };
            
            for (int j=0; j<VERTEXNUMBER; j++) {
                allVertex.push_back(vecs[j]);

            }
            for (int j=0; j<6; j++) {
                allColors.push_back(vColor[j]);
                
            }
            for (int j=0; j<9; j++) {
                allIndex.push_back(faces[j]);
                
            }
            
        }
    }

    if (allVertex.size()>0) {
        //cout << "allVertex.size()" << allVertex.size() << endl;
        vbo2.setVertexData( &allVertex[0], allVertex.size(), GL_DYNAMIC_DRAW );
        vbo2.setColorData(&allColors[0], allColors.size(), GL_DYNAMIC_DRAW);
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
    
    for (int i = 0; i<myFlocks.size(); i++) {
        ofPushMatrix();
        if (myFlocks[i].isActive){
            trxFlock * flock = &myFlocks[i];
            ofTranslate(flock->position.x,ofGetHeight()-flock->position.y,0);
            ofRotate(flock->rotation);
            flock->draw();
            int connectionSize = flock->myConnections.size();
            for (int j = 0; j < connectionSize; j++) {
                int index = flock->myConnections.at(j)-myFlocks.size();
                trxConverter * converter = &myConverters.at(index);
                if (!converter->isActive)
                {
                    ofPushMatrix();
                    ofRotate(90.0*j);
                    ofTranslate(200, 0);
                    converter->drawAsConnection();
                    ofPopMatrix();
                }
            }
        }
        ofPopMatrix();
        
    }
    for (int i = 0; i<myConverters.size(); i++) {
        
        ofPushMatrix();
        if (myConverters[i].isActive){
            trxConverter * converter = &myConverters[i];
            ofTranslate(converter->position.x,ofGetHeight()-converter->position.y,0);
            ofRotate(converter->rotation);
            converter->draw();
            int connectionSize = converter->myConnections.size();
            for (int j = 0; j < connectionSize; j++) {
                int index = converter->myConnections.at(j);
                trxFlock * flock = &myFlocks.at(index);
                if (!flock->isActive)
                {
                    int dist = 200;
                    float x = dist*sin(ofDegToRad(90.0*i));
                    float y = dist*cos(ofDegToRad(90.0*i));
                    
                    ofSetLineWidth(2.0);
                    ofLine(0, 0, x, y);
                    
                    ofPushMatrix();
                    ofRotate(90.0*j);
                    ofTranslate(200, 0);
                    flock->drawAsConnection();
                    ofPopMatrix();
                }
            }
        }
        ofPopMatrix();

        
    }

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
        for (int i=0; i<harvesters.size(); i++) {
            harvesters.at(i).drawInfo();
            
        }
      
        
        
        camera.end();
        ofSetColor(0,255,255, 100);
        //ofCircle(ray[0].x,ray[0].y,0, 50);
        
        ofRect(0, 0, 250, 90);

        ofSetColor(0);
        string info = "FPS "+ofToString(ofGetFrameRate(), 0) + "\n";
        info += "Total Points "+ofToString(allMyBoids.size())+"\n";
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

        default:
            break;
    }
 
}

//----- Eventlistener

void testApp::tuioObjectAdded(ofxTuioObject & tuioObject){
    ofPoint loc = ofPoint(getCorrectedPosition( tuioObject.getX(),tuioObject.getY()));
    
   // trxFlock thisFlock = trxFlock(tuioObject.getX()*ofGetWidth(),tuioObject.getY()*ofGetHeight(),ofRandom(0, 600.0f),tuioObject.getFiducialId(),&harvesters,xml.getIntValue(tuioObject.getFiducialId(), "START_BOIDS"));
    //thisFlock.texture = &textures.at(myFlocks.size());
    //myFlocks.push_back(thisFlock);
    
    
    if (myFlocks.size() > tuioObject.getFiducialId()) {
        trxFlock * thisFlock = &myFlocks.at(tuioObject.getFiducialId());
        thisFlock->position = ofVec3f(loc.x,loc.y,myFlocks.at(tuioObject.getFiducialId()).position.z);
        thisFlock->isActive = true;
        thisFlock->rotation = ofRadToDeg(tuioObject.getAngle());
        //cout << "Object n" << tuioObject.getSessionId() << " add at " << loc << endl;
        activeFlocks.push_back(tuioObject.getFiducialId());
    }
    if (tuioObject.getFiducialId() >= myFlocks.size()) {
        int atPos = tuioObject.getFiducialId()-myFlocks.size();
        trxConverter * thisConverter = &myConverters.at(atPos);
        thisConverter->position = ofVec3f(loc.x,loc.y,myFlocks.at(atPos).position.z);
        thisConverter->isActive = true;
        thisConverter->rotation = ofRadToDeg(tuioObject.getAngle());
        //cout << "Object n" << tuioObject.getSessionId() << " add at " << loc << endl;
        
    }



}

void testApp::tuioObjectUpdated(ofxTuioObject & tuioObject){
    
    ofPoint loc = ofPoint(getCorrectedPosition( tuioObject.getX(),tuioObject.getY()));
    if (myFlocks.size() > tuioObject.getFiducialId()) {
        trxFlock * thisFlock = & myFlocks.at(tuioObject.getFiducialId());
        thisFlock->position = ofVec3f(loc.x,loc.y,myFlocks.at(tuioObject.getFiducialId()).position.z);
        thisFlock->unprojectedPosition = screenPosition(thisFlock->position, &camera);
        thisFlock->rotation = ofRadToDeg(tuioObject.getAngle());
        //cout << "Object n" << tuioObject.getSessionId() << " updated at " << loc << endl;
        //cout << "angle: " << tuioObject.getAngle() << endl;
    }
    if (tuioObject.getFiducialId() >= myFlocks.size()) {
        int atPos = tuioObject.getFiducialId()-myFlocks.size();
        trxConverter * thisConverter = &myConverters.at(atPos);
        thisConverter->position = ofVec3f(loc.x,loc.y,myFlocks.at(atPos).position.z);
        thisConverter->unprojectedPosition = screenPosition(thisConverter->position, &camera);
        thisConverter->rotation = ofRadToDeg(tuioObject.getAngle());
        //cout << "Object n" << tuioObject.getSessionId() << " add at " << loc << endl;
        
    }
}

void testApp::tuioObjectRemoved(ofxTuioObject & tuioObject){
    ofPoint loc = ofPoint(getCorrectedPosition( tuioObject.getX(),tuioObject.getY()));
    if (myFlocks.size() > tuioObject.getFiducialId()) {
        trxFlock * thisFlock = &myFlocks.at(tuioObject.getFiducialId());
        thisFlock->isActive = false;
    }
    if (tuioObject.getFiducialId() >= myFlocks.size()) {
        int atPos = tuioObject.getFiducialId()-myFlocks.size();
        trxConverter * thisConverter = &myConverters.at(atPos);
        thisConverter->isActive = false;
        //cout << "Object n" << tuioObject.getSessionId() << " add at " << loc << endl;
        
    }
    
    
    for (int i = 0; i < activeFlocks.size(); i++) {
        if (activeFlocks[i] == tuioObject.getFiducialId()) {
            activeFlocks.erase(activeFlocks.begin()+i);
        }
    }

    
    //cout << "Object n" << tuioObject.getSessionId() << " remove at " << loc << endl;
}


void testApp::tuioCursorAdded(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(getCorrectedPosition(tuioCursor.getX(),tuioCursor.getY()));
    trxHarvester thisHarvester = trxHarvester(loc.x,loc.y,DEPTH,tuioCursor.getSessionId());
    thisHarvester.unprojectedPosition = thisHarvester.screenPosition(&camera);
    harvesters.push_back(thisHarvester);
	//cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
}

void testApp::tuioCursorUpdated(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(getCorrectedPosition(tuioCursor.getX(),tuioCursor.getY()));
    for (int i = 0; i < harvesters.size(); i++) {
        trxHarvester *thisHarvester = &harvesters[i];
        
        if (thisHarvester->id == tuioCursor.getSessionId()) {
            thisHarvester->unprojectedPosition = thisHarvester->screenPosition(&camera);
            thisHarvester->position = ofVec3f(loc.x,loc.y,thisHarvester->position.z);
        }
    }
    
	//cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
}

void testApp::tuioCursorRemoved(ofxTuioCursor &tuioCursor){
    ofPoint loc = ofPoint(getCorrectedPosition(tuioCursor.getX(),tuioCursor.getY()));
    for (int i = 0; i < harvesters.size(); i++) {
        trxHarvester * thisHarvester = &harvesters[i];
        if (thisHarvester->id == tuioCursor.getSessionId()) {
            thisHarvester->clearCatch();
            harvesters.erase(harvesters.begin()+i);
            
        }
    }
	
	//cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
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

vector<trxVehicle *> testApp::getAllBoidsFromFlocks(vector<trxFlock> * _myFlocks){
    vector<trxVehicle *> allBoids;
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
    ofEnableAlphaBlending();
    shader1.begin();
    //shader1.setUniform1f("zPosition", ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 600));
    //textures[0].bind();
    vbo2.drawElements( GL_TRIANGLES, allIndex.size());
    //textures[0].unbind();
    shader1.end();
    ofDisableAlphaBlending();
    camera.end();
    glDisable(GL_DEPTH_TEST);
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


void testApp::drawBoid(trxVehicle * tmpBoid){

    
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



drawBoidReturn testApp::getVertexData(trxVehicle * tmpBoid){
    
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

bool testApp::isIDsame(trxFlock * flock)
{
    bool ret = 0;
    for (int i=0; i<activeFlocks.size(); i++) {
        if (flock->id == activeFlocks[i]) {
            ret = 1;
            break;
        }
    }
    return ret;
}

void testApp::updateMouseRay(){
	// Define ray in screen space
    
    ofVec2f mouse = ofVec2f(ofGetMouseX(),ofGetMouseY());
    
    ray[0] = ofVec3f(mouse.x, mouse.y, 0.777);
    ray[1] = ofVec3f(mouse.x, mouse.y, 0.370);
    if (harvesters.size()>0){
	//ray[0] = ofVec3f(harvesters[0].position.x, harvesters[0].position.y, 0);
    
	//ray[1] = ofVec3f(harvesters[0].position.x, harvesters[0].position.y, 0);
    
	// Transform ray into world space
	
    }
    ray[0] = camera.screenToWorld(ray[0]);
	ray[1] = camera.screenToWorld(ray[1]);
}

void testApp::catchBoid(trxHarvester * _myHarverster)
{
    for (int i=0; i<allMyBoids.size(); i++){
        
        trxVehicle * boid = allMyBoids.at(i);
        
        if (_myHarverster->myCatch.size() < 20 && !boid->caught) {
        ofVec3f bPos= boid->position;
        ofVec3f hPos= ofVec3f (_myHarverster->position.x,ofGetHeight()-_myHarverster->position.y,0);
        bPos = camera.worldToScreen(bPos);
        //hPos = camera.screenToWorld(hPos, viewMain);
        float dist = hPos.distance(bPos);
        if (dist <= _myHarverster->radius) {
            _myHarverster->myCatch.push_back(boid);
            boid->addTarget(&_myHarverster->unprojectedPosition);
            boid->caught = true;
        }
        if (dist <= 2*(_myHarverster->radius) && !boid->caught)
        {
            boid->fleeTargets.push_back(&_myHarverster->position);
        }
        }
    }
}

ofVec2f testApp::getCorrectedPosition(float _x, float _y){
    float x = ofMap(_x, 0.0+SCREEN_X_MIN, 1.0-SCREEN_X_MAX, 0.0, 1.0)*ofGetWidth();
    float y = ofMap(_y, 0.0+SCREEN_Y_MIN, 1.0-SCREEN_Y_MAX, 0.0, 1.0)*ofGetHeight();
    
    return ofVec2f(x,y);
    
}

ofVec3f testApp::screenPosition(ofVec3f _position, ofCamera * cam){
    ofVec3f pos= ofVec3f (_position.x,_position.y,_position.z);
    pos = cam->worldToScreen(pos,viewMain);
    return pos;
}

