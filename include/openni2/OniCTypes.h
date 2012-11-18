/*****************************************************************************
*                                                                            *
*  OpenNI 2.x Alpha                                                          *
*  Copyright (C) 2012 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of OpenNI.                                              *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
#ifndef _ONI_TYPES_H_
#define _ONI_TYPES_H_

#include "OniPlatform.h"
#include "OniCEnums.h"

/** Basic types **/
typedef int OniBool;

#ifndef TRUE
#define TRUE 1
#endif //TRUE
#ifndef FALSE
#define FALSE 0
#endif //FALSE

#define ONI_MAX_STR 256
#define ONI_MAX_SENSORS 10

struct OniCallbackHandleImpl;
typedef struct OniCallbackHandleImpl* OniCallbackHandle;

/** Hold a version */
typedef struct  
{
	int major;
	int minor;
	int maintenance;
	int build;
} OniVersion;

typedef int OniHardwareVersion;


/** Description of the output: format and resolution */
typedef struct  
{
	OniPixelFormat pixelFormat;
	int resolutionX;
	int resolutionY;
	int fps;
} OniVideoMode;

/** List of supported video modes by a specific source */
typedef struct
{
	OniSensorType sensorType;
	int numSupportedVideoModes;
	OniVideoMode *pSupportedVideoModes;
} OniSensorInfo;

/** Basic description of a device */
typedef struct
{
	char uri[ONI_MAX_STR];
	char vendor[ONI_MAX_STR];
	char name[ONI_MAX_STR];
	uint16_t usbVendorId;
	uint16_t usbProductId;
} OniDeviceInfo;

/** Default device */
#define ONI_ANY_DEVICE NULL

struct _OniDevice;
typedef _OniDevice* OniDeviceHandle;

struct _OniStream;
typedef _OniStream* OniStreamHandle;

struct _OniRecorder;
typedef _OniRecorder* OniRecorderHandle;

/** All information of the current frame */
typedef struct  
{
	int dataSize;
	void* data;

	OniSensorType sensorType;
	uint64_t timestamp;
	int frameIndex;

	int width;
	int height;

	OniVideoMode videoMode;
	OniBool croppingEnabled;
	int cropOriginX;
	int cropOriginY;

	int stride;
} OniFrame;

typedef void (ONI_CALLBACK_TYPE* OniNewFrameCallback)(OniStreamHandle stream, void* pCookie);
typedef void (ONI_CALLBACK_TYPE* OniGeneralCallback)(void* pCookie);
typedef void (ONI_CALLBACK_TYPE* OniDeviceInfoCallback)(const OniDeviceInfo* pInfo, void* pCookie);
typedef void (ONI_CALLBACK_TYPE* OniDeviceStateCallback)(const OniDeviceInfo* pInfo, OniDeviceState deviceState, void* pCookie);

typedef struct
{
	OniDeviceInfoCallback		deviceConnected;
	OniDeviceInfoCallback		deviceDisconnected;
	OniDeviceStateCallback		deviceStateChanged;
} OniDeviceCallbacks;

typedef struct  
{
	int enabled;
	int originX;
	int originY;
	int width;
	int height;
} OniCropping;

// Pixel types
typedef uint16_t OniDepthPixel;

typedef uint16_t OniGrayscale16Pixel;

#pragma pack (push, 1)

typedef struct  
{
	char r;
	char g;
	char b;
} OniRGB888Pixel;

#pragma pack (pop)

typedef struct
{
	char u;
	char y1;
	char v;
	char y2;
} OniYUV422DoublePixel;

typedef struct 
{
	int frameIndex;
	OniStreamHandle stream;
} OniSeek;

#endif // _ONI_TYPES_H_
