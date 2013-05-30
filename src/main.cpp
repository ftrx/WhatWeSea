#include "whatWeSea.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1920,1080, OF_FULLSCREEN);			// <-------- setup the GL context  set to OF_GAME_MODE for nicer fullscreen
    window.setGlutDisplayString("rgba double samples>=4 alpha");
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new whatWeSea());

}
