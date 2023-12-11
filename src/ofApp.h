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
    void pruneTrackingObjects();
    void clearCanvasFbo();
    void saveScreen();
    void draw();
    ofPath polyToPath(ofPolyline poly);
    ofPolyline lerpPolyline(ofPolyline poly1, ofPolyline poly2);
    
    ofxKinectV2 kinect;
    ofTexture irTex;
    ofPixels irPixels;
    ofTexture depthTex;
    ofPixels depthPixels;
    
    float xMultiplier, yMultiplier;
    
    ofRectangle drawBounds;
    ofRectangle drawBoundsTopLeft, drawBoundsTopRight, drawBoundsBottomLeft, drawBoundsBottomRight;
    
    ofFbo canvasFbo;
    ofFbo visionFbo;
    ofFbo irFbo;
    ofFbo depthFbo;
    ofPixels blobPixels;
    ofImage blobImage;
    
    // For now, just one
    std::map<int, ofPolyline> prevNagiTrackingMap;
    std::map<int, ofPolyline> nagiTrackingMap;
    // We're going to be using these to determine
    // which objects we're still tracking
    // (and idsToDelete is to send a delete message to the other side)
    std::vector<int> presentIds = {};
    std::vector<int> idsToDelete = {};
    
    ofxCv::ContourFinder contourFinder;
    
    ofxButton clearScreenButton;
    ofxButton takePhotoButton;
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
    
    ofParameter<int> secondsToClear;
    int ellapsedMillisSinceNagiSeen;
    
    ofxPanel guiPanel;
    
    
};
