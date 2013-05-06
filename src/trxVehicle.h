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
    bool onWay = false;
    bool dead = false;
    
    vector<ofVec3f> bones;
    float bonelength = 10.0;
    
    vector <ofVec3f *> fleeTargets;
	
	trxVehicle(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : BiologicalVehicle(_x, _y, _z) {
        bones.assign(4, ofVec3f(_x,_y,_z));
	}
    
    template<typename Type> void flock(vector<Type>& vehicles)
	{
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
        
        
        if (target) {
            if(caught){
                isCaughtAt(target);
                position += targetMovment;
                /*
                for (int i=0; i< trails.size(); i++) {
                    trails[i] += targetMovment;
                }*/

            }
            else {
                arriveTarget(target);
            }
        }
        
        for (int i=0; i<fleeTargets.size(); i++) {
            fleeTarget(fleeTargets[i]);
        }
        fleeTargets.clear();
        
        if (dead)
        {
        //wander();
        }
    }
    
    void update();
    void arriveTarget(ofVec3f * _target);
    void isCaughtAt(ofVec3f *_target);
    void fleeTarget(ofVec3f * _target);
    void addTarget(ofVec3f * _target){
        if (!target){
        target = _target;
        }
    }
    void addTargetMovment(ofVec3f * _targetMovement){
        position += *_targetMovement;
    }
    
    
    void clearTargets(){
        target = NULL;
    }


};




