#include "pano.h"

//--------------------------------------------------------------
void Pano::setup(){
    ofBackground(0);
    
	// defaults
	width = 1024;
	height = 1024;

	tv = .01;
	rv = .1;
	mod = 1;

	displayIndex = 0;

	// sphere holds pano
    sphere.setScale(.5);
    sphere.setResolution(50);
    sphere.setPosition(0, 0, 0);
    sphere.setOrientation(ofVec3f(90,0,0));
    sphere.mapTexCoords(1, 0, 0, 1);
    
	// domemaster render
    domemaster.setup();
    domemaster.setCameraPosition(0,0,10);
    
	// image buffer 
    bufferCnt = 3;
    for (int i=0; i<bufferCnt; i++){
        ofImage img;
        images.push_back(img);
        display.push_back(i);
        fIndex.push_back(i);
    }
}

//--------------------------------------------------------------
void Pano::update(){
    if (startup && images[display[0]].getTextureReference().isAllocated()){
        startup = false;
	}
    
    if (files.size() == 0){
        loading = false;
    }
    else if (files.size() == 1){
        if (images[display[0]].getTextureReference().isAllocated())
            loading = false;
        else
            loading = true;
    }
    else if (files.size() == 2){
        if (images[display[0]].getTextureReference().isAllocated() && images[display[1]].getTextureReference().isAllocated())
            loading = false;
        else
            loading = true;
    }
    else {
        if (images[display[0]].getTextureReference().isAllocated() && images[display[1]].getTextureReference().isAllocated() && images[display[2]].getTextureReference().isAllocated())
            loading = false;
        else
            loading = true;
    }
    
	// keyboard	
	if (ofGetKeyPressed(OF_KEY_SHIFT)){
        mod = 2;
    }
	else {
		mod = 1;
	}
	if (ofGetKeyPressed(OF_KEY_COMMAND) || ofGetKeyPressed(OF_KEY_CONTROL)){
		if (ofGetKeyPressed(OF_KEY_LEFT)){
            move(tv*mod, 0, 0);
        }
        if (ofGetKeyPressed(OF_KEY_RIGHT)){
            move(-tv*mod, 0, 0);
        }
        if (ofGetKeyPressed(OF_KEY_UP)){
            move(0, -tv*mod, 0);
        }
        if (ofGetKeyPressed(OF_KEY_DOWN)){
            move(0, tv*mod, 0);
        }
    }
	else if (ofGetKeyPressed(OF_KEY_ALT)) {
        if (ofGetKeyPressed(OF_KEY_UP)){
            move(0, 0, tv*mod);
        }
        if (ofGetKeyPressed(OF_KEY_DOWN)){
            move(0, 0, -tv*mod);
        }
    }
    else {
		if (ofGetKeyPressed(OF_KEY_LEFT)){
            rotate(rv*mod, 0, -1, 0);
        }
        if (ofGetKeyPressed(OF_KEY_RIGHT)){
            rotate(rv*mod, 0, 1, 0);
        }
        if (ofGetKeyPressed(OF_KEY_UP)){
            rotate(rv*mod, 1, 0, 0);
        }
        if (ofGetKeyPressed(OF_KEY_DOWN)){
            rotate(rv*mod, -1, 0, 0);
        }
    }
}

//--------------------------------------------------------------
void Pano::draw(){
    ofBackground(0);
    
    if (startup)
        ofSetColor(45);
    else
        ofSetColor(255);
    
	// draw sphere
    ofEnableNormalizedTexCoords();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
        
    images[display[0]].bind();
  
		for (int i=0; i<domemaster.renderCount; i++){
			domemaster.begin(i);
			sphere.draw();
			domemaster.end(i);
		}    
    images[display[0]].unbind();

    glDisable(GL_CULL_FACE);
    ofDisableNormalizedTexCoords();

	// draw domemaster
    domemaster.draw();
    
	// draw text
    ofSetColor(255);
    ofDrawBitmapString("look    arrows", width-215, 20);
    ofDrawBitmapString("move    ctrl/cmd + arrows", width-215, 33);
    ofDrawBitmapString("zoom    alt + up/down", width-215, 46);
    ofDrawBitmapString("faster  shift", width-215, 59);
    ofDrawBitmapString("reset   r", width-215, 72);
    if (!loading){
        ofDrawBitmapString("PANO 2 DOME", 20, 20);
        ofDrawBitmapString("drop equirectangular pano files", 20, 33);
        ofDrawBitmapString("previous file   -", 20, 46);
        ofDrawBitmapString("next file       +", 20, 59);
    }
    else if (loading) {
        ofDrawBitmapString("loading", 20, 20);
	}
}

void Pano::move(float x, float y, float z){
	sphere.move(x, y, z);
}

void Pano::rotate(float degrees, float vx, float vy, float vz){
	sphere.rotate(degrees, vx, vy, vz);
}

//--------------------------------------------------------------
void Pano::keyPressed(int key){
    if (key == 'r'){
        reset();
    }
    else if (key == 61){ // +
        next();
    }
    else if (key == 45){ // -
        previous();
    }
}

//--------------------------------------------------------------
void Pano::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void Pano::reset(){
    sphere.setPosition(ofVec3f(0,0,0));
    sphere.setOrientation(ofVec3f(90,0,0));
}

//--------------------------------------------------------------
void Pano::windowResized(int w, int h){
    height = ofGetWindowHeight();
    width = height;
    domemaster.resize(width, height);
}

//--------------------------------------------------------------
void Pano::dragEvent(ofDragInfo dragInfo){
    startup = true;
    
    files.clear();
    files = dragInfo.files;
    
    if (files.size()){
        images[0].clear();
        loader.loadFromDisk(images[0], files[0]);
        fIndex[0] = 0;
        loading = true;
        
        if (files.size() > 2){
            display[2] = 2;
            images[2].clear();
            fIndex[2] = files.size()-1;
            loader.loadFromDisk(images[2], files[files.size()-1]);
        }
        if (files.size() > 1){
            display[1] = 1;
            images[1].clear();
            loader.loadFromDisk(images[1], files[1]);
            displayIndex = 0;
        }
	}
}

//--------------------------------------------------------------
void Pano::next(){
    if (files.size() < 2 || loading) return;
       
    if (files.size() > 3){
		loading = true;

        display[2] = display[0];
        display[0] = display[1];
        
        fIndex[2] = fIndex[0];
        fIndex[0] = fIndex[1];
        
        fileIndex = fIndex[0] + 1;
        
        if (fileIndex >= files.size())
            fileIndex = 0;
        
        for (int i=0; i<images.size(); i++){
            if (i != display[0] && i != display[2]){
                images[i].clear();
                loader.loadFromDisk(images[i], files[fileIndex]);
                display[1] = i;
                fIndex[1] = fileIndex;
                break;
            }
        }
    }
	else {
        displayIndex++;
        if (displayIndex >= files.size())
            displayIndex = 0;
        display[0] = displayIndex;		
	}
}

//--------------------------------------------------------------
void Pano::previous(){
    if (files.size() < 2 || loading) return;
    
    if (files.size() > 3){
        loading = true;

        display[1] = display[0];
        display[0] = display[1];
        
        fIndex[1] = fIndex[0];
        fIndex[0] = fIndex[1];        
        
        fileIndex = fIndex[0] - 1;
        
        if (fileIndex < 0)
            fileIndex = files.size()-1;
        
        for (int i=0; i<images.size(); i++){
            if (i != display[0] && i != display[1]){
                images[i].clear();
                loader.loadFromDisk(images[i], files[fileIndex]);
                display[2] = i;
                fIndex[2] = fileIndex;
                break;
            }
        }
    }
	else {
        displayIndex--;
        if (displayIndex < 0)
			displayIndex = files.size()-1;
        display[0] = displayIndex;		
	}
}

//--------------------------------------------------------------
void Pano::exit(){
    loader.waitForThread();
}