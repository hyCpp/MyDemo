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
 * @file VR_StartServiceCondition.h
 * @brief Declaration file of class VR_StartServiceCondition.
 *
 * This file includes the declaration of class VR_StartServiceCondition.
 *
 * @attention used for C++ only.
 */

#ifndef VR_STARTSERVICECONDITION_H
#define VR_STARTSERVICECONDITION_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif


// define start condition error code
enum Start_Condition_Error
{
    Start_SearchFun_Error = 0,
    Start_DictionData_Error,
    Start_LangData_Error,
    Start_ContactData_Error,
    Start_MediaVOCS_Error,
    Start_MicroPhone_Error,
    Start_PTTSW_Error,
    Start_HigherInterput_Error,
    Start_Condition_Ok,
    Start_Condition_Unkown,
};

enum Condition_Data_Type
{
    Search_Func = 0,
    Dic_Data,
    Lang_Data,
    ContactData,
    MediaVOCS,
    Micphone,
    Pttsw,
    Higher_Interupt,
    Dic_Lang_Data,
    Conditon_Unkown,
};

enum Start_Region_Type
{
    North_America_Region,
    Europen_Region,
    General_Region,
};

enum Start_Product_Type
{
    Product_T0,
    Product_T1,
    Product_T2,
    Product_TEMV,
    Product_L1,
    Product_L1_5,
    Product_L2,
};

class EV_EventContext;
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EventSenderIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(EV_EventContext);

/**
 * @brief The VR_StartCondition class
 *
 * class declaration
 */
class VR_StartCondition
{
public:
    // we always set the data false, initdata when dm start
    // but in test, the value may set avaliable
    VR_StartCondition() : m_bSearchFunc(true), m_bDictionData(true),
        m_bLangData(true),
        m_bContactData(true),
        m_bMediaVOCSData(true),
        m_bMicrophone(true),
        m_bPTTSW(true),
        m_bHigherInterrupt(true)
    {
    }

    Start_Condition_Error CheckConditionData() const;

    // these data use for pulic
    bool  m_bSearchFunc;
    bool  m_bDictionData;
    bool  m_bLangData;
    bool  m_bContactData;
    bool  m_bMediaVOCSData;
    bool  m_bMicrophone;
    bool  m_bPTTSW;
    bool  m_bHigherInterrupt;
};

/**
 * @brief The VR_StartServiceCondition class
 *
 * class declaration
 */
class VR_StartServiceCondition
{
public:
    // this function diff region , product , ,
    VR_StartServiceCondition(spVR_DialogEngineIF spEngineIF, spVR_EventSenderIF spSenderIF);
    virtual ~VR_StartServiceCondition();

    void OnReqestCommonProperty(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void OnReplyCommonProperty();
    void OnMessage(const VR_ProtoMessageBase& msg);
    void Initialize();

private:
    Start_Condition_Error CheckStartData();
    void  SetInitState();
    void  SetServiceData(int nValue, Condition_Data_Type dataType);
    void  InitializeData();

    bool  CheckPttSW();
    bool  CheckMircophone();
    bool  CheckSearchFunc();
    void  CheckLangDicData();
    bool  CheckMediaVOCS();
    bool  CheckContactData();
    bool  CheckHigerInterrupt();

private:
    spVR_DialogEngineIF    m_spEngineIF;
    spVR_EventSenderIF     m_spEventIF;
    spEV_EventContext      m_spEventContext;
    VR_StartCondition      m_vrstart_condition;
    Start_Condition_Error  m_errorCode;
    Start_Region_Type      m_RegionType;
    Start_Product_Type     m_ProductType;
};

#endif /* VR_STARTSERVICECONDITION_H */
/* EOF */
