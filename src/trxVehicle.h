//
//  trxVehicle.h
//  WhatWeSea
//
//  Created by Fabian Troxler on 21.04.13.
//
//

#pragma once

#include "BiologicalVehicle.h"
#include "general.h"


class trxVehicle : public BiologicalVehicle {
	
protected:
	
	
	
public:
	int myTypeID; // ID of flock == fishType
    
    ofVec3f position2D;
    
	ofVec3f * target = NULL;

    ofVec3f targetMovment;
    bool caught = false;
    bool onWay = false;
    bool dead = false;
    
    bool fleeFromHarvester = true;
    float fleeSpeed = 8.0;
    
    bool isJellyFish = false;
    float contractScale = 0.5;
    int contractSpeed = 100;
    int expandSpeed = 200;
    float scaleFactor = 1;
    
    int contractionTimestamp = 0;
    
    int maxGroupSize = 500;
    
    
    ofColor color = standardColor;
    
    float pathRadius = 40.0;
    
    float maxStandardSpeed = 0;
    
    vector<ofVec3f> bones;
    vector<ofVec3f> boneNormals;
    vector<ofVec3f> vertexes;
    
    // size of the vehicle
    float length = 40.0;
    float standardLength = 40.0;
    int numberOfBones = 4;
    float bonelength = length/(numberOfBones-1);
    float standardBoneLength = bonelength;
    
    vector <ofVec3f> fleeTargets;
    vector <trxVehicle *>* predators = NULL;
    vector <trxVehicle *> prey;
    
    vector <ofVec3f> paths;
    vector <ofVec3f> circlePaths;
	
	trxVehicle(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : BiologicalVehicle(_x, _y, _z) {
        
	}
    
    
    virtual ~trxVehicle()
    {
        fleeTargets.clear();
        predators = NULL;
        prey.clear();
        target = NULL;
        paths.clear();
        circlePaths.clear();
        
    }
    
    
    template<typename Type> void flock(vector<Type> &vehicles)
	{
        if (position.x != position.x) {
            cout<<"error nan"<<endl;
        }
        ofVec3f averageVelocity;
		ofVec3f averagePosition;
		int inSightCnt = 0;
		
		averageVelocity.set(velocity);
		
		for (int i = 0; i < vehicles.size(); i++)
		{
			if (vehicles[i]->getId() == getId()) continue;

            if (vehicles[i]->caught != caught) continue;
			if (!inSight(vehicles[i]->position)) continue;
			
			averageVelocity += vehicles[i]->velocity;
			averagePosition += vehicles[i]->position;
			inSightCnt++;
			
			if (tooClose(vehicles[i]->position) || inSightCnt > maxGroupSize)
			{
				flee(vehicles[i]->position);
			}
            
		}
		
		if (inSightCnt > 0)
		{
			averagePosition *= 1.0f / inSightCnt;
			seek(averagePosition);
			averageVelocity *= 1.0f / inSightCnt;
			steeringForce += averageVelocity - velocity;
		}
        if (position.x != position.x) {
            cout<<"error nan"<<endl;
        }
        /*
        if (target != NULL) {
            arriveTarget(target);
        }
        */
                
        // there are some problems with that, because of the pointers…
        if  (!caught){
            if (maxSpeed > maxStandardSpeed) {
                maxSpeed -= 0.1f;
            }
            if (maxSpeed < maxStandardSpeed) {
                maxSpeed += 0.1f;
            }
            
            for (int i=0; i<fleeTargets.size(); i++) {
                    ofVec3f pos = ofVec3f(fleeTargets[i].x,fleeTargets[i].y,0);
                    if (position2D.distance(pos) < HARVESTER_RADIUS+20.0){
                        maxSpeed = fleeSpeed;
                        fleeTarget(&fleeTargets[i]);
                    }
            }
            
            if (predators != NULL) {
                for (int i=0; i<predators->size(); i++){
                    if(predators->at(i))
                    {
                        if (inSight(predators->at(i)->position) && !predators->at(i)->dead) {
                            maxSpeed = fleeSpeed;
                            evade(*predators->at(i));
                            if (position.distance(predators->at(i)->position) < 10) {
                                //dead = true;
                            }
                        }
                    }
                }
            }
            
            
            for (int i=0; i<prey.size(); i++){
                //if (!inSight(prey[i]->position)) continue;
                if (prey.at(i) != NULL) {
                    if (!prey.at(i)->dead) {
                        pursue(*prey.at(i));
                    }
                    else {
                        prey.erase(prey.begin()+i);
                    }
                }
            }
            
        }
        
        //fleeTargets.clear();
        if (position.x != position.x) {
            cout<<"error nan"<<endl;
        }
    }
    
    
    void catchFlock()
	{
        if (position.x != position.x) {
            cout<<"error nan"<<endl;
        }
        if (target) {
            isCaughtAt(*target);
            //position += targetMovment;
        }
    }
    
    void update();
    void arriveTarget(ofVec3f * _target);
    void isCaughtAt(ofVec3f _target);
    void fleeTarget(ofVec3f *_target);
    
    void addTarget(ofVec3f * _target){
        target = _target;
    }
    void clearTargets(){
        target = NULL;
    }
    
    void addFleeTarget(ofVec3f _target){
        fleeTargets.push_back(_target);
    }
    void clearFleeTargets(){
        fleeTargets.clear();
    }
    
    void addPredator(trxVehicle * _target){
      //  predators.clear();
       // predators.push_back(_target);
    }
    
    void addPrey(trxVehicle * _target){
        prey.clear();
        prey.push_back(_target);
    }
    void addTargetMovment(ofVec3f _targetMovement){
        position += _targetMovement;
        //targetMovment = *_targetMovement;
    }

    
 
    void evade(trxVehicle& target);

};




