/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
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

#include "stdafx.h"
#include "VC_PlayerGeneral.h"

#ifndef CXX_CL_AUTOSYNC_H
#	include "CL_AutoSync.h"
#endif

#include "BL_FullFile.h"

#ifndef CXX_APLTHREADNAME_H
#	include "AplThreadName.h"
#endif

#include "VC_Factory.h"
#include "VC_CommonDef.h"
#include "VC_AudioOut.h"
#include "VC_DecEventOgg.h"
#include "VC_AudioOutGeneral.h"

static const INT VC_RETRY_MAX = 100;
static const INT VC_WAIT_INTERVAL = 10;    // in millseconds
static const int VC_BUFFER_SIZE_DEFAULT = 1408;		// 1408 is the max data size one time of keda tts engine
static const int VC_BUFFER_COUNT_DEFAULT = 16;		// buffer count default is 16

#define CHECK_DECODER(pluginId) { do { \
    if (NULL == m_pDecoders[(pluginId)]) { \
        VC_LOGE("Decoder plugin %d is null\n", (pluginId)); \
        return CL_FALSE; \
    } } while (0); }

VC_PlayerGeneral::VC_PlayerGeneral(VC_AUDIO_TYPE audioType)
    :m_audioType(audioType)
    , m_state(ST_NONE)
    , m_pAudioOut(NULL)
    , m_dwBufCount(0)
    , m_pDecoder(NULL)
    , m_pListener(NULL)
    , m_pEvent(NULL)
    , m_eventType(-1)
    , m_dwIndexR(0)
    , m_dwIndexW(0)
    , m_bStop(CL_FALSE)
    , m_bStopNormal(CL_FALSE)
    , m_iFragCount(0)
    , m_tokenId(0)
    , m_dwDecodeLen(0)
    , m_dwPlayLen(0)
    , m_bLastPhrase(false)
{
    for (INT i = 0; i<VC_DECODER_NUM; ++i) {
        m_pDecoders[i] = NULL;
    }

    for (INT i = 0; i < VC_AUDIO_BUF_COUNT; ++i) {
        m_buffers[i] = NULL;
    }
}

VC_PlayerGeneral::~VC_PlayerGeneral()
{
    for (INT i = 0; i<VC_DECODER_NUM; ++i) {
        delete m_pDecoders[i];
    }

    InitPlayer();
}

CL_ERROR VC_PlayerGeneral::Init()
{
    CL_BOOL needDec = CL_FALSE;
    for (INT i = 0; i<VC_DECODER_NUM; ++i) {
        m_pDecoders[i] = VC_Factory::Instance()->CreateDecoder(m_audioType, static_cast<VC_DECODER_TYPE>(i));
        if (NULL != m_pDecoders[i]) {
            CL_ERROR err = m_pDecoders[i]->Init(NULL);
            if (APL_ERR_NO_ERROR != err) {
                VC_LOGE("Initialize decoder error, error code=%d, decoder index=%d", err, i);
            }
            needDec = CL_TRUE;
        }
    }

    m_pAudioOut = VC_Factory::Instance()->CreateAudioOut(m_audioType);
    if (!m_pAudioOut) {
        VC_LOGE("VC_PlayerGeneral::Init(), m_pAudioOut new(MEM_Voice) failed!");
    }

    if (needDec) {
        // Start decoder thread
        RegisterName(TTS_SYNTHE_THREAD);
        StartRegistThread();
    }
    return APL_ERR_NO_ERROR;
}

CL_BOOL VC_PlayerGeneral::Play(const BL_String& fileName, LONG lOffset, 
        LONG lSize, VC_Player::Listener* pListener, int tokenid, bool last_phrase)
{
    VC_LOGI("Play filename: [%s], Offset: %d, size: %d", fileName.GetString(), lOffset, lSize);
    BL_String filePath;
    /* BL_FullFile dataFile;
    if (!dataFile.MakePhysicalFullPath(fileName, filePath)) {
        VC_LOGE("build full path failed with file name, file=%s\n", fileName.GetString());
        return CL_FALSE;
    } */

    CHECK_DECODER(VC_DECODER_OGG);
    m_pDecoder = m_pDecoders[VC_DECODER_OGG];
    m_pDecoder->SetSampleRate(fileName, lOffset);
    m_pEvent = new(MEM_Voice) VC_DecEventOgg(fileName, lOffset, lSize, m_pDecoder, this);
    m_eventType = -1;
    m_bLastPhrase = last_phrase;
    if (!m_pEvent) {
        VC_LOGE("m_pEvent new(MEM_Voice) failed!");
        return CL_FALSE;
    }

    m_pListener = pListener;
    m_state = ST_PENDING;
    m_tokenId = tokenid;

    Notify();
    return CL_TRUE;
}

CL_BOOL VC_PlayerGeneral::IsPCM(const VOID *buf)
{
    return (!memcmp("RIFF", static_cast<const char*>(buf), 4));
}

static size_t parseWaveDataPos(const char* pbuf)
{
    const char* p = pbuf + 12;
    while (1) {
        if (!memcmp(p, "fmt ", 4)) {
            p += (8 + *(reinterpret_cast<const uint32_t*>(p + 4)));
        }
        else if (!memcmp(p, "fact", 4)) {
            p += (8 + *(reinterpret_cast<const uint32_t*>(p + 4)));
        }
        else if (!memcmp(p, "data", 4)) {
            return (p + 8 - pbuf);
        }
        else {
            return 0;
        }
    }
}

CL_ERROR VC_PlayerGeneral::DoDecodePcm(const BYTE *dataBuf, LONG leftSize, int tokenid)
{
    DWORD dwBufferSize = 0;
    DWORD dwReadSize = 0;
    DWORD dwHeadSize = parseWaveDataPos(reinterpret_cast<const char*>(dataBuf));
    DWORD dwLeftSize = leftSize - dwHeadSize;
    VC_AudioBuffer* buf;

    InitPlayer();

    // open audio out
    // INT iSampleRate = m_pDecoder->GetSampleRate();
    INT iSampleRate;
    memcpy(&iSampleRate, (dataBuf + 24), 4);
    VC_LOGD("beep sampleRate=%d, head[%d], left[%d]", iSampleRate, dwHeadSize, dwLeftSize);
    INT iFragSize = 0;
    CL_BOOL rc = m_pAudioOut->Open(&iSampleRate, &iFragSize, &m_iFragCount, tokenid);
    if (!rc || 0 >= iFragSize) {
        VC_LOGE("audio out open error!");
        m_pListener->OnPlayComplete((0 == dwLeftSize));
        return APL_ERR_NO_ERROR;
    }

    iFragSize = MAX(VC_BUFFER_SIZE_DEFAULT, iFragSize);
    m_iFragCount = MIN(VC_BUFFER_COUNT_DEFAULT, m_iFragCount);
    for (int i = 0; i < m_iFragCount + 4; ++i) {
        VC_AudioBuffer *pBuf = m_pAudioOut->AllocBuffer(iFragSize);
        if (!pBuf) {
            VC_LOGE("DoPlay(), AllocBuffer error!");
            if (0 != m_dwBufCount) {
                break;
            }
            else {
                m_pListener->OnPlayComplete((0 == dwLeftSize));
                return APL_ERR_NO_ERROR;
            }
        }

        m_buffers[i] = pBuf;
        m_dwBufCount = i + 1;
    }

    m_state = ST_PLAYING;
    const BYTE* pPcmBuf = dataBuf + dwHeadSize;
    do {
        dwReadSize = 0;

        buf = OnDecBuffer();
        if (!buf) {
            VC_LOGE("allocate buffer error\n");
            break;
        }

        BYTE* pBuffer = reinterpret_cast<BYTE*>(buf->addr());
        dwBufferSize = buf->size();
        dwReadSize = MIN(dwBufferSize, dwLeftSize);
        memcpy(pBuffer, pPcmBuf, dwReadSize);
        pPcmBuf += dwReadSize;
        dwLeftSize -= dwReadSize;
        buf->setLen(dwReadSize);

        OnDecReady(buf);
    } while ((0 < dwLeftSize) && (CL_FALSE == m_bStop));

    if (m_dwIndexW < m_iFragCount) {
        Flush();
    }

    if (!m_bStop) {	// normal stop
        if (m_pAudioOut->Stop(CL_FALSE, m_bLastPhrase)) {
            m_state = ST_WAITING;
            while (!m_bStop && !m_bStopNormal) {
                VC_LOGD("wait for play over ...");
                Wait();
                VC_LOGD("wait for play over end");
            }
            if (m_bStop) {
                m_pAudioOut->Stop(CL_TRUE);
            }
        }
    }
    else {	// force stop
        VC_LOGD("Force stop start");
        m_pAudioOut->Stop(CL_TRUE);
    }

    VC_LOGI("end, stop=%d", m_bStop);
    m_pAudioOut->Close();
    m_pListener->OnPlayComplete((0 == dwLeftSize));

    m_state = ST_NONE;
    return APL_ERR_NO_ERROR;
}

CL_BOOL VC_PlayerGeneral::Play(const VOID* pBuf, LONG lLen, VC_Player::Listener* pListener,
        int tokenid, bool last_phrase)
{
    VC_LOGI("Play Buffer pBuf=%p, len=%d", pBuf, lLen);
    if (NULL == m_pDecoders[VC_DECODER_OGG]) {
        VC_LOGE("Decoder plugin OGG is null\n");
        return CL_FALSE;
    }

    CHECK_DECODER(VC_DECODER_OGG);
    m_pDecoder = m_pDecoders[VC_DECODER_OGG];
    m_pEvent = new(MEM_Voice) VC_DecEventOgg(pBuf, lLen, m_pDecoder, this);
    m_eventType = 0;
    if (!m_pEvent) {
        VC_LOGE("m_pEvent new failed!");
        return CL_FALSE;
    }

    m_pListener = pListener;
    m_state = ST_PENDING;
    m_tokenId = tokenid;
    m_bLastPhrase = last_phrase;

    Notify();
    return CL_TRUE;
}

CL_BOOL VC_PlayerGeneral::Play(const BL_String& ttsText, DWORD dwLanguage, VC_Player::Listener* pListener,
        int tokenid, bool last_phrase)
{
    /* VC_LOGI("Play Tts, lang=%d", dwLanguage);
    if (ST_NONE != m_state) {
        VC_LOGE("invalid OP, It's busy, state=%d\n", m_state);
        return CL_FALSE;
    }

    CHECK_DECODER(VC_DECODER_TTS);
    m_pDecoder = m_pDecoders[VC_DECODER_TTS];
    m_pEvent = new(MEM_Voice) VC_DecEventTts(ttsText, dwLanguage, m_pDecoder, this);
    m_eventType = 1;	// 1 is tts event
    if (!m_pEvent) {
        VC_LOGE("m_pEvent new(MEM_Voice) failed!");
        return CL_FALSE;
    }

    m_pListener = pListener;
    m_state = ST_PENDING;
    Notify(); */
    return CL_TRUE;
}

CL_BOOL VC_PlayerGeneral::Stop()
{
    VC_LOGI("User Stop enter, state=%d", m_state);
    if (ST_NONE == m_state) { // Not error
        return CL_TRUE;
    }

    m_pDecoder->Stop();
    m_bStop = CL_TRUE;
    Notify();
    INT iCount = 0;
    while (ST_NONE != m_state && iCount < VC_RETRY_MAX) {
        VC_LOGD("Stop play, state=%d", m_state);
        m_waitStop.Wait(VC_WAIT_INTERVAL);
        ++iCount;
    }

    if (ST_NONE != m_state) {
        m_waitStop.Wait();
        VC_LOGD("user stop end");
    }

    VC_LOGD("Stop play end, state=%d", m_state);
    return CL_TRUE;
}

INT VC_PlayerGeneral::GetVolume() const
{
    return m_pAudioOut->GetVolume();
}

CL_ERROR VC_PlayerGeneral::SetVolume(INT vol)
{
    if (NULL == m_pAudioOut) {
        VC_LOGE("VC_PlayerGeneral::SetVolume, Audio out handler is null!");
        return APL_ERR_VC_INVALID_OP;
    }
    m_pAudioOut->SetVolume(vol);
    return APL_ERR_NO_ERROR;
}

void VC_PlayerGeneral::Run()
{
    while (true) {
        VC_LOGI("VC_PlayerGeneral thread waiting ...");
        Wait();
        VC_LOGI("VC_PlayerGeneral thread wake up");

        if (TRUE == CheckQuit()) {
            VC_LOGI("VC_PlayerGeneral: quit thread!");
            break;
        }

        if (NULL != m_pEvent) {
            DoPlay();
        }
    }
}

VOID VC_PlayerGeneral::DoPlay()
{
    VC_LOGD("Player DoPlay enter...");
    if (0 == m_eventType) {
        VC_DecEventOgg *pevt = static_cast<VC_DecEventOgg*>(m_pEvent);
        if (IsPCM(pevt->m_pBuf)) {	// beep file
            DoDecodePcm(static_cast<const BYTE*>(pevt->m_pBuf), pevt->m_dwLen, m_tokenId);
            delete m_pEvent;
            m_pEvent = NULL;
            return;
        }
    }

    if (!m_pAudioOut) {
        VC_LOGE("m_pAudioOut is NULL!");
        return;
    }

    // clear last audio out buffers and initialize all flags
    InitPlayer();

    // open audio out
    INT iSampleRate = m_pDecoder->GetSampleRate();
    VC_LOGD("Sample Rate is: [%d]\n", iSampleRate);
    INT iFragSize = 0;
    CL_BOOL rc = m_pAudioOut->Open(&iSampleRate, &iFragSize, &m_iFragCount, m_tokenId);
    if (!rc || 0 >= iFragSize) {
        VC_LOGE("audio out open error!");
        goto EXIT;
    }

    VC_LOGD("sampleRate = %d, iFragSize=%d, m_iFragCount=%d", iSampleRate, iFragSize, m_iFragCount);

    iFragSize = MAX(VC_BUFFER_SIZE_DEFAULT, iFragSize);
    m_iFragCount = MIN(VC_BUFFER_COUNT_DEFAULT, m_iFragCount);
    for (int i = 0; i < m_iFragCount + 4; ++i) {
        VC_AudioBuffer *pBuf = m_pAudioOut->AllocBuffer(iFragSize);
        if (!pBuf) {
            VC_LOGE("DoPlay(), AllocBuffer error!");
            if (0 != m_dwBufCount) {
                break;
            }
            else {
                goto EXIT;
            }
        }

        m_buffers[i] = pBuf;
        m_dwBufCount = i + 1;
    }

    m_state = ST_PLAYING;
    CL_ERROR err;
    err = m_pEvent->DoAction();
    if (m_dwIndexW < m_iFragCount) {
        Flush();
    }

    VC_LOGD("PlayerGeneral decode over!");
    if (!m_bStop) {	// normal stop
        if (m_pAudioOut->Stop(CL_FALSE, m_bLastPhrase)) {
            m_state = ST_WAITING;
            while (!m_bStop && !m_bStopNormal) {
                VC_LOGD("wait for play over ...");
                Wait();
                VC_LOGD("wait for play over end");
            }
            if (m_bStop) {
                m_pAudioOut->Stop(CL_TRUE);
            }
        }
        VC_LOGD("Normal stop end");
    }
    else {	// force stop
        VC_LOGD("Force stop start");
        m_pAudioOut->Stop(CL_TRUE);
        VC_LOGD("Force stop end");
    }

EXIT:
    m_pAudioOut->Close();
    VC_LOGD("Play length:%d", m_dwPlayLen);

    delete m_pEvent;
    m_pEvent = NULL;

    VC_Player::Listener* pListener = m_pListener;
    m_pListener = NULL;
    m_state = ST_NONE;
    m_waitStop.Notify();

    if (NULL != pListener) {
        pListener->OnPlayComplete(CL_TRUE);
    }
    VC_LOGD("Player DoPlay end");
}

// Inherited from VC_Decoder::Listener
VC_AudioBuffer* VC_PlayerGeneral::OnDecBuffer()
{
    VC_LOGD("Player indexR=%d, indexW=%d, bufCount=%d", m_dwIndexR, m_dwIndexW, m_dwBufCount);
       while (!m_bStop && m_dwIndexR + m_dwBufCount <= m_dwIndexW) {
        VC_LOGD("decode buffer wait ..., stop=%d", m_bStop);
        Wait();
        VC_LOGD("decode buffer wait end");
    }

    if (m_bStop) {
        VC_LOGD("m_bStop is true");
        RETURN_NULL;
    }

    if (0 == m_dwBufCount) {
        VC_LOGE("VC_PlayerGeneral::OnDecBuffer, m_dwBufCount is 0");
        RETURN_NULL;
    }
    memset(m_buffers[m_dwIndexW % m_dwBufCount]->addr(), 0, m_buffers[m_dwIndexW % m_dwBufCount]->size());
    return m_buffers[m_dwIndexW % m_dwBufCount];
}

VOID VC_PlayerGeneral::OnDecReady(VC_AudioBuffer *pBuf)
{
    ++m_dwIndexW;
    VC_LOGI("VC_PlayerGeneral DecReady, w=%d", m_dwIndexW);

    if (m_bStop) {
        VC_LOGD("m_stop is true");
        return;
    }

    VC_LOGD("m_dwIndexW=%d, m_iFragCount=%d", m_dwIndexW, m_iFragCount);
    m_dwDecodeLen += pBuf->size();
    if (m_dwIndexW > m_iFragCount) {
        m_pAudioOut->Write(pBuf);
    }
    else if (m_iFragCount == m_dwIndexW) {
        Flush();
    } 
    else {
    }
}

CL_BOOL VC_PlayerGeneral::Flush()
{
    VC_LOGD_FUNC();
    CL_BOOL rc = CL_TRUE;
    for (INT i = 0; i < m_dwIndexW; ++i) {
        VC_AudioBuffer *buf = m_buffers[i];
        if (!m_pAudioOut->Write(buf)) {
            rc = CL_FALSE;
        }
    }

    m_pAudioOut->Start(this);
    VC_LOGD("audio out start...");
    return rc;
}

VOID VC_PlayerGeneral::OnAudioOutDone(VC_AudioBuffer* pBuffer)
{
    ++m_dwIndexR;
    VC_LOGI("OnAudioOutDone, r=%d", m_dwIndexR);
    if (!pBuffer) {
        VC_LOGE("pBuffer is NULL");
    }
    else {
        m_dwPlayLen += pBuffer->size();
    }

    Notify();
}

VOID VC_PlayerGeneral::OnAudioOutComplete(CL_BOOL success)
{
    VC_LOGI("OnAudioOutComplete, success=%d", success);
    m_bStopNormal = CL_TRUE;
    Notify();
}

VOID VC_PlayerGeneral::InitPlayer()
{
    for (INT i = 0; i < VC_AUDIO_BUF_COUNT; ++i) {
        m_buffers[i] = NULL;
    }

    m_dwBufCount = 0;

    m_dwIndexR = 0;
    m_dwIndexW = 0;
    m_bStopNormal = CL_FALSE;
    m_bStop = CL_FALSE;
    m_iFragCount = 0;
    m_dwDecodeLen = 0;
    m_dwPlayLen = 0;
}

DWORD VC_PlayerGeneral::GetDuration(const BL_String& text)
{
    return 0;
}

void VC_PlayerGeneral::StopPlayer()
{
    VC_LOGI("StopPlayer");
    if (NULL != m_pAudioOut) {
        m_pAudioOut->StopOutThread();
    }
    StopThread();

}
/* EOF */
