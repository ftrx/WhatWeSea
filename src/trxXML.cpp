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



typedef struct{
    string character;
    string code;
}charSubstitution;

void subsChars(string & origString){
    charSubstitution chars[]={
        {"à","\xE0"}, {"á","\xE1"}, {"â","\xE2"}, {"ã","\xE3"}, {"ä","\xE4"}, {"æ","\xE6"},
        {"ò","\xF2"}, {"ó","\xF3"} ,{"ô","\xF4"}, {"õ","\xF5"}, {"ö","\xF6"},
        {"ù","\xF9"}, {"ú","\xFA"}, {"û","\xFB"}, {"ü","\xFC"},
        {"è","\xE8"}, {"é","\xE9"}, {"ê","\xEA"}, {"ë","\xEB"},
        {"ì","\xEC"}, {"í","\xED"}, {"î","\xEE"}, {"ï","\xEF"},
        {"ç","\xE7"}, {"Ç","\xC7"}
    };
    
    for(int i=0; i<24; i++){
        
        while(origString.find(chars[i].character)!=string::npos){
            origString = origString.substr(0,origString.find(chars[i].character)) + chars[i].code + origString.substr(origString.find(chars[i].character)+2);
            
        }
    };
}



string trxXML::getString(string _default="", string _tag=""){
    string text;
    //we push into the _flockID Flock tag
    //this temporarirly treats the tag as
    //the document root.

        text = XML.getValue(_tag, _default);
    //cout << texturePath << endl;
    //this pops us out of the STROKE tag
    //sets the root back to the xml document
    subsChars(text);
    return text;
}

int trxXML::getIntValue(int _default=0, string _tag=""){
    int value;
    //we push into the _flockID Flock tag
    //this temporarirly treats the tag as
    //the document root.

    value = XML.getValue(_tag, _default);
    //cout << texturePath << endl;
    //this pops us out of the STROKE tag
    //sets the root back to the xml document

    return value;
}

float trxXML::getFloatValue(float _default=0.0, string _tag=""){
    float value;
    //we push into the _flockID Flock tag
    //this temporarirly treats the tag as
    //the document root.

    value = XML.getValue(_tag, _default);
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
