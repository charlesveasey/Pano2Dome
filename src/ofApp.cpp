#include "ofApp.h"

bool leftKey = false;
bool rightKey = false;
bool upKey = false;
bool downKey = false;
bool ctrlKey = false;
bool altKey = false;

float tv = .01;
float rv = .1;
float mod = 1;

vector<string> files;
int bufferCnt;
bool bAllocated = false;
int displayIndex = 0;
int nextDisplayIndex = -1;
int prevDisplayIndex = -1;

vector<int> display;
vector<int> fIndex;
bool loading = false;
bool startup = true;

int width = 1024;
int height = 1024;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetColor(255);
    
    sphere.setScale(.5);
    sphere.setResolution(50);
    sphere.setPosition(0, 0, 0);
    sphere.setOrientation(ofVec3f(90,0,0));
    
    domemaster.setup();
    domemaster.setCameraPosition(0,0,10);
    
    bufferCnt = 4;
    
    for (int i=0; i<bufferCnt; i++){
        ofImage img;
        images.push_back(img);
        display.push_back(i);
        fIndex.push_back(i);
    }
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    if (startup && images[display[0]].getTextureReference().isAllocated())
        startup = false;
    
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
    
    
    if (ofGetKeyPressed(OF_KEY_COMMAND)){
        if (leftKey){
            sphere.move(tv*mod, 0, 0);
        }
        if (rightKey){
            sphere.move(-tv*mod, 0, 0);
        }
        if (upKey){
            sphere.move(0, -tv*mod, 0);
        }
        if (downKey){
            sphere.move(0, tv*mod, 0);
        }
    }
    else if (ofGetKeyPressed(OF_KEY_ALT)) {
        if (upKey){
            sphere.move(0, 0, tv*mod);
        }
        if (downKey){
            sphere.move(0, 0, -tv*mod);
        }
    }
    else {
        if (leftKey){
            sphere.rotate(rv*mod, 0, -1, 0);
        }
        if (rightKey){
            sphere.rotate(rv*mod, 0, 1, 0);
        }
        if (upKey){
            sphere.rotate(rv*mod, -1, 0, 0);
        }
        if (downKey){
            sphere.rotate(rv*mod, 1, 0, 0);
        }
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    if (startup)
        ofSetColor(30);
    else
        ofSetColor(255);
    
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

    
    domemaster.draw();
    
    
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
    else if (loading)
        ofDrawBitmapString("loading", 20, 20);
    
    
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    parseKey(key, true);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    parseKey(key, false);
}

//--------------------------------------------------------------
void ofApp::parseKey(int key, bool down){
    
    if (key == OF_KEY_LEFT){
        leftKey = down;
    }
    else if (key == OF_KEY_RIGHT){
        rightKey = down;
    }
    else if (key == OF_KEY_UP){
        upKey = down;
    }
    else if (key == OF_KEY_DOWN){
        downKey = down;
    }
    else if (key == OF_KEY_CONTROL){
        ctrlKey = down;
    }
    else if (key == OF_KEY_SUPER){
        ctrlKey = down;
    }
    else if (key == OF_KEY_ALT){
        altKey = down;
    }
    else if (key == OF_KEY_SHIFT){
        if (down) mod = 2;
        else mod = 1;
    }
    else if (key == 'r' && down){
        reset();
    }
    else if (key == 61 && down){ // +
        next();
    }
    else if (key == 45 && down){ // -
        previous();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::reset(){
    sphere.setPosition(ofVec3f(0,0,0));
    sphere.setOrientation(ofVec3f(90,0,0));
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    height = ofGetWindowHeight();
    width = height;
    domemaster.resize(width, height);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    startup = true;
    
    files.clear();
    files = dragInfo.files;
    
    if (files.size()){
        bAllocated = false;
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
void ofApp::next(){
    
    if (files.size() < 2) return;
    
    if (loading) return;
    loading = true;
    
    
    if (files.size() > 2){
        
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
    else if (files.size() == 2){
        displayIndex++;
        if (displayIndex >= 2)
            displayIndex = 0;
        display[0] = displayIndex;
    }
    
}


//--------------------------------------------------------------
void ofApp::previous(){
    if (files.size() < 2) return;
    
    if (loading) return;
    loading = true;
    
    if (files.size() > 2){
        
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
    else if (files.size() == 2){
        displayIndex--;
        if (displayIndex < 0)
            displayIndex = 1;
        display[0] = displayIndex;
    }
}



//--------------------------------------------------------------
void ofApp::exit(){
    loader.waitForThread();
}







