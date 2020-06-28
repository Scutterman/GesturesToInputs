#pragma once
#include "PerformanceTimer.h"
#include <mfidl.h>
#include <mfapi.h>
#include <Mfreadwrite.h>
#include <mutex>

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
        GUID videoFormat;

        unsigned char* buffer1;
        unsigned char* buffer2;
        bool readFromBuffer1 = false;
        
        std::mutex cv_m;
        std::condition_variable cv;

        PerformanceTimer timer;
        CRITICAL_SECTION critsec;
        long refCount = 0;

        HRESULT InitialiseReader(IMFActivate* device);
        HRESULT IsMediaTypeSupported(IMFMediaType* pType);
        HRESULT GetDefaultStride(IMFMediaType* type, LONG* stride);
    public:
        int framesCollected = 0;
        MediaFoundationWebcam();
        ~MediaFoundationWebcam();
        void CreateVideoCaptureDevice();
        HRESULT Close();
        
        unsigned int getWidth() const;
        unsigned int getHeight() const;
        unsigned int getBytesPerPixel() const;
        GUID getVideoFormat() const;
        unsigned char* getData();
        void wait();

        // Inherited via IMFSourceReaderCallback        
        STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;

        STDMETHODIMP_(ULONG) AddRef(void) override;

        STDMETHODIMP_(ULONG) Release(void) override;

        STDMETHODIMP OnFlush(DWORD dwStreamIndex) override;

        STDMETHODIMP OnEvent(DWORD dwStreamIndex, IMFMediaEvent* pEvent) override;

        HRESULT OnReadSample(HRESULT hrStatus, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample* pSample);
    };
}
