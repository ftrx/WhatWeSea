//
//  trxVehicle.h
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 21.04.13.
//
//

#pragma once

#include "BiologicalVehicle.h"

class trxVehicle : public BiologicalVehicle {
	
protected:
	
	
	
public:
	
	ofVec3f * target = NULL;
    ofVec3f targetMovment;
    bool caught = false;
    bool dead = false;
    
    vector <ofVec3f *> fleeTargets;
	
	trxVehicle(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : BiologicalVehicle(_x, _y, _z) {
        
	}
    
    template<typename Type> void flock(vector<Type>& vehicles)
	{
        SteeredVehicle::flock(vehicles);
        
        if (target) {
            if(caught){
                isCaughtAt(target);
                //position += targetMovment;
                
                for (int i=0; i< trails.size(); i++) {
                    trails[i] += targetMovment;
                }
                
                arriveTarget(target);
            }
            else {
                arriveTarget(target);
            }
        }
        
        for (int i=0; i<fleeTargets.size(); i++) {
            fleeTarget(fleeTargets[i]);
        }
        fleeTargets.clear();
        //wander();
        
    }
    
    
    void arriveTarget(ofVec3f * _target);
    void isCaughtAt(ofVec3f *_target);
    void fleeTarget(ofVec3f * _target);
    void addTarget(ofVec3f * _target){
        
        if (!target){
        target = _target;
        }
    }
    void addTargetMovment(ofVec3f * _targetMovement){
     /*
        if (!targetMovment){
            targetMovment = *_targetMovement;
        }
      */
    }
    
    
    void clearTargets(){
        target = 0;
    }
};




