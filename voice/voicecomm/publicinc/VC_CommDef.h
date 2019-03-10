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
 * @file VC_CommDef.h                                            
 * @brief Summary for VC_AudioBuffer.
 *
 * This file includes the declaration of class VC_AudioBuffer and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_COMMDEF_H
#define VC_COMMDEF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once
#include "VC_CommonDef.h"

// enum { VC_DECODER_OGG = 0, VC_DECODER_TTS, VC_DECODER_NUM };

/**
 * Summary for class VC_AudioBuffer
 * 
 * This is a detail description for class VC_AudioBuffer
 *
 */
class VC_AudioBuffer 
{
public:
    virtual  ~VC_AudioBuffer() {}
    virtual VOID* addr() = 0;
    virtual INT   size() = 0;
    virtual VOID  setLen(INT len) = 0;
};


#endif /* VC_COMMDEF_H */
/* EOF */