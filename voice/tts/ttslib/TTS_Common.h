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
 * @file TTS_Common.h 
 * @brief Declaration file of class TTS_Common.                  
 *
 * This file includes the declaration of class TTS_Common and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */
#pragma once

#ifndef _CXX_TTS_COMMON_H_
#define _CXX_TTS_COMMON_H_

// thread ID define
enum TTS_THREAD_ID
{
    TTS_INVALIDTHREAD_ID = 100,
    TTS_PLAYLISTMANAGERTHREAD_ID,
    TTS_PLAYMANAGERTHREAD_ID,
};

#endif /* _CXX_TTS_COMMON_H_ */
/* EOF */
