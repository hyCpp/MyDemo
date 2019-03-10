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
 * @file  VR_DictationStatusCheck.h
 * @brief Declaration file of class VR_DictationStatusCheck.
 *
 * This file includes the declaration of class VR_DictationStatusCheck.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DICTATIONSTATUSCHECK_H
#define VR_DICTATIONSTATUSCHECK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EventSenderIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(EV_EventContext);

/**
 * @brief  The VR_DictationStatusCheck class
 *
 * class declaration
 */
class  VR_DictationStatusCheck
{
public:
    VR_DictationStatusCheck(spVR_EventSenderIF  eventSender);
    virtual  ~VR_DictationStatusCheck();

    virtual  void  OnReqestCommonProperty(const EV_EventContext& context, const VR_ProtoMessageBase& msg);

private:
    bool  GetDictationStatus() const;
    bool  GetRegionCheck()     const;
    bool  GetProductCheck()    const;
    bool  GetUnitCheck()       const;
    bool  GetDcmCheck()        const;

private:
    spVR_EventSenderIF      m_spVR_EventSender;
};

#endif // VR_DICTATIONSTATUSCHECK_H
/* EOF */
