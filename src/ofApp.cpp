// ofApp.cpp
#include "ofApp.h"

void ofApp::setup()
{
    int width = ofGetScreenWidth();
    int height = ofGetScreenHeight();
    ofSetWindowShape(width, height);
    
    // Set up canvas
    ofBackground(255);
    
    // Set up GUI panel
    guiPanel.setup("DEPTH_DOTS", "settings.json");
    
    minDepth.set("Min depth", 0.5f, 0.5f, 8.f);
    maxDepth.set("Max depth", 1.3f, 0.5f, 8.f);
    
    anchorDepth.set("Base pixel size", .75f, 0.f, 5.f);
        
    showDepthMap.set("Show Kinect Depth Map", false);
    
    guiPanel.add(showDepthMap);
    guiPanel.add(minDepth);
    guiPanel.add(maxDepth);
    guiPanel.add(anchorDepth);
        
    // Set up Kinect
    settings.enableRGB = false;
    settings.enableIR = false;
    settings.enableRGBRegistration = false;
    
    settings.config.MinDepth = minDepth;
    settings.config.MaxDepth = maxDepth;
    
    kinect.open(0, settings);
}

void ofApp::update()
{
    kinect.update();
    
    // Only load the data if there is a new frame to process.
    if (kinect.isFrameNew())
    {
        depthPixels = kinect.getDepthPixels();
        depthTex.loadData(depthPixels);
    }
}

void ofApp::draw()
{
    ofSetColor(ofColor(0, 0, 0));
    ofFill();
    
    if (showDepthMap) {
        ofSetColor(255);
        ofFill();
        
        depthTex.draw(ofGetScreenWidth()/2 - depthPixels.getWidth()/2, ofGetScreenHeight()/2 - depthPixels.getHeight()/2);
        
        // Get the point distance using the SDK function (in meters).
        float distAtMouse = kinect.getDistanceAt(ofGetMouseX(), ofGetMouseY());
        ofDrawBitmapStringHighlight(ofToString(distAtMouse, 3), ofGetMouseX(), ofGetMouseY() - 10);
        
        // Get the point depth using the texture directly (in millimeters).
        const ofFloatPixels& rawDepthPix = kinect.getRawDepthPixels();
        int depthAtMouse = rawDepthPix.getColor(ofGetMouseX(), ofGetMouseY()).r;
        ofDrawBitmapStringHighlight(ofToString(depthAtMouse), ofGetMouseX() + 16, ofGetMouseY() + 10);
        
    }
    else {
        float xMultiplier = (float) ofGetScreenWidth() / depthPixels.getWidth();
        float yMultiplier = (float) ofGetScreenHeight() / depthPixels.getHeight() + 0.05;
        
        for (int y = 0; y < depthPixels.getHeight(); y += 2) {
            for (int x = 0; x < depthPixels.getWidth(); x += 2) {
                float dist = kinect.getDistanceAt(x, y);
                
                if (dist > minDepth && dist < maxDepth) {
                    float radius = ofMap(dist, minDepth, maxDepth, anchorDepth + 1, 1);
                    ofDrawCircle(x * xMultiplier + anchorDepth, y * yMultiplier + anchorDepth, radius);
                }
            }
        }
    }
    
    guiPanel.draw();
}
