#pragma once

#include "ofMain.h"
#include "pano.h"

#ifdef TARGET_WIN32
#include "ofxXboxController.h"
#endif

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);

	Pano pano;

#ifdef TARGET_WIN32
	ofxXboxController xbox;
	Gamepad* gamepad;
	bool bNext;
	bool bPrevious;
#endif

};