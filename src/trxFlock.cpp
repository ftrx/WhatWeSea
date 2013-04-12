//
//  trxFlock.cpp
//  ofxBoid_test2
//
//  Created by Fabian Troxler on 01.04.13.
//
//

#include "trxFlock.h"


trxFlock::trxFlock(float _x, float _y, float _z, int _id, vector <trxHarvester>* _harvesters){
    id = _id;
    position = ofVec3f(_x,_y,_z);
    myHarvesters = _harvesters;
    //harvesters.push_back(trxHarvester(ofGetMouseX(),ofGetMouseY(),250));
    //ofLoadImage(texture, "Thunfisch.png");
	
	boidNum = 200;
    color = ofColor(255,255,255);
	target = ofVec3f(0, 0, 0);
	
	for (int i = 0; i < 0; i++)
	{
		createNewBoid();
	}
	
	
    
    // upload the data to the vbo
	int total = (int)boids.size();
	vbo.setVertexData(&points[0], total, GL_DYNAMIC_DRAW);
	vbo.setNormalData(&sizes[0], total, GL_DYNAMIC_DRAW);
    
    // trx: only 1 shader can be loaded per time, so if we want to change shader we must load each shader subsequently
    // load the shader
	/*if(shader.load("shader")) {
		printf("Shader is loaded\n");
	}*/
}

void trxFlock::update(){
    
    //myHarvesters[0].position = ofVec3f(ofGetMouseX(),ofGetMouseY(),0);
    
    points.clear();
    sizes.clear();
    vector <BiologicalVehicle> tmp;
	for (int i = 0; i < boids.size(); i++)
	{
		boids[i].flock(boids,myHarvesters);
		boids[i].update();
		boids[i].bounce(ofGetWidth(), ofGetHeight(), 600);
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
	int total = (int)boids.size();
	vbo.setVertexData(&points[0], total, GL_DYNAMIC_DRAW);
	vbo.setNormalData(&sizes[0], total, GL_DYNAMIC_DRAW);

}

void trxFlock::draw(){
    glDepthMask(GL_FALSE);
	
	ofSetColor(color);
	
	// this makes everything look glowy :)

	
	// bind the shader and camera
	// everything inside this function
	// will be effected by the shader/camera
	//shader.begin();
	//camera->begin();
	
	// bind the texture so that when all the points
	// are drawn they are replace with our dot image
    

	texture->bind();
	vbo.draw(GL_POINTS, 0, (int)boids.size());
	texture->unbind();

	//camera->end();
	//shader.end();

	
    
    /*
    ofColor(255,255,255);
    for (int i=0; i<boids.size(); i++) {
        ofCircle(ofPoint(boids[i].position),2);
    }
    */
   
	// check to see if the points are
	// sizing to the right size
		
	glDepthMask(GL_TRUE);
	

}

void trxFlock::drawCircles(){
    ofSetColor(255,255,255);
    for (int i=0; i<boids.size(); i++) {
        ofCircle(ofPoint(boids[i].position),2);
    }

}

void trxFlock::drawInfo(){
    //harvesters[0].draw();
    ofPushMatrix();
    ofTranslate(position);

	ofSetColor(color);
    string info = "Flockposition:"+ofToString(position)+"\n";
	info += "Total Points "+ofToString(points.size())+"\n";
	info += "Total Boids "+ofToString(boids.size())+"\n";
    ofDrawBitmapString(info, 0,0);
    ofPopMatrix();
}

void trxFlock::removeDeathVehicle(BiologicalVehicle* _v){
    
    

}

int trxFlock::returnID(){

    return id;
}



void trxFlock::createNewBoid(){
    BiologicalVehicle v(position.x+ofRandom(-200,200),position.y+ofRandom(-200,200),position.z+ofRandom(-400, 400));
    v.velocity = ofVec3f(ofRandom(0.1f,2.0f),ofRandom(0.1f,2.0f),ofRandom(0.1f,2.0f));
    v.lifeSpan = ofRandom(3.0f,6.0f);
    v.maxSpeed = 2.0f;
    v.maxForce = 0.5f;
    v.inSightDist = 140.0f;
    v.tooCloseDist = 40.0f;
    v.maxTrailSize= 20.0f;
    boids.push_back(v);
    points.push_back(v.position);
    sizes.push_back(ofVec3f(190));
}