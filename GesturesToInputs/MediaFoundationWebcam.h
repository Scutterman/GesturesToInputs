#pragma once
#include "PerformanceTimer.h"
#include <mfidl.h>
#include <mfapi.h>
#include <Mfreadwrite.h>

namespace GesturesToInputs {
    class MediaFoundationWebcam : IMFSourceReaderCallback
    {
    private:
        bool isDestructing = false;
        IMFSourceReader* reader;
        unsigned int width = 0;
        unsigned int height = 0;
        long stride = 0;
        int bytesPerPixel = 0;
        bool hasNewFrame = false;

        unsigned char* buffer1;
        unsigned char* buffer2;
        bool readFromBuffer1 = false;
        
        PerformanceTimer timer;
        CRITICAL_SECTION critsec;
        long refCount = 0;

        HRESULT InitialiseReader(IMFActivate* device);
        HRESULT IsMediaTypeSupported(IMFMediaType* pType);
        HRESULT GetDefaultStride(IMFMediaType* type, LONG* stride);
    public:
        GUID videoFormat;
        int framesCollected = 0;
        MediaFoundationWebcam();
        ~MediaFoundationWebcam();
        HRESULT CreateVideoCaptureDevice();
        HRESULT Close();
        bool newFrameAvailable();
        unsigned char* getData();

        // Inherited via IMFSourceReaderCallback        
        STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;

        STDMETHODIMP_(ULONG) AddRef(void) override;

        STDMETHODIMP_(ULONG) Release(void) override;

        STDMETHODIMP OnFlush(DWORD dwStreamIndex) override;

        STDMETHODIMP OnEvent(DWORD dwStreamIndex, IMFMediaEvent* pEvent) override;

        HRESULT OnReadSample(HRESULT hrStatus, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample* pSample);
    };
}
