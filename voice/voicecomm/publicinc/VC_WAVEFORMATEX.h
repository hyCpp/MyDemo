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
 * @file VC_WAVEFORMATEX
 * @brief inteface
 *
 *
 * @attention used for C++ only.
 */

#ifndef _VC_WAVEFORMATEX_H_
#define _VC_WAVEFORMATEX_H_

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif
#pragma once

struct VC_WAVEFORMATEX 
{
    char fId[4];                // 'fmt'
    uint32_t pcm_header_len;    // 长度
    uint16_t wFormatTag;        // 编码格式
    uint16_t nChannels;         // 声道数目
    uint32_t nSamplesPerSec;    // 采样频率
    uint32_t nAvgBytesPerSec;   // 每秒所需的字节数
    uint16_t nBlockAlign;       // 每个采样所需的字节数
    uint16_t nBitsPerSample;    // 每个采样所需的bit数
    uint16_t nAddByte;          // 附加的两个字节
};

#endif // _VC_WAVEFORMATEX_H_
/* EOF */
