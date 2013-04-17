//
//  trxXML.cpp
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 17.04.13.
//
//

#include "trxXML.h"

trxXML::trxXML() {
    filename = "fishSettings.xml";
}


trxXML::trxXML(string _filename) {
    filename = _filename;
    
}

void trxXML::setup(){

    
	if( XML.loadFile(filename) ){
		message = "mySettings.xml loaded!";
	}else{
		message = "unable to load mySettings.xml check data/ folder";
	}
    XML.pushTag("FLOCKS");
    flockNumber = XML.getNumTags("FLOCK");
    XML.popTag();
}


string trxXML::getTexture(int _flockID){
    string texturePath;
    //we push into the _flockID Flock tag
    //this temporarirly treats the tag as
    //the document root.
    XML.pushTag("FLOCKS");
        texturePath = XML.getValue("FLOCK:TEXTURE_PATH", "futter.png",_flockID);
    //cout << texturePath << endl;
    //this pops us out of the STROKE tag
    //sets the root back to the xml document
    XML.popTag();
    return texturePath;
}

int trxXML::getIntValue(int _flockID, string _tag){
    int value;
    //we push into the _flockID Flock tag
    //this temporarirly treats the tag as
    //the document root.
    XML.pushTag("FLOCKS");
    value = XML.getValue("FLOCK:"+_tag, 0,_flockID);
    //cout << texturePath << endl;
    //this pops us out of the STROKE tag
    //sets the root back to the xml document
    XML.popTag();
    return value;
}

float trxXML::getFloatValue(int _flockID, string _tag){
    float value;
    //we push into the _flockID Flock tag
    //this temporarirly treats the tag as
    //the document root.
    XML.pushTag("FLOCKS");
    value = XML.getValue("FLOCK:"+_tag, 0.0,_flockID);
    //cout << texturePath << endl;
    //this pops us out of the STROKE tag
    //sets the root back to the xml document
    XML.popTag();
    return value;
}


