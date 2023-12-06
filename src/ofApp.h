// ofApp.h
#pragma once

#include "ofMain.h"
#include "ofxKinectV2.h"
#include "ofxGui.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void updateBlobs();
    void updateCanvas();
    void updateOutlines();
    void draw();
    
    ofxKinectV2 kinect;
    ofTexture irTex;
    ofPixels irPixels;
    ofTexture depthTex;
    ofPixels depthPixels;
    
    ofRectangle drawBounds;
    ofRectangle drawBoundsTopLeft, drawBoundsTopRight, drawBoundsBottomLeft, drawBoundsBottomRight;
        
    ofFbo canvasFbo;
    ofFbo visionFbo;
    ofFbo irFbo;
    ofFbo depthFbo;
    ofPixels blobPixels;
    ofImage blobImage;
    
    // For now, just one
    cv::Rect prevBrush;
    cv::Rect currBrush;
    
    ofxCv::ContourFinder contourFinder;
    
    ofParameter<bool> showDebugGrid;
    ofParameter<bool> showIntermediary;
    
    // Controls for Kinect
    ofxGuiGroup kinectGuiGroup;
    ofParameter<bool> showDepthMap;
    ofParameter<float> minDepth;
    ofParameter<float> maxDepth;
    ofParameter<int> anchorDepth;
    ofParameter<bool> showIRMap;
    ofParameter<int> minIR;
    ofParameter<int> maxIR;
    
    // Controls for Contour Finder
    ofxGuiGroup contourFinderGuiGroup;
    ofParameter<bool> showContours;
    ofParameter<float> minContourArea;
    ofParameter<float> maxContourArea;
    ofParameter<int> persistence;
    
    ofParameter<int> millisToClear;
    int ellapsedMillisSinceClear;
    
    ofxPanel guiPanel;
    
    
};
