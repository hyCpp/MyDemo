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
 * @file  VR_VehicleInfoProxy.h
 * @brief Declaration of class VR_VehicleInfoProxy
 */

#ifndef VR_VEHICLEINFOPROXY_H
#define VR_VEHICLEINFOPROXY_H

#ifndef __cplusplus
# error ERROR : This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#    include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

#ifndef VR_VEHICLEINFOREPLY_H
#    include "VR_VehicleInfoReply.h"
#endif

#include "VR_HAVCTypedefine.h"
// #include <vehicleinfo/NCVehicleInfo.h>
// #include <evip/NEReplyReceiver.h>
#include <boost/scoped_ptr.hpp>
#include <ncore/NCTypesDefine.h>
#include <ncore/NCVariant.h>
#include "MEM_list.h"
#include "BL_Timer.h"
// #include "VR_VoiceList.hpp"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ActionEventSender);

class VR_VehicleInfoProxy;
typedef  void(VR_VehicleInfoProxy::*call_back_get)();
typedef  void(VR_VehicleInfoProxy::*call_back_set)(const nutshell::NCVariant&);
typedef  void(VR_VehicleInfoProxy::*call_back_send)(bool, const nutshell::NCVariant&);

/**
 * @brief The VR_VehicleData
 *
 * class VR_VehicleData
 */
class VR_VehicleData
{
public:
    VR_VehicleData() : m_nActionId(VR_INVALID_VALUE),
        m_bVrCommand(false),
        m_pStrCommand(NULL),
        m_pncValue(NULL),
        m_pGetCallFunc(NULL),
        m_pSetCallFunc(NULL),
        m_pSendCallFunc(NULL)
    {
    }

    virtual ~VR_VehicleData()
    {
    }

    int                   m_nActionId;
    bool                  m_bVrCommand;
    std::string*          m_pStrCommand;
    nutshell::NCVariant*  m_pncValue;
    call_back_get         m_pGetCallFunc;
    call_back_set         m_pSetCallFunc;
    call_back_send        m_pSendCallFunc;
    
private:
    VR_VehicleData(const VR_VehicleData& other);
    VR_VehicleData& operator =(const VR_VehicleData& other);
};


/**
 * @brief The VR_VehicleTimer class
 *
 * class declaration
 */
class  VR_VehicleTimer : public BL_Timer
{
public:
    VR_VehicleTimer();
    virtual ~VR_VehicleTimer();
    virtual VOID OnTimer();
};

/**
 * @brief The VR_VehicleInfoProxy class
 *
 * class declaration
 */
class  VR_VehicleInfoProxy
{
public:
    virtual ~VR_VehicleInfoProxy();

    static VR_VehicleInfoProxy& CreateInstance();
    void SubScribleProperty();
    VR_VehicleData* Front();
    // status info
    void GetACStatus(int nActionId);
    void GetAutoStatus(int nActionId);
    void GetTemperatureStatus(int nActionId);
    void GetTempRangleStatus(int nActionId);
    void GetFanSpeedStatus(int nActionId);
    void GetDaulModeStatus(int nActionId);
    void GetWiperDeicerStatus(int nActionId);
    void GetRearDefoggerStatus(int nActionId);
    void GetBlowerModeStatus(int nActionId);
    void GetClimateConciergeModeStatus(int nActionId);
    // facility switch
    void SetAutoStatus(int nActionId, bool bAutoStatus);
    void SetACStatus(int nActionId, bool bACStatus);
    void SetDaulModeStatus(int nActionId, bool bDualStatus);
    void SetWiperDeicerStatus(int nActionId, bool bWDStatus);
    void SetRearSeatAutoStatus(int nActionId, bool bRSAStatus);
    void SetRearDefoggerStatus(int nActionId, bool bRearDefStatus);
    void SetClimateConciergeStatus(int nActionId, bool bCConciergeStatus);
    // fan climate
    void ChangeCurrentFanSpeed(int nActionId, int nSpeed);
    void ChangeFanWindMode(int nActionId, int nWindMode);
    // show screen
    void ShowHVACScreen(int nActionId, int nShowMode);
    // tune climate
    void SetTemperature(int nActionId, Temperature_Mode mode);
    void ChangeCurrentTemperature(int nActionId, float fTempValue);

    bool HandleData(const nutshell::NCVariant& var);
    bool HandleDataError(const nutshell::NCCHAR* error);
    bool GetShowScreenAsync(const nutshell::NCCHAR* perperty);
    VR_VehicleTimer*  GetVehicleTimer();

protected:
    bool PushGetData(const VR_VehicleData& data);
    bool PushSetData(const VR_VehicleData& data);
    void Remove();
    // VehicleData* Front();
    bool IsReqListEmpty() const;

    bool SetPropertyAsync(const nutshell::NCCHAR* property, const nutshell::NCVariant& var);
    bool GetPropertyAsync(const nutshell::NCCHAR* property);

private:
    void GetACStatus();
    void GetAutoStatus();
    void GetTemperatureStatus();
    void GetTempRangleStatus();
    void GetFanSpeedStatus();
    void GetDaulModeStatus();
    void GetWiperDeicerStatus();
    void GetRearDefoggerStatus();
    void GetBlowerModeStatus();
    void GetClimateConciergeModeStatus();

    void SetAutoStatus(const nutshell::NCVariant& value);
    void SetACStatus(const nutshell::NCVariant& value);
    void SetDaulModeStatus(const nutshell::NCVariant& value);
    void SetWiperDeicerStatus(const nutshell::NCVariant& value);
    void SetRearSeatAutoStatus(const nutshell::NCVariant& value);
    void SetRearDefoggerStatus(const nutshell::NCVariant& value);
    void SetClimateConciergeStatus(const nutshell::NCVariant& value);

    void ChangeCurrentFanSpeed(const nutshell::NCVariant& value);
    void ChangeFanWindMode(const nutshell::NCVariant& value);

    void ShowHVACScreen(const nutshell::NCVariant& value);

    void SetTemperature(const nutshell::NCVariant& value);
    void ChangeCurrentTemperature(const nutshell::NCVariant& value);

    void HandleGetACStatus(bool bSuccess, const nutshell::NCVariant& ncValue);
    void HandleGetAutoACStatus(bool bSuccess, const nutshell::NCVariant& ncValue);
    void HandleGetTemperature(bool bSuccess, const nutshell::NCVariant& ncValue);
    void HandleGetTempRange(bool bSuccess, const nutshell::NCVariant& value);
    void HandleGetFanSpeed(bool bSuccess, const nutshell::NCVariant& ncValue);
    void HandleGetDModeStatus(bool bSuccess, const nutshell::NCVariant& ncValue);
    void HandleGetWiperDeicerStatus(bool bSuccess, const nutshell::NCVariant& value);
    void HandleGetRearDefStatus(bool bSuccess, const nutshell::NCVariant& value);
    void HandleGetBlowerModeStatus(bool bSuccess, const nutshell::NCVariant& value);
    void HandleGetClimateConciergeMode(bool bSuccess, const nutshell::NCVariant& value);

    void HandleAutoAcOn(bool bSuccess, const nutshell::NCVariant& value);
    void HandleAutoAcOff(bool bSuccess, const nutshell::NCVariant& value);
    void HandleAcOn(bool bSuccess, const nutshell::NCVariant& value);
    void HandleAcOff(bool bSuccess, const nutshell::NCVariant& value);
    void HandleDualModelOn(bool bSuccess, const nutshell::NCVariant& value);
    void HandleDualModeOff(bool bSuccess, const nutshell::NCVariant& value);
    void HandleWDeicerOn(bool bSuccess, const nutshell::NCVariant& value);
    void HandleWDeicerOff(bool bSuccess, const nutshell::NCVariant& value);
    void HandleRearSCOn(bool bSuccess, const nutshell::NCVariant& value);
    void HandleRearSCOff(bool bSuccess, const nutshell::NCVariant& value);
    void HandleRDefoggerOn(bool bSuccess, const nutshell::NCVariant& value);
    void HandleRDefoggerOff(bool bSuccess, const nutshell::NCVariant& value);
    void HandleDecreaseTemp(bool bSuccess, const nutshell::NCVariant& value);
    void HandleIncreaseTemp(bool bSuccess, const nutshell::NCVariant& value);
    void HandleChangeTemp(bool bSuccess, const nutshell::NCVariant& value);
    void HandleAfewWarmer(bool bSuccess, const nutshell::NCVariant& value);
    void HandleAfewCooler(bool bSuccess, const nutshell::NCVariant& value);
    void HandleTempMax(bool bSuccess, const nutshell::NCVariant& value);
    void HandleTempMin(bool bSuccess, const nutshell::NCVariant& value);
    void HandChangeFanSpeed(bool bSuccess, const nutshell::NCVariant& value);
    void HandleFanSpeedMax(bool bSuccess, const nutshell::NCVariant& value);
    void HandleFanSpeedMin(bool bSuccess, const nutshell::NCVariant& value);
    void HandleBMFace(bool bSuccess, const nutshell::NCVariant& value);
    void HandleBMFoot(bool bSuccess, const nutshell::NCVariant& value);
    void HandleFaceAndFoot(bool bSuccess, const nutshell::NCVariant& value);
    void HandleWinAndFoot(bool bSuccess, const nutshell::NCVariant& value);
    void HandleCConiergerOn(bool bSuccess, const nutshell::NCVariant& value);
    void HandleShowFrontScreen(bool bSuccess, const nutshell::NCVariant& value);
    void HandleShowRearScreen(bool bSuccess, const nutshell::NCVariant &value);
    void HandleShowSteeringScreen(bool bSuccess, const nutshell::NCVariant &value);
    void HandleShowFrontSeatScreen(bool bSuccess, const nutshell::NCVariant &value);
    void HandleShowRearSeatScreen(bool bSuccess, const nutshell::NCVariant &value);
    void HandleShowLexusConciergeScreen(bool bSuccess, const nutshell::NCVariant &value);
    void HandleShowSeatOperationScreen(bool bSuccess, const nutshell::NCVariant &value);
    void HandleFrontSeatVentilationScreen(bool bSuccess, const nutshell::NCVariant &value);
    void HandleRearSeatVentilationScreen(bool bSuccess, const nutshell::NCVariant &value);

    VR_VehicleInfoProxy();
    VR_VehicleInfoProxy(const VR_VehicleInfoProxy& other);
    VR_VehicleInfoProxy& operator =(const VR_VehicleInfoProxy& other);

    void RegisterReplyReceiver();
    void Clear();

private:
    static VR_VehicleInfoProxy*                     m_pInstance;
    // nutshell::ncsp<nutshell::NCVehicleInfo>::sp     m_spProxy;
    // nutshell::ncsp<nutshell::NEReplyReceiver>::sp   m_spReceiver;
    VoiceList<std::string>::type                    m_propertyList;
    VoiceList<VR_VehicleData*>::type                m_reqList;
    VR_VehicleTimer                                 m_Timer;
};

#endif /* VR_VEHICLEINFOPROXY_H */
/* EOF */













