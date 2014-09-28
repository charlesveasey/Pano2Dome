#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	pano.setup();
    
    

#ifdef TARGET_WIN32
	xbox.setup();
	gamepad = xbox.getGamepadPtr(0);
	bNext = false;
	bPrevious = false;
#endif
}

//--------------------------------------------------------------
void ofApp::update(){
	pano.update();

#ifdef TARGET_WIN32
	if (gamepad->connected) {
		bNext = gamepad->dPadRight;
		bPrevious = gamepad->dPadLeft;

		xbox.update();

		if (bNext && !gamepad->dPadRight)
			pano.next();
		else if (bPrevious && !gamepad->dPadLeft)
			pano.previous();

		float tx = ofMap(gamepad->thumbLX, -1, 1, pano.tv, -pano.tv);
		float ty = ofMap(gamepad->thumbLY, -1, 1, pano.tv, -pano.tv);
		float tz = 0;

		if (gamepad->rightTrigger)		
			tz = ofMap(gamepad->rightTrigger, -1, 1, -pano.tv, pano.tv);
		else if (gamepad->leftTrigger)		
			tz = ofMap(gamepad->leftTrigger, -1, 1, pano.tv, -pano.tv);

		pano.move(tx, ty, tz);
	
		float rx = ofMap(gamepad->thumbRX, -1, 1, -pano.rv, pano.rv); 
		float ry = ofMap(gamepad->thumbRY, -1, 1, -pano.rv, pano.rv); 

		pano.rotate(rx, 0, 1, 0);
		pano.rotate(ry, 1, 0, 0);

		if (gamepad->y) pano.reset();
	}
#endif
}

//--------------------------------------------------------------
void ofApp::draw(){
	pano.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	pano.keyPressed(key);
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	pano.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	pano.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
	pano.dragEvent(dragInfo);
}

//--------------------------------------------------------------
void ofApp::exit(){
	pano.exit();
#ifdef TARGET_WIN32
	xbox.exit();
#endif
}