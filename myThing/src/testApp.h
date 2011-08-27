#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofx3DModelLoader.h"
//#include "ofx3DUtils.h"


class testApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();
		void exit();
	
		void drawPointCloud();
	
		void blink(int status);
		void blinkoff(int status);

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
	
		ofxKinect kinect;

		ofxCvColorImage		colorImg;

		ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayThresh;
		ofxCvGrayscaleImage 	grayThreshFar;

		ofxCvContourFinder 	contourFinder;
    
        //ofxCamera camera;

	
		int					blobX;
		int					blobY;
	
		bool				blinking;
		
		bool				bThreshWithOpenCV;
		bool				drawPC;

		int 				nearThreshold;
		int					farThreshold;

		int					angle;
		
		int 				pointCloudRotationY;
	
        int				prevTime;
	
		ofx3DModelLoader eyeModel;
		ofx3DModelLoader eyeModelR;
};
