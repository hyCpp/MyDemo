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

#include "TTS_DurationCalculater.h"
#include <fstream>
#include <dlfcn.h>
#include "TTS_DurationDefine.h"
#include "BL_SystemLanguage_DEF.h"

#define TTS_ENGINE_CONFIG_PATH "/mnt/sdcard2/.kanaviid/NDATA/Sounds/TTS" 	// nuance data path

using namespace std;

TTS_DurationCalculater::TTS_DurationCalculater()
: m_dataLen(0)
, m_textLen(0)
, m_pitch(50)
, m_speed(100)
, m_volume(80)
{
    m_buffer = new(MEM_Voice) unsigned char[TTS_BUFFER_LEN];
}

TTS_DurationCalculater::~TTS_DurationCalculater()
{
    delete[] m_buffer;
}

#if 0
bool TTS_DurationCalculater::SpeechBuffer(BYTE** pBuffer, DWORD* dwSize)
{
    *pBuffer = m_buffer;
    *dwSize = TTS_BUFFER_LEN;
    return true;
}

void TTS_DurationCalculater::SpeechReady(BYTE* pData, DWORD dwLen)
{
    m_dataLen += dwLen;
}

#endif

int TTS_DurationCalculater::CaculateDuration(int lang, const char* fileName)
{
    int duration = 0;
    ifstream fileStream;
    fileStream.open(fileName, ios_base::in);
    if (!fileStream.is_open()) {
        T_Debug("file open failed, file=%s", fileName);
        return 0;
    }

    void *handler = dlopen("/usr/lib/libttsenginenuance - navi.so", RTLD_LAZY);
    if (!handler) {
        T_Debug("load library failed!");
        return 0;
    }

    GetInstance_t engineInstance = (GetInstance_t)(dlsym(handler, "GetInstance"));
    if (!engineInstance) {
        T_Debug("dlsym GetInstance failed");
        return 0;
    }

    TTS_Engine *engineInst = engineInstance();
    if (!engineInst) {
        T_Debug("new engine failed");
        return 0;
    }

    /* string isoLangType;
    if (!GetTTSLangType(lang, isoLangType)) {
        T_Debug("GetTTSLangType failed...lang=%d", lang);
        return 0;
    } */
    
    T_Debug("init..................");
    if (0 != engineInst->Init(TTS_ENGINE_CONFIG_PATH, BL_LANGUAGE_TYPE_INDONESIAN)) {
        T_Debug("Initialize engine failed");
        return 0;
    }

    DoCalculate(engineInst, fileStream);

    dlclose(handler);
    return duration;
}

#if 0
bool TTS_DurationCalculater::GetTTSLangType(int lang, string &isoLangType)
{
    bool ret = true;
    switch (lang) {
    case ENG:
        isoLangType = "ENG";
        break;
    case MNC:
        isoLangType = "MNC";
        break;
    case THT:
        isoLangType = "THT";
        break;
    case SPE:
        isoLangType = "SPE";
        break;
    case PTP:
        isoLangType = "PTP";
        break;
    case ARW:
        isoLangType = "ARW";
        break;
    case IDI:
        isoLangType = "IDI";
        break;
    default:
        ret = false;
        break;
    }

    return ret;
}

#endif

void TTS_DurationCalculater::DoCalculate(TTS_Engine *engine, ifstream &file)
{
#define TEXT_BUF_SIZE 16 * 1024
    
    engine->SetPitch(m_pitch);
    engine->SetVolume(m_volume);
    engine->SetSpeed(m_speed);

    char *textBuf = new(MEM_Voice) char[TEXT_BUF_SIZE];
    string line;
    int count = 0;
    while (!file.eof()) {
        if (!file.getline(textBuf, TEXT_BUF_SIZE)) {
            T_Debug("getline error...");
            break;
        }
        T_Debug("line=%s", textBuf);
        ++count;
        m_dataLen = 0;
        if (!engine->SynthesizeText(textBuf, this)) {
            T_Debug("SynthesizeText failed...");
            return;
        }

        line = textBuf;
        m_textLen = line.length();
        double textDur = m_dataLen * 1000 / (22050*2); // ms
        double coff = m_textLen * 1000 / textDur;
        m_durs.push_back(coff);
        T_Debug("coff=%f, textLen=%d, textDur=%f", coff, m_textLen, textDur);
    }

    double coefficient = 0.0;
    VoiceVector<double>::iterator it = m_durs.begin();
    for (; it != m_durs.end(); ++it) {
        T_Debug("current coefficient=%f", *it);
        coefficient += *it;
    }

    m_durs.clear();

    coefficient /= count;
    
    T_Debug("coefficient=%f, count=%d", coefficient, count);
    delete[] textBuf;

}
/* EOF */
