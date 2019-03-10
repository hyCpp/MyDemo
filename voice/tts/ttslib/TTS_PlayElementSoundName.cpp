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
#include "TTS_PlayThread.h"
#include "VC_CommDef.h"
#include "TTS_EngineManager.h"
#include "TTS_PlayElementSoundName.h"
#include "TTS_WaveFile.h"

#include <fstream>


TTS_PlayElementSoundName::TTS_PlayElementSoundName()
:	m_bValidfile(true)
{
}

TTS_PlayElementSoundName::~TTS_PlayElementSoundName()
{
}

CL_BOOL TTS_PlayElementSoundName::DoPlay(TTS_EngineBuffers* buffer)
{
    TTS_LOGD_FUNC();
    if (!buffer) {
        TTS_LOGE("audioOut is NULL");
        return CL_FALSE;
    }

    if (!m_bValidfile) {
        TTS_ERROR_MSG(VOICE_TTS_ERR_WAVE_FILE, 0, "invalid file");
        return false;
    }

    int datalen = m_waveFile.GetDataLen();

    int pos = 0;

    while (pos < datalen) {
        int len = 0;
        BYTE *buf =  buffer->GetWriteBuffer(len);
        if (NULL == buf || len <= 0) {
            TTS_LOGI("can not get write buffer");
            return false;
        }

        if (datalen - pos < len) {
            len = datalen - pos;
        }

        if (!m_waveFile.ReadData(reinterpret_cast<char*>(buf), pos, len)) {
            TTS_LOGE("can not read file!");
            return false;
        }
        
        buffer->SetWriteLen(len);

        pos += len;
    }

    return true;
}
/* EOF */
