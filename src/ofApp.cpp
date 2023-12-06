// ofApp.cpp
#include "ofApp.h"

const int KINECT_DEPTH_WIDTH = 512;
const int KINECT_DEPTH_HEIGHT = 424;

void ofApp::setup()
{
    int width = ofGetScreenWidth();
    int height = ofGetScreenHeight();
    ofSetWindowShape(width, height);
    
    // Set up canvas
    ofBackground(255);
    
    // Set up drawing bounds for mapping 512x424 to full screen later
    drawBounds.set(0, 0, KINECT_DEPTH_WIDTH, KINECT_DEPTH_HEIGHT);
    drawBounds.scaleTo(ofGetCurrentViewport(), OF_SCALEMODE_FILL);
    
    int halfWidth = ofGetWidth() / 2;
    int halfHeight = ofGetHeight() / 2;
    int halfKinectWidth = KINECT_DEPTH_WIDTH / 2;
    int halfKinectHeight = KINECT_DEPTH_HEIGHT / 2;
    drawBoundsTopLeft.set(0, 0, halfWidth, halfHeight);
    drawBoundsTopRight.set(0, 0, halfWidth, halfHeight);
    drawBoundsBottomLeft.set(0, 0, halfWidth, halfHeight);
    drawBoundsBottomRight.set(0, 0, halfWidth, halfHeight);
    
    drawBoundsTopLeft.scaleTo(ofRectangle(0, 0, halfWidth, halfHeight), OF_SCALEMODE_FILL);
    drawBoundsTopRight.scaleTo(ofRectangle(halfWidth, 0, halfWidth, halfHeight), OF_SCALEMODE_FILL); // I have no idea why -320 is neeeded
    drawBoundsBottomLeft.scaleTo(ofRectangle(0, halfHeight, halfWidth, halfHeight), OF_SCALEMODE_FILL);
    drawBoundsBottomRight.scaleTo(ofRectangle(halfWidth, halfHeight, halfWidth, halfHeight), OF_SCALEMODE_FILL);
    
    // Set up frame buffer
    ofFboSettings fboSettings;
    fboSettings.width = width;
    fboSettings.height = height;
    canvasFbo.allocate(fboSettings);
    
    // Set up GUI panel
    guiPanel.setup("BRUSH_STROKES", "settings.json");
    guiPanel.add(showDebugGrid.set("Show debug grid", false));
    guiPanel.add(millisToClear.set("ms before clear", 600, 0, 10000));
    
    // Kinect GUI options
    kinectGuiGroup.setup("Kinect");
    kinectGuiGroup.add(minDepth.set("Min depth", 0.5f, 0.5f, 8.f));
    kinectGuiGroup.add(maxDepth.set("Max depth", 5.f, 0.5f, 8.f));
    kinectGuiGroup.add(minIR.set("Min IR value", 0, 0, 255));
    kinectGuiGroup.add(maxIR.set("Max IR value", 255, 255, 255));
    kinectGuiGroup.add(anchorDepth.set("Base pixel size", 1, 1, 20));
    guiPanel.add(&kinectGuiGroup);
    
    // Contour finder GUI options
    contourFinderGuiGroup.setup("Contour Finder");
    contourFinderGuiGroup.add(showContours.set("Show contours", false));
    contourFinderGuiGroup.add(minContourArea.set("Min area", 0.01f, 0, 300.f));
    contourFinderGuiGroup.add(maxContourArea.set("Max area", 0.4f, 0, 300.f));
    contourFinderGuiGroup.add(persistence.set("Persistence", 15, 0, 1000));
    guiPanel.add(&contourFinderGuiGroup);
    
    guiPanel.loadFromFile("settings.json");
    
    // Set up Kinect
    ofxKinectV2::Settings kinectSettings;
    kinectSettings.enableRGB = false;
    kinectSettings.enableIR = true;
    kinectSettings.enableRGBRegistration = false;
    //    kinectSettings.config.MinDepth = minDepth;
    //    kinectSettings.config.MaxDepth = maxDepth;
    kinect.open(0, kinectSettings);
    kinect.irExposure = 0.1f; // this was totally not documented
    
    ellapsedMillisSinceClear = 0;
}

void ofApp::update()
{
    kinect.update();
    
    // Only load the data if there is a new frame to process.
    if (kinect.isFrameNew())
    {
        // Depth code
        depthPixels = kinect.getDepthPixels();
        depthTex.loadData(depthPixels);
        
        // Infrared luminosity code
        irPixels = kinect.getIRPixels();
        irTex.loadData(irPixels);
        
        // Update blobs, outlines, and canvas FBOs
        updateBlobs();
        updateOutlines();
        updateCanvas();
    }
}

void ofApp::updateBlobs() {
    blobPixels.allocate(KINECT_DEPTH_WIDTH, KINECT_DEPTH_HEIGHT, OF_IMAGE_GRAYSCALE);
    
    for (int y = 0; y < irPixels.getHeight(); y++) {
        for (int x = 0; x < irPixels.getWidth(); x++) {
            float ir = irPixels.getColor(x, y).r;
            blobPixels.setColor(x, y, ir > minIR ? ofColor::white :  ofColor::black);
        }
    }
    
    blobImage.setFromPixels(blobPixels);
}

void ofApp::updateCanvas() {
    canvasFbo.begin();
    ofSetColor(ofColor::black);
    ofFill();
    float xMultiplier = (float) ofGetScreenWidth() / depthPixels.getWidth();
    float yMultiplier = (float) ofGetScreenHeight() / depthPixels.getHeight();
    
    for (int _x = 0; _x < currBrush.width; _x++) {
        for (int _y = 0; _y < currBrush.height; _y++) {
            float dist = kinect.getDistanceAt(currBrush.x + _x, currBrush.y + _y);

            if (dist > minDepth && dist < maxDepth) {
                int newX = ofLerp(prevBrush.x + _x, currBrush.x + _x, 0.1);
                int newY = ofLerp(prevBrush.y + _y, currBrush.y + _y, 0.1);

                float alpha = ofMap(dist, minDepth, maxDepth, 255, 100);
                float radius = ofMap(dist, minDepth, maxDepth, anchorDepth, 0);
                float paintSplatter = ofNoise(newX, newY);
                float randomPaintSplatter = ofRandom(0, anchorDepth);
                
                ofColor color = ofColor(0, 0, 0, alpha);
                ofDrawCircle(newX * xMultiplier + paintSplatter * anchorDepth, newY * yMultiplier + paintSplatter * anchorDepth, radius);
            }
        }
    }
    
    if (ofGetElapsedTimeMillis() - ellapsedMillisSinceClear < millisToClear) {
        ofEnableAlphaBlending();
        ofSetColor(255, 2);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    } else {
        ellapsedMillisSinceClear = ofGetElapsedTimeMillis();
        ofBackground(255);
    }
    
    canvasFbo.end();
}

void ofApp::updateOutlines() {
    visionFbo.allocate(KINECT_DEPTH_WIDTH, KINECT_DEPTH_HEIGHT);
    
    // Using OpenCV's contour finder to find the different bounding boxes
    contourFinder.setMinAreaRadius(minContourArea);
    contourFinder.setMaxAreaRadius(maxContourArea);
    contourFinder.getTracker().setPersistence(persistence);
    contourFinder.findContours(showDepthMap ? depthPixels : irPixels);
    std::vector<cv::Rect> blobs = contourFinder.getBoundingRects();
    
    // Draw the contour in its own FBO to render later
    visionFbo.begin();
    ofClear(255);
    ofSetColor(ofColor::white);
    ofNoFill();
    for(int i = 0; i < blobs.size(); i++) {
        int label = contourFinder.getLabel(i);
        ofColor color = ofColor::red;
        cv::Rect boundingRect = blobs[i];
        
        color.setHueAngle(color.getHueAngle() + label * 5);
        ofSetColor(color);
        
        ofDrawRectangle(boundingRect.x, boundingRect.y, boundingRect.width, boundingRect.height);
    }
    
    // for now, just set one nagi
    if (blobs.size() > 0) {
        prevBrush = currBrush;
        currBrush = blobs[0];
    }
    
    visionFbo.end();
}

void ofApp::draw()
{
    ofBackground(ofColor::white);
    if (showDebugGrid) {
        depthTex.draw(drawBoundsTopLeft);
        irTex.draw(drawBoundsTopRight);
        canvasFbo.draw(drawBoundsBottomRight);
        blobImage.draw(drawBoundsBottomLeft);
        visionFbo.draw(drawBoundsBottomLeft);
        
        // Get the point distance using the SDK function (in meters).
        float mouseX = ofGetMouseX();
        float mouseY = ofGetMouseY();
        float mappedY = ofMap(mouseY, 0, ofGetHeight()/2, 0, KINECT_DEPTH_HEIGHT);
        // If cursor is in depth map
        if (drawBoundsTopLeft.inside(mouseX, mouseY)) {
            float mappedX = ofMap(mouseX, 0, ofGetWidth()/2, 0, KINECT_DEPTH_WIDTH);
            float distAtMouse = kinect.getDistanceAt(mappedX, mappedY);
            ofDrawBitmapStringHighlight("Depth: " + ofToString(distAtMouse, 3), mouseX, mouseY  + 10);
        }
        // If cursor is in IR map
        else if (drawBoundsTopRight.inside(mouseX, mouseY)) {
            float mappedX = ofMap(mouseX, ofGetWidth()/2, ofGetWidth(), 0, KINECT_DEPTH_WIDTH);
            float ir = irPixels.getColor(mappedX, mappedY).r;
            ofDrawBitmapStringHighlight("IR: " + ofToString(ir, 3), mouseX, mouseY  + 10);
        }
        
        
        
        
    } else {
        canvasFbo.draw(0, 0);
        
        if (showContours) {
            visionFbo.draw(drawBounds);
        }
    }
    
    guiPanel.draw();
}
