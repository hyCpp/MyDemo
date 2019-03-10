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
 * @file VR_UIStartingCondition.h
 * @brief Declaration file of class VR_UIStartingCondition.
 *
 * This file includes the declaration of class VR_UIStartingCondition.
 *
 * @attention used for C++ only.
 */

#ifndef VR_UISTARTINGCONDITION_H
#define VR_UISTARTINGCONDITION_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EventSenderIF);

enum Update_Type
{
    startsearch_func           =      0,
    startdictionary_data       =      1,
    startlanguage_data         =      2,
    startcontact_data          =      3,
    startmediavocs             =      4,
    startpvrflag               =      5
};

class EV_EventContext;

/**
 * @brief The VR_UIStartingCondition class
 *
 * class declaration
 */
class VR_UIStartingCondition
{
public:
    VR_UIStartingCondition(spVR_DialogEngineIF spEngine, spVR_EventSenderIF spEventSender);
    virtual ~VR_UIStartingCondition();

    void OnReqestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    void OnMessage(const VR_ProtoMessageBase& msg);
    void NotifyAllData();
    void InitializeData();
    void TransforLanguage();

protected:
    void UpdateData(Update_Type type);
    void NotifySearchFunc();
    void NotifyDictionaryData();
    void NotifyLanguageData();
    void NotifyContactData();
    void NotifyMedaiVOCSData();
    void NotifyPVRFlagData();
    void NotifyPhoneState();
    void NoitfyUnitState();

private:
    spVR_DialogEngineIF         m_spEngineIF;
    spVR_EventSenderIF          m_spEventSenderIF;

    bool                        m_bSearchFunc;
    bool                        m_bDictionaryData;
    bool                        m_bLanguageData;
    bool                        m_bContactData;
    bool                        m_bMediaVOCSData;
    bool                        m_bPVRFlagData;
};

#endif /* VR_UISTARTINGCONDITION_H */
/* EOF */
