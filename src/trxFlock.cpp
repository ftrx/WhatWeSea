//
//  trxFlock.cpp
//  ofxBoid_test2
//
//  Created by Fabian Troxler on 01.04.13.
//
//

#include "trxFlock.h"
#include "trxFlockUpdater.h"


trxFlock::trxFlock(float _x, float _y, float _z, int _id, int _startBoidNum) : trxObject(_x,_y,_z,_id){

   // myConnectionSlot = &trxConnectionSlot(this);
    
	maxBoidNum = 200;
    startBoidNum = _startBoidNum;
    maxSpeed = 2.0f;
	target = ofVec3f(0, 0, 0);
	//cout << startBoidNum << endl;
    
   // flockUpdater = new trxFlockUpdater();
   // flockUpdater->flock = this;
    growBackTimestamp = ofGetElapsedTimeMillis();
   
}

// Sort-Function
bool sortOnZPosition(ofVec3f   point1, ofVec3f   point2)
{
    return (point1.z < point2.z);
}

void trxFlock::update(){
    
    
    
	for (int i = 0; i < boids.size(); i++)
	{
        //long long timeElapsed =  ofGetElapsedTimeMicros() - timeStamp;
        //boids[i]->timeElapsed = timeElapsed;
        //timeStamp = ofGetElapsedTimeMicros();
        if (boids[i]->caught) {
            boids[i]->catchFlock();
        }
        else {
         
            boids[i]->flock(boids);
        }
		boids[i]->update();
		boids[i]->bounce(ofGetWidth(), ofGetHeight(), DEPTH);
	}
    
   
    
    if (growBack && ofGetElapsedTimeMillis()- growBackTimestamp > growBackTime) {
        if (boids.size() < maxBoidNum)
        {
            createNewBoid(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()),0);
        }
        growBackTimestamp = ofGetElapsedTimeMillis();
    }
    

}

void trxFlock::draw(){
    trxObject::draw();
}

void trxFlock::drawCircles(){
    ofSetColor(255,255,255);
    for (int i=0; i<boids.size(); i++) {
        ofCircle(ofPoint(boids[i]->position),2);
    }
}

void trxFlock::drawInfo(){
    
    trxObject::drawInfo();
    
    ofPushMatrix();
    ofTranslate(unprojectedPosition);
    ofPushStyle();
    ofColor(200,200,200,100);
	ofSetColor(myColor);
    
    string info = "\n";

	info += "Total Boids "+ofToString(boids.size())+"\n";
    ofDrawBitmapString(info, 0,0);
    ofPopMatrix();
}


bool checkDead( trxVehicle *p ){return p->dead;}

void trxFlock::removeDeadBoids(){
    
    vector<trxVehicle*>::iterator it = boids.begin(); 
    for (;it != boids.end();){
        if( (*it)->dead){
            delete * it;
            it = boids.erase(it);
        }
        else {
            ++it;
        }
    }
     
}

void trxFlock::removeFirstVehicle(){
    if (boids.size() > 0) {
        delete boids[0];
        boids.erase(boids.begin());
    }
    
}


void trxFlock::freeCatchedBoids(){
    for (int i=0; i < boids.size(); i++) {
        
        if (boids[i]->caught && !boids[i]->dead)
        {
            boids[i]->clearTargets();
            boids[i]->paths.clear();
            boids[i]->caught = false;
            boids[i]->onWay = false;
            boids[i]->maxSpeed = maxSpeed;
            boids[i]->inSightDist = sightDistance;
            boids[i]->tooCloseDist = tooCloseDistance;
            
        }
    }
}



void trxFlock::sortDeadsOut(){

    
}

int trxFlock::returnID(){return id;}

void trxFlock::generateBoids(){
    for (int i = 0; i < startBoidNum; i++)
	{
		createNewBoid(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()),ofRandom(50, DEPTH-50));
	}
}

void trxFlock::createNewBoid(float _x, float _y, float _z){
    trxVehicle* v = new trxVehicle(_x,_y,_z);
    v->velocity = ofVec3f(ofRandom(-1.0f,1.0f),ofRandom(-1.0f,1.0f),ofRandom(-1.0f,1.0f));
    v->lifeSpan = ofRandom(3.0f,6.0f);
    v->maxSpeed = maxSpeed + ofRandom(-maxSpeed*0.3,maxSpeed*0.3);
    v->maxStandardSpeed = maxSpeed;
    v->length = length + ofRandom(-length*0.3,length*0.4);
    v->numberOfBones = numberOfBones;
    v->bonelength = v->length/(numberOfBones-1);
    v->standardLength = v->length;
    v->maxForce = 0.5f;
    v->inSightDist = sightDistance;
    v->tooCloseDist = tooCloseDistance;
    v->maxTrailSize= 0;
    v->myTypeID = id;
    v->pathThreshold = 20.0f;
    v->pathLoop = true;
    v->fleeFromHarvester = fleeFromHarvester;
    v->fleeSpeed = fleeSpeed;
    
    v->maxGroupSize = maxGroupSize;
    
    // jellyFishSettings
    v->isJellyFish = isJellyFish;
    v->contractScale = contractScale + ofRandom(-contractScale*0.1,contractScale*0.1);
    v->contractSpeed = contractSpeed + ofRandom(-contractSpeed*0.1,contractSpeed*0.1);;
    v->expandSpeed = expandSpeed + ofRandom(-expandSpeed*0.1,expandSpeed*0.1);;
    v->contractionTimestamp = ofRandom(1000.0);
    
    
    ofColor color;
    int r = standardColor.r + int(ofRandom(-5.0, 10.0));
    int g = standardColor.g + int(ofRandom(-10.0, 15.0));
    int b = standardColor.b + int(ofRandom(-10.0, 15.0));
    color.r = r;
    color.g = g;
    color.b = b;
    v->color = color;
    
    ofVec3f vel = v->velocity;
    vel.normalize();
    vel *= length;
    v->bones.assign(numberOfBones, ofVec3f(v->position));
    for (int i=0; i<v->bones.size(); i++) {
        v->bones[i] = v->position + vel*-i;
    }
    
    //
    boids.push_back(v);
}

int trxFlock::countDead(){
    int dead= 0;
    for (int i=0; i<boids.size(); i++) {
        if (boids[i]->dead) { dead++;}
    }
    return dead;
}

int trxFlock::countOnWay(){
    int onWay= 0;
    for (int i=0; i<boids.size(); i++) {
        if (boids[i]->onWay) { onWay++;}
    }
    return onWay;
}