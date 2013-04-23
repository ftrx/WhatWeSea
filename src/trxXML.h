//
//  trxXML.h
//  BoidsAndTUIO
//
//  Created by Fabian Troxler on 17.04.13.
//
//


#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"

class trxXML {


    
public:
    trxXML();
    trxXML(string _filename);
    void setup();
    
    string getString(int _flockID, string _tag);
    int getIntValue(int _flockID, string _tag);
    float getFloatValue(int _flockID, string _tag);
    
    vector<int> getConnections(int _flockID);
    
    ofxXmlSettings XML;
    ofTrueTypeFont TTF;
    string xmlStructure;
    string message;
    string filename;
    
    int flockNumber;
    int converterNumber;

};