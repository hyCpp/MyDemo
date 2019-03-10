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
 * @file TTS_DurationDefine.h 
 * @brief Declaration file of define.                  
 *
 * This file includes the declaration of
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#pragma once

#ifndef _CXX_TTS_DURATION_DEFINE_H_
#define _CXX_TTS_DURATION_DEFINE_H_

#define T_Debug(...) \
    do {					\
        printf("%s:%d: ", __PRETTY_FUNCTION__, __LINE__);	\
        printf(__VA_ARGS__); \
        printf("\n"); \
    }while (0)


// define languages
#define ENG 1
#define MNC 2
#define THT 3
#define SPE 4
#define IDI 5
#define PTP 6
#define ARW 7

#define IDI 8

#endif /* _CXX_TTS_DURATION_DEFINE_H_ */
/* EOF */
