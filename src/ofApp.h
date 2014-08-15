#pragma once

#include "ofMain.h"
#include "ofxDomemaster.h"
#include "ofxThreadedImageLoader.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void drawScene();
    
    ofxDomemaster domemaster;
    ofSpherePrimitive sphere;
    
    ofxThreadedImageLoader loader;
    vector<ofImage> images;
    int total;
    ofImage* image;
    ofTexture texture;
    
    void parseKey(int key, bool down);
    void reset();
    void previous();
    void next();
    int fileIndex;
};
