//
//  trxHarvester.cpp
//  ofxBoid_test2
//
//  Created by Fabian Troxler on 01.04.13.
//
//

#include "trxHarvester.h"



trxHarvester::trxHarvester(float _x, float _y, float _z,int _id){
    id = _id;
    position.set(ofVec3f(_x,_y,_z));
    lastPosition.set(position);       
}




void trxHarvester::update(){
    
    movment = unprojectedPosition-lastPosition;
    lastPosition = unprojectedPosition;
    if (longline) {
        ofPoint * positionPoint = new ofPoint(position.x,position.y,0);
        if (longlinePoints.size()>0) {
            ofPoint * lastPoint = longlinePoints[longlinePoints.size()-1];
            if (positionPoint->distance(*lastPoint) >= 30.0) {
               ofPoint * projectedPoint = new ofPoint(pointToProjection(*positionPoint));
                longlineUnprojectedPoints.push_back(projectedPoint);
                longlinePoints.push_back(positionPoint);
                longlineHooks.push_back(0);
            }
        }
        else{
            ofPoint * projectedPoint = new ofPoint(pointToProjection(*positionPoint));
            
            longlinePoints.push_back(longlineTargetPosition);
            //longlinePoints.push_back(new ofPoint(longlineTargetPosition->x,longlineTargetPosition->y+40));
            longlineUnprojectedPoints.push_back(projectedPoint);
            longlinePoints.push_back(positionPoint);
            
            longlineHooks.push_back(0);
        }
        for (int i=0; i<myCatch.size(); i++) {
            trxVehicle * boid = myCatch.at(i);
                boid->pathRadius = 0;
                
        }
    }
    
    
    if (!longline) {
        for (int i=0; i<myCatch.size(); i++) {
            trxVehicle * boid = myCatch.at(i);
                ofVec3f boidMovement = movment * ofMap(boid->position.z, 0, 600, 2.5f, 1.0f);
                boid->addTargetMovment(boidMovement);
                boid->isCaughtAt(unprojectedPosition);
                boid->pathRadius = radius;
                boid->caught = true;

        }
        for (int i=0; i<myBycatch.size(); i++) {
            trxVehicle * boid = myBycatch.at(i);
                ofVec3f boidMovement = movment * ofMap(boid->position.z, 0, 600, 2.5f, 1.0f);
                boid->addTargetMovment(boidMovement);
                boid->isCaughtAt(unprojectedPosition);
                boid->pathRadius = radius;
                boid->caught = true;
        }
    }
    
}

void trxHarvester::draw(){
    
    ofPushStyle();
    ofSetCircleResolution(100);
    
    ofEnableAlphaBlending();
    if (longline) {
        ofNoFill();
        ofSetLineWidth(2.0);
        ofSetColor(255, 255, 255,255);
        ofBeginShape();
        for (int i=0; i<longlinePoints.size(); i++) {
            if (i == 0) {
                ofCurveVertex(*longlinePoints[i]);
                ofCurveVertex(*longlinePoints[i]);
            }
            else {
                randomWiggle = ofVec3f(ofSignedNoise(ofGetElapsedTimef()*wobbleSpeed+2.222*i),ofSignedNoise(ofGetElapsedTimef()*wobbleSpeed+2.222*i),0)*maxAmplitude;
                ofPoint point = ofPoint(*longlinePoints[i]+randomWiggle);
                ofCurveVertex(point);
            }
            
            //ofCircle(*longlinePoints[i], 10);
        }
        ofCurveVertex(position.x, position.y,0);
        ofCurveVertex(position.x, position.y,0);
        ofEndShape(false);
    }
    
    
    
    else {
        ofPushMatrix();
        ofTranslate(position.x,position.y,0);
        ofFill();
        
        ofSetColor(255, 255, 255, 30);
        ofCircle(0,0,radius);
        ofSetColor(255, 255, 255, 50);
        ofCircle(0, 0, 10);
        ofSetColor(255, 255, 255,255);
        
        ofNoFill();
        ofSetLineWidth(1.0);
        
        ofCircle(0,0,radius);
        ofFill();
        ofPopMatrix();
    }
   
    ofDisableAlphaBlending();
    ofPopStyle();
    
    
    }

void trxHarvester::drawInfo(){
    ofPushStyle();
    //ofEnableAlphaBlending();
    ofPushMatrix();
    ofTranslate(unprojectedPosition);
    ofColor(200,200,200,100);
    ofSetLineWidth(2.0);
    ofLine(0, 0, DEPTH-position.z, 0, 0, -DEPTH);
   
    
    ofSetColor(255, 255, 255,50);
    ofSphere(0, 0, 0, 20.0);
     
    
	ofSetColor(color);
    
    string info = "Harvesterposition:"+ofToString(position)+"\n";
    ofDrawBitmapString(info, 0,0);
    ofPopMatrix();
    
    
    for (int i=0; i<longlineUnprojectedPoints.size(); i++) {
        ofSetColor(0, 255, 0);
        if (longlineHooks[i]) {
            ofSetColor(255, 0, 0);
        }
        
        ofNoFill();
        ofCircle(*longlineUnprojectedPoints[i], LONGLINE_RADIUS);
    }
    //ofDisableAlphaBlending();
    ofPopStyle();
    
   
    
}



void trxHarvester::createNewBoid(){
    
}

void trxHarvester::clearCatch(){
    for (int i=0; i<myCatch.size(); i++) {
        trxVehicle * boid = myCatch.at(i);
        boid->caught = false;
        boid->clearTargets(); /// !!! fast target eraser, but delets all targets
    }
    myCatch.clear();
}
/*
void trxHarvester::moveMyCatch(ofCamera * cam){
    for (int i = 0; i < myCatch.size(); i++) {
        trxVehicle * boid = myCatch[i];
    }
}
 */
ofVec3f trxHarvester::movmentToBoidZ(ofCamera * cam, ofVec3f _pos){
    ofVec3f pos= _pos;
    
    pos = cam->screenToWorld(pos);
    pos *= ofMap(_pos.z, 0, 900.0, 1.0f, 0.0f);
    pos.z = 0;
    return pos;
}


ofVec3f trxHarvester::screenPosition(ofCamera * cam){
    ofVec3f pos= ofVec3f (position.x,position.y,0.666666);
    pos = cam->screenToWorld(pos, ofRectangle(0,0,ofGetWidth(),ofGetHeight()));
    return pos;
}
/*
void trxHarvester::removeBoids(){
    for (int i=0; i<myCatch.size(); i++) {
        trxVehicle * boid = myCatch.at(i);
        boid->dead = true;
        boid->clearTargets(); /// !!! fast target eraser, but delets all targets
    }
    myCatch.clear();

}
*/
void trxHarvester::moveBoidsToTarget(ofVec3f * _target, ofVec3f *_movment,float _targetRadius){
    for (int i=0; i<myCatch.size(); i++) {
        trxVehicle * boid = myCatch.at(i);
        boid->maxSpeed = 8.0f;
        boid->onWay = true;
        boid->caught = true;
        boid->clearTargets(); /// !!! fast target eraser, but delets all targets
        boid->addTarget(_target);
        boid->pathRadius = _targetRadius;
        //boid->addTargetMovment(_movment);
    }
    for (int i=0; i<myBycatch.size(); i++) {
        trxVehicle * boid = myBycatch.at(i);
        boid->maxSpeed = 8.0f;
        boid->onWay = true;
        boid->caught = true;
        boid->clearTargets(); /// !!! fast target eraser, but delets all targets
        boid->addTarget(_target);
        //boid->addTargetMovment(_movment);
    }
    myCatch.clear();
    myBycatch.clear();
}

void trxHarvester::clearLonglinePoints(){
    longlinePoints.clear();
    longlineHooks.clear();
}

ofPoint trxHarvester::pointToProjection(ofPoint _point){
    ofVec3f point= ofVec3f (_point.x,_point.y,0.666666);
    point = myCamera->screenToWorld(point,ofRectangle(0,0,ofGetWidth(),ofGetHeight()));
    return point;
    
}

void trxHarvester::setID(int _id){
    id = _id;
}

void trxHarvester::setPosition(ofVec3f _pos){
    position = _pos;
}

void trxHarvester::setUnprojectedPosition(ofVec3f _pos){
    unprojectedPosition = _pos;
}

