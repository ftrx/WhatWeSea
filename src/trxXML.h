//
//  trxXML.h
//  WhatWeSea
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
    void setup(string _category, string _items);
    
    string getString(string _default, string _tag);
    int getIntValue(int _default, string _tag);
    float getFloatValue(float _default, string _tag);
    
    vector<int> getConnections(int _flockID);
    
    ofxXmlSettings XML;
    ofTrueTypeFont TTF;
    string xmlStructure;
    string message;
    string filename;
    
    int flockNumber;
    int converterNumber;
    int objectNumber;

};