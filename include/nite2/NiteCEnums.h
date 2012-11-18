/*******************************************************************************
*                                                                              *
*   PrimeSense NiTE 2.0                                                        *
*   Copyright (C) 2012 PrimeSense Ltd.                                         *
*                                                                              *
*******************************************************************************/

#ifndef _NITE_C_ENUM_H_
#define _NITE_C_ENUM_H_

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

#endif // _NITE_C_ENUM_H_