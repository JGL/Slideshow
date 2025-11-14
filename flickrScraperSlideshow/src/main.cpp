#include "ofMain.h"
#include "testApp.h"


//========================================================================
int main( ){

	ofSetupOpenGL(SLIDESHOW_SCREEN_WIDTH, SLIDESHOW_SCREEN_HEIGHT, OF_WINDOW);			// <-------- setup the GL context
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	
	testApp app;
	ofRunApp(&app);
	
}
