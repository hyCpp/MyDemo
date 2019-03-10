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
#include "VC_IAudioInFile.h"
#include "VC_AudioSession.h"
#include "VC_Factory.h"
#include "CL_WaitObj.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <iomanip>

#ifdef HAVE_NUTSHELL_OS
#define VC_INPUT_LIST_PATH_USB "/udisk/VoiceRecogInput/vr_rec_sequences.txt"
#define VC_INPUT_DIR_PATH_USB "/udisk/VoiceRecogInput/"
#else
#define VC_INPUT_LIST_PATH_USB "/tmp/vr_record/vr_rec_sequences.txt"
#define VC_INPUT_DIR_PATH_USB "/tmp/vr_record/"
#endif

#define VC_INPUT_LIST_PATH_LOCAL "/tmp/vr_rec_sequences.txt"
#define VC_INPUT_DIR_PATH_LOCAL "/tmp/vr_rec_input/"

#define VC_AUDIOIN_BUF_LEN 1280
using namespace std;

static std::string& trim(std::string &s)
{
    if (s.empty()) {
        return s;
    }

    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

void VC_IAudioInFile::VC_IAudioInCheckFilePath()
{
    DIR *dir = opendir(strRecListDir.c_str());
    if (dir != NULL) {
        VC_LOGD("USB mode start TestInput");
        if (closedir(dir) != 0) {
            VC_LOGE("Unable to close directory.");
        }
    }
    else {
        strRecListPath = VC_INPUT_LIST_PATH_LOCAL;
        strRecListDir = VC_INPUT_DIR_PATH_LOCAL;
        VC_LOGD("Loacl mode start TestInput");
    }
}

VC_IAudioInFile::VC_IAudioInFile()
: index(0)
, strRecListPath(VC_INPUT_LIST_PATH_USB)
, strRecListDir(VC_INPUT_DIR_PATH_USB)
, fileData(NULL)
, fileSize(0)
, m_isStart(false)
, m_eBuffer(NULL)
{
    VC_LOGD_FUNC();
    VC_IAudioInCheckFilePath();
    reload();
    m_eBuffer = new(MEM_Voice) char[VC_AUDIOIN_BUF_LEN];
    if (m_eBuffer != NULL) {
        memset(m_eBuffer, 0x01, VC_AUDIOIN_BUF_LEN);
    }
    else {
        VC_LOGE("out of memory limit");
    }
}

VC_IAudioInFile::~VC_IAudioInFile()
{
    VC_LOGD_FUNC();
    wavFile.Close();
    if (fileData != NULL) {
        delete[] fileData;
        fileData = NULL;
    }

    fileSize = 0;

    if (m_eBuffer != NULL) {
        delete[] m_eBuffer;
        m_eBuffer = NULL;
    }
}

bool VC_IAudioInFile::Open(int sampleRate, int mode) 
{
    VC_LOGD_FUNC();
    if (sequences.size() == 0) {
        RETURN(false);
    }

    if (sequences.size() <= index) {
        index = 0;
    }

    m_isStart = true;
    m_sampleRate = sampleRate;

    return wavFile.Open(sequences[index++], sampleRate, false);
}

bool VC_IAudioInFile::GetParam(int type, void **buf, int* size)
{
    VC_LOGD_FUNC();
    if (buf != NULL && size != NULL) {
        *buf = fileData;
        *size = fileSize;
        return (fileSize > 0);
    }
    else {
        return false;
    }
}

bool VC_IAudioInFile::reload()
{
    VC_LOGD_FUNC();

    // load records list from plain text
    string line;
    ifstream ifile(strRecListPath.c_str());
    VC_LOGD("reload listpath : %s", strRecListPath.c_str());
    if (ifile.is_open()) {
        sequences.clear();
        while (getline(ifile, line)) {
            sequences.push_back(trim(line));
        }
        ifile.close();

        index = 0;

        if (sequences.size() > 0) {
            return true;
        }
    }
    else {
        VC_LOGD("reload listpath fail");
    }

    // load records list from directory
    DIR *dir = opendir(strRecListDir.c_str());
    VC_LOGD("reload listDir : %s", strRecListDir.c_str());
    if (dir != NULL) {
        sequences.clear();

        int fileId = 1;
        while (0 < fileId && fileId < 1000) {
            std::ostringstream oss;
            oss << strRecListDir << "v";
            oss << setfill('0') << setw(3) << fileId;
            oss << ".wav";
            std::string filePath = oss.str();
            VC_LOGD("reload file name is : %s", filePath.c_str());
            FILE * pFile = NULL;
            pFile = fopen(filePath.c_str(), "r");
            if (pFile != NULL) {
                sequences.push_back(filePath);
                fclose(pFile);
                VC_LOGD("reload record : %s", filePath.c_str());
            }
            else {
                VC_LOGD("reload %s fail", filePath.c_str());
                oss.str("");
                break;
            }
            fileId++;
            oss.str("");
        }

        if (closedir(dir) != 0) {
            VC_LOGE("Unable to close directory.");
        }

        index = 0;
    }

    return (sequences.size() > 0);
}

int VC_IAudioInFile::Read(void *buf, int size)
{
    VC_LOGD_FUNC();
    if (m_isStart) {
        int readSize = wavFile.Read(buf, size);
        int sampleSize = 2;
        if (-1 != readSize && 0 != readSize && m_sampleRate != 0 && sampleSize != 0) {
            float waitTimeTmp = (1.0 / m_sampleRate) * (readSize * 1.0 / sampleSize);
            int waitTime = int(waitTimeTmp * 1000);
            Wait(waitTime);
        }

        if (readSize == -1) {
            Wait(100);
            if (size < VC_AUDIOIN_BUF_LEN) {
                memcpy(buf, (void *)m_eBuffer, size);
            }
            else {
                memcpy(buf, (void *)m_eBuffer, VC_AUDIOIN_BUF_LEN);
                size = VC_AUDIOIN_BUF_LEN;
            }
            readSize = size;
        }
        return readSize;
    }
    else {
        return -1;
    }
}

bool VC_IAudioInFile::Stop()
{
    VC_LOGD_FUNC();
    m_isStart = false;
    return true;
}

bool VC_IAudioInFile::Close()
{
    VC_LOGD_FUNC();
    wavFile.Close();
    return true;
}
/* EOF */

