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
    XML.pushTag("Objects");
   
    objectNumber = XML.getNumTags("Object");
}


string trxXML::getString(int _flockID, string _tag){
    string texturePath;
    //we push into the _flockID Flock tag
    //this temporarirly treats the tag as
    //the document root.
    XML.pushTag("Objects");
        texturePath = XML.getValue("Object:"+_tag, "",_flockID);
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
    XML.pushTag("Objects");
    value = XML.getValue("Object:"+_tag, 0,_flockID);
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
    XML.pushTag("Objects");
    value = XML.getValue("Object:"+_tag, 0.0,_flockID);
    //cout << texturePath << endl;
    //this pops us out of the STROKE tag
    //sets the root back to the xml document
    XML.popTag();
    return value;
}

vector<int> trxXML::getConnections(int _flockID){
    vector<int> connections;
    XML.pushTag("Objects");
    XML.pushTag("Object", _flockID);
    XML.pushTag("Connections");
    int ids = XML.getNumTags("CID");
    for (int i=0; i < ids; i++) {
        connections.push_back(XML.getValue("CID", NULL,i));
    }
    XML.popTag();
    XML.popTag();
    XML.popTag();
    return connections;
}


