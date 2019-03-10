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

/**
 * @file VR_WavFile.h
 * @brief inteface
 *
 *
 * @attention used for C++ only.
 */

#ifndef _VC_WAVFILEWRITER_H_
#define _VC_WAVFILEWRITER_H_

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <stdio.h>
#include <string>
#include <vector>
#include "VC_WAVEFORMATEX.h"

/**
 * Summary for class VC_WavFileWriter
 * 
 * This is to used to write file
 *
 */
class VC_API VC_WavFileWriter // used to write file
{
public:
    VC_WavFileWriter();
    ~VC_WavFileWriter();

/**
* 打开音频文件
*
* 根据传入的参数按照指定格式打开音频文件，否则返回错误
*
* @param [IN] filenamei  :音频文件的路径
* @param [IN] format : 要打开音频文件的fmt格式
* 
*
* @return true: 打开成功
* @retval false：打开音频文件失败
*
*/
    bool Open(const std::string &filename, VC_WAVEFORMATEX &format);

/**
* 构造默认的音频文件的fmt格式，打开音频文件
*
* 构造默认的音频文件的fmt格式，打开音频文件，否则返回错误
* 构造的fmt格式如下
* 1 PCM编码; 2 sample rate:16k; 3 单声道; 4 16bit;
*
* @param [IN] filenamei  :音频文件的路径
*
* @return true: 打开成功
* @retval false：打开音频文件失败
*
*/
    bool Open(const std::string &filename);

    int Write(void *pData, int iLen);
    bool Close();

private:
    VC_WavFileWriter(const VC_WavFileWriter& o);
    VC_WavFileWriter& operator= (const VC_WavFileWriter& o);

    bool WriteWaveHeader(VC_WAVEFORMATEX format);
    bool WriteWaveRiffInfo();
    bool WriteWaveFmtInfo(VC_WAVEFORMATEX format);
    bool WriteWaveDataInfo();
    bool WriteWaveLen();

private:
    FILE *m_pFile;
    long int m_lDataLenSub;
    long int m_lDataLen;
};

#endif // _VC_WAV_FILE_H_
/* EOF */
