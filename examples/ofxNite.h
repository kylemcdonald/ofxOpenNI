#include "NiTE.h"


class ofxNite : nite::UserTracker::Listener
{

	void shutdown(){
		nite::NiTE::shutdown();
	}

private:

deque<nite::UserTrackerFrameRef> userTrackerFrameDeque;
nite::UserTrackerFrameRef userTrackerFrame;
nite::UserTracker userTracker;



void testApp::onNewFrame()
{
	nite::UserTrackerFrameRef userTrackerFrame;
	nite::Status niteRc = userTracker.readFrame(&userTrackerFrame);
	if (niteRc != NITE_STATUS_OK)
	{
		printf("Get next frame failed\n");
	}

	userTrackerFrameDeque.push_front(userTrackerFrame);

	if (userTrackerFrameDeque.size() > 10)
	{
		userTrackerFrameDeque.pop_back();
	}
}


};


//draw users heads,,,
if (0 && !userTrackerFrameDeque.empty())
{

	userTrackerFrame = userTrackerFrameDeque.front();

	const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();
	for (int i = 0; i < users.getSize(); ++i)
	{
		const nite::UserData& user = users[i];
		updateUserState(user,userTrackerFrame.getTimestamp());
		if (user.getState() == nite::USER_STATE_NEW)
		{
			userTracker.startSkeletonTracking(user.getId());
		}
		else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
		{
			const nite::SkeletonJoint& head = user.getSkeleton().getJoint(nite::JOINT_HEAD);
			if (head.getPositionConfidence() > .5)
			{

				ofVec2f headScreenPos;
				userTracker.convertJointCoordinatesToDepth(head.getPosition().x,head.getPosition().y,head.getPosition().z,&headScreenPos.x, &headScreenPos.y);
				ofCircle(ofPoint(headScreenPos), 10);
				printf("%d. (%5.2f, %5.2f, %5.2f)\n", user.getId(), head.getPosition().x, head.getPosition().y, head.getPosition().z);
			}
		}
	}
}





//////////////////////////////////////////////////////////////////////////
// Print user state
#define MAX_USERS 10
nite::UserState g_userStates[MAX_USERS] = {(nite::UserState)-1};
nite::SkeletonState g_skeletonStates[MAX_USERS] = {nite::SKELETON_NONE};

#define USER_MESSAGE(msg) \
{printf("[%08llu] User #%d:\t%s\n",ts, user.getId(),msg); break;}

void updateUserState(const nite::UserData& user, unsigned long long ts)
{
	if(g_userStates[user.getId()] != user.getState())
	{
		switch(g_userStates[user.getId()] = user.getState())
		{
		case nite::USER_STATE_NEW:
			USER_MESSAGE("New")
		case nite::USER_STATE_VISIBLE:
			USER_MESSAGE("Visible")
		case nite::USER_STATE_OUT_OF_SCENE:
			USER_MESSAGE("Out of scene...")
		case nite::USER_STATE_LOST:
			USER_MESSAGE("Lost.")
		}
	}
	if(g_skeletonStates[user.getId()] != user.getSkeleton().getState())
	{
		switch(g_skeletonStates[user.getId()] = user.getSkeleton().getState())
		{
		case nite::SKELETON_NONE:
			USER_MESSAGE("Stopped tracking.")
		case nite::SKELETON_CALIBRATING:
			USER_MESSAGE("Calibrating...")
		case nite::SKELETON_TRACKED:
			USER_MESSAGE("Tracking!")
		case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
		case nite::SKELETON_CALIBRATION_ERROR_HANDS:
		case nite::SKELETON_CALIBRATION_ERROR_LEGS:
		case nite::SKELETON_CALIBRATION_ERROR_HEAD:
		case nite::SKELETON_CALIBRATION_ERROR_TORSO:
			USER_MESSAGE("Calibration Failed... :-|")
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// setup

int testApp::setupNite()
{
	nite::Status niteRc;

	niteRc = nite::NiTE::initialize();
	if (niteRc != NITE_STATUS_OK)
	{
		printf("NiTE initialization failed\n");
		return 1;
	}

	niteRc = userTracker.create();
	if (niteRc != NITE_STATUS_OK)
	{
		printf("Couldn't create user tracker\n");
		return 3;
	}
	userTracker.addListener(this);



	return 0;
}

//////////////////////////////////////////////////////////////////////////

