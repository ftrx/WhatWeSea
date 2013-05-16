//
//  trxFlock.cpp
//  ofxBoid_test2
//
//  Created by Fabian Troxler on 01.04.13.
//
//

#include "trxFlock.h"
#include "trxFlockUpdater.h"


trxFlock::trxFlock(float _x, float _y, float _z, int _id, vector <trxHarvester>* _harvesters, int _startBoidNum) : trxObject(_x,_y,_z,_id){

   // myConnectionSlot = &trxConnectionSlot(this);
    
    myHarvesters = _harvesters;
	boidNum = 200;
    startBoidNum = _startBoidNum;
    maxSpeed = 2.0f;
	target = ofVec3f(0, 0, 0);
	//cout << startBoidNum << endl;
    
    flockUpdater = new trxFlockUpdater();
    flockUpdater->flock = this;
    
   
}

// Sort-Function
bool sortOnZPosition(ofVec3f   point1, ofVec3f   point2)
{
    return (point1.z < point2.z);
}

void trxFlock::update(){

    vector <trxVehicle> tmp;

	for (int i = 0; i < boids.size(); i++)
	{
		boids[i].flock(boids);
		boids[i].update();
		boids[i].bounce(ofGetWidth(), ofGetHeight(), DEPTH);

        
        
	}

    
    if (boids.size() < boidNum)
    {
       //createNewBoid();
    }

   

}

void trxFlock::draw(){
    
    trxObject::draw();
    
}

void trxFlock::drawCircles(){
    ofSetColor(255,255,255);
    for (int i=0; i<boids.size(); i++) {
        ofCircle(ofPoint(boids[i].position),2);
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

void trxFlock::removeVehicles(vector<trxVehicle*> *_v){

}

bool checkDead( trxVehicle &p ){return p.dead;}

void trxFlock::removeDeadBoids(){
    ofRemove(boids, checkDead);
    /*
    for (int i=0;i<boids.size();i++){
        if (boids[i].dead) {
            boids.erase(boids.begin()+i);
        }
        else i++;
    }*/
}


void trxFlock::freeCatchedBoids(){
    for (int i=0; i < boids.size(); i++) {
        boids[i].clearTargets();
        if (boids[i].caught)
        {
            
            boids[i].caught = false;
            boids[i].maxSpeed = maxSpeed;
            boids[i].inSightDist = sightDistance;
            boids[i].tooCloseDist = tooCloseDistance;
            
        }
    }
}



void trxFlock::sortDeadsOut(){

    
}

int trxFlock::returnID(){return id;}

void trxFlock::generateBoids(){
    for (int i = 0; i < startBoidNum; i++)
	{
		createNewBoid();
	}
}

void trxFlock::createNewBoid(){
    trxVehicle v(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()),ofRandom(50, DEPTH-50));
    v.velocity = ofVec3f(ofRandom(-1.0f,1.0f),ofRandom(-1.0f,1.0f),ofRandom(-1.0f,1.0f));
    v.lifeSpan = ofRandom(3.0f,6.0f);
    v.maxSpeed = maxSpeed;
    v.maxStandardSpeed = maxSpeed;
    v.length = length;
    v.numberOfBones = numberOfBones;
    v.bonelength = length/(numberOfBones-1);
    v.maxForce = 0.5f;
    v.inSightDist = sightDistance;
    v.tooCloseDist = tooCloseDistance;
    v.maxTrailSize= 0;
    v.myTypeID = id;
    v.bones.assign(numberOfBones, ofVec3f(v.position));
    boids.push_back(v);
}

int trxFlock::countDead(){
    int dead= 0;
    for (int i=0; i<boids.size(); i++) {
        if (boids[i].dead) { dead++;}
    }
    return dead;
}

int trxFlock::countOnWay(){
    int onWay= 0;
    for (int i=0; i<boids.size(); i++) {
        if (boids[i].onWay) { onWay++;}
    }
    return onWay;
}