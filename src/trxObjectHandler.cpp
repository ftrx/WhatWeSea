//
//  trxObjectHandler.cpp
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 24.04.13.
//
//

#include "trxObjectHandler.h"


trxObjectHandler::trxObjectHandler()
{
    initXML();
    generateObjects();
    allMyBoids = getAllBoidsFromFlocks(&myFlocks);
    timeStamp = ofGetElapsedTimeMillis();
}


void trxObjectHandler::update()
{
    allMyBoids = getAllBoidsFromFlocks(&myFlocks);
    allVertex.clear();
    allIndex.clear();
    allColors.clear();
    
    float timer = ofGetElapsedTimeMillis() - timeStamp;
    
    for (int i=0; i<harvesters.size();i++){
        catchBoid(&harvesters[i]);
    }
    
    
    // 16.66f is about 1000/60 so about 60FPS
    if(timer>16.6f){
        for (int i = 0; i<myFlocks.size(); i++) {
            myFlocks[i].update();
        }
        timeStamp = ofGetElapsedTimeMillis();
    }
    
    
    //blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 1, 4));
    updateAllVertexes();
    
    if (allVertex.size()>0) {
        //cout << "allVertex.size()" << allVertex.size() << endl;
        vbo.setVertexData( &allVertex[0], allVertex.size(), GL_DYNAMIC_DRAW );
        vbo.setColorData(&allColors[0], allColors.size(), GL_DYNAMIC_DRAW);
        vbo.setIndexData( &allIndex[0], allIndex.size(), GL_DYNAMIC_DRAW );
        //vbo2.drawElements( GL_TRIANGLES, allIndex.size());
        
    }

}

void trxObjectHandler::draw()
{
    ofPushStyle();
    
    for (int i = 0; i<activeFlocks.size(); i++) {
        ofPushMatrix();
        trxFlock * flock = activeFlocks[i];
        ofTranslate(flock->position.x,ofGetHeight()-flock->position.y,0);
        ofRotate(flock->rotation);
        flock->draw();
        for (int j=0; j<activeConverters.size(); j++) {
            trxConverter * converter = activeConverters[j];
            int dist = 200;
            float x = dist*sin(ofDegToRad(90.0*i));
            float y = dist*cos(ofDegToRad(90.0*i));
                    
            ofSetLineWidth(2.0);
            ofLine(0, 0, x, y);
            ofPushMatrix();
            ofRotate(90.0*j);
            ofTranslate(200, 0);
            converter->drawAsConnection();
            ofPopMatrix();
              
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

    
    ofPopStyle();
}

void trxObjectHandler::draw3D(){
    //drawAllBoids();
    
    if(debug){
        for (int i = 0; i<myFlocks.size(); i++) {
            myFlocks[i].drawInfo();
        }
        for (int i=0; i<harvesters.size(); i++) {
            harvesters.at(i).drawInfo();
            
        }
    }

}


void trxObjectHandler::drawAllBoids(){

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    ofEnableAlphaBlending();

    //shader1.setUniform1f("zPosition", ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 600));
    //textures[0].bind();
    vbo.drawElements( GL_TRIANGLES, allIndex.size());
    //textures[0].unbind();

    ofDisableAlphaBlending();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);


}




/// XML - STUFF
void trxObjectHandler::initXML()
{
    xml = trxXML();
    xml.setup();
}

void trxObjectHandler::generateObjects()
{
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
    for (int i=0; i<myFlocks.size(); i++) {
        trxConnectionSlot thisSlot = trxConnectionSlot(&myFlocks[i]);
        myConnections.push_back(thisSlot);
    }
}

vector<trxVehicle *> trxObjectHandler::getAllBoidsFromFlocks(vector<trxFlock> * _myFlocks){
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

void trxObjectHandler::catchBoid(trxHarvester * _myHarverster)
{
    for (int i=0; i<allMyBoids.size(); i++){
        
        trxVehicle * boid = allMyBoids.at(i);
        
        if (_myHarverster->myCatch.size() < 20 && !boid->caught) {
            ofVec3f bPos= boid->position;
            ofVec3f hPos= ofVec3f (_myHarverster->position.x,ofGetHeight()-_myHarverster->position.y,0);
            bPos = myCamera->worldToScreen(bPos);
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

void trxObjectHandler::addObject(ofxTuioObject & tuioObject)
{
    ofPoint loc = ofPoint(getCorrectedPosition( tuioObject.getX(),tuioObject.getY()));    
    if (myFlocks.size() > tuioObject.getFiducialId()) {
        trxFlock * thisFlock = &myFlocks.at(tuioObject.getFiducialId());
        thisFlock->position = ofVec3f(loc.x,loc.y,myFlocks.at(tuioObject.getFiducialId()).position.z);
        thisFlock->isActive = true;
        thisFlock->rotation = ofRadToDeg(tuioObject.getAngle());
        //cout << "Object n" << tuioObject.getSessionId() << " add at " << loc << endl;
        activeFlocks.push_back(thisFlock);
    }
    if (tuioObject.getFiducialId() >= myFlocks.size() && tuioObject.getFiducialId() < myFlocks.size()+myConverters.size()) {
        int atPos = tuioObject.getFiducialId()-myFlocks.size();
        trxConverter * thisConverter = &myConverters.at(atPos);
        thisConverter->position = ofVec3f(loc.x,loc.y,myFlocks.at(atPos).position.z);
        thisConverter->isActive = true;
        thisConverter->rotation = ofRadToDeg(tuioObject.getAngle());
        activeConverters.push_back(thisConverter);
        //cout << "Object n" << tuioObject.getSessionId() << " add at " << loc << endl;
    }
}

void trxObjectHandler::updateObject(ofxTuioObject & tuioObject)
{
    ofPoint loc = ofPoint(getCorrectedPosition( tuioObject.getX(),tuioObject.getY()));
    if (myFlocks.size() > tuioObject.getFiducialId()) {
        trxFlock * thisFlock = & myFlocks.at(tuioObject.getFiducialId());
        thisFlock->position = ofVec3f(loc.x,loc.y,myFlocks.at(tuioObject.getFiducialId()).position.z);
        thisFlock->unprojectedPosition = screenPosition(thisFlock->position, myCamera);
        thisFlock->rotation = ofRadToDeg(tuioObject.getAngle());
        //cout << "Object n" << tuioObject.getSessionId() << " updated at " << loc << endl;
        //cout << "angle: " << tuioObject.getAngle() << endl;
    }
    if (tuioObject.getFiducialId() >= myFlocks.size() && tuioObject.getFiducialId() < myFlocks.size()+myConverters.size()) {
        int atPos = tuioObject.getFiducialId()-myFlocks.size();
        trxConverter * thisConverter = &myConverters.at(atPos);
        thisConverter->position = ofVec3f(loc.x,loc.y,myFlocks.at(atPos).position.z);
        thisConverter->unprojectedPosition = screenPosition(thisConverter->position, myCamera);
        thisConverter->rotation = ofRadToDeg(tuioObject.getAngle());
        //cout << "Object n" << tuioObject.getSessionId() << " add at " << loc << endl;
    }
}

void trxObjectHandler::removeObject(ofxTuioObject & tuioObject)
{
    ofPoint loc = ofPoint(getCorrectedPosition( tuioObject.getX(),tuioObject.getY()));
    if (myFlocks.size() > tuioObject.getFiducialId()) {
        trxFlock * thisFlock = &myFlocks.at(tuioObject.getFiducialId());
        thisFlock->isActive = false;
    }
    if (tuioObject.getFiducialId() >= myFlocks.size() && tuioObject.getFiducialId() < myFlocks.size()+myConverters.size()) {
        int atPos = tuioObject.getFiducialId()-myFlocks.size();
        trxConverter * thisConverter = &myConverters.at(atPos);
        thisConverter->isActive = false;
        //cout << "Object n" << tuioObject.getSessionId() << " add at " << loc << endl;
    }
    for (int i = 0; i < activeFlocks.size(); i++) {
        if (activeFlocks[i]->id == tuioObject.getFiducialId()) {
            activeFlocks.erase(activeFlocks.begin()+i);
        }
    }
    for (int i = 0; i < activeConverters.size(); i++) {
        if (activeConverters[i]->id == tuioObject.getFiducialId()) {
            activeConverters.erase(activeConverters.begin()+i);
        }
    }
}

void trxObjectHandler::addCursor(ofxTuioCursor & tuioCursor)
{
    ofPoint loc = ofPoint(getCorrectedPosition(tuioCursor.getX(),tuioCursor.getY()));
    trxHarvester thisHarvester = trxHarvester(loc.x,loc.y,DEPTH,tuioCursor.getSessionId());
    thisHarvester.unprojectedPosition = thisHarvester.screenPosition(myCamera);
    harvesters.push_back(thisHarvester);
	//cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
}

void trxObjectHandler::updateCursor(ofxTuioCursor & tuioCursor)
{
    ofPoint loc = ofPoint(getCorrectedPosition(tuioCursor.getX(),tuioCursor.getY()));
    for (int i = 0; i < harvesters.size(); i++) {
        trxHarvester *thisHarvester = &harvesters[i];
        
        if (thisHarvester->id == tuioCursor.getSessionId()) {
            thisHarvester->unprojectedPosition = thisHarvester->screenPosition(myCamera);
            thisHarvester->position = ofVec3f(loc.x,loc.y,thisHarvester->position.z);
        }
    }
    
	//cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;

}
void trxObjectHandler::removeCursor(ofxTuioCursor & tuioCursor)
{
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

ofVec2f trxObjectHandler::getCorrectedPosition(float _x, float _y){
    float x = ofMap(_x, 0.0+SCREEN_X_MIN, 1.0-SCREEN_X_MAX, 0.0, 1.0)*ofGetWidth();
    float y = ofMap(_y, 0.0+SCREEN_Y_MIN, 1.0-SCREEN_Y_MAX, 0.0, 1.0)*ofGetHeight();

    return ofVec2f(x,y);
    
}

ofVec3f trxObjectHandler::screenPosition(ofVec3f _position, ofCamera * cam){
    ofVec3f pos= ofVec3f (_position.x,_position.y,_position.z);
    pos = cam->worldToScreen(pos);
    return pos;
}


void  trxObjectHandler::updateAllVertexes(){
    for (int i = 0; i<allMyBoids.size(); i++) {
        trxVehicle * tmpBoid = allMyBoids.at(i);
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
                    if (activeFlocks.at(active)->id == tmpBoid->myTypeID) {
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
}
