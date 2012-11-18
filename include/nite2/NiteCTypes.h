/*******************************************************************************
*                                                                              *
*   PrimeSense NiTE 2.0                                                        *
*   Copyright (C) 2012 PrimeSense Ltd.                                         *
*                                                                              *
*******************************************************************************/

#ifndef _NITE_C_TYPES_H_
#define _NITE_C_TYPES_H_

#include <OniCTypes.h>
#include "NiteCEnums.h"

typedef short int NiteUserId;

typedef struct NiteUserTracker* NiteUserTrackerHandle;

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
	NitePoint3f min;
	NitePoint3f max;
} NiteBoundingBox;

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

/** Hold a version */
typedef struct  
{
	int major;
	int minor;
	int maintenance;
	int build;
} NiteVersion;


typedef struct NiteHandTracker* NiteHandTrackerHandle;

#endif // _NITE_C_TYPES_H_
