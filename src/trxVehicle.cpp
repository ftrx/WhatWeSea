//
//  trxVehicle.cpp
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 21.04.13.
//
//

#include "trxVehicle.h"


void trxVehicle::update(){
    
    bones[0] = position;
    for (int i=1; i<bones.size(); i++) {
        ofVec3f direction = bones[i] - bones[i-1];
        direction.normalize();
        direction *= bonelength;
        bones[i] = bones[i-1]+ direction;
    }
    
    
    SteeredVehicle::update();
}

void trxVehicle::arriveTarget(ofVec3f * _target){
    
    arrive(* _target);
    
}

void trxVehicle::isCaughtAt(ofVec3f * _target){
    
    //position = *_target;
    arrive(* _target);
    
}

void trxVehicle::fleeTarget(ofVec3f * _target){
    flee(* _target);
}