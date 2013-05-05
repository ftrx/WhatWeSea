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
    myStoryHandler = trxStoryHandler(&myFlocks,&myConverters,&myConnections);
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
       // harvesters[i].moveMyCatch(myCamera);
        harvesters[i].update();
        catchBoid(&harvesters[i]);
        
    }
    
    
    // 16.66f is about 1000/60 so about 60FPS
    if(timer>16.6f){
        for (int i = 0; i<myFlocks.size(); i++) {
            myFlocks[i].update();
        }
        timeStamp = ofGetElapsedTimeMillis();
    }

    updateAllVertexes();
    
    if (allVertex.size()>0) {
        //cout << "allVertex.size()" << allVertex.size() << endl;
        vbo.setVertexData( &allVertex[0], allVertex.size(), GL_DYNAMIC_DRAW );
        vbo.setColorData(&allColors[0], allColors.size(), GL_DYNAMIC_DRAW);
        vbo.setIndexData( &allIndex[0], allIndex.size(), GL_DYNAMIC_DRAW );
        //vbo2.drawElements( GL_TRIANGLES, allIndex.size());
        
    }
    myStoryHandler.update();

}

void trxObjectHandler::draw()
{
    ofPushStyle();
        //draw Connections
    vector<int> objectsPositionArray;
    objectsPositionArray.assign(maxID+1,0);
    for(int i=0; i < myConnections.size(); i++){
        trxConnectionSlot * thisSlot = &myConnections[i];
        trxFlock * flock = thisSlot->myFlock;
        trxConverter * converter = thisSlot->myConverter;
        if(thisSlot != myActiveConnection)
        {
            ofEnableAlphaBlending();
            if (myActiveConnection) {
                ofSetColor(255, 255, 255,50);
            }
            else {
                ofSetColor(255, 255, 255,100);
            }
            thisSlot->drawPossibleConnection(objectsPositionArray.at(flock->id),objectsPositionArray.at(converter->id));
            objectsPositionArray[flock->id]++;
            objectsPositionArray[converter->id]++;
            ofDisableAlphaBlending();
            
        }
        else
        {
            thisSlot->draw();
        }
    }
    //draw active Objects
    for (int i = 0; i<activeFlocks.size(); i++) {
        ofPushMatrix();
        trxFlock * flock = activeFlocks[i];
        ofTranslate(flock->position.x,flock->position.y,0);
        ofRotate(flock->rotation);
        flock->draw();
        ofPopMatrix();
    }
    for (int i = 0; i<activeConverters.size(); i++) {
        ofPushMatrix();
        trxConverter * converter = activeConverters[i];
        ofTranslate(converter->position.x,converter->position.y,0);
        ofRotate(converter->rotation);
        converter->draw();
        ofPopMatrix();
    }


    
    // draw harvester / finger-cursor
    for (int i=0; i<harvesters.size(); i++) {
        harvesters.at(i).draw();
        
    }
    ofPopStyle();
    
    
    if (myActiveConnection) {
        myStoryHandler.draw();
    }
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
    xml.setup("Objects","Object");
}
void trxObjectHandler::generateObjects()
{
    cout << xml.objectNumber <<endl;
    for (int i=0; i < xml.objectNumber; i++)
    {
        xml.XML.pushTag("Objects");
        xml.XML.pushTag("Object",i);
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
        xml.XML.popTag();
        xml.XML.popTag();
    }
    for (int i=0; i<xml.objectNumber; i++) {
        
        xml.XML.pushTag("Objects");
        xml.XML.pushTag("Object",i);
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
        xml.XML.popTag();
        xml.XML.popTag();
    }
    for (int i=0; i<myFlocks.size(); i++) {
        myFlocks[i].generateBoids();
        for (int j=0; j<myFlocks[i].myConnections.size(); j++) {
            trxConnectionSlot thisSlot = trxConnectionSlot();
            thisSlot.myFlock = &myFlocks[i];
            thisSlot.myConverter = &myConverters[j];
            myConnections.push_back(thisSlot);
        }
    }
}

vector<trxVehicle *> trxObjectHandler::getAllBoidsFromFlocks(vector<trxFlock> * _myFlocks){
    vector<trxVehicle *> allBoids;
    for(int i=0; i< _myFlocks->size(); i++)
    {
        trxFlock * tmpFlock = &_myFlocks->at(i);
        
        for (int j=0; j<tmpFlock->boids.size();j++)
        {
            trxVehicle * thisVehicle = &tmpFlock->boids.at(j);
            // check if dead, when dead then do not draw
            //if (!thisVehicle->dead) {
                allBoids.push_back(thisVehicle);
            //}
        }
    }
    return allBoids;
}

void trxObjectHandler::catchBoid(trxHarvester * _myHarverster)
{
    for (int i=0; i<allMyBoids.size(); i++){
        
        trxVehicle * boid = allMyBoids.at(i);
        ofVec3f bPos= boid->position;
        ofVec3f hPos= ofVec3f (_myHarverster->position.x,_myHarverster->position.y,0);
        bPos = myCamera->worldToScreen(bPos);
        //hPos = camera.screenToWorld(hPos, viewMain);
        float dist = hPos.distance(bPos);
        if (myStoryHandler.myActiveTask &&!myStoryHandler.showMessage) {
            if (_myHarverster->myCatch.size() < myStoryHandler.myActiveTask->catchSize && !boid->caught && boid->myTypeID == myStoryHandler.myActiveTask->catchID) {
                if (dist <= _myHarverster->radius) {
                    _myHarverster->myCatch.push_back(boid);
                    boid->addTarget(&_myHarverster->unprojectedPosition);
                    //boid->addTargetMovment(&_myHarverster->movment);
                    boid->caught = true;
                }
            }
        }
        
        if (dist <= 2*(_myHarverster->radius) && !boid->caught)
        {
            boid->fleeTargets.push_back(&_myHarverster->unprojectedPosition);
        }
    


    }
}

void trxObjectHandler::addObject(ofxTuioObject & tuioObject)
{
    ofPoint loc = ofPoint(getCorrectedPosition( tuioObject.getX(),tuioObject.getY()));
    trxFlock * thisFlock = getFlockWithID(tuioObject.getFiducialId());
    trxConverter * thisConverter = getConverterWithID(tuioObject.getFiducialId());
    if (thisFlock) {
        thisFlock->position = ofVec3f(loc.x,loc.y,thisFlock->position.z);
        thisFlock->isActive = true;
        thisFlock->rotation = ofRadToDeg(tuioObject.getAngle());
        //cout << "Object n" << tuioObject.getSessionId() << " add at " << loc << endl;
        activeFlocks.push_back(thisFlock);
    }
    if (thisConverter) {
        thisConverter->position = ofVec3f(loc.x,loc.y,thisConverter->position.z);
        thisConverter->isActive = true;
        thisConverter->rotation = ofRadToDeg(tuioObject.getAngle());
        activeConverters.push_back(thisConverter);
        //cout << "Object n" << tuioObject.getSessionId() << " add at " << loc << endl;
    }
    checkIfActiveSlot();
}

void trxObjectHandler::updateObject(ofxTuioObject & tuioObject)
{
    ofPoint loc = ofPoint(getCorrectedPosition( tuioObject.getX(),tuioObject.getY()));
    trxFlock * thisFlock = getFlockWithID(tuioObject.getFiducialId());
    trxConverter * thisConverter = getConverterWithID(tuioObject.getFiducialId());
    if (thisFlock) {
        thisFlock->position = ofVec3f(loc.x,loc.y,thisFlock->position.z);
        thisFlock->unprojectedPosition = screenPosition(thisFlock->position, myCamera);
        thisFlock->rotation = ofRadToDeg(tuioObject.getAngle());
        //cout << "Object n" << tuioObject.getSessionId() << " updated at " << loc << endl;
        //cout << "angle: " << tuioObject.getAngle() << endl;
    }
    if (thisConverter) {
        thisConverter->position = ofVec3f(loc.x,loc.y,thisConverter->position.z);
        thisConverter->unprojectedPosition = screenPosition(thisConverter->position, myCamera);
        thisConverter->rotation = ofRadToDeg(tuioObject.getAngle());
        //cout << "Object n" << tuioObject.getSessionId() << " add at " << loc << endl;
    }
    checkIfActiveSlot();
}

void trxObjectHandler::removeObject(ofxTuioObject & tuioObject)
{
    ofPoint loc = ofPoint(getCorrectedPosition( tuioObject.getX(),tuioObject.getY()));
    trxFlock * thisFlock = getFlockWithID(tuioObject.getFiducialId());
    trxConverter * thisConverter = getConverterWithID(tuioObject.getFiducialId());
    if (thisFlock) {
        thisFlock->isActive = false;
        for (int i = 0; i < activeFlocks.size(); i++) {
            if (activeFlocks[i]->id == thisFlock->id) {
                activeFlocks.erase(activeFlocks.begin()+i);
            }
        }
    }
    if (thisConverter) {
        thisConverter->isActive = false;
        for (int i = 0; i < activeConverters.size(); i++) {
            if (activeConverters[i]->id == thisConverter->id) {
                activeConverters.erase(activeConverters.begin()+i);
            }
        }
    }
    //cout << "Object n" << tuioObject.getSessionId() << " removed at " << loc << endl;
    checkIfActiveSlot();
    checkIfStillActiveSlot();
    
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
            //thisHarvester->clearCatch();
            //thisHarvester->removeBoids();
            thisHarvester->moveBoidsToTarget(&myStoryHandler.myActiveTask->targetPosition);
            harvesters.erase(harvesters.begin()+i);
        }
    }
    if (myStoryHandler.showMessage) {
        //cout << "location: " << loc << endl;
        //cout << "buttonPos : " << myStoryHandler.messageButton.position << endl;
        if (myStoryHandler.messageButton.clickOverButton(loc)) {
            //cout << " message closed" << endl;
            myStoryHandler.closeMessage();
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
        
        if(tmpBoid->bones.size()>= 4){
            ofVec3f vecs[] = {
                ofVec3f((float)tmpBoid->position.x,  (float)tmpBoid->position.y,  (float)tmpBoid->position.z),
                ofVec3f((float)tmpBoid->bones.at(1).x-5.0f,  (float)tmpBoid->bones.at(1).y,  (float)tmpBoid->bones.at(1).z),
                ofVec3f((float)tmpBoid->bones.at(1).x+5.0f,  (float)tmpBoid->bones.at(1).y,  (float)tmpBoid->bones.at(1).z),
                ofVec3f((float)tmpBoid->bones.at(2).x,  (float)tmpBoid->bones.at(2).y,  (float)tmpBoid->bones.at(2).z),
                ofVec3f((float)tmpBoid->bones.at(3).x-3.0f,  (float)tmpBoid->bones.at(3).y,  (float)tmpBoid->bones.at(3).z),
                ofVec3f((float)tmpBoid->bones.at(3).x+3.0f,  (float)tmpBoid->bones.at(3).y,  (float)tmpBoid->bones.at(3).z)};
            
            ofFloatColor vColor[6];
            float alpha = 1.0;
            if (activeFlocks.size() > 0 && !myStoryHandler.myActiveTask) {
                alpha = 0.2;
                for (int active=0; active<activeFlocks.size(); active++) {
                    if (activeFlocks.at(active)->id == tmpBoid->myTypeID) {
                        alpha = 1.0;
                    }
                }
            }
            
            if (myStoryHandler.myActiveTask) {
                alpha = 0.2;
                if (myStoryHandler.myActiveTask->catchID == tmpBoid->myTypeID) {
                    alpha = 1.0;
                }

            }
            
            for (int col=0; col<6; col++) {
                if (debug) {
                    if (tmpBoid->caught)
                    {
                        vColor[col].r = 1.0;
                        vColor[col].g = 0.0;
                        vColor[col].b = 0.0;
                    }
                    else
                    {
                        vColor[col].r = 0.0;
                        vColor[col].g = 1.0;
                        vColor[col].b = 0.0;
                    }
                }
                else
                {
                    if (tmpBoid->caught)
                    {
                        vColor[col].r = 1.0;
                        vColor[col].g = 1.0;
                        vColor[col].b = 1.0;
                    }
                    else
                    {
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

// Method to check if this there an active Connection when an new Object is placed on the table, or if there is a connection which has a shorter Distance between flock & converter, when an object is moved
void trxObjectHandler::checkIfActiveSlot(){
    //check all Connections (even the not active)
    for (int i=0; i<myConnections.size(); i++) {
        trxConnectionSlot * thisSlot = &myConnections[i];
        
        //if both members (flock&converter) of an aconnection ar active (on the Table) then set the state of the connection to active
        if(thisSlot->myFlock->isActive && thisSlot->myConverter->isActive)
        {
            thisSlot->state = true;
            
            //check if theres an running(active) connection
            if (myActiveConnection) {
                //if there is an running connection, then check which connection is shorter (nearest distance)
                float thisSlotDistance = thisSlot->myFlock->position.distance(thisSlot->myConverter->position);
                float activeSloteDistance = myActiveConnection->myFlock->position.distance(myActiveConnection->myConverter->position);
                //if the new connections distances ar shorter, then check if its not the existing connection, if not, set it to the existing.
                if ( thisSlotDistance < activeSloteDistance) {
                    if (myActiveConnection != thisSlot) {
                        
                        myActiveConnection = thisSlot;
                    }
                }
            }
            
            //if there is no existing connection then set this one to active
            else
            {
                myActiveConnection = thisSlot;

            }        
        }
        // if one member is not active the connection state should be false
        else
        {
            thisSlot->state = false;
        }
    }
    // prevents multiple storystarting when only the existing connection is moved
    if(myActiveConnection != myLastActiveConnection){
        myLastActiveConnection = myActiveConnection;
        myStoryHandler.startStory(myActiveConnection);
    }
    
}

// Method is called on a removed Object
// It checks if the current active Connection ist still viable
void trxObjectHandler::checkIfStillActiveSlot(){
    //only needed when an active Connection
    if (myActiveConnection) {
        //if the current Connection is not valid (either the Flock or the Converter was removed from the Table) reset all, and stop the current Story.
        if (!myActiveConnection->myFlock->isActive || !myActiveConnection->myConverter->isActive) {
            myActiveConnection = NULL;
            myLastActiveConnection = NULL;
            myStoryHandler.stopStory();
            cout << "set activeConnection to NULL" << endl;
        }
    }
}

trxFlock* trxObjectHandler::getFlockWithID(int _id){
    for (int i=0; i< myFlocks.size(); i++) {
        trxFlock * thisFlock = & myFlocks.at(i);
        if (thisFlock->id == _id) {
            return thisFlock;
        }
    }
    return NULL; 
}

trxConverter* trxObjectHandler::getConverterWithID(int _id){
    for (int i=0; i< myConverters.size(); i++) {
        trxConverter * thisConverter = & myConverters.at(i);
        if (thisConverter->id == _id) {
            return thisConverter;
        }
    }
    return NULL;
}
