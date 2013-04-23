//
//  trxFlock.cpp
//  ofxBoid_test2
//
//  Created by Fabian Troxler on 01.04.13.
//
//

#include "trxFlock.h"


trxFlock::trxFlock(float _x, float _y, float _z, int _id, vector <trxHarvester>* _harvesters, int _startBoidNum) : trxObject(_x,_y,_z,_id){

    myHarvesters = _harvesters;
	boidNum = 200;
    startBoidNum = _startBoidNum;
    maxSpeed = 2.0f;
	target = ofVec3f(0, 0, 0);
	
	for (int i = 0; i < startBoidNum; i++)
	{
		createNewBoid();
	}
	cout << startBoidNum << endl;
    // upload the data to the vbo
	int total = (int)boids.size();
	vbo.setVertexData(&points[0], total, GL_DYNAMIC_DRAW);
	vbo.setNormalData(&sizes[0], total, GL_DYNAMIC_DRAW);
}

// Sort-Function
bool sortOnZPosition(ofVec3f   point1, ofVec3f   point2)
{
    return (point1.z < point2.z);
}

void trxFlock::update(){    
    points.clear();
    sizes.clear();
    vector <trxVehicle> tmp;
	for (int i = 0; i < boids.size(); i++)
	{
		boids[i].flock(boids);
		boids[i].update();
		boids[i].bounce(ofGetWidth(), ofGetHeight(), DEPTH);
        boids[i].aging(0.005f);
        if (!boids[i].isDead())
        {
            tmp.push_back(boids[i]);
            points.push_back(boids[i].position);
            sizes.push_back(ofVec3f(190*boids[i].remainingLifePer));
            
        }
        
	}
    boids = tmp;
    
    if (boids.size() < boidNum)
    {
       createNewBoid();
    }
    
    // upload the data to the vbo
    
    std::sort( points.begin(), points.end(), sortOnZPosition);
    
	//int total = (int)boids.size();
	//vbo.setVertexData(&points[0], total, GL_DYNAMIC_DRAW);
	//vbo.setNormalData(&sizes[0], total, GL_DYNAMIC_DRAW);

}

void trxFlock::draw(){
    ofPushStyle();
    ofPushMatrix();
    ofSetCircleResolution(100);
    ofTranslate(position.x,ofGetHeight()-position.y,0);
    ofRotate(rotation);
    ofEnableAlphaBlending();
    
    if(isActive){
        ofSetColor(255, 255, 255, 50);
        ofCircle(0,0,80);
        ofSetColor(255, 255, 255);
        ofNoFill();
        ofSetLineWidth(3.0);
        ofCircle(0,0,80);   
        ofFill();
        ofDrawBitmapString(title, 80,80);
    }
    ofDisableAlphaBlending();
    ofPopMatrix();
    ofPopStyle();
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
    ofTranslate(position.x,position.y,position.z);
    ofPushStyle();
    ofColor(200,200,200,100);
	ofSetColor(myColor);
    
    string info = "\n";
	info += "Total Points "+ofToString(points.size())+"\n";
	info += "Total Boids "+ofToString(boids.size())+"\n";
    ofDrawBitmapString(info, 0,0);
    ofPopMatrix();
}

void trxFlock::removeDeathVehicle(trxVehicle* _v){
    
    

}

int trxFlock::returnID(){

    return id;
}



void trxFlock::createNewBoid(){
    trxVehicle v(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()),ofRandom(100, DEPTH-100));
    v.velocity = ofVec3f(ofRandom(0.1f,2.0f),ofRandom(0.1f,2.0f),ofRandom(0.1f,2.0f));
    v.lifeSpan = ofRandom(3.0f,6.0f);
    v.maxSpeed = maxSpeed;
    v.maxForce = 0.5f;
    v.inSightDist = 140.0f;
    v.tooCloseDist = 40.0f;
    v.maxTrailSize= 16;
    v.myTypeID = id;
    boids.push_back(v);
    points.push_back(v.position);
    sizes.push_back(ofVec3f(190));
}

