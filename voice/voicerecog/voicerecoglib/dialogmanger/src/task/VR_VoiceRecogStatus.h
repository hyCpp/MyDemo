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
 * @file VR_VoiceRecogStatus.h
 * @brief Declaration file of class VR_VoiceRecogStatus.
 *
 * This file includes the declaration of class VR_VoiceRecogStatus.
 *
 * @attention used for C++ only.
 */
#ifndef VR_VOICERECOGSTATUS_H
#define VR_VOICERECOGSTATUS_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"
#include "VR_DialogEngineIF.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);

/**
 * @brief The VR_VoiceRecogStatus class
 *
 * class declaration
 */
class VR_VoiceRecogStatus
{
public:
    VR_VoiceRecogStatus(spVR_DialogEngineIF spDialogEngine);
    virtual  ~VR_VoiceRecogStatus();

    virtual  void     Start();
    virtual  void     Stop();
    virtual  void     OnRecevieMessage(const VR_ProtoMessageBase& msg);
    void     SetStatus(bool bStatus);
    bool     GetStatus()      const;

private:
    bool     m_bStatus;
    spVR_DialogEngineIF m_spDialogEngine;
};

#endif // VR_VOICERECOGSTATUS_H
/* EOF */
