// ofApp.h
#pragma once

#include "ofMain.h"
#include "ofxKinectV2.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    ofxKinectV2 kinect;
    ofxKinectV2::Settings settings;
    ofTexture depthTex;
    
    ofPixels depthPixels;
    
    ofParameter<float> minDepth;
    ofParameter<float> maxDepth;
    ofParameter<float> anchorDepth;
    
    ofParameter<bool> showDepthMap;
    
    ofxPanel guiPanel;
};
