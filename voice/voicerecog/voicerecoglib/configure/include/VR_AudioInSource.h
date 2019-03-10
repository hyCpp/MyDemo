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

/**
 * @file VR_AudioInSource.h
 * @brief process list data in DE
 *
 *
 * @attention used for C++ only.
 */
#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef _VR_AUDIOINSOURCE_H
#define _VR_AUDIOINSOURCE_H

#include <atomic>

#include "VC_AudioIn.h"
#include "Vr_Asr_Audio_In.h"
#include "VC_CommonIF.h"
#include "MEM_list.h"
#include <boost/shared_ptr.hpp>
#include "CL_WaitObj.h"
#include "CL_SyncObj.h"
#include "VC_WavFileWriter.h"
#include "VR_Def.h"

/**
 * @brief The VR_AudioInSource class
 *
 * verbose monitor class
 */
class VR_API VR_AudioInSource : public N_Vr::N_Asr::C_Audio_In, public VC_AudioIn::Listener
{
public:
    /**
    * Class Listener
    *
    * For bargein module play beep when Audio in device start
    */
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void OnAudioInStartedAction() {}
    };

    VR_AudioInSource();
    virtual ~VR_AudioInSource();

    // Inherited from N_Vr::N_Asr::C_Audio_In
    virtual void M_Fn_Get_Data_Type_Audio_SSE(void ** ppvoid_Data_Type_Audio_SSE, size_t * pi_Size_Data_Type_Audio_SSE);
    virtual int M_Fn_Start();
    virtual int M_Fn_Stop();
    virtual int M_Fn_Data_Available_Check(bool * pb_Data_Available);
    virtual int M_Fn_Data_Fetch(void ** ppvoid_Buffer, size_t * pi_Size);
    virtual int M_Fn_Data_Return(void * const & rpvoid_Buffer);

    // Inherited from VC_AudioIn::Listener
    virtual VOID OnAudioInStarted();
    virtual VOID OnAudioInData(VOID * pData, INT len);
    virtual VOID OnAudioInCustom(int type, VOID* data, INT len);
    virtual VOID OnAudioInStopped();

    // for DE
    virtual int Prepare();
    virtual int UnPrepare();
    virtual void setAudioMode(int mode, Listener* pListener = NULL);

    virtual void Clear();
    virtual void IgnoreAudioInByTime(int milliseconds);

private:
    int CreateOutFolder();

private:
    // Disable copy & assign constructors
    VR_AudioInSource(VR_AudioInSource const &);
    VR_AudioInSource & operator = (VR_AudioInSource const &);

    enum
    {
        BUF_COUNT = 4,
        BUF_SIZE = 4096,
        RATE = 16000,
    };

    int  m_nAudioMode;
    int m_Offset;

    std::atomic<bool> m_bStarted;
    std::atomic<bool> m_hasMoreData;
    VoiceList<std::pair<BYTE*, size_t> >::type m_buffers;
    VC_AudioIn* m_pAudioIn;
    boost::shared_ptr<VC_WavFileWriter> m_spWavWriter;
    boost::shared_ptr<VC_WavFileWriter> m_spWavFetchWriter;
    CL_WaitObj m_waitObj;
    CL_SyncObj m_syncObj;
    int m_nFileNum;
    bool m_bOutFolderCreated;
    bool m_bInputWavOption;
    std::string m_recordFilePath;

    char* m_pAecData;
    int m_nAecDataSize;
    // Only for performance test
    bool m_bFirstFrame;
    Listener* m_pListener;

    int m_iIgnoreLength;
};

#endif // _VR_AUDIOINSOURCE_H
/* EOF */
