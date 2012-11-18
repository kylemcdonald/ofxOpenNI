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
*****************************************************************************/#ifndef _ONI_ENUMS_H_
#define _ONI_ENUMS_H_

namespace openni
{

/** Possible failure values */
typedef enum
{
	STATUS_OK,
	STATUS_ERROR,
	STATUS_NOT_IMPLEMENTED,
	STATUS_NOT_SUPPORTED,
	STATUS_BAD_PARAMETER,
	STATUS_OUT_OF_FLOW,
	STATUS_NO_DEVICE
} Status;

/** The source of the stream */
typedef enum
{
	SENSOR_IR = 1,
	SENSOR_COLOR,
	SENSOR_DEPTH

} SensorType;

/** All available formats of the output of a stream */
typedef enum
{
	// Depth
	PIXEL_FORMAT_DEPTH_1_MM = 100,
	PIXEL_FORMAT_DEPTH_100_UM = 101,
	PIXEL_FORMAT_SHIFT_9_2 = 102,
	PIXEL_FORMAT_SHIFT_9_3 = 103,

	// Color
	PIXEL_FORMAT_RGB888 = 200,
	PIXEL_FORMAT_YUV422 = 201,
	PIXEL_FORMAT_GRAY8 = 202,
	PIXEL_FORMAT_GRAY16 = 203,
	PIXEL_FORMAT_JPEG = 204,
} PixelFormat;

typedef enum
{
	DEVICE_STATE_OK,
	DEVICE_STATE_ERROR,
	DEVICE_STATE_NOT_READY
} DeviceState;

typedef enum
{
	IMAGE_REGISTRATION_OFF				= 0,
	IMAGE_REGISTRATION_DEPTH_TO_COLOR,
} ImageRegistrationMode;

static const int TIMEOUT_NONE = 0;
static const int TIMEOUT_FOREVER = -1;

} // namespace openni

#endif // _ONI_ENUMS_H_