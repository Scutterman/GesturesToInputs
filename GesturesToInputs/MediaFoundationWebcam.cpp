#include "MediaFoundationWebcam.h"

#include <Mfreadwrite.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mferror.h>
#include <Shlwapi.h>
#include <iostream>

// TODO:: for more functionality see:
// https://docs.microsoft.com/en-us/windows/win32/medfound/audio-video-capture-in-media-foundation
// https://docs.microsoft.com/en-us/windows/win32/medfound/processing-media-data-with-the-source-reader
// https://elcharolin.wordpress.com/2017/08/28/webcam-capture-with-the-media-foundation-sdk/
namespace GesturesToInputs {
    template <class T> void SafeRelease(T** ppT)
    {
        if (*ppT)
        {
            (*ppT)->Release();
            *ppT = NULL;
        }
    }

    MediaFoundationWebcam::MediaFoundationWebcam()
    {
        InitializeCriticalSection(&critsec);
        refCount = 1;
        width = 0;
        height = 0;
        reader = NULL;
        buffer1 = NULL;
        buffer2 = NULL;
    }

    MediaFoundationWebcam::~MediaFoundationWebcam()
    {
        EnterCriticalSection(&critsec);
        SafeRelease(&reader);
            
        if (buffer1) {
            delete buffer1;
            buffer1 = NULL;
        }
        
        if (buffer2) {
            delete buffer2;
            buffer2 = NULL;
        }

        LeaveCriticalSection(&critsec);
        DeleteCriticalSection(&critsec);
    }

    HRESULT MediaFoundationWebcam::CreateVideoCaptureDevice()
    {
        HRESULT hr = S_OK;

        //this is important!!
        hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

        UINT32 count = 0;
        IMFAttributes* attributes = NULL;
        IMFActivate** devices = NULL;

        // Create an attribute store to hold the search criteria.
        hr = MFCreateAttributes(&attributes, 1);

        // Request video capture devices.
        if (SUCCEEDED(hr))
        {
            hr = attributes->SetGUID(
                MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
            );
        }

        // Enumerate the devices,
        if (SUCCEEDED(hr))
        {
            hr = MFEnumDeviceSources(attributes, &devices, &count);
        }

        // Create a media source for the first device in the list.
        if (SUCCEEDED(hr))
        {
            if (count > 0)
            {
                hr = InitialiseReader(devices[0]);
                if (SUCCEEDED(hr))
                {
                    bytesPerPixel = abs(stride) / width;
                    buffer1 = new unsigned char[width * height * bytesPerPixel];
                    buffer2 = new unsigned char[width * height * bytesPerPixel];
                }
                //hr = ->ActivateObject(IID_PPV_ARGS(ppSource));
            }
        }

        SafeRelease(&attributes);

        for (DWORD i = 0; i < count; i++) {
            SafeRelease(&devices[i]);
        }

        CoTaskMemFree(devices);
        return hr;
    }

    HRESULT MediaFoundationWebcam::InitialiseReader(IMFActivate* device)
    {
        HRESULT hr = S_OK;

        IMFMediaSource* source = NULL;
        IMFAttributes* attributes = NULL;
        IMFMediaType* mediaType = NULL;

        EnterCriticalSection(&critsec);
        hr = device->ActivateObject(__uuidof(IMFMediaSource), (void**)&source);
        if (FAILED(hr))
        {
            goto done;
        }
        
        hr = MFCreateAttributes(&attributes, 2);
        if (FAILED(hr))
        {
            goto done;
        }

        hr = attributes->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, TRUE);
        if (FAILED(hr))
        {
            goto done;
        }

        hr = attributes->SetUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, this);
        if (FAILED(hr))
        {
            goto done;
        }

        hr = MFCreateSourceReaderFromMediaSource(source, attributes, &reader);
        if (FAILED(hr))
        {
            goto done;
        }

        for (DWORD i = 0; ; i++)
        {
            hr = reader->GetNativeMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, i, &mediaType);
            if (FAILED(hr)) { break; }

            hr = IsMediaTypeSupported(mediaType);
            if (FAILED(hr)) { break; }
            
            //Get width and height
            MFGetAttributeSize(mediaType, MF_MT_FRAME_SIZE, &width, &height);
            if (mediaType)
            {
                mediaType->Release(); mediaType = NULL;
            }

            if (SUCCEEDED(hr)) {
                // Found an output type.
                break;
            }
        }

        if (SUCCEEDED(hr))
        {
            hr = reader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, NULL, NULL, NULL, NULL);
        }

        if (FAILED(hr))
        {
            if (source)
            {
                source->Shutdown();
            }
        }
        
    done:
        SafeRelease(&source);
        SafeRelease(&attributes);
        SafeRelease(&mediaType);
        LeaveCriticalSection(&critsec);
        return hr;
    }

    HRESULT MediaFoundationWebcam::IsMediaTypeSupported(IMFMediaType* pType)
    {
        HRESULT hr = S_OK;

        BOOL bFound = FALSE;
        GUID subtype = { 0 };

        //Get the stride for this format so we can calculate the number of bytes per pixel
        GetDefaultStride(pType, &stride);

        if (FAILED(hr)) { return hr; }
        hr = pType->GetGUID(MF_MT_SUBTYPE, &subtype);

        if (FAILED(hr)) { return hr; }

        videoFormat = subtype;
        if (subtype == MFVideoFormat_Base) { std::cout << "MFVideoFormat_Base" << std::endl; }
        if (subtype == MFVideoFormat_RGB32) { std::cout << "MFVideoFormat_RGB32" << std::endl; }
        if (subtype == MFVideoFormat_ARGB32) { std::cout << "MFVideoFormat_ARGB32" << std::endl; }
        if (subtype == MFVideoFormat_RGB24) { std::cout << "MFVideoFormat_RGB24" << std::endl; }
        if (subtype == MFVideoFormat_RGB555) { std::cout << "MFVideoFormat_RGB555" << std::endl; }
        if (subtype == MFVideoFormat_RGB565) { std::cout << "MFVideoFormat_RGB565" << std::endl; }
        if (subtype == MFVideoFormat_RGB8) { std::cout << "MFVideoFormat_RGB8" << std::endl; }
        if (subtype == MFVideoFormat_L8) { std::cout << "MFVideoFormat_L8" << std::endl; }
        if (subtype == MFVideoFormat_L16) { std::cout << "MFVideoFormat_L16" << std::endl; }
        if (subtype == MFVideoFormat_D16) { std::cout << "MFVideoFormat_D16" << std::endl; }
        if (subtype == MFVideoFormat_AI44) { std::cout << "MFVideoFormat_AI44" << std::endl; }
        if (subtype == MFVideoFormat_AYUV) { std::cout << "MFVideoFormat_AYUV" << std::endl; }
        if (subtype == MFVideoFormat_YUY2) { std::cout << "MFVideoFormat_YUY2" << std::endl; }
        if (subtype == MFVideoFormat_YVYU) { std::cout << "MFVideoFormat_YVYU" << std::endl; }
        if (subtype == MFVideoFormat_YVU9) { std::cout << "MFVideoFormat_YVU9" << std::endl; }
        if (subtype == MFVideoFormat_UYVY) { std::cout << "MFVideoFormat_UYVY" << std::endl; }
        if (subtype == MFVideoFormat_NV11) { std::cout << "MFVideoFormat_NV11" << std::endl; }
        if (subtype == MFVideoFormat_NV12) { std::cout << "MFVideoFormat_NV12" << std::endl; }
        if (subtype == MFVideoFormat_YV12) { std::cout << "MFVideoFormat_YV12" << std::endl; }
        if (subtype == MFVideoFormat_I420) { std::cout << "MFVideoFormat_I420" << std::endl; }
        if (subtype == MFVideoFormat_IYUV) { std::cout << "MFVideoFormat_IYUV" << std::endl; }
        if (subtype == MFVideoFormat_Y210) { std::cout << "MFVideoFormat_Y210" << std::endl; }
        if (subtype == MFVideoFormat_Y216) { std::cout << "MFVideoFormat_Y216" << std::endl; }
        if (subtype == MFVideoFormat_Y410) { std::cout << "MFVideoFormat_Y410" << std::endl; }
        if (subtype == MFVideoFormat_Y416) { std::cout << "MFVideoFormat_Y416" << std::endl; }
        if (subtype == MFVideoFormat_Y41P) { std::cout << "MFVideoFormat_Y41P" << std::endl; }
        if (subtype == MFVideoFormat_Y41T) { std::cout << "MFVideoFormat_Y41T" << std::endl; }
        if (subtype == MFVideoFormat_Y42T) { std::cout << "MFVideoFormat_Y42T" << std::endl; }
        if (subtype == MFVideoFormat_P210) { std::cout << "MFVideoFormat_P210" << std::endl; }
        if (subtype == MFVideoFormat_P216) { std::cout << "MFVideoFormat_P216" << std::endl; }
        if (subtype == MFVideoFormat_P010) { std::cout << "MFVideoFormat_P010" << std::endl; }
        if (subtype == MFVideoFormat_P016) { std::cout << "MFVideoFormat_P016" << std::endl; }
        if (subtype == MFVideoFormat_v210) { std::cout << "MFVideoFormat_v210" << std::endl; }
        if (subtype == MFVideoFormat_v216) { std::cout << "MFVideoFormat_v216" << std::endl; }
        if (subtype == MFVideoFormat_v410) { std::cout << "MFVideoFormat_v410" << std::endl; }
        if (subtype == MFVideoFormat_MP43) { std::cout << "MFVideoFormat_MP43" << std::endl; }
        if (subtype == MFVideoFormat_MP4S) { std::cout << "MFVideoFormat_MP4S" << std::endl; }
        if (subtype == MFVideoFormat_M4S2) { std::cout << "MFVideoFormat_M4S2" << std::endl; }
        if (subtype == MFVideoFormat_MP4V) { std::cout << "MFVideoFormat_MP4V" << std::endl; }
        if (subtype == MFVideoFormat_WMV1) { std::cout << "MFVideoFormat_WMV1" << std::endl; }
        if (subtype == MFVideoFormat_WMV2) { std::cout << "MFVideoFormat_WMV2" << std::endl; }
        if (subtype == MFVideoFormat_WMV3) { std::cout << "MFVideoFormat_WMV3" << std::endl; }
        if (subtype == MFVideoFormat_WVC1) { std::cout << "MFVideoFormat_WVC1" << std::endl; }
        if (subtype == MFVideoFormat_MSS1) { std::cout << "MFVideoFormat_MSS1" << std::endl; }
        if (subtype == MFVideoFormat_MSS2) { std::cout << "MFVideoFormat_MSS2" << std::endl; }
        if (subtype == MFVideoFormat_MPG1) { std::cout << "MFVideoFormat_MPG1" << std::endl; }
        if (subtype == MFVideoFormat_DVSL) { std::cout << "MFVideoFormat_DVSL" << std::endl; }
        if (subtype == MFVideoFormat_DVSD) { std::cout << "MFVideoFormat_DVSD" << std::endl; }
        if (subtype == MFVideoFormat_DVHD) { std::cout << "MFVideoFormat_DVHD" << std::endl; }
        if (subtype == MFVideoFormat_DV25) { std::cout << "MFVideoFormat_DV25" << std::endl; }
        if (subtype == MFVideoFormat_DV50) { std::cout << "MFVideoFormat_DV50" << std::endl; }
        if (subtype == MFVideoFormat_DVH1) { std::cout << "MFVideoFormat_DVH1" << std::endl; }
        if (subtype == MFVideoFormat_DVC) { std::cout << "MFVideoFormat_DVC" << std::endl; }
        if (subtype == MFVideoFormat_H264) { std::cout << "MFVideoFormat_H264" << std::endl; }
        if (subtype == MFVideoFormat_H265) { std::cout << "MFVideoFormat_H265" << std::endl; }
        if (subtype == MFVideoFormat_MJPG) { std::cout << "MFVideoFormat_MJPG" << std::endl; }
        if (subtype == MFVideoFormat_420O) { std::cout << "MFVideoFormat_420O" << std::endl; }
        if (subtype == MFVideoFormat_HEVC) { std::cout << "MFVideoFormat_HEVC" << std::endl; }
        if (subtype == MFVideoFormat_HEVC_ES) { std::cout << "MFVideoFormat_HEVC_ES" << std::endl; }
        if (subtype == MFVideoFormat_VP80) { std::cout << "MFVideoFormat_VP80" << std::endl; }
        if (subtype == MFVideoFormat_VP90) { std::cout << "MFVideoFormat_VP90" << std::endl; }
        if (subtype == MFVideoFormat_ORAW) { std::cout << "MFVideoFormat_ORAW" << std::endl; }
        
        if (subtype == MFVideoFormat_RGB32 || subtype == MFVideoFormat_RGB24) {
            return S_OK;
        }
        else {
            return S_FALSE;
        }

        return hr;
    }
    
    HRESULT MediaFoundationWebcam::GetDefaultStride(IMFMediaType* type, LONG* stride)
    {
        LONG tempStride = 0;

        // Try to get the default stride from the media type.
        HRESULT hr = type->GetUINT32(MF_MT_DEFAULT_STRIDE, (UINT32*)&tempStride);
        if (FAILED(hr))
        {
            //Setting this atribute to NULL we can obtain the default stride
            GUID subtype = GUID_NULL;

            UINT32 width = 0;
            UINT32 height = 0;

            // Obtain the subtype
            hr = type->GetGUID(MF_MT_SUBTYPE, &subtype);
            //obtain the width and height
            if (SUCCEEDED(hr))
                hr = MFGetAttributeSize(type, MF_MT_FRAME_SIZE, &width, &height);
            //Calculate the stride based on the subtype and width
            if (SUCCEEDED(hr))
                hr = MFGetStrideForBitmapInfoHeader(subtype.Data1, width, &tempStride);
            // set the attribute so it can be read
            if (SUCCEEDED(hr))
                (void)type->SetUINT32(MF_MT_DEFAULT_STRIDE, UINT32(tempStride));
        }

        if (SUCCEEDED(hr))
            *stride = tempStride;
        return hr;
    }

    STDMETHODIMP MediaFoundationWebcam::OnReadSample(HRESULT status, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample* sample)
    {
        //timer.Start();
        HRESULT hr = S_OK;
        IMFMediaBuffer* mediaBuffer = NULL;

        EnterCriticalSection(&critsec);
        if (FAILED(status)) {
            hr = status;
        }

        if (SUCCEEDED(hr) && sample)
        {
            hr = sample->GetBufferByIndex(0, &mediaBuffer);
            
            if (SUCCEEDED(hr))
            {
                unsigned char* data;
                mediaBuffer->Lock(&data, NULL, NULL);
                if (readFromBuffer1 == false) {
                    CopyMemory(buffer1, data, width * height * bytesPerPixel);
                }
                else {
                    CopyMemory(buffer2, data, width * height * bytesPerPixel);
                }
                readFromBuffer1 = !readFromBuffer1;
                hasNewFrame = true;
                // TODO:: Raise an event that notifies of a new frame.
                // If there wasn't a new frame last time the program checked it will need a hint to check again.
                framesCollected++;
                //timer.End();
            }
        }
        
        if (SUCCEEDED(hr)) {
            hr = reader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, NULL, NULL, NULL, NULL);
        }
        
        SafeRelease(&mediaBuffer);
        LeaveCriticalSection(&critsec);
        return hr;
    }

    bool MediaFoundationWebcam::newFrameAvailable()
    {
        return hasNewFrame;
    }

    unsigned char* MediaFoundationWebcam::getData()
    {
        hasNewFrame = false;
        return readFromBuffer1 ? buffer1 : buffer2;
    }

    HRESULT MediaFoundationWebcam::Close()
    {
        EnterCriticalSection(&critsec);
        SafeRelease(&reader);
        LeaveCriticalSection(&critsec);
        return S_OK;
    }

    unsigned int MediaFoundationWebcam::getWidth()
    {
        return width;
    }

    unsigned int MediaFoundationWebcam::getHeight()
    {
        return height;
    }

    unsigned int MediaFoundationWebcam::getBytesPerPixel()
    {
        return bytesPerPixel;
    }

    GUID MediaFoundationWebcam::getVideoFormat()
    {
        return videoFormat;
    }

    // IUnknown methods
    STDMETHODIMP MediaFoundationWebcam::QueryInterface(REFIID iid, void** ppv)
    {
        static const QITAB qit[] =
        {
            QITABENT(MediaFoundationWebcam, IMFSourceReaderCallback),
            { 0 },
        };
        return QISearch(this, qit, iid, ppv);
    }
    
    STDMETHODIMP_(ULONG) MediaFoundationWebcam::AddRef()
    {
        return InterlockedIncrement(&refCount);
    }
    
    STDMETHODIMP_(ULONG) MediaFoundationWebcam::Release()
    {
        ULONG uCount = InterlockedDecrement(&refCount);
        if (uCount == 0)
        {
            delete this;
        }
        return uCount;
    }

    // Unused IMFSourceReaderCallback Methods

    STDMETHODIMP MediaFoundationWebcam::OnEvent(DWORD, IMFMediaEvent*)
    {
        return S_OK;
    }

    STDMETHODIMP MediaFoundationWebcam::OnFlush(DWORD)
    {
        return S_OK;
    }
}
