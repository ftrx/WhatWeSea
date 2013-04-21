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
	
	vector <ofVec3f *> targets;
    vector <ofVec3f *> fleeTargets;
	
	trxVehicle(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : BiologicalVehicle(_x, _y, _z) {
        
	}
    
    template<typename Type> void flock(vector<Type>& vehicles)
	{
        SteeredVehicle::flock(vehicles);
        
        for (int i=0; i<targets.size(); i++) {
            arriveTarget(targets[i]);
        }
        
        for (int i=0; i<fleeTargets.size(); i++) {
            fleeTarget(fleeTargets[i]);
        }
        fleeTargets.clear();
        
    }
    
    
    void arriveTarget(ofVec3f * _target);

    void fleeTarget(ofVec3f * _target);
    void addTarget(ofVec3f * _target){
        targets.push_back(_target);
    }
    
    
    void clearTargets(){
        targets.clear();
    }
};




