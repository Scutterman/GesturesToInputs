#pragma once
#include <mfidl.h>
#include <Mfreadwrite.h>

namespace GesturesToInputs {
    class MediaFoundationWebcam : IMFSourceReaderCallback
    {
    private:
        IMFMediaSource* device;
        IMFSourceReader* reader;
        bool hasNewFrame = false;

        unsigned char* buffer1;
        unsigned int buffer1Length;
        unsigned char* buffer2;
        unsigned int buffer2Length;
        unsigned char** activeBuffer;
        unsigned int* activeBufferLength;

        CRITICAL_SECTION critsec;
        long refCount;

        HRESULT InitialiseReader(IMFSourceReader** ppReader);
    public:
        MediaFoundationWebcam();
        ~MediaFoundationWebcam();
        bool newFrameAvailable();

        // Inherited via IMFSourceReaderCallback        
        STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;

        STDMETHODIMP_(ULONG) AddRef(void) override;

        STDMETHODIMP_(ULONG) Release(void) override;

        STDMETHODIMP OnFlush(DWORD dwStreamIndex) override;

        STDMETHODIMP OnEvent(DWORD dwStreamIndex, IMFMediaEvent* pEvent) override;

        HRESULT OnReadSample(HRESULT hrStatus, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample* pSample);
    };
}
