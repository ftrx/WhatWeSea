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

void trxXML::setup(string _category, string _items){
	if( XML.loadFile(filename) ){
		message = ofToString(filename)+ " loaded!";
	}else{
		message = "unable to load " + ofToString(filename) + " check data/ folder";
	}
    cout << message << endl;
    XML.pushTag(_category);
    objectNumber = XML.getNumTags(_items);
    XML.popTag();
}


string trxXML::getString(int _flockID, string _tag){
    string texturePath;
    //we push into the _flockID Flock tag
    //this temporarirly treats the tag as
    //the document root.

        texturePath = XML.getValue(_tag, "");
    //cout << texturePath << endl;
    //this pops us out of the STROKE tag
    //sets the root back to the xml document

    return texturePath;
}

int trxXML::getIntValue(int _flockID, string _tag){
    int value;
    //we push into the _flockID Flock tag
    //this temporarirly treats the tag as
    //the document root.

    value = XML.getValue(_tag, 0);
    //cout << texturePath << endl;
    //this pops us out of the STROKE tag
    //sets the root back to the xml document

    return value;
}

float trxXML::getFloatValue(int _flockID, string _tag){
    float value;
    //we push into the _flockID Flock tag
    //this temporarirly treats the tag as
    //the document root.

    value = XML.getValue(_tag, 0.0);
    //cout << texturePath << endl;
    //this pops us out of the STROKE tag
    //sets the root back to the xml document

    return value;
}

vector<int> trxXML::getConnections(int _flockID){
    vector<int> connections;
   

    XML.pushTag("Connections");
    int ids = XML.getNumTags("CID");
    for (int i=0; i < ids; i++) {
        connections.push_back(XML.getValue("CID", NULL,i));
    }
    XML.popTag();


    return connections;
}


