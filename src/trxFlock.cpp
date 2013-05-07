//
//  trxFlock.cpp
//  ofxBoid_test2
//
//  Created by Fabian Troxler on 01.04.13.
//
//

#include "trxFlock.h"



trxFlock::trxFlock(float _x, float _y, float _z, int _id, vector <trxHarvester>* _harvesters, int _startBoidNum) : trxObject(_x,_y,_z,_id){

   // myConnectionSlot = &trxConnectionSlot(this);
    
    myHarvesters = _harvesters;
	boidNum = 200;
    startBoidNum = _startBoidNum;
    maxSpeed = 2.0f;
	target = ofVec3f(0, 0, 0);
	cout << startBoidNum << endl;
  
}

// Sort-Function
bool sortOnZPosition(ofVec3f   point1, ofVec3f   point2)
{
    return (point1.z < point2.z);
}

void trxFlock::update(){
    //points.clear();
    //sizes.clear();
    //removeDeadBoids();
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
    
    // upload the data to the vbo
    
    std::sort( points.begin(), points.end(), sortOnZPosition);
    
	//int total = (int)boids.size();
	//vbo.setVertexData(&points[0], total, GL_DYNAMIC_DRAW);
	//vbo.setNormalData(&sizes[0], total, GL_DYNAMIC_DRAW);

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
	info += "Total Points "+ofToString(points.size())+"\n";
	info += "Total Boids "+ofToString(boids.size())+"\n";
    ofDrawBitmapString(info, 0,0);
    ofPopMatrix();
}

void trxFlock::removeVehicles(vector<trxVehicle*> *_v){

}

bool checkDead( trxVehicle &p ){return p.dead;}

void trxFlock::removeDeadBoids(){
    ofRemove(boids, checkDead);

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
    v.maxForce = 0.5f;
    v.inSightDist = sightDistance;
    v.tooCloseDist = tooCloseDistance;
    v.maxTrailSize= 0;
    v.myTypeID = id;
    boids.push_back(v);
    points.push_back(v.position);
    sizes.push_back(ofVec3f(190));
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