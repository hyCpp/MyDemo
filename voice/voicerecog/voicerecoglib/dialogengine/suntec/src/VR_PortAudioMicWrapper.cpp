/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#include "VR_PortAudioMicWrapper.h"
#include "VR_Log.h"
#include "VR_InteractionManager.h"

#ifndef VR_CONFIGUREIF_H
#    include "VR_ConfigureIF.h"
#endif
#ifndef VR_CONFIGURE_H
#    include "VR_Configure.h"
#endif

#ifndef CXX_GOOASSPUBLIC_H
#    include "GooAssPublic.h"
#endif

#ifndef VR_BAIDUSDS_IF_H
#    include "VR_BaiduSDS_IF.h"
#endif

#ifndef VR_DICTATIONMANAGER_H
#    include "VR_DictationManager.h"
#endif

using avsCommon::avs::AudioInputStream;

static const int NUM_INPUT_CHANNELS = 1;
static const int NUM_OUTPUT_CHANNELS = 0;
static const double SAMPLE_RATE = 16000;
// static const unsigned long PREFERRED_SAMPLES_PER_CALLBACK = SAMPLE_RATE * 0.01;
static const double PREFERRED_SAMPLES_PER_CALLBACK = (unsigned long)(SAMPLE_RATE * 0.01);

#define NUM_BUFFER     96000

// #define GOOGLE_FILE_NAME "/data/vr/google.wav"

AudioInStreamer*           VR_PortAudioMicWrapper::m_pGoogleStreamer = nullptr;
VR_AudioInStreamer*        VR_PortAudioMicWrapper::m_pBaiduStreamer = nullptr;
std::string                VR_PortAudioMicWrapper::m_strEngineValue = "Alexa";
std::string                VR_PortAudioMicWrapper::m_strMicState = "IDLE";
// boost::shared_ptr<VC_WavFileWriter>          VR_PortAudioMicWrapper::m_spWavWriter = nullptr;

std::unique_ptr<VR_PortAudioMicWrapper> VR_PortAudioMicWrapper::create(std::shared_ptr<AudioInputStream> stream)
{
    if (!stream) {
        VR_LOGI("Invalid stream passed to VR_PortAudioMicWrapper");
        return nullptr;
    }
    std::unique_ptr<VR_PortAudioMicWrapper> portAudioMicrophoneWrapper(new VR_PortAudioMicWrapper(stream));
    if (!portAudioMicrophoneWrapper->initialize()) {
        VR_LOGI("Failed to initialize VR_PortAudioMicWrapper");
        return nullptr;
    }
    return portAudioMicrophoneWrapper;
}

VR_PortAudioMicWrapper::VR_PortAudioMicWrapper(std::shared_ptr<AudioInputStream> stream) :
        m_audioInputStream{ stream },
        m_paStream{ nullptr },
        m_stopReadWareFile{ false },
        m_audioMicrophone{ nullptr },
        m_numfile{ 0 },
        m_srcName(""),
        m_writeOverLoad(false),
        m_dictationManager{ nullptr }
{
        m_buffer = new short[NUM_BUFFER];
        if (NULL != m_buffer) {
            memset(m_buffer, 0, sizeof(short)*NUM_BUFFER);
        }
        m_topbuffer = m_buffer;
}

VR_PortAudioMicWrapper::~VR_PortAudioMicWrapper()
{
    // Pa_StopStream(m_paStream);
    // Pa_CloseStream(m_paStream);
    // Pa_Terminate();
    if (m_buffer != NULL) {
        delete[] m_buffer;
        m_buffer = NULL;
    }
}

bool VR_PortAudioMicWrapper::initialize()
{
    VR_LOGD_FUNC();


    // if (!m_spWavWriter) {
    //     m_spWavWriter = new VC_WavFileWriter();
    //     if (NULL = m_spWavWriter) {
    //         m_spWavWriter->Open(GOOGLE_FILE_NAME);
    //     }
    //     else {
    //         VR_LOGI("create VC_WavFileWriter failed");
    //     }
    // }

    m_writer = m_audioInputStream->createWriter(AudioInputStream::Writer::Policy::NONBLOCKABLE);
    if (!m_writer) {
        VR_LOGI("Failed to create stream writer");
        return false;
    }

    // PaError err;
    // err = Pa_Initialize();
    // if (err != paNoError) {
    //     VR_LOGI("Failed to initialize PortAudio");
    //     return false;
    // }
    // err = Pa_OpenDefaultStream(
    //     &m_paStream,
    //     NUM_INPUT_CHANNELS,
    //     NUM_OUTPUT_CHANNELS,
    //     paInt16,
    //     SAMPLE_RATE,
    //     PREFERRED_SAMPLES_PER_CALLBACK,
    //     PortAudioCallback,
    //     this);
    // if (err != paNoError) {
    //     VR_LOGI("Failed to open PortAudio default stream");
    //     return false;
    // }

    return true;
}

void VR_PortAudioMicWrapper::SetInteractionObj(std::shared_ptr<VR_InteractionManager> interactionManager)
{
    VR_LOGD_FUNC();

    m_interactionManager = interactionManager;

    if (NULL == m_interactionManager) {
        VR_LOGI("m_interactionManager is null");
    }
}

void VR_PortAudioMicWrapper::SetMicrophoneObj(alexaClientSDK::sampleApp::AudioMicrophone* audioMicrophone)
{
    VR_LOGD_FUNC();

    m_audioMicrophone = audioMicrophone;

    if (NULL == m_audioMicrophone) {
        VR_LOGI("m_audioMicrophone is null");
    }
}

bool VR_PortAudioMicWrapper::startStreamingMicrophoneData()
{
    VR_LOGD_FUNC();
    std::lock_guard<std::mutex> lock(m_mutex);
    bool startStream = false;
    if (nullptr != m_audioMicrophone) {
        startStream = m_audioMicrophone->startStreamingMicrophoneData();
    }
    if (!startStream) {
        VR_LOGI("Failed to startStreamingMicrophoneData");
        return false;
    }
    return true;
}

bool VR_PortAudioMicWrapper::stopStreamingMicrophoneData()
{
    VR_LOGD_FUNC();

    std::lock_guard<std::mutex> lock(m_mutex);
    bool stopStream = false;

    if (nullptr != m_audioMicrophone) {
        stopStream = m_audioMicrophone->stopStreamingMicrophoneData();
    }

    if (!stopStream) {
        VR_LOGI("Failed to stopStreamingMicrophoneData");
        return false;
    }
    return true;
}

void VR_PortAudioMicWrapper::SaveMicToBuffer(const short* inputBuffer, unsigned long numSamples)
{
    std::lock_guard<std::mutex> lock(m_mutexbuffer);
    m_topbuffer = SaveMicToBuffer(inputBuffer, numSamples, m_buffer, m_topbuffer);
}

short* VR_PortAudioMicWrapper::SaveMicToBuffer(const short *inputBuffer, unsigned long numSamples, short *buffer, short *ptop)
{
    if (NULL == inputBuffer || NULL == buffer || NULL == ptop) {
        return ptop;
    }
    int startPos = std::abs(ptop - buffer);
    int leastPos = NUM_BUFFER - startPos;
    if (numSamples > NUM_BUFFER) {
        memcpy(buffer, inputBuffer + numSamples - NUM_BUFFER, NUM_BUFFER * sizeof(short));
        return buffer + NUM_BUFFER;
    }
    if (leastPos < numSamples) {
        memcpy(ptop, inputBuffer, leastPos);
        memcpy(buffer, inputBuffer + leastPos, (numSamples - leastPos) * sizeof(short));
        m_writeOverLoad = true;
        return buffer + numSamples - leastPos;

    }
    else {
        memcpy(ptop, inputBuffer, numSamples*sizeof(short));
        return ptop + numSamples;
    }
}

void VR_PortAudioMicWrapper::SaveMicToFile(std::string strMicState)
{
    if (10000 < m_numfile++) {
        return;
    }
    std::string strNum = std::to_string(time(NULL));
    std::string srcName=VR_ConfigureIF::Instance()->getUsrPath() + strMicState + strNum + ".pcm";
    m_srcName = srcName;
    {
        std::lock_guard<std::mutex> lock(m_mutexbuffer);
        FILE *pFile = fopen(srcName.c_str(), "wb+");
        if (NULL != pFile) {

            int num = m_topbuffer - m_buffer;
            std::abs(num);
            if (m_writeOverLoad) {
                fwrite(m_topbuffer, sizeof(short), NUM_BUFFER - num, pFile);
            }
            fwrite(m_buffer, sizeof(short), num, pFile);
            m_writeOverLoad = false;
            fclose(pFile);
            memset(m_buffer, 0, sizeof(short)*NUM_BUFFER);
            m_topbuffer = m_buffer;
        }
    }
}

int VR_PortAudioMicWrapper::WriteAudioDataToSDK(const void* inputBuffer, unsigned long numSamples)
{

    SaveMicToBuffer((short*)inputBuffer, numSamples);

    if (nullptr != m_dictationManager) {
        m_dictationManager->DicTationData(inputBuffer, numSamples);
    }

    if ("Baidu" == m_strEngineValue) {
        void* inputBuf = const_cast<void*>(inputBuffer);
        short* buf = static_cast<short*>(inputBuf);
        size_t len = static_cast<size_t>(numSamples);

        if (nullptr != m_pBaiduStreamer) {
            m_pBaiduStreamer->onAudioInData(buf, len);
            // VR_LOGI("baidu onAudioInData is called");
        }
        return 1;
    }
    if ("Google" == m_strEngineValue) {
        void* inputBuf = const_cast<void*>(inputBuffer);
        short* buf = static_cast<short*>(inputBuf);
        size_t len = static_cast<size_t>(numSamples);
        // VR_LOGI("len is: %d", len);

        if (nullptr != m_pGoogleStreamer) {
            m_pGoogleStreamer->onAudioInData(buf, len);
            // VR_LOGI("google onAudioInData is called");
        }
        return 1;
    }
    if ("Alexa" == m_strEngineValue) {
        ssize_t returnCode = m_writer->write(inputBuffer, numSamples);
        // VR_LOGI("alexa onAudioInData is called");
        if (returnCode <= 0) {
            VR_LOGI("Failed to write to stream.");
            return paAbort;
        }
    }
    return 1;
}

int VR_PortAudioMicWrapper::PortAudioCallback(
    const void* inputBuffer,
    void* outputBuffer,
    unsigned long numSamples,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    // if ("Baidu" == m_strEngineValue) {
    //     void* inputBuf = const_cast<void*>(inputBuffer);
    //     short* buf = static_cast<short*>(inputBuf);
    //     size_t len = static_cast<size_t>(numSamples);

    //     if (nullptr != m_pBaiduStreamer) {
    //         m_pBaiduStreamer->onAudioInData(buf, len);
    //         // VR_LOGI("baidu onAudioInData is called");
    //     }

    //     return paContinue;
    // }

    // if ("Google" == m_strEngineValue) {
    //     void* inputBuf = const_cast<void*>(inputBuffer);
    //     short* buf = static_cast<short*>(inputBuf);
    //     size_t len = static_cast<size_t>(numSamples);
    //     VR_LOGI("len is: %d", len);


    //     // static int count = 0;
    //     // if (count < 100) {
    //     //     m_spWavWriter->Write(inputBuf, sizeof(short)*numSamples);
    //     //     VR_LOGI("Write to google.wav, count is: %d", count);
    //     //     count++;
    //     // }
    //     // if (count >= 100) {
    //     //     m_spWavWriter->Close();
    //     //     if (!m_spWavWriter) {
    //     //         delete m_spWavWriter;
    //     //         m_spWavWriter = NULL;
    //     //     }
    //     // }

    //     // static int count = 0;
    //     // if (count < 1000) {
    //     //     count++;
    //     //     std::ofstream ofs;
    //     //     ofs.open ("/tmp/google.pcm", std::ofstream::out | std::ofstream::app | std::ofstream::binary);
    //     //     ofs.write((char*)inputBuf, sizeof(short)*numSamples);
    //     //     ofs.close();
    //     // }

    //     if (nullptr != m_pGoogleStreamer) {
    //         m_pGoogleStreamer->onAudioInData(buf, len);
    //         VR_LOGI("google onAudioInData is called");
    //     }

    //     return paContinue;
    // }

    // if ("Alexa" == m_strEngineValue) {
    //     VR_PortAudioMicWrapper* wrapper = static_cast<VR_PortAudioMicWrapper*>(userData);
    //     // VR_LOGI("PortAudioCallback writer");
    //     ssize_t returnCode = wrapper->m_writer->write(inputBuffer, numSamples);

    //     if (returnCode <= 0) {
    //         VR_LOGI("Failed to write to stream.");
    //         return paAbort;
    //     }
    // }

    return paContinue;
}

bool VR_PortAudioMicWrapper::startStreamingData(const std::string& audioFile)
{
    VR_LOGD_FUNC();

    // std::lock_guard<std::mutex> lock{m_mutex};
    m_stopReadWareFile = false;

    m_audioSenderFile = audioFile;

    // m_executor.submit([this]() {
    //     VR_LOGI("let other thread do");
    //     threadFunc();
    // });
    // m_thread = std::thread(&VR_PortAudioMicWrapper::threadFunc, this);

    std::thread tA(&VR_PortAudioMicWrapper::threadFunc, this);
    tA.detach();

    return true;
}

bool VR_PortAudioMicWrapper::stopStreamingData()
{
    VR_LOGD_FUNC();

    // std::lock_guard<std::mutex> lock{m_mutex};
    // m_stopReadWareFile = true;

    // if (m_thread.joinable()) {
    //     m_thread.join();
    // }

    return true;
}

void VR_PortAudioMicWrapper::threadFunc()
{
    VR_LOGD_FUNC();

    // const int WRITE_INTERVAL = 1000; // 1ms
    // usleep(WRITE_INTERVAL);

    if (!m_writer) {
        VR_LOGI("stream writer is null");
        return;
    }

    FILE* fpWavHandler = fopen(m_audioSenderFile.c_str(), "rb");
    if (NULL == fpWavHandler) {
        VR_LOGI("fpWavHandler is null");
        if (NULL != m_interactionManager) {
            m_interactionManager->NotifyCloseSuggestedSession();
        }
        startStreamingMicrophoneData();
        return;
    }

    const size_t numSamples = 160;
    short inputBuffer[numSamples] = { 0 };

    while (!m_stopReadWareFile) {
        VR_LOGI("read pcm data and write to alexa");
        int readLen = fread(inputBuffer, sizeof(short), numSamples, fpWavHandler);
        VR_LOGI("readLen : %d", readLen);
        if (readLen <= 0) {
            m_stopReadWareFile = true;
            break;
        }
        int returnCode = m_writer->write(inputBuffer, numSamples);
        VR_LOGI("returnCode : %d", returnCode);
        if (returnCode <= 0) {
            VR_LOGI("Failed to write to stream.");
            break;
        }
    }

    // TODO: close file
    fclose(fpWavHandler);
    fpWavHandler = NULL;

    if (NULL != m_interactionManager) {
        m_interactionManager->stopFileAudio();
        // std::lock_guard<std::mutex> lock{m_mutex};
        m_stopReadWareFile = true;
    }
    else {
        VR_LOGI("m_interactionManager is null");
    }

    return;
}

void VR_PortAudioMicWrapper::SetGoogleAudioIn(AudioInStreamer* stream)
{
    VR_LOGD_FUNC();
    m_pGoogleStreamer = stream;
    return;
}

void VR_PortAudioMicWrapper::SetBaiduAudioIn(VR_AudioInStreamer* stream)
{
    VR_LOGD_FUNC();
    m_pBaiduStreamer = stream;
    return;
}

void VR_PortAudioMicWrapper::SetEngineValue(const std::string& strEngineValue)
{
    VR_LOGD_FUNC();

    m_strEngineValue = strEngineValue;

    return;
}

void VR_PortAudioMicWrapper::SetMicState(const std::string& strMicState)
{
    VR_LOGD_FUNC();

    m_strMicState = strMicState;

    return;
}

void VR_PortAudioMicWrapper::SetDictationManager(std::shared_ptr<VR_DictationManager> dictationManager)
{
    VR_LOGD_FUNC();
    m_dictationManager = dictationManager;
    return;
}

/* EOF */
