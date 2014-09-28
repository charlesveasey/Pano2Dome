#include "ofMain.h"
#include "ofApp.h"
#include "ofxMultiGLFWWindow.h"

//========================================================================
int main( ){
	ofxMultiGLFWWindow win;
    win.windowCount = 1;
    ofSetupOpenGL(&win, 1024,1024, OF_WINDOW);
	ofRunApp(new ofApp());
}
