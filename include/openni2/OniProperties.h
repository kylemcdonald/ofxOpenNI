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
#ifndef _ONI_PROPERTIES_H_
#define _ONI_PROPERTIES_H_

// Device properties
enum
{
	ONI_DEVICE_PROPERTY_FIRMWARE_VERSION		= 0, // By implementation
	ONI_DEVICE_PROPERTY_DRIVER_VERSION		= 1, // OniVersion
	ONI_DEVICE_PROPERTY_HARDWARE_VERSION		= 2, // int
	ONI_DEVICE_PROPERTY_SERIAL_NUMBER		= 3, // string
	ONI_DEVICE_PROPERTY_ERROR_STATE			= 4, // ??
	ONI_DEVICE_PROPERTY_IMAGE_REGISTRATION		= 5, // OniImageRegistrationMode

	// Files
	ONI_DEVICE_PROPERTY_PLAYBACK_SPEED		= 100, // float
	ONI_DEVICE_PROPERTY_PLAYBACK_LOOP_MODE			= 101, // OniBool

	ONI_DEVICE_PROPERTY_PRIVATE_BASE		= 65536
};

// Stream properties
enum
{
	ONI_STREAM_PROPERTY_CROPPING			= 0, // OniCropping*
	ONI_STREAM_PROPERTY_HORIZONTAL_FOV		= 1, // float: radians
	ONI_STREAM_PROPERTY_VERTICAL_FOV		= 2, // float: radians
	ONI_STREAM_PROPERTY_VIDEO_MODE			= 3, // OniVideoMode*

	ONI_STREAM_PROPERTY_MAX_VALUE			= 4, // int
	ONI_STREAM_PROPERTY_MIN_VALUE			= 5, // int

	ONI_STREAM_PROPERTY_STRIDE			= 6, // int
	ONI_STREAM_PROPERTY_MIRRORING			= 7, // OniBool

	ONI_STREAM_PROPERTY_NUMBER_OF_FRAMES		= 8, // int

	ONI_STREAM_PROPERTY_PRIVATE_BASE		= 65536
};

// Device commands (for Invoke)
enum
{
	ONI_DEVICE_COMMAND_SEEK				= 1, // OniSeek
	ONI_DEVICE_COMMAND_PRIVATE_BASE			= 65536
};
// Stream commands (for Invoke)
enum
{
	ONI_STREAM_COMMAND_PRIVATE_BASE			= 65536
};

#endif // _ONI_PROPERTIES_H_