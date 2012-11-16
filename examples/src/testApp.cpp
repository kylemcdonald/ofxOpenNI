#include "testApp.h"

#include "OpenNI.h"
using namespace openni;

//--------------------------------------------------------------
void testApp::setup(){	

	ofSetCircleResolution(100);
	//ofSetFrameRate(300);
//	ofSetFullscreen(true);
	ofSetVerticalSync(true);

	setupOpenNi();
	setupNite();

	start();

	faceTracker.setup();
}


//--------------------------------------------------------------
void testApp::update(){
	camString = stringstream();

//	onNewFrame(depthStream);
}


//--------------------------------------------------------------
void testApp::draw(){

	ofShortPixels* depthPixels = depthPixelsDoubleBuffer[0];

	unsigned short* p = depthPixels->getPixels();
	for (int i=0; i < depthPixels->size(); i++)
	{
		unsigned short d = p[i] - 00;
		unsigned short k = (d-400); //400, 4500 -> 0 - 4000;
		k = k >> 3; // - > 0 - 1000
		colorPixels[3*i + 0] = 0;
		colorPixels[3*i + 1] = k & 0xff;
		colorPixels[3*i + 2] = k & 0xff;
	}

	depthTexture.loadData(colorPixels);


	ofSetHexColor(0xffffff);
	depthTexture.draw(0,0, depthTexture.getWidth(), depthTexture.getHeight());

	ofCircle(mouseX, mouseY,20);
	
	ofSetHexColor(0x333333);
	ofDrawBitmapString("fps:" + ofToString(ofGetFrameRate()), 10,10);

}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


//TODO: exception?
int testApp::setupOpenNi()
{
	Status rc = OpenNI::initialize();
	if (rc != ONI_STATUS_OK)
	{
		printf("Initialize failed\n%s\n", OpenNI::getExtendedError());
		return 1;
	}
	OpenNI::addListener(this);

	rc = device.open(ONI_ANY_DEVICE);
	if (rc != ONI_STATUS_OK)
	{
		printf("Couldn't open device\n%s\n", OpenNI::getExtendedError());

		printf("opening file");
		rc = device.open("C:\\f\\q.oni");
		if (rc != ONI_STATUS_OK)
		{
			return 2;
		}
	}


	if (device.getStreamSourceInfo(ONI_STREAM_SOURCE_DEPTH) != NULL)
	{
		rc = depthStream.create(device, ONI_STREAM_SOURCE_DEPTH);
		if (rc != ONI_STATUS_OK)
		{
			printf("Couldn't create depth stream\n%s\n", OpenNI::getExtendedError());
		}
	}

	
//	stream.readFrame(&frame);


	int w = depthStream.getVideoMode().getXResolution();
	int h = depthStream.getVideoMode().getYResolution();
	
	depthTexture.allocate(w, h, GL_RGB);

	
	for (int i=0 ; i<2; i++)
	{
		depthPixelsDoubleBuffer[i] = new ofShortPixels();
		depthPixelsDoubleBuffer[i]->allocate(w, h, OF_IMAGE_GRAYSCALE);
	}
	colorPixels.allocate(w, h, OF_IMAGE_COLOR);

//	onNewFrame(depthStream);

	return 0;

}

void testApp::onNewFrame( Stream& stream )
{
	stream.readFrame(&frame);
	
	const unsigned short* data = (const unsigned short*)frame.getData();

	bool debugPrintMiddlePixel = false;
	if (debugPrintMiddlePixel)
	{
		int middleIndex = (frame.getHeight()+1)*frame.getWidth()/2;
		DepthPixel* pDepth = (DepthPixel*)frame.getData();
		printf("[%08llu] %8d\n", frame.getTimestamp(), pDepth[middleIndex]);
	}

	depthPixelsDoubleBuffer[1]->setFromPixels(data, frame.getWidth(), frame.getHeight(), OF_IMAGE_GRAYSCALE);

	swap(depthPixelsDoubleBuffer[0],depthPixelsDoubleBuffer[1]);
	//InterlockedExchangePointer(depthPixelsDoubleBuffer[0],depthPixelsDoubleBuffer[1]);

	
	nite::HandTrackerFrameRef handTrackerFrame;
	nite::Status niteRc = handTracker.readFrame(&handTrackerFrame);
	if (niteRc != NITE_STATUS_OK)
	{
		printf("Get next frame failed\n");
	}

	const nite::Array<nite::GestureData>& gestures = handTrackerFrame.getGestures();
	for (int i = 0; i < gestures.getSize(); ++i)
	{
		if (gestures[i].getState() == NITE_GESTURE_STATE_COMPLETED)
		{
			nite::HandId newId;
			handTracker.startHandTracking(gestures[i].getCurrentPosition(), &newId);
		}
	}

	const nite::Array<nite::HandData>& hands = handTrackerFrame.getHands();
	for (int i = 0; i < hands.getSize(); ++i)
	{
		const nite::HandData& hand = hands[i];
		if (hand.getState() != NITE_HAND_STATE_LOST)
		{
			printf("%d. (%5.2f, %5.2f, %5.2f)\n", hand.getId(), hand.getPosition().x, hand.getPosition().y, hand.getPosition().z);
		}
	}


		//notify face?
}



//--------------------------------------------------------------
void testApp::exit(){

	faceTracker.stopThread();
	faceTracker.waitForThread();

	depthStream.removeListener(this);
	depthStream.stop();
	depthStream.destroy();
	device.close();

	OpenNI::shutdown();
	nite::NiTE::shutdown();

}

int testApp::setupNite()
{
	nite::Status niteRc;

	niteRc = nite::NiTE::initialize();
	if (niteRc != NITE_STATUS_OK)
	{
		printf("NiTE initialization failed\n");
		return 1;
	}

	niteRc = handTracker.create();
	if (niteRc != NITE_STATUS_OK)
	{
		printf("Couldn't create user tracker\n");
		return 3;
	}


	return 0;
}

int testApp::start()
{
	Status rc = depthStream.start();
	if (rc != ONI_STATUS_OK)
	{
		printf("Couldn't start the depth stream\n%s\n", OpenNI::getExtendedError());
	}

	// Register to new frame
	rc = depthStream.addListener(this);
	if (rc != ONI_STATUS_OK)
	{
		printf("Couldn't register listener for the depth stream\n%s\n", OpenNI::getExtendedError());
	}


	handTracker.startGestureDetection(NITE_GESTURE_HAND_RAISE);
	handTracker.startGestureDetection(NITE_GESTURE_WAVE);
	printf("\nWave your hand to start tracking it...\n");

	return 0;

}
