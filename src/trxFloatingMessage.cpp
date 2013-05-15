//
//  trxFloatingMessage.cpp
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 07.05.13.
//
//

#include "trxFloatingMessage.h"


trxFloatingMessage::trxFloatingMessage(){
    ofTrueTypeFont::setGlobalDpi(72);
	font.loadFont("fonts/NewsGot-Reg.otf", textSize, true, true);
	font.setLineHeight(textSize+4.0);
	font.setLetterSpacing(1.037);
    text = "";
}


void trxFloatingMessage::update(){
    if (position.z < endHight) {
        position+=velocity;
        
        if (position.z < targetHight) {
            color.a = ofMap(position.z, 0, targetHight, 0, 200);
        }
        else{
            color.a = ofMap(position.z, targetHight, endHight, 200, 0);
        }
    }
    
    if (position.z >= endHight) {
        color.a = 0;
        active = false;
    }
}

void trxFloatingMessage::draw(){
    ofPushMatrix();
    ofTranslate(position);
    ofPushStyle();
    ofSetColor(color);
    font.drawString(text, -bounds.width/2.0, -bounds.height/2.0);
    ofPopStyle();
    ofPopMatrix();
}
string trxFloatingMessage::wrapString(string text, int width) {
	
	string typeWrapped = "";
	string tempString = "";
	vector <string> words = ofSplitString(text, " ");
	
	for(int i=0; i<words.size(); i++) {
		
		string wrd = words[i];
		
		// if we aren't on the first word, add a space
		if (i > 0) {
			tempString += " ";
		}
		tempString += wrd;
		
		int stringwidth = font.stringWidth(tempString);
        
		if(stringwidth >= width) {
			typeWrapped += "\n";
			tempString = wrd;		// make sure we're including the extra word on the next line
		} else if (i > 0) {
			// if we aren't on the first word, add a space
			typeWrapped += " ";
		}
		
		typeWrapped += wrd;
	}
	
	return typeWrapped;
	
}


void trxFloatingMessage::setText(string _text){
    text = _text;
    
    text = wrapString(text, 1000);
    
    
    bounds = font.getStringBoundingBox(text, 0, 0);
}

void trxFloatingMessage::setPosition(ofVec3f _position){
    position = _position;
}


