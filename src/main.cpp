#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	//ofSetupOpenGL(750,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
    ofGLFWWindowSettings settings;
    //settings.resizable = false;
    settings.setSize(655, 768);
	//ofRunApp(new ofApp());
    ofCreateWindow(settings);
    return ofRunApp(new ofApp);
}
