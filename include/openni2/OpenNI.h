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
#ifndef _OPENNI_H_
#define _OPENNI_H_

#include "OniPlatform.h"
#include "OniProperties.h"
#include "OniEnums.h"

#include "OniCAPI.h"
#include "OniCProperties.h"

/**
openni is the namespace of the entire C++ API of OpenNI
*/
namespace openni
{
// primitives
typedef OniDepthPixel				DepthPixel;
typedef OniGrayscale16Pixel			Grayscale16Pixel;

// structs
typedef OniVersion					Version;
typedef OniRGB888Pixel				RGB888Pixel;
typedef OniYUV422DoublePixel		YUV422DoublePixel;

template<class T>
class Array
{
public:
	Array() : m_data(NULL), m_count(0), m_owner(false) {}
	Array(const T* data, int count) : m_owner(false) { setData(data, count); }
	~Array()
	{
		clear();
	}

	void setData(const T* data, int count, bool isOwner = false)
	{
		clear();
		m_count = count;
		m_owner = isOwner;
		if (!isOwner)
		{
			m_data = data;
		}
		else
		{
			m_data = new T[count];
			memcpy((void*)m_data, data, count*sizeof(T));
		}
	}

	int getSize() const { return m_count; }
	const T& operator[](int index) const {return m_data[index];}
private:
	Array(const Array<T>&);
	Array<T>& operator=(const Array<T>&);

	void clear()
	{
		if (m_owner && m_data != NULL)
			delete []m_data;
		m_owner = false;
		m_data = NULL;
		m_count = 0;
	}

	const T* m_data;
	int m_count;
	bool m_owner;
};

class VideoMode : private OniVideoMode
{
public:
	VideoMode()
	{}
	VideoMode(const VideoMode& other)
	{
		*this = other;
	}
	VideoMode& operator=(const VideoMode& other)
	{
		setPixelFormat(other.getPixelFormat());
		setResolution(other.getResolutionX(), other.getResolutionY());
		setFps(other.getFps());

		return *this;
	}

	PixelFormat getPixelFormat() const { return (PixelFormat)pixelFormat; }
	int getResolutionX() const { return resolutionX; }
	int getResolutionY() const {return resolutionY;}
	int getFps() const { return fps; }

	void setPixelFormat(PixelFormat format) { this->pixelFormat = (OniPixelFormat)format; }
	void setResolution(int resolutionX, int resolutionY) 
	{ 
		this->resolutionX = resolutionX;
		this->resolutionY = resolutionY;
	}
	void setFps(int fps) { this->fps = fps; }

	friend class SensorInfo;
	friend class VideoStream;
	friend class VideoFrameRef;
};

class SensorInfo
{
public:
	SensorType getSensorType() const { return (SensorType)m_pInfo->sensorType; }
	const Array<VideoMode>& getSupportedVideoModes() const { return m_videoModes; }

private:
	SensorInfo(const SensorInfo&);
	SensorInfo& operator=(const SensorInfo&);

	SensorInfo() : m_pInfo(NULL), m_videoModes(NULL, 0) {}
	SensorInfo(const OniSensorInfo* pInfo) : m_pInfo(NULL), m_videoModes(NULL, 0)
	{
		_setInternal(pInfo);
	}
	void _setInternal(const OniSensorInfo* pInfo)
	{
		m_pInfo = pInfo;
		if (pInfo == NULL)
		{
			m_videoModes.setData(NULL, 0);
		}
		else
		{
			m_videoModes.setData(static_cast<VideoMode*>(pInfo->pSupportedVideoModes), pInfo->numSupportedVideoModes);
		}
	}

	const OniSensorInfo* m_pInfo;
	Array<VideoMode> m_videoModes;

	friend class VideoStream;
	friend class Device;
};

class DeviceInfo : private OniDeviceInfo
{
public:
	const char* getUri() const { return uri; }
	const char* getVendor() const { return vendor; }
	const char* getName() const { return name; }
	uint16_t getUsbVendorId() const { return usbVendorId; }
	uint16_t getUsbProductId() const { return usbProductId; }

	friend class DeviceInfoList;
	friend class Device;
	friend class OpenNI;
};

/**
The Frame class encapsulates a specific frame - the output of a Stream at a specific time.
*/
class VideoFrameRef
{
public:
	/** Create a new Frame object. This object will be invalid, until initialized by a Stream object.*/
	VideoFrameRef()
	{
		m_pFrame = NULL;
	}

	/** Destroy this frame. Release the internal OniFrame, if it is valid. */
	~VideoFrameRef()
	{
		release();	
	}

	VideoFrameRef(const VideoFrameRef& other) : m_pFrame(NULL)
	{
		_setFrame(other.m_pFrame);
	}

	/** Make this Frame object refer to the same internal OniFrame as another Frame object */
	VideoFrameRef& operator=(const VideoFrameRef& other)
	{
		_setFrame(other.m_pFrame);
		return *this;
	}

	inline int getDataSize() const
	{
		return m_pFrame->dataSize;
	}

	inline void* getData() const
	{
		return m_pFrame->data;
	}

	inline SensorType getSensorType() const
	{
		return (SensorType)m_pFrame->sensorType;
	}

	inline const VideoMode& getVideoMode() const
	{
		return static_cast<const VideoMode&>(m_pFrame->videoMode);
	}

	inline uint64_t getTimestamp() const 
	{
		return m_pFrame->timestamp;
	}

	inline int getFrameIndex() const
	{
		return m_pFrame->frameIndex;
	}

	inline int getWidth() const
	{
		return m_pFrame->width;
	}

	inline int getHeight() const
	{
		return m_pFrame->height;
	}

	inline bool getCroppingEnabled() const
	{
		return m_pFrame->croppingEnabled == TRUE;
	}

	inline int getCropOriginX() const
	{
		return m_pFrame->cropOriginX;
	}

	inline int getCropOriginY() const
	{
		return m_pFrame->cropOriginY;
	}

	inline int getStrideInBytes() const
	{
		return m_pFrame->stride;
	}

	/** Check if internal frame is valid - if it actually points to any OniFrame object */
	inline bool isValid() const
	{
		return m_pFrame != NULL;
	}

	/** Invalidate this Frame object */
	void release()
	{
		if (m_pFrame != NULL)
		{
			oniFrameRelease(m_pFrame);
			m_pFrame = NULL;
		}
	}

	void _setFrame(OniFrame* pFrame)
	{
		setReference(pFrame);
		if (pFrame != NULL)
		{
			oniFrameAddRef(pFrame);
		}
	}
	OniFrame* _getFrame()
	{
		return m_pFrame;
	}

private:
	friend class VideoStream;
	inline void setReference(OniFrame* pFrame)
	{
		// Initial - don't addref. This is the reference from OpenNI
		release();
		m_pFrame = pFrame;
	}

	OniFrame* m_pFrame; // const!!?
};

/**
The Stream object encapsulates a specific stream.
This stream has a specific video mode (see VideoMode), and one can get Frame objects from it when the device has them.
*/
class Device;
class VideoStream
{
public:
	/**
	This object allows registering to the events that the Stream object offers.
	It is intended for users to inherit it and implement the methods matching the events they want to handle.
	*/
	class Listener
	{
	public:
		Listener()
		{
			m_newFrameCallback = newFrame_Callback;
		}


		/**
		Handle the event that the Stream object has a new Frame available
		*/
		virtual void onNewFrame(VideoStream&) {}

	private:
		OniNewFrameCallback m_newFrameCallback;

		friend class VideoStream;
		OniNewFrameCallback getNewFrameCallback()
		{
			return m_newFrameCallback;
		}
		OniCallbackHandle getCallbackHandle()
		{
			return m_callbackHandle;
		}
		void setCallbackHandle(OniCallbackHandle handle)
		{
			m_callbackHandle = handle;
		}
		static void ONI_CALLBACK_TYPE newFrame_Callback(OniStreamHandle streamHandle, void* pCookie)
		{
			Listener* pCallbacks = (Listener*)pCookie;
			VideoStream stream;
			stream._setHandle(streamHandle);
			pCallbacks->onNewFrame(stream);
			stream._setHandle(NULL);
		}
		OniCallbackHandle m_callbackHandle;
	};

	/**
	Create a Stream object. This object will be invalid, until it is initialized by the Device object, using its CreateStream API
	*/
	VideoStream() : m_stream(NULL), m_sensorInfo()
	{}

	~VideoStream()
	{
		destroy();
	}

	/**
	Check if this object currently points to a valid stream
	*/
	bool isValid() const
	{
		return m_stream != NULL;
	}

	/**
	Create a new stream from a device, using a specific source type.
	*/
	inline Status create(const Device& device, SensorType sensorType);

	/**
	Destroy this stream.
	*/
	void destroy()
	{
		if (!isValid())
		{
			return;
		}

		if (m_stream != NULL)
		{
			oniStreamDestroy(m_stream);
			m_stream = NULL;
		}
	}

	const SensorInfo& getSensorInfo() const
	{
		return m_sensorInfo;
	}

	/**
	Have the Stream object start generating data
	*/
	Status start()
	{
		if (!isValid())
		{
			return STATUS_ERROR;
		}

		return (Status)oniStreamStart(m_stream);
	}

	/**
	Have the Stream object stop generating data
	*/
	void stop()
	{
		if (!isValid())
		{
			return;
		}

		oniStreamStop(m_stream);
	}

	/**
	Get the next frame from this stream.
	This method will block until a frame is available.
	To avoid blocking, check out OpenNI::WaitForStreams
	*/
	Status readFrame(VideoFrameRef* pFrame)
	{
		if (!isValid())
		{
			return STATUS_ERROR;
		}

		OniFrame* pOniFrame;
		Status rc = (Status)oniStreamReadFrame(m_stream, &pOniFrame);

		pFrame->setReference(pOniFrame);
		return rc;
	}

	/**
	Add a new listener to the this Stream's events
	*/
	Status addListener(Listener* pListener)
	{
		if (!isValid())
		{
			return STATUS_ERROR;
		}

		OniCallbackHandle handle;
		Status rc = (Status)oniStreamRegisterNewFrameCallback(m_stream, pListener->getNewFrameCallback(), pListener, &handle);
		if (rc == STATUS_OK)
		{
			pListener->setCallbackHandle(handle);
		}
		return rc;
	}

	/**
	Remove a listener from this Stream's events
	*/
	void removeListener(Listener* pListener)
	{
		if (!isValid())
		{
			return;
		}

		oniStreamUnregisterNewFrameCallback(m_stream, pListener->getCallbackHandle());
		pListener->setCallbackHandle(NULL);
	}

	/**
	Get an internal handle. This handle can be used via the C API.
	*/
	OniStreamHandle _getHandle() const
	{
		return m_stream;
	}

	///// Properties
	/**
	Get the value of a general property of the stream.
	There are convenience functions for all the commonly used properties
	*/
	Status getProperty(int propertyId, void* data, int* dataSize) const
	{
		if (!isValid())
		{
			return STATUS_ERROR;
		}

		return (Status)oniStreamGetProperty(m_stream, propertyId, data, dataSize);
	}
	/**
	Set a new value to a general property of the stream
	*/
	Status setProperty(int propertyId, const void* data, int dataSize)
	{
		if (!isValid())
		{
			return STATUS_ERROR;
		}

		return (Status)oniStreamSetProperty(m_stream, propertyId, data, dataSize);
	}

	///////////////////////// Properties Convenience Layer
	/**
	Get the current video mode of this stream. This includes its resolution, fps and format (See VideoMode)
	*/
	VideoMode getVideoMode() const
	{
		VideoMode videoMode;
		getProperty<OniVideoMode>(STREAM_PROPERTY_VIDEO_MODE, static_cast<OniVideoMode*>(&videoMode));
		return videoMode;
	}

	/**
	Change the current video mode of this stream
	*/
	Status setVideoMode(const VideoMode& videoMode)
	{
		return setProperty<OniVideoMode>(STREAM_PROPERTY_VIDEO_MODE, static_cast<const OniVideoMode&>(videoMode));
	}
	/////
	/**
	Get the maximum value available in this stream
	*/
	int getMaxPixelValue() const
	{
		int maxValue;
		Status rc = getProperty<int>(STREAM_PROPERTY_MAX_VALUE, &maxValue);
		if (rc != STATUS_OK)
		{
			return 0;
		}
		return maxValue;
	}
	/////
	/**
	Get the minimum value available in this stream
	*/
	int getMinPixelValue() const
	{
		int minValue;
		Status rc = getProperty<int>(STREAM_PROPERTY_MIN_VALUE, &minValue);
		if (rc != STATUS_OK)
		{
			return 0;
		}
		return minValue;
	}
	
	/////
	/**
	Check if this stream supported cropping
	*/
	bool isCroppingSupported() const
	{
		return isPropertySupported(STREAM_PROPERTY_CROPPING);
	}

	/**
	Get the current cropping configuration
	*/
	bool getCropping(int* pOriginX, int* pOriginY, int* pWidth, int* pHeight) const
	{
		OniCropping cropping;
		bool enabled = false;

		Status rc = getProperty<OniCropping>(STREAM_PROPERTY_CROPPING, &cropping);

		if (rc == STATUS_OK)
		{
			*pOriginX = cropping.originX;
			*pOriginY = cropping.originY;
			*pWidth = cropping.width;
			*pHeight = cropping.height;
			enabled = (cropping.enabled == TRUE);
		}

		return enabled;
	}
	/**
	Change the cropping configuration to a new configuration
	*/
	Status setCropping(int originX, int originY, int width, int height)
	{
		OniCropping cropping;
		cropping.enabled = true;
		cropping.originX = originX;
		cropping.originY = originY;
		cropping.width = width;
		cropping.height = height;
		return setProperty<OniCropping>(STREAM_PROPERTY_CROPPING, cropping);
	}

	/**
	Stop cropping
	*/
	Status resetCropping()
	{
		OniCropping cropping;
		cropping.enabled = false;
		return setProperty<OniCropping>(STREAM_PROPERTY_CROPPING, cropping);
	}
	/////
	/**
	Check if this stream is currently mirrored
	*/
	bool getMirroringEnabled() const
	{
		OniBool enabled;
		Status rc = getProperty<OniBool>(STREAM_PROPERTY_MIRRORING, &enabled);
		if (rc != STATUS_OK)
		{
			return false;
		}
		return enabled == TRUE;
	}

	/**
	Change the mirror state of this stream
	*/
	Status setMirroringEnabled(bool isEnabled)
	{
		return setProperty<OniBool>(STREAM_PROPERTY_MIRRORING, isEnabled ? TRUE : FALSE);
	}

	/**
	Gets the field of view of this stream.
	The output will be in radians.
	*/
	float getHorizontalFieldOfView() const
	{
		float horizontal = 0;
		getProperty<float>(STREAM_PROPERTY_HORIZONTAL_FOV, &horizontal);
		return horizontal;
	}
	float getVerticalFieldOfView() const
	{
		float vertical = 0;
		getProperty<float>(STREAM_PROPERTY_VERTICAL_FOV, &vertical);
		return vertical;
	}

	template <class T>
	Status setProperty(int propertyId, const T& value)
	{
		return setProperty(propertyId, &value, sizeof(T));
	}

	template <class T>
	Status getProperty(int propertyId, T* value) const
	{
		int size = sizeof(T);
		return getProperty(propertyId, value, &size);
	}
	// Stride
private:

	bool isPropertySupported(int propertyId) const
	{
		if (!isValid())
		{
			return false;
		}

		return oniStreamIsPropertySupported(m_stream, propertyId) == TRUE;
	}

	Status invoke(int commandId, void* data, int dataSize)
	{
		if (!isValid())
		{
			return STATUS_ERROR;
		}

		return (Status)oniStreamInvoke(m_stream, commandId, data, dataSize);
	}

	bool isCommandSupported(int commandId) const
	{
		if (!isValid())
		{
			return false;
		}

		return (Status)oniStreamIsCommandSupported(m_stream, commandId) == TRUE;
	}

	friend class Device;

	void _setHandle(OniStreamHandle stream)
	{
		m_sensorInfo._setInternal(NULL);
		m_stream = stream;

		if (stream != NULL)
		{
			m_sensorInfo._setInternal(oniStreamGetSensorInfo(m_stream));
		}
	}

private:
	VideoStream(const VideoStream& other);
	VideoStream& operator=(const VideoStream& other);

	OniStreamHandle m_stream;
	SensorInfo m_sensorInfo;
};

/**
The Device object encapsulates a specific device.
This device can tell the user of the possible sources it offers.
It can also create new streams on those sources
*/
class Device
{
public:
	/**
	Create a Device object. This object will be invalid, until it is initialized by the OpenNI, using its OpenDevice API
	*/
	Device() : m_device(NULL)
	{
		clearSensors();
	}

	~Device()
	{
		if (m_device != NULL)
		{
			close();
		}
	}

	/**
	Open a device, using its uri. The uri is available from the DeviceInfo object, that is received either in the DeviceConnected event (see OpenNI::Listener), or from the list (available through OpenNI::GetDeviceInfoList)
	*/
	Status open(const char* uri)
	{
		OniDeviceHandle deviceHandle;
		Status rc = (Status)oniDeviceOpen(uri, &deviceHandle);
		if (rc != STATUS_OK)
		{
			return rc;
		}

		_setHandle(deviceHandle);
		return STATUS_OK;
	}

	/**
	Close the device.
	*/
	void close()
	{
		if (m_device != NULL)
		{
			oniDeviceClose(m_device);
			m_device = NULL;
		}
	}

	/**
	Get the general information about this device
	*/
	const DeviceInfo* getDeviceInfo() const
	{
		return const_cast<DeviceInfo*>(static_cast<const DeviceInfo*>(&m_deviceInfo));
	}
	/**
	Check if this device supports a specific stream source
	*/
	bool hasSensor(SensorType sensorType)
	{
		int i;
		for (int i; (i < ONI_MAX_SENSORS) && (m_aSensorInfo[i].m_pInfo != NULL); ++i)
		{
			if (m_aSensorInfo[i].getSensorType() == sensorType)
			{
				return true;
			}
		}

		if (i == ONI_MAX_SENSORS)
		{
			return false;
		}

		const OniSensorInfo* pInfo = oniDeviceGetSensorInfo(m_device, (OniSensorType)sensorType);

		if (pInfo == NULL)
		{
			return false;
		}

		m_aSensorInfo[i]._setInternal(pInfo);

		return true;
	}
	/**
	Get the possible configurations available for a specific source
	*/
	const SensorInfo* getSensorInfo(SensorType sensorType)
	{
		int i;
		for (i = 0; (i < ONI_MAX_SENSORS) && (m_aSensorInfo[i].m_pInfo != NULL); ++i)
		{
			if (m_aSensorInfo[i].getSensorType() == sensorType)
			{
				return &m_aSensorInfo[i];
			}
		}

		// not found. check to see we have additional space
		if (i == ONI_MAX_SENSORS)
		{
			return NULL;
		}

		const OniSensorInfo* pInfo = oniDeviceGetSensorInfo(m_device, (OniSensorType)sensorType);
		if (pInfo == NULL)
		{
			return NULL;
		}

		m_aSensorInfo[i]._setInternal(pInfo);
		return &m_aSensorInfo[i];
	}

	/**
	Get an internal handle. This handle can be used via the C API.
	*/
	OniDeviceHandle _getHandle() const
	{
		return m_device;
	}

	///// Properties
	/**
	Get the value of a general property of the device.
	There are convenience functions for all the commonly used properties
	*/
	Status getProperty(int propertyId, void* data, int* dataSize) const
	{
		return (Status)oniDeviceGetProperty(m_device, propertyId, data, dataSize);
	}

	/**
	Set a new value to a general property of the stream
	*/
	Status setProperty(int propertyId, const void* data, int dataSize)
	{
		return (Status)oniDeviceSetProperty(m_device, propertyId, data, dataSize);
	}
	///////////////////////// Properties Convenience Layer
	/**
	Check if this device can register the image between its different streams
	*/
	bool isImageRegistrationModeSupported(ImageRegistrationMode mode) const
	{
		return (oniDeviceIsImageRegistrationModeSupported(m_device, (OniImageRegistrationMode)mode) == TRUE);
	}

	/**
	Get the current image registration mode.
	*/
	ImageRegistrationMode getImageRegistrationMode() const
	{
		ImageRegistrationMode mode;
		Status rc = getProperty<ImageRegistrationMode>(DEVICE_PROPERTY_IMAGE_REGISTRATION, &mode);
		if (rc != STATUS_OK)
		{
			return IMAGE_REGISTRATION_OFF;
		}
		return mode;
	}

	/**
	Change the image registration mode
	*/
	Status setImageRegistrationMode(ImageRegistrationMode mode)
	{
		return setProperty<ImageRegistrationMode>(DEVICE_PROPERTY_IMAGE_REGISTRATION, mode);
	}

	// FirmwareVersion
	// DriverVersion
	// HardwareVersion
	// SerialNumber
	// ErrorState

	/**
	Does the device object represent an open device.SetHandle
	*/
	bool isValid() const
	{
		return m_device != NULL;
	}

	bool isFile() const
	{
		return isPropertySupported(DEVICE_PROPERTY_PLAYBACK_SPEED) &&
			isPropertySupported(DEVICE_PROPERTY_PLAYBACK_REPEAT_ENABLED) &&
			isCommandSupported(DEVICE_COMMAND_SEEK);
	}

	Status setDepthColorSyncEnabled(bool isEnabled)
	{
		Status rc = STATUS_OK;

		if (isEnabled)
		{
			rc = (Status)oniDeviceEnableDepthColorSync(m_device);
		}
		else
		{
			oniDeviceDisableDepthColorSync(m_device);
		}
		
		return rc;
	}

	template <class T>
	Status setProperty(int propertyId, const T& value)
	{
		return setProperty(propertyId, &value, sizeof(T));
	}
	template <class T>
	Status getProperty(int propertyId, T* value) const
	{
		int size = sizeof(T);
		return getProperty(propertyId, value, &size);
	}

	template <class T>
	Status invoke(int propertyId, const T& value)
	{
		return invoke(propertyId, &value, sizeof(T));
	}
private:
	Device(const Device&);
	Device& operator=(const Device&);

	void clearSensors()
	{
		for (int i = 0; i < ONI_MAX_SENSORS; ++i)
		{
			m_aSensorInfo[i]._setInternal(NULL);
		}
	}

	bool isPropertySupported(int propertyId) const
	{
		return oniDeviceIsPropertySupported(m_device, propertyId) == TRUE;
	}

	Status invoke(int commandId, const void* data, int dataSize)
	{
		return (Status)oniDeviceInvoke(m_device, commandId, data, dataSize);
	}

	bool isCommandSupported(int commandId) const
	{
		return oniDeviceIsCommandSupported(m_device, commandId) == TRUE;
	}

	Status _setHandle(OniDeviceHandle deviceHandle)
	{
		if (m_device == NULL)
		{
			m_device = deviceHandle;

			clearSensors();

			oniDeviceGetInfo(m_device, &m_deviceInfo);
			// Read deviceInfo
			return STATUS_OK;
		}

		return STATUS_OUT_OF_FLOW;
	}

private:
	OniDeviceHandle m_device;
	OniDeviceInfo m_deviceInfo;
	SensorInfo m_aSensorInfo[ONI_MAX_SENSORS];
};

class PlaybackControl
{
public:
	PlaybackControl() : m_pDevice(NULL)
	{}
	~PlaybackControl()
	{
		detach();
	}

	Status attach(Device* device)
	{
		if (!device->isValid() || !device->isFile())
		{
			return STATUS_ERROR;
		}

		detach();
		m_pDevice = device;

		return STATUS_OK;
	}
	void detach()
	{
		m_pDevice = NULL;
	}

	/**
	Get the current playback speed
	*/
	float getSpeed() const
	{
		if (m_pDevice == NULL)
		{
			return 0.0f;
		}
		float speed;
		Status rc = m_pDevice->getProperty<float>(DEVICE_PROPERTY_PLAYBACK_SPEED, &speed);
		if (rc != STATUS_OK)
		{
			return 1.0f;
		}
		return speed;
	}
	/**
	Change the playback speed
	*/
	Status setSpeed(float speed)
	{
		if (m_pDevice == NULL)
		{
			return STATUS_NO_DEVICE;
		}
		return m_pDevice->setProperty<float>(DEVICE_PROPERTY_PLAYBACK_SPEED, speed);
	}

	/**
	Get the current repeat mode
	*/
	bool getRepeatEnabled() const
	{
		if (m_pDevice == NULL)
		{
			return false;
		}

		OniBool repeat;
		Status rc = m_pDevice->getProperty<OniBool>(DEVICE_PROPERTY_PLAYBACK_REPEAT_ENABLED, &repeat);
		if (rc != STATUS_OK)
		{
			return false;
		}

		return repeat == TRUE;
	}
	/**
	Change the repeat mode
	*/
	Status setRepeateEnabled(bool repeat)
	{
		if (m_pDevice == NULL)
		{
			return STATUS_NO_DEVICE;
		}

		return m_pDevice->setProperty<OniBool>(DEVICE_PROPERTY_PLAYBACK_REPEAT_ENABLED, repeat ? TRUE : FALSE);
	}

	/**
	Seek the stream to the frame ID.
	*/
	Status seek(const VideoStream& stream, int frameIndex)
	{
		if (m_pDevice == NULL)
		{
			return STATUS_NO_DEVICE;
		}
		OniSeek seek;
		seek.frameIndex = frameIndex;
		seek.stream = stream._getHandle();
		return m_pDevice->invoke(DEVICE_COMMAND_SEEK, seek);
	}

	/**
	Get the number of frames in the stream
	*/
	int getNumberOfFrames(const VideoStream& stream) const
	{
		int numOfFrames = -1;
		Status rc = stream.getProperty<int>(STREAM_PROPERTY_NUMBER_OF_FRAMES, &numOfFrames);
		if (rc != STATUS_OK)
		{
			return 0;
		}
		return numOfFrames;
	}

private:
	Device* m_pDevice;
};

/**
The OpenNI class is a static entry point to the library.
Through it you can get events of devices connecting and disconnecting, as well as open devices and wait for frames on streams.
*/
class OpenNI
{
public:
	/**
	This object allows registering to the events that the Stream object offers.
	It is intended for users to inherit it and implement the methods matching the events they want to handle.
	*/
	class Listener
	{
	public:
		Listener()
		{
			m_deviceCallbacks.deviceConnected = contextListener_DeviceConnected;
			m_deviceCallbacks.deviceDisconnected = contextListener_DeviceDisconnected;
			m_deviceCallbacks.deviceStateChanged = contextListener_DeviceStateChanged;
		}

		/**
		Handle the event that a device changed its error state
		*/
		virtual void onDeviceStateChanged(const DeviceInfo*, DeviceState /*state*/) {}
		/**
		Handle the event that a new device was connected and is now available
		*/
		virtual void onDeviceConnected(const DeviceInfo*) {}
		/**
		Handle the event that a device was disconnected and is no longer available
		*/
		virtual void onDeviceDisconnected(const DeviceInfo*) {}

	private:
		static void ONI_CALLBACK_TYPE contextListener_DeviceConnected(const OniDeviceInfo* pInfo, void* pCookie)
		{
			Listener* pCallbacks = (Listener*)pCookie;
			pCallbacks->onDeviceConnected(static_cast<const DeviceInfo*>(pInfo));
		}
		static void ONI_CALLBACK_TYPE contextListener_DeviceDisconnected(const OniDeviceInfo* pInfo, void* pCookie)
		{
			Listener* pCallbacks = (Listener*)pCookie;
			pCallbacks->onDeviceDisconnected(static_cast<const DeviceInfo*>(pInfo));
		}
		static void ONI_CALLBACK_TYPE contextListener_DeviceStateChanged(const OniDeviceInfo* pInfo, OniDeviceState deviceState, void* pCookie)
		{
			Listener* pListener = (Listener*)pCookie;
			pListener->onDeviceStateChanged(static_cast<const DeviceInfo*>(pInfo), (DeviceState)deviceState);
		}

		friend class OpenNI;

		OniDeviceCallbacks m_deviceCallbacks;
		OniCallbackHandle m_callbackHandle;
	};

	/**
	Initialize the library.
	This will load all available drivers, and see which devices are available
	*/
	static Status initialize()
	{
		return (Status)oniInitialize(ONI_API_VERSION); // provide version of API, to make sure proper struct sizes are used
	}

	/**
	Stop using the library. Unload all drivers, close all streams and devices.
	*/
	static void shutdown()
	{
		oniShutdown();
	}

	/**
	Get the version of OpenNI
	*/
	static Version getVersion()
	{
		return oniGetVersion();
	}

	/**
	Get the internal error string created by the last API call
	*/
	static const char* getExtendedError()
	{
		return oniGetExtendedError();
	}

	/**
	Initialize a DeviceInfoList object with all the devices that are available
	*/
	static void enumerateDevices(Array<DeviceInfo>* deviceInfoList)
	{
		OniDeviceInfo* m_pDeviceInfos;
		int m_deviceInfoCount;
		oniGetDeviceList(&m_pDeviceInfos, &m_deviceInfoCount);
		deviceInfoList->setData((DeviceInfo*)m_pDeviceInfos, m_deviceInfoCount, true);
		oniReleaseDeviceList(m_pDeviceInfos);
	}

	/**
	Wait for a new frame from any of the streams provided.
	A timeout is passed so that blocking will be optional
	*/
	static const int ONI_MAX_STREAMS = 50;
	static Status waitForAnyStream(VideoStream** pStreams, int streamCount, int* pReadyStreamIndex, int timeout = TIMEOUT_FOREVER)
	{
		OniStreamHandle streams[ONI_MAX_STREAMS];

		if (streamCount > ONI_MAX_STREAMS)
		{
			printf("Too many streams for wait: %d > %d\n", streamCount, ONI_MAX_STREAMS);
			return STATUS_BAD_PARAMETER;
		}

		*pReadyStreamIndex = -1;
		for (int i = 0; i < streamCount; ++i)
		{
			if (pStreams[i] != NULL)
			{
				streams[i] = pStreams[i]->_getHandle();
			}
			else
			{
				streams[i] = NULL;
			}
		}
		Status rc = (Status)oniWaitForAnyStream(streams, streamCount, pReadyStreamIndex, timeout);

		return rc;
	}

	/**
	Add a listener to the events that OpenNI can raise
	*/
	static Status addListener(Listener* pListener)
	{
		return (Status)oniRegisterDeviceCallbacks(&pListener->m_deviceCallbacks, pListener, &pListener->m_callbackHandle);
	}

	/**
	Remove a listener from the events that OpenNI can raise
	*/
	static void removeListener(Listener* pListener)
	{
		oniUnregisterDeviceCallbacks(pListener->m_callbackHandle);
		pListener->m_callbackHandle = NULL;
	}

private:
	OpenNI()
	{
	}
};

class CoordinateConverter
{
public:
	static Status convertWorldToDepth(const VideoStream& depthStream, float worldX, float worldY, float worldZ, int* pDepthX, int* pDepthY, DepthPixel* pDepthZ)
	{
		float depthX, depthY, depthZ;
		Status rc = (Status)oniCoordinateConverterWorldToDepth(depthStream._getHandle(), worldX, worldY, worldZ, &depthX, &depthY, &depthZ);
		*pDepthX = (int)depthX;
		*pDepthY = (int)depthY;
		*pDepthZ = (DepthPixel)depthZ;
		return rc;
	}

	static Status convertWorldToDepth(const VideoStream& depthStream, float worldX, float worldY, float worldZ, float* pDepthX, float* pDepthY, float* pDepthZ)
	{
		return (Status)oniCoordinateConverterWorldToDepth(depthStream._getHandle(), worldX, worldY, worldZ, pDepthX, pDepthY, pDepthZ);
	}

	static Status convertDepthToWorld(const VideoStream& depthStream, int depthX, int depthY, DepthPixel depthZ, float* pWorldX, float* pWorldY, float* pWorldZ)
	{
		return (Status)oniCoordinateConverterDepthToWorld(depthStream._getHandle(), float(depthX), float(depthY), float(depthZ), pWorldX, pWorldY, pWorldZ);
	}

	static Status convertDepthToWorld(const VideoStream& depthStream, float depthX, float depthY, float depthZ, float* pWorldX, float* pWorldY, float* pWorldZ)
	{
		return (Status)oniCoordinateConverterDepthToWorld(depthStream._getHandle(), depthX, depthY, depthZ, pWorldX, pWorldY, pWorldZ);
	}

	static Status convertDepthToColor(const VideoStream& depthStream, const VideoStream& colorStream, int depthX, int depthY, DepthPixel depthZ, int* pColorX, int* pColorY)
	{
		return (Status)oniCoordinateConverterDepthToColor(depthStream._getHandle(), colorStream._getHandle(), depthX, depthY, depthZ, pColorX, pColorY);
	}
};

/**
 * The Recorder class encapsulates a specific recorder. A recorder is an entity
 * which is capable of recording the state of streams and frames into a *.ONI
 * file.
 */
class Recorder
{
// A handy macro, validating that the Recorder has been initialized before
// use. This macro reduces code duplicate significantly.
#define SHOULD_BE_VALID if (!isValid()) { return STATUS_ERROR; } else {}

public:
    /**
     * Creates a recorder. The recorder is not valid, i.e. IsValid() returns
     * false. You must initialize the recorder before use with Initialize().
     */
    Recorder() : m_recorder(NULL)
    {
    }

    /**
     * Destroys a recorder. This will also stop recording.
     */
    ~Recorder()
    {
		destroy();
    }

    /**
     * Initializes a recorder. You can initialize the recorder only once.
	 *
     * @param	[in]	fileName	The name of a file which will contain the recording.
     */
    Status create(const char* fileName)
    {
        if (!isValid())
        {
            return (Status)oniCreateRecorder(fileName, &m_recorder);
        }
        return STATUS_ERROR;
    }

    /**
     * Verifies if the recorder is valid, i.e. if one can record with this recorder.
	 *
     * @retval	true	if the recorder is valid.
     */
    bool isValid() const
    {
        return NULL != getHandle();
    }    

    /**
     * Attaches a stream to the recorder. Note, this won't start recording, you
     * should explicitly start it using Start() method. As soon as the recording
     * process has been started, no more streams can be attached to the recorder.
     */
    Status attach(VideoStream& stream, bool allowLossyCompression = false)
    {
        SHOULD_BE_VALID
        if (!stream.isValid())
        {
            return STATUS_ERROR;
        }
        return (Status)oniRecorderAttachStream(
                m_recorder, 
                stream._getHandle(),
                allowLossyCompression);
    }

    /**
     * Starts recording.
     */
    Status start()
    {
        SHOULD_BE_VALID
        return (Status)oniRecorderStart(m_recorder);
    }

    /**
     * Stops recording. You may use Start() to resume the recording.
     */
    void stop()
    {
		if (isValid())
		{
			oniRecorderStop(m_recorder);
		}
    }

	void destroy()
	{
		if (isValid())
		{
			oniRecorderDestroy(&m_recorder);
		}
	}

private:
	Recorder(const Recorder&);
	Recorder& operator=(const Recorder&);

    /**
     * Returns a handle of this recorder.
     */
    OniRecorderHandle getHandle() const
    {
        return m_recorder;
    }


    OniRecorderHandle m_recorder;

// Get rid of needless macro.
#undef SHOULD_BE_VALID
};

// Implemetation
Status VideoStream::create(const Device& device, SensorType sensorType)
{
	OniStreamHandle streamHandle;
	Status rc = (Status)oniDeviceCreateStream(device._getHandle(), (OniSensorType)sensorType, &streamHandle);
	if (rc != STATUS_OK)
	{
		return rc;
	}

	_setHandle(streamHandle);
	return STATUS_OK;
}


}

#endif // _OPEN_NI_HPP_
