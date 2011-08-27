#include "testApp.h"

GLfloat lightOnePosition[] = {0, 40, 100.0, 0.0};
GLfloat lightOneColor[] = {0.99, 0.99, 0.99, 1.0};

GLfloat lightTwoPosition[] = {-40.0, 40, 100.0, 0.0};
GLfloat lightTwoColor[] = {0.99, 0.99, 0.99, 1.0};

//--------------------------------------------------------------
void testApp::setup() {
		
	
		
	
	ofSetVerticalSync(true);
	
	//some model / light stuff
    glEnable (GL_DEPTH_TEST);
    glShadeModel (GL_SMOOTH);
	
    /* initialize lighting */
    glLightfv (GL_LIGHT0, GL_POSITION, lightOnePosition);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, lightOneColor);
    glEnable (GL_LIGHT0);
    glLightfv (GL_LIGHT1, GL_POSITION, lightTwoPosition);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, lightTwoColor);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
    glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable (GL_COLOR_MATERIAL);
	
    //load the squirrel model - the 3ds and the texture file need to be in the same folder
    eyeModel.loadModel("3DS/eye.3ds", 20);
	  eyeModelR.loadModel("3DS/eye.3ds", 20);
    //you can create as many rotations as you want
    //choose which axis you want it to effect
    //you can update these rotations later on
	blobX=165;
	blobY=0;
    eyeModel.setRotation(0, blobY, 1, 0, 0);
	eyeModel.setRotation(0, blobX, 0, 1, 0);
      eyeModel.setScale(1.8, 1.8, 1.8);
    eyeModel.setPosition(ofGetWidth()*.85, ofGetHeight()*.5, -800);
	
	eyeModelR.setRotation(0, 0, 1, 0, 0);
	eyeModelR.setRotation(0, 190, 0, 1, 0);
	eyeModelR.setScale(1.8, 1.8, 1.8);
    eyeModelR.setPosition(ofGetWidth()*.15, ofGetHeight()*.5, -800);
	
	//kinect.init(true);  //shows infrared image
	kinect.init();
	kinect.setVerbose(true);
	kinect.open();
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThresh.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
	
	nearThreshold = 230;
	farThreshold  = 70;
	bThreshWithOpenCV = true;
	
	ofSetFrameRate(60);
	
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	// start from the front
	pointCloudRotationY = 180;
	
	drawPC = false;
	
	//
	
	
}

//--------------------------------------------------------------
void testApp::update() {
	ofBackground(20, 70, 30);
	
	kinect.update();
	if(kinect.isFrameNew())	// there is a new frame and we are connected
	{

		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
			
		//we do two thresholds - one for the far plane and one for the near plane
		//we then do a cvAnd to get the pixels which are a union of the two thresholds.	
		if( bThreshWithOpenCV ){
			grayThreshFar = grayImage;
			grayThresh = grayImage;
			grayThresh.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThresh.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		}else{
		
			//or we do it ourselves - show people how they can work with the pixels
		
			unsigned char * pix = grayImage.getPixels();
			int numPixels = grayImage.getWidth() * grayImage.getHeight();

			for(int i = 0; i < numPixels; i++){
				if( pix[i] < nearThreshold && pix[i] > farThreshold ){
					pix[i] = 255;
				}else{
					pix[i] = 0;
				}
			}
		}

		//update the cv image
		grayImage.flagImageChanged();
	
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    	// also, find holes is set to true so we will get interior contours as well....
    	contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);
		
		if (contourFinder.nBlobs >=1) {
			blobX = ofMap(contourFinder.blobs[0].centroid.x, 0, kinect.width, 70, -50) ;
			blobY = ofMap(contourFinder.blobs[0].centroid.y, 0, kinect.width, 110, 260) ;
			
			/*if (blobY > 240) {
				angle--;
				if(angle<-30) angle=-30;
				kinect.setCameraTiltAngle(angle);
			}
			if (blobY < 150) {
				angle++;
				if(angle<-30) angle=-30;
				kinect.setCameraTiltAngle(angle);
			}
			 */
		}else {
			
		}

		
		
		eyeModel.setRotation(0, blobY, 1, 0, 0);
		eyeModel.setRotation(1, blobX, 0, 1, 0);
		
		eyeModelR.setRotation(0, blobY, 1, 0, 0);
		eyeModelR.setRotation(1, blobX-20, 0, 1, 0);
		
		
		//eyeModel.setRotation(1, 270 + ofGetElapsedTimef() * 60, 0, 1, 0);
				
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	ofSetColor(255, 255, 255);
	if(drawPC){
		ofPushMatrix();
		ofTranslate(ofGetWidth()/4,ofGetHeight()/2);
		// we need a proper camera class
		drawPointCloud();
		ofPopMatrix();
		pointCloudRotationY = pointCloudRotationY * -1;
		ofPushMatrix();
		ofTranslate(ofGetWidth()*.75,ofGetHeight()/2);
		// we need a proper camera class
		drawPointCloud();
		ofPopMatrix();
		pointCloudRotationY = pointCloudRotationY * -1;
		
	}else{
		//kinect.drawDepth(10, 10, 400, 300);
		//kinect.draw(420, 10, 400, 300);

		//grayImage.draw(10, 320, 400, 300);
		contourFinder.draw(0, 0, 2560, 1024);
	
	

	ofSetColor(255, 255, 255);
	stringstream reportStream;
	reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
								 << ofToString(kinect.getMksAccel().y, 2) << " / " 
								 << ofToString(kinect.getMksAccel().z, 2) << endl
				 << "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
				 << "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
				 << "set near threshold " << nearThreshold << " (press: + -)" << endl
				 << "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.nBlobs
				 	<< ", fps: " << ofGetFrameRate() << endl
				 << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl
				 << "press UP and DOWN to change the tilt angle: " << angle << " degrees";
	ofDrawBitmapString(reportStream.str(),20,666);
	
	glPushMatrix();
	
	//draw in middle of the screen
	//glTranslatef(ofGetWidth()/2,ofGetHeight()/2,0);
	//tumble according to mouse
	//glRotatef(-mouseY,1,0,0);
	//glRotatef(mouseX,0,1,0);
	//glTranslatef(-ofGetWidth()/2,-ofGetHeight()/2,0);
	
	ofSetColor(255, 255, 255, 255);
	eyeModel.draw();
	eyeModelR.draw();
	glPopMatrix();
		}
	
}

void testApp::drawPointCloud() {
	ofScale(400, 400, 400);
	int w = 640;
	int h = 480;
	ofRotateY(pointCloudRotationY);
	float* distancePixels = kinect.getDistancePixels();
	glBegin(GL_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			ofPoint cur = kinect.getWorldCoordinateFor(x, y);
			ofColor color = kinect.getCalibratedColorAt(x,y);
			glColor3ub((unsigned char)color.r,(unsigned char)color.g,(unsigned char)color.b);
			glVertex3f(cur.x, cur.y, cur.z);
		}
	}
	glEnd();
}

//--------------------------------------------------------------
void testApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	switch (key) {
		case ' ':
			bThreshWithOpenCV = !bThreshWithOpenCV;
		break;
		case'p':
			drawPC = !drawPC;
			break;
	
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
		case '<':		
		case ',':		
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
		case '-':		
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
		case 'o':
			kinect.setCameraTiltAngle(angle);	// go back to prev tilt
			kinect.open();
			break;
		case 'c':
			kinect.setCameraTiltAngle(0);		// zero the tilt
			kinect.close();
			break;

		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;

		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
	pointCloudRotationY = x;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}

