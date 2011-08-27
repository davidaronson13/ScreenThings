#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 2560, 1024, OF_WINDOW);
	//ofSetFullscreen(true);
	ofRunApp(new testApp());
}
