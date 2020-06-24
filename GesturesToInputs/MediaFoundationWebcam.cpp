#include "MediaFoundationWebcam.h"

#include <Mfreadwrite.h>
#include <mfapi.h>
#include <mferror.h>
#include <Shlwapi.h>

// TODO:: for more functionality see:
// https://docs.microsoft.com/en-us/windows/win32/medfound/audio-video-capture-in-media-foundation
// https://docs.microsoft.com/en-us/windows/win32/medfound/processing-media-data-with-the-source-reader
namespace GesturesToInputs {
    MediaFoundationWebcam::MediaFoundationWebcam(): device(NULL)
    {
        InitializeCriticalSection(&critsec);
        CreateVideoCaptureDevice(&device);
        InitialiseReader(&reader);
    }

    MediaFoundationWebcam::~MediaFoundationWebcam()
    {
        SafeRelease(&reader);
        Release();
        device->Shutdown();
        device->Release();
        device = NULL;
    }
    
    HRESULT MediaFoundationWebcam::InitialiseReader(IMFSourceReader** ppReader)
    {
        HRESULT hr = S_OK;
        IMFAttributes* pAttributes = NULL;

        hr = MFCreateAttributes(&pAttributes, 1);
        if (FAILED(hr))
        {
            goto done;
        }

        hr = pAttributes->SetUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, this);
        if (FAILED(hr))
        {
            goto done;
        }

        hr = MFCreateSourceReaderFromMediaSource(device, pAttributes, ppReader);

    done:
        SafeRelease(&pAttributes);
        return hr;
    }

    // IMFSourceReaderCallback methods
    STDMETHODIMP MediaFoundationWebcam::OnReadSample(HRESULT hrStatus, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample* pSample)
    {
        EnterCriticalSection(&critsec);
        if (SUCCEEDED(hrStatus))
        {
            if (pSample)
            {
                // Do something with the sample.
            }
        }

        LeaveCriticalSection(&critsec);
        return S_OK;
    }

    bool MediaFoundationWebcam::newFrameAvailable()
    {
        return hasNewFrame;
    }

    HRESULT CreateVideoCaptureDevice(IMFMediaSource** ppSource)
    {
        *ppSource = NULL;

        UINT32 count = 0;

        IMFAttributes* pConfig = NULL;
        IMFActivate** ppDevices = NULL;

        // Create an attribute store to hold the search criteria.
        HRESULT hr = MFCreateAttributes(&pConfig, 1);

        // Request video capture devices.
        if (SUCCEEDED(hr))
        {
            hr = pConfig->SetGUID(
                MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
            );
        }

        // Enumerate the devices,
        if (SUCCEEDED(hr))
        {
            hr = MFEnumDeviceSources(pConfig, &ppDevices, &count);
        }

        // Create a media source for the first device in the list.
        if (SUCCEEDED(hr))
        {
            if (count > 0)
            {
                hr = ppDevices[0]->ActivateObject(IID_PPV_ARGS(ppSource));
            }
            else
            {
                hr = MF_E_NOT_FOUND;
            }
        }

        for (DWORD i = 0; i < count; i++)
        {
            ppDevices[i]->Release();
        }
        CoTaskMemFree(ppDevices);
        return hr;
    }

    template <class T> void SafeRelease(T** ppT)
    {
        if (*ppT)
        {
            (*ppT)->Release();
            *ppT = NULL;
        }
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
