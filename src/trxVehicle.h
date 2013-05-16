//
//  trxVehicle.h
//  BoidsAndTUIO
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
    
    
    float maxStandardSpeed = 0;
    
    vector<ofVec3f> bones;
    vector<ofVec3f> boneNormals;
    vector<ofVec3f> vertexes;
    // size of the vehicle
    float length = 40.0;
    int numberOfBones = 4;
    float bonelength = length/(numberOfBones-1);
    
    vector <ofVec3f*> fleeTargets;
    vector <trxVehicle *> predators;
    vector <trxVehicle *> prey;
	
	trxVehicle(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : BiologicalVehicle(_x, _y, _z) {
        
	}
    
    template<typename Type> void flock(vector<Type>& vehicles)
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
			if (vehicles[i].getId() == getId()) continue;

            if (vehicles[i].caught != caught) continue;
			if (!inSight(vehicles[i].position)) continue;
			
			averageVelocity += vehicles[i].velocity;
			averagePosition += vehicles[i].position;
			inSightCnt++;
			
			if (tooClose(vehicles[i].position))
			{
				flee(vehicles[i].position);
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
        
        if (target) {
            if(caught){
                isCaughtAt(target);
                //position += targetMovment;
            }
            else{
                arriveTarget(target);
            }
        }
        
        
        // there are some problems with that because of the pointers…
        if  (!caught){
            if (maxSpeed >= maxStandardSpeed) {
                maxSpeed -= 0.1f;
            }
            
            for (int i=0; i<fleeTargets.size(); i++) {
                ofVec3f pos = ofVec3f(fleeTargets[i]->x,fleeTargets[i]->y,0);
                if (position2D.distance(pos) < HARVESTER_RADIUS+20.0){
                    maxSpeed = 8.0f;
                    fleeTarget(*fleeTargets[i]);
                }
            }
            
          
            
            for (int i=0; i<predators.size(); i++){
                if (inSight(predators[i]->position) && !predators[i]->dead) {
                    maxSpeed = 8.0f;
                    evade(*predators[i]);
                }
            }
            
            for (int i=0; i<prey.size(); i++){
                //if (!inSight(prey[i]->position)) continue;
                if (!prey[i]->dead) {
                    pursue(*prey[i]);
                }
                
            }
            
        }
        
        //fleeTargets.clear();
        
        if (dead)
        {
        //wander();
        }
        if (position.x != position.x) {
            cout<<"error nan"<<endl;
        }
    }
    
    void update();
    void arriveTarget(ofVec3f * _target);
    void isCaughtAt(ofVec3f *_target);
    void fleeTarget(ofVec3f _target);
    
    void addTarget(ofVec3f * _target){
        target = _target;
    }
    void clearTargets(){
        target = NULL;
    }
    
    void addFleeTarget(ofVec3f * _target){
        fleeTargets.push_back(_target);
    }
    void clearFleeTargets(){
        fleeTargets.clear();
    }
    
    void addPredator(trxVehicle * _target){
        predators.clear();
        predators.push_back(_target);
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




