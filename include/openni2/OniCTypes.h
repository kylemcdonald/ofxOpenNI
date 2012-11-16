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

/** Basic types **/
typedef int OniBool;

#ifndef TRUE
#define TRUE 1
#endif //TRUE
#ifndef FALSE
#define FALSE 0
#endif //FALSE

#define ONI_MAX_STR 256
#define ONI_MAX_SOURCES 10

struct OniCallbackHandleImpl;
typedef struct OniCallbackHandleImpl* OniCallbackHandle;

/** Possible failure values */
typedef enum
{
	ONI_STATUS_OK,
	ONI_STATUS_ERROR,
	ONI_STATUS_NOT_IMPLEMENTED,
	ONI_STATUS_NOT_SUPPORTED,
	ONI_STATUS_BAD_PARAMETER,
	ONI_STATUS_OUT_OF_FLOW
} OniStatus;

/** Hold a version */
typedef struct  
{
	int major;
	int minor;
	int maintenance;
	int build;
} OniVersion;

typedef int OniHardwareVersion;

/** The source of the stream */
typedef enum
{
	ONI_STREAM_SOURCE_IR = 1,
	ONI_STREAM_SOURCE_IR_LEFT = ONI_STREAM_SOURCE_IR,
	ONI_STREAM_SOURCE_IR_RIGHT,

	ONI_STREAM_SOURCE_COLOR,
	ONI_STREAM_SOURCE_COLOR_LEFT = ONI_STREAM_SOURCE_COLOR,
	ONI_STREAM_SOURCE_COLOR_RIGHT,

	ONI_STREAM_SOURCE_DEPTH
} OniStreamSource;

/** All available formats of the output of a stream */
typedef enum
{
	// Depth
	ONI_PIXEL_FORMAT_DEPTH_1_MM = 100,
	ONI_PIXEL_FORMAT_DEPTH_100_UM = 101,
	ONI_PIXEL_FORMAT_SHIFT_9_2 = 102,
	ONI_PIXEL_FORMAT_SHIFT_9_3 = 103,

	// Color
	ONI_PIXEL_FORMAT_RGB888 = 200,
	ONI_PIXEL_FORMAT_YUV422 = 201,
	ONI_PIXEL_FORMAT_GRAY8 = 202,
	ONI_PIXEL_FORMAT_GRAY16 = 203,
	ONI_PIXEL_FORMAT_JPEG = 204,

	// Custom
	ONI_PIXEL_FORMAT_PRIVATE_BASE = 65536,

} OniPixelFormat;

/** Description of the output: format and resolution */
typedef struct  
{
	OniPixelFormat pixelFormat;
	int xResolution;
	int yResolution;
	int fps;
} OniVideoMode;

/** List of supported video modes by a specific source */
typedef struct
{
	OniStreamSource streamSource;
	int numSupportedVideoModes;
	OniVideoMode *pSupportedVideoModes;
} OniStreamSourceInfo;

/** Basic description of a device */
typedef struct
{
	char uri[ONI_MAX_STR];
	char vendor[ONI_MAX_STR];
	char name[ONI_MAX_STR];
	unsigned short usbVendorId;
	unsigned short usbProductId;
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

	OniStreamSource streamSource;
	unsigned long long timestamp;
	int frameIndex;

	int width;
	int height;

	OniVideoMode videoMode;
	OniBool croppingEnabled;
	int cropXOrigin;
	int cropYOrigin;

	int stride;
} OniFrame;

typedef enum
{
	DEVICE_STATE_OK,
	DEVICE_STATE_ERROR,
	DEVICE_STATE_NOT_READY
} OniDeviceState;

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

typedef enum
{
	ONI_IMAGE_REGISTRATION_OFF				= 0,
	ONI_IMAGE_REGISTRATION_DEPTH_TO_IMAGE,
} OniImageRegistrationMode;

typedef struct  
{
	int enabled;
	int originX;
	int originY;
	int width;
	int height;
} OniCropping;

static const int ONI_TIMEOUT_NONE = 0;
static const int ONI_TIMEOUT_FOREVER = -1;

// Pixel types
typedef unsigned short OniDepthPixel;

typedef unsigned short OniGrayscale16Pixel;

typedef struct  
{
	char r;
	char g;
	char b;
} OniRGB888Pixel;

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
