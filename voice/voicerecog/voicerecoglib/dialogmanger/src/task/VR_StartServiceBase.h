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
 * @file  VR_StartServiceBase.h
 * @brief Declaration file of class VR_StartServiceBase.
 *
 * This file includes the declaration of class VR_StartServiceBase.
 *
 * @attention used for C++ only.
 */
#ifndef VR_STARTSERVICEBASE_H
#define VR_STARTSERVICEBASE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

namespace  navi
{
namespace  VoiceRecog
{
    class  VrUnitStatusResult;
    class  VrStartStatusResult;
}
}

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
    Start_ChinaLanguage_Error,
    Start_TslInstalling_Error,
    Start_PhoneIncoming_Error,
    Start_PhoneDialing_Error,
    Start_PersonData_Error,
    Start_SmartAgent_Error,
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
    China_Region,
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
    VR_StartCondition()
        : m_bSearchFunc(true)
        , m_bDictionData(true)
        , m_bLangData(true)
        , m_bContactData(true)
        , m_bMediaVOCSData(true)
        , m_bMicrophone(false)
        , m_bPTTSW(true)
        , m_bHigherInterrupt(true)
        , m_bChinaLanguage(true)
        , m_bTslInstalling(true)
        , m_bPhoneIncoming(true)
        , m_bPhoneDialing(true)
        , m_bPersonData(true)
        , m_bSmartAgent(true)
        , m_bInternetStatus(true)
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
    bool  m_bChinaLanguage;
    bool  m_bTslInstalling;
    bool  m_bPhoneIncoming;
    bool  m_bPhoneDialing;
    bool  m_bPersonData;
    bool  m_bSmartAgent;
    bool  m_bInternetStatus;
};

/**
 *  @brief  The VR_CheckStartReason /////<<<<<<  for  avclan 4.03
 *
 *   class  declaration
 */
class  VR_CheckStartReason
{
public:
    bool    m_bInInitializing;
    bool    m_bMapUpdateError;
    bool    m_bUpdatingMapData;
    bool    m_bNavigationDemo;
    bool    m_bUpdatingDictionary;

public:
    VR_CheckStartReason()
        : m_bInInitializing(false)
        , m_bMapUpdateError(false)
        , m_bUpdatingMapData(false)
        , m_bNavigationDemo(false)
        , m_bUpdatingDictionary(false)
    {

    }

    void  SetCheckReason()
    {
        m_bInInitializing = false;
        m_bMapUpdateError = false;
        m_bUpdatingMapData = false;
        m_bNavigationDemo = false;
        m_bUpdatingDictionary = false;
    }
};

/**
 *  @brief  The  VR_CheckStartStatus   ///////<<<<<  for avclan 4.04
 *
 *
 *    class  VR_CheckStartStatus
 */
class   VR_CheckStartStatus
{
public:
    bool    m_bInInitializing;
    bool    m_bNavigationDemo;
    bool    m_bNavigationUpdating;
    bool    m_bDictionaryUpdating;
    bool    m_bEngineBusy;
    bool    m_bEngineTask;
    bool    m_bSessionUsing;
    bool    m_bInOtherStatus;
    bool    m_bStatusReserved;
    bool    m_bTtsCrashStatus;

public:
    VR_CheckStartStatus()
        : m_bInInitializing(true)
        , m_bNavigationDemo(false)
        , m_bNavigationUpdating(false)
        , m_bDictionaryUpdating(false)
        , m_bEngineBusy(true)
        , m_bEngineTask(false)
        , m_bSessionUsing(false)
        , m_bInOtherStatus(false)
        , m_bStatusReserved(false)
        , m_bTtsCrashStatus(true)
    {

    }

    void  ReSetStartStatus()
    {
        m_bInInitializing       =  false;
        m_bNavigationDemo       =  false;
        m_bNavigationUpdating   =  false;
        m_bDictionaryUpdating   =  false;
        m_bEngineBusy           =  false;
        m_bEngineTask           =  false;
        m_bSessionUsing         =  false;
        m_bInOtherStatus        =  false;
        m_bStatusReserved       =  false;
    }
};

/**
 * @brief The VR_StartServiceBase class
 *
 * class declaration
 */
class VR_StartServiceBase
{
public:
    VR_StartServiceBase();
    virtual ~VR_StartServiceBase();

    virtual void Initialize() = 0;
    virtual void NotifyTslState() = 0;
    virtual void OnReplyCommonProperty() = 0;
    virtual bool OnVoiceSettingAvailable() = 0;
    virtual void OnNotifyVoiceSettingAvailable() = 0;
    virtual void OnNotifyTslServiceAvailable() {}
    virtual void OnNotifyVRServiceIdleStatus() {}
    virtual void OnNotifyVoiceTagAvailable() {}
    virtual void OnReplyCommonPropertyWithHints(const VR_ProtoMessageBase& msg) = 0;

    virtual void OnMessage(const VR_ProtoMessageBase& msg) = 0;
    virtual void OnStartMessage(const VR_ProtoMessageBase& msg);
    virtual void OnReceiveMessage(const VR_ProtoMessageBase& msg) {}
    virtual void NotifyUICrash(const VR_ProtoMessageBase& msg) = 0;
    virtual void OnMessage(const navi::VoiceRecog::VrUnitStatusResult& statusResult) = 0;
    virtual void OnMessage(const navi::VoiceRecog::VrStartStatusResult& statusResult) = 0;

    virtual void RequestVrTslState(const EV_EventContext& context, const VR_ProtoMessageBase& msg) = 0;
    virtual void OnReqestCommonProperty(const EV_EventContext& context, const VR_ProtoMessageBase& msg) = 0;
    virtual void OnReqestVRStatusWithHints(const EV_EventContext& context, const VR_ProtoMessageBase& msg) = 0;
    virtual void OnReqestVRServiceIdleStatus(const EV_EventContext& context, const VR_ProtoMessageBase& msg) = 0;
    virtual void OnReqestVoiceTagStatus(const EV_EventContext& context, const VR_ProtoMessageBase& msg) = 0;

/********** Manual VR ****************/
    virtual void OnMMVRRequestVRServiceStatus(const EV_EventContext& context, const VR_ProtoMessageBase& msg) = 0;
/********** SDL VR *******************/
    virtual void OnSDLVRRequestVRServiceStatus(const EV_EventContext& context, const VR_ProtoMessageBase& msg) = 0;
    // amazon
    virtual bool CheckAwakenStatus() { return true;}

protected:
    void          SetInitState();
    virtual void  InitializeData();
    virtual bool  CheckPttSW();
    virtual bool  CheckMircophone();
    virtual bool  CheckSearchFunc();
    virtual void  CheckLangDicData();
    virtual bool  CheckMediaVOCS();
    virtual bool  CheckContactData();
    virtual bool  CheckHigerInterrupt();
    virtual bool  CheckChinaLanguage();
    virtual bool  CheckTslInstalling();
    virtual bool  CheckPhoneCall();
    virtual bool  CheckPersionData();
    virtual bool  CheckSmartAgent();
    virtual void  CheckStartReason();
    virtual void  CheckStartStatus();

    virtual Start_Condition_Error CheckStartData();
    void          SetServiceData(int nValue, Condition_Data_Type dataType);

protected:
    VR_StartCondition      m_vrstart_condition;
    Start_Condition_Error  m_errorCode;
    Start_Region_Type      m_RegionType;
    Start_Product_Type     m_ProductType;
    u_int32_t              m_nPeerPID;
    VR_CheckStartReason    m_checkstart_reason;
    VR_CheckStartStatus    m_checkstart_status;
    VR_String              m_cHintsString;
    bool                   m_bUICrashStatus;
};

#endif // VR_STARTSERVICEBASE_H
/* EOF */
