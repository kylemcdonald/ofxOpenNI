/*******************************************************************************
*                                                                              *
*   PrimeSense NiTE 2.0                                                        *
*   Copyright (C) 2012 PrimeSense Ltd.                                         *
*                                                                              *
*******************************************************************************/

#ifndef _NITE_C_TYPES_H_
#define _NITE_C_TYPES_H_

#include <OpenNI.h>

typedef short int NiteUserId;

typedef struct NiteUserTracker* NiteUserTrackerHandle;

/** Available joints in skeleton */
typedef enum
{
	NITE_JOINT_HEAD,
	NITE_JOINT_NECK,

	NITE_JOINT_LEFT_SHOULDER,
	NITE_JOINT_RIGHT_SHOULDER,
	NITE_JOINT_LEFT_ELBOW,
	NITE_JOINT_RIGHT_ELBOW,
	NITE_JOINT_LEFT_HAND,
	NITE_JOINT_RIGHT_HAND,

	NITE_JOINT_TORSO,

	NITE_JOINT_LEFT_HIP,
	NITE_JOINT_RIGHT_HIP,
	NITE_JOINT_LEFT_KNEE,
	NITE_JOINT_RIGHT_KNEE,
	NITE_JOINT_LEFT_FOOT,
	NITE_JOINT_RIGHT_FOOT,
} NiteJointType;

#define NITE_JOINT_COUNT 15

/** 3D Point */
typedef struct
{
	float x, y, z;
} NitePoint3f;

/** Quaternion */
typedef struct 
{
	float x, y, z, w;
} NiteQuaternion;

/** Single joint of a skeleton */
typedef struct  
{
	/** Type of the joint */
	NiteJointType jointType;

	/** Position of the joint - in real world coordinates */
	NitePoint3f position;
	float positionConfidence;

	/** Orientation of the joint */
	NiteQuaternion orientation;
	float orientationConfidence;
} NiteSkeletonJoint;

/** 3D Box */
typedef struct  
{
	NitePoint3f mins;
	NitePoint3f maxs;
} NiteBoundingBox;

/** Possible states of the skeleton */
typedef enum
{
	/** No skeleton - skeleton was not requested */
	NITE_SKELETON_NONE,
	/** Skeleton requested, but still unavailable */
	NITE_SKELETON_CALIBRATING,
	/** Skeleton available */
	NITE_SKELETON_TRACKED,

	/** Possible reasons as to why skeleton is unavailable */
	NITE_SKELETON_CALIBRATION_ERROR_NOT_IN_POSE,
	NITE_SKELETON_CALIBRATION_ERROR_HANDS,
	NITE_SKELETON_CALIBRATION_ERROR_HEAD,
	NITE_SKELETON_CALIBRATION_ERROR_LEGS,
	NITE_SKELETON_CALIBRATION_ERROR_TORSO

} NiteSkeletonState;

/** Possible states of the user */
typedef enum
{
	/** User is new - this is the first time the user is available */
	NITE_USER_STATE_NEW,
	/** User is visible and already known */
	NITE_USER_STATE_VISIBLE,
	/** Known user is currently out of scene, but not lost */
	NITE_USER_STATE_OUT_OF_SCENE,
	/** User is lost. This is the last time this user will be seen */
	NITE_USER_STATE_LOST
} NiteUserState;

/** Skeleton - a set of joints */
typedef struct  
{
	NiteSkeletonJoint joints[NITE_JOINT_COUNT];
	NiteSkeletonState state;
} NiteSkeleton;

/** Snapshot of a specific user */
typedef struct  
{
	NiteUserId id;
	NiteBoundingBox boundingBox;
	NitePoint3f centerOfMass;

	NiteUserState state;

	NiteSkeleton skeleton;
} NiteUserData;

/** Snapshot of the scene segmentation*/
typedef struct
{
	NiteUserId* pixels;

	int width;
	int height;

	int stride;
} NiteUserMap;

/** 3D Plane */
typedef struct  
{
	NitePoint3f point;
	NitePoint3f normal;
} NitePlane;

/** Output snapshot of the User Tracker algorithm */
typedef struct  
{
	/** Number of users */
	int userCount;
	/** List of users */
	NiteUserData* pUser;

	/** Scene segmentation map */
	NiteUserMap userMap;
	/** The depth frame from which this data was learned */
	OniFrame* pDepthFrame;

	unsigned long long timestamp;
	int frameIndex;

	/** Confidence of the floor plane */
	float floorConfidence;
	/** Floor plane */
	NitePlane floor;
} NiteUserTrackerFrame;

typedef struct  
{
	OniGeneralCallback readyForNextFrame;
} NiteUserTrackerCallbacks;

/** Possible failure values */
typedef enum
{
	NITE_STATUS_OK,
	NITE_STATUS_ERROR,
	NITE_STATUS_BAD_USER_ID
} NiteStatus;
/*
	NOT_INITIALIZED
	BAD_USER_ID
*/

/** Available gestures types */
typedef enum
{
	NITE_GESTURE_WAVE,
	NITE_GESTURE_CLICK,
	NITE_GESTURE_HAND_RAISE
} NiteGestureType;

/** Possible state of a gesture. Currently only 'Complete' is used. */
typedef enum
{
	NITE_GESTURE_STATE_NEW,			// Unused
	NITE_GESTURE_STATE_IN_PROGRESS,	// Unused
	NITE_GESTURE_STATE_ABORTED,		// Unused
	NITE_GESTURE_STATE_COMPLETED
} NiteGestureState;

/** Possible state of a hand */
typedef enum
{
	/** This is a new hand - it is the first frame in which it is available*/
	NITE_HAND_STATE_NEW,
	/** This is a known hand */
	NITE_HAND_STATE_TRACKED,
	/** This hand was lost. It is the last frame in which it will provided */
	NITE_HAND_STATE_LOST,
} NiteHandState;

typedef short NiteHandId;

/** A snapshot of a specific hand */
typedef struct
{
	NiteHandId id;
	NitePoint3f position;
	NiteHandState state;
} NiteHandData;

/** A snapshot of a specific gesture */
typedef struct  
{
	NiteGestureType type;
	NitePoint3f currentPosition;
	NiteGestureState state;
} NiteGestureData;

/** Output snapshot of the Hand Tracker algorithm */
typedef struct
{
	/** Number of hands */
	int handCount;
	/** List of hands */
	NiteHandData* pHands;

	/** Number of gestures */
	int gestureCount;
	/** List of gestures */
	NiteGestureData* pGestures;

	/** The depth frame from which this data was learned */
	OniFrame* pDepthFrame;

	unsigned long long timestamp;
	int frameIndex;

} NiteHandTrackerFrame;

typedef struct  
{
	OniGeneralCallback readyForNextFrame;
} NiteHandTrackerCallbacks;

typedef struct NiteHandTracker* NiteHandTrackerHandle;

#endif // _NITE_C_TYPES_H_
