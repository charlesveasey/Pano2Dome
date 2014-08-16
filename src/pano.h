#pragma once

#include "ofxDomemaster.h"
#include "ofxThreadedImageLoader.h"

class Pano {
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    
    void reset();
    void previous();
    void next();
	void move(float x, float y, float z);
	void rotate(float degrees, float vx, float vy, float vz);

	float tv;
	float rv;
	float mod;

private:
    ofxDomemaster domemaster;
    ofSpherePrimitive sphere;
    
    ofxThreadedImageLoader loader;
    vector<ofImage> images;
    int total;
    int fileIndex;

	int width; 
	int height;

	vector<string> files;
	vector<int> display;
	vector<int> fIndex;

	int bufferCnt;
	int displayIndex;

	bool loading;
	bool startup;
};