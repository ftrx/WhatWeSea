//
//  trxVehicle.cpp
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 21.04.13.
//
//

#include "trxVehicle.h"

void trxVehicle::arriveTarget(ofVec3f * _target){
    
    arrive(* _target);
    
}

void trxVehicle::isCaughtAt(ofVec3f * _target){
    
    
    
    position = *_target;
    //arrive(* _target);
    
}

void trxVehicle::fleeTarget(ofVec3f * _target){
    flee(* _target);
}