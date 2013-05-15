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
    
    vector<ofVec3f> normals;
    for (int i=0; i<bones.size()-1; i++) {
        ofVec3f dir0 = ofVec3f(0,0,1);
        ofVec3f dir1 = bones.at(i+1)- bones.at(i);
        ofVec3f norm =dir0.cross(dir1).normalize();
        if (i>0) {
            norm = (norm+normals.at(i-1)).normalize();
        }
        normals.push_back(norm);
    }
    ofVec3f dir0 = ofVec3f(0,0,1);
    ofVec3f dir1 = bones.at(bones.size()-1)- bones.at(bones.size()-2);
    ofVec3f norm =dir0.cross(dir1).normalize();
    normals.push_back(norm);

    boneNormals = normals;
    
    
    float width = length*0.5*0.5; // texture ratio 100px:200px -> 0.5
    
    vector<ofVec3f> vecs;
    for (int bone=0; bone<bones.size(); bone++) {
        vecs.push_back(ofVec3f(bones[bone].x-boneNormals[bone].x*width,bones[bone].y-boneNormals[bone].y*width,bones[bone].z));
        vecs.push_back(ofVec3f(bones[bone].x+boneNormals[bone].x*width,bones[bone].y+boneNormals[bone].y*width,bones[bone].z));
    }
    vertexes = vecs;
    
    SteeredVehicle::update();
}

void trxVehicle::arriveTarget(ofVec3f * _target){
    
    arrive(* _target);
    
}

void trxVehicle::isCaughtAt(ofVec3f * _target){
    
    //position = *_target;
    arrive(* _target);
    float dist = position.distance(*_target);
    if (dist <= 50.0 && onWay){
        maxSpeed = 2.0f;
        tooCloseDist = 1.0f;
        wanderDistance = 1.0f;
        dead=true;
    }
    
}

void trxVehicle::fleeTarget(ofVec3f _target){
    flee(_target);
}


void trxVehicle::evade(trxVehicle& target)
{
	float lookAheadTime = (float)position.distance(target.position) / maxSpeed;
	
	ofVec3f targetVelocity;
	targetVelocity.set(target.velocity);
	targetVelocity *= lookAheadTime;
	
	ofVec3f predictedTarget;
	predictedTarget.set(target.position);
	predictedTarget -= targetVelocity;
	
	flee(predictedTarget);

}