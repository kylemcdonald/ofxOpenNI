#ifndef _TEST_APP
#define _TEST_APP

#include "OpenNI.h"

#include "ofMain.h"
#include "ofxFaceTrackerThreaded.h"
#include "..\Scene.h"

#define MAX_DEVICES 2
#define MAX_HANDS 4

class testApp : public ofBaseApp, public openni::Stream::Listener, public openni::OpenNI::Listener
{

public:

	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	virtual void onNewFrame( openni::Stream& stream);

private:

//	void handEvent(ofxOpenNIHandEvent & event);
	int setupOpenNiDevice();



	ofxFaceTrackerThreaded faceTracker;

	ofEasyCam handCam;
	ofEasyCam sceneCam;


	struct Finger
	{
		std::deque<ofVec3f> position;
		bool isTracked;
		Finger() : isTracked(false){ position.push_front(ofVec3f());}
		ofVec3f getFilteredPosition(float a = 0.5f);

		static const int historySize = 10;
	};

	Finger fingers[MAX_HANDS];
	Scene scene;


	bool drawDebugString;
	bool drawOpenNiDebug;

	stringstream camString;

	ofVec3f facePos;
	ofVec2f screenPoint;
	deque<ofVec2f> screenPointHistory;


private:
	openni::FrameRef m_frame;

	openni::Stream depth;
	openni::Device device;

	ofShortPixels depthPixels;
	ofTexture depthTexture;


};

#endif
