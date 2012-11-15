#include "testApp.h"

#include "OpenNI.h"
using namespace openni;

//--------------------------------------------------------------
void testApp::setup(){	
	counter = 0;
	ofSetCircleResolution(100);
	texScreen.allocate(300,300,GL_RGB);
	ofBackground(230,230,240);

	ofSetFrameRate(90);

	setupOpenNiDevice();
	faceTracker.setup();
}


//--------------------------------------------------------------
void testApp::update(){
	counter += 5;
	camString = stringstream();

	//onNewFrame(depthStream);
}


//--------------------------------------------------------------
void testApp::draw(){

	// 1st, draw on screen:
	ofSetHexColor(0x66CC33);	
	ofRect(100,100,300,300);

	ofSetHexColor(0xffffff);
	glPushMatrix();
	glTranslatef(200,200,0);
	glRotatef(counter,0,0,1);
	ofCircle(0,0,80);
	ofCircle(100,0,10);	// a small one
	glPopMatrix();
	ofSetHexColor(0x333333);
	ofDrawBitmapString("(a) on screen", 150,200);

	ofShortPixels* depthPixels = depthPixelsDoubleBuffer[0];

	ofPixels colorPixels;
	colorPixels.allocate(depthPixels->getWidth(), depthPixels->getHeight(), OF_IMAGE_COLOR);
	
	
	unsigned short* p = depthPixels->getPixels();
	for (int i=0; i < depthPixels->size(); i++)
	{
		unsigned short d = p[i] - 00;
		unsigned short k = (d-400); //400, 4500 -> 0 - 4000;
		k = k >> 3; // - > 0 - 1000

		/*
		unsigned char g = ((d & (0x1f << 5)) >> 5) * 6;
		unsigned char b = (d & 0x1f) * 6;
		b = g ? (0x1f*6) : b;
		
		
		ofColor c = ofColor::fromHsb(k, 255, 255, 1.0f);
		*/
		colorPixels[3*i + 0] = k & 0x0f;
		colorPixels[3*i + 1] = k & 0xff;
		colorPixels[3*i + 2] = k & 0xfff;


	}

	depthTexture.loadData(colorPixels);

	ofSetHexColor(0xffffff);

	depthTexture.draw(0,0, depthTexture.getWidth(), depthTexture.getHeight());

	//ofSetHexColor(0xFFCC33);	
	ofCircle(mouseX, mouseY,20);


	// 2nd, grab a portion of the screen into a texture
	// this is quicker then grabbing into an ofImage
	// because the transfer is done in the graphics card
	// as opposed to bringing pixels back to memory
	// note: you need to allocate the texture to the right size
	texScreen.loadScreenData(100,100,300,300);


	// finally, draw that texture on screen, how ever you want
	// (note: you can even draw the texture before you call loadScreenData, 
	// in order to make some trails or feedback type effects)
	glPushMatrix();
	ofSetHexColor(0xffffff);
	glTranslatef(550,300,0);
	//glRotatef(counter, 0.1f, 0.03f, 0);
	float width = 200 + 100 * sin(counter/200.0f);
	float height = 200 + 100 * sin(counter/200.0f);;
	texScreen.draw(-width/2,-height/2,width,height);
	glPopMatrix();

	glPushMatrix();
	ofSetHexColor(0xffffff);
	glTranslatef(700,210,0);
	glRotatef(counter, 0.1f, 0.03f, 0);
	texScreen.draw(-50,-50,100,100);

	ofSetHexColor(0xffffff);

	depthTexture.draw(0,0, depthTexture.getWidth(), depthTexture.getHeight());

	glPopMatrix();

	ofSetHexColor(0x333333);
	ofDrawBitmapString("(b) in a texture, very meta!", 500,200);

	ofDrawBitmapString(ofToString(ofGetFrameRate()), 500,500);

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
int testApp::setupOpenNiDevice()
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
		return 2;
	}


	if (device.getStreamSourceInfo(ONI_STREAM_SOURCE_DEPTH) != NULL)
	{
		rc = depthStream.create(device, ONI_STREAM_SOURCE_DEPTH);
		if (rc != ONI_STATUS_OK)
		{
			printf("Couldn't create depth stream\n%s\n", OpenNI::getExtendedError());
		}
	}
	rc = depthStream.start();
	
	int w = depthStream.getVideoMode().getXResolution();
	int h = depthStream.getVideoMode().getYResolution();
	
	depthTexture.allocate(w, h, GL_RGB);
	
	for (int i=0 ; i<2; i++)
	{
		depthPixelsDoubleBuffer[i] = new ofShortPixels();
		depthPixelsDoubleBuffer[i]->allocate(w, h, OF_IMAGE_GRAYSCALE);
	}

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

	return 0;

}

void testApp::onNewFrame( Stream& stream )
{
	stream.readFrame(&frame);
	
	const unsigned short* data = (const unsigned short*)frame.getData();

	int middleIndex = (frame.getHeight()+1)*frame.getWidth()/2;
	DepthPixel* pDepth;
	pDepth = (DepthPixel*)frame.getData();
	printf("[%08llu] %8d\n", frame.getTimestamp(),
		pDepth[middleIndex]);

	depthPixelsDoubleBuffer[1]->setFromPixels(data, frame.getWidth(), frame.getHeight(), OF_GRADIENT_CIRCULAR);

	InterlockedExchangePointer(depthPixelsDoubleBuffer[0],depthPixelsDoubleBuffer[1]);
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
}
