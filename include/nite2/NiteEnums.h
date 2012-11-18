/*******************************************************************************
*                                                                              *
*   PrimeSense  2.0                                                        *
*   Copyright (C) 2012 PrimeSense Ltd.                                         *
*                                                                              *
*******************************************************************************/

#ifndef _NITE_ENUMS_H_
#define _NITE_ENUMS_H_

/** Available joints in skeleton */
typedef enum
{
	JOINT_HEAD,
	JOINT_NECK,

	JOINT_LEFT_SHOULDER,
	JOINT_RIGHT_SHOULDER,
	JOINT_LEFT_ELBOW,
	JOINT_RIGHT_ELBOW,
	JOINT_LEFT_HAND,
	JOINT_RIGHT_HAND,

	JOINT_TORSO,

	JOINT_LEFT_HIP,
	JOINT_RIGHT_HIP,
	JOINT_LEFT_KNEE,
	JOINT_RIGHT_KNEE,
	JOINT_LEFT_FOOT,
	JOINT_RIGHT_FOOT,
} JointType;

/** Possible states of the skeleton */
typedef enum
{
	/** No skeleton - skeleton was not requested */
	SKELETON_NONE,
	/** Skeleton requested, but still unavailable */
	SKELETON_CALIBRATING,
	/** Skeleton available */
	SKELETON_TRACKED,

	/** Possible reasons as to why skeleton is unavailable */
	SKELETON_CALIBRATION_ERROR_NOT_IN_POSE,
	SKELETON_CALIBRATION_ERROR_HANDS,
	SKELETON_CALIBRATION_ERROR_HEAD,
	SKELETON_CALIBRATION_ERROR_LEGS,
	SKELETON_CALIBRATION_ERROR_TORSO

} SkeletonState;

/** Possible states of the user */
typedef enum
{
	/** User is new - this is the first time the user is available */
	USER_STATE_NEW,
	/** User is visible and already known */
	USER_STATE_VISIBLE,
	/** Known user is currently out of scene, but not lost */
	USER_STATE_OUT_OF_SCENE,
	/** User is lost. This is the last time this user will be seen */
	USER_STATE_LOST
} UserState;

/** Possible failure values */
typedef enum
{
	STATUS_OK,
	STATUS_ERROR,
	STATUS_BAD_USER_ID
} Status;
/*
	NOT_INITIALIZED
	BAD_USER_ID
*/

/** Available gestures types */
typedef enum
{
	GESTURE_WAVE,
	GESTURE_CLICK,
	GESTURE_HAND_RAISE
} GestureType;

/** Possible state of a gesture. Currently only 'Complete' is used. */
typedef enum
{
	GESTURE_STATE_NEW,			// Unused
	GESTURE_STATE_IN_PROGRESS,	// Unused
	GESTURE_STATE_ABORTED,		// Unused
	GESTURE_STATE_COMPLETED
} GestureState;

/** Possible state of a hand */
typedef enum
{
	/** This is a new hand - it is the first frame in which it is available*/
	HAND_STATE_NEW,
	/** This is a known hand */
	HAND_STATE_TRACKED,
	/** This hand was lost. It is the last frame in which it will provided */
	HAND_STATE_LOST,
} HandState;

#endif // _NITE_ENUMS_H_