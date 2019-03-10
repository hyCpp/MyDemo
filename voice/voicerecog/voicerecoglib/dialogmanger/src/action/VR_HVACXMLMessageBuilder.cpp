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
 * @file   VR_HVACXMLMessageBuilder.cpp
 * @brief  Implementation class of VR_HVACXMLMessageBuilder
 */
#include "stdafx.h"
#include <boost/format.hpp>
#include "VR_VehicleInfoApi.h"
#include "VR_VehicleInfoProxy.h"
#include "VR_ConfigureIF.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_HVACXMLMessageBuilder.h"
#include "vehicle/NDAirconInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAgentActivation.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

using namespace nutshell::vehicle;
using namespace navi::VoiceRecog;

VR_HVACXMLMessageBuilder* VR_HVACXMLMessageBuilder::m_pMessageBuilder = NULL;

VR_HVACXMLMessageBuilder::VR_HVACXMLMessageBuilder(spVR_DialogEngineIF engineIF) : m_EngineIF(engineIF),
    m_pAvcSenderIF(avcSenderIF),
    m_nLowTemp(60),
    m_nHighTemp(85),
    m_nTempUnit(1),
    m_bHvac_fan_speed(false),
    m_bHvac_change_temperature(false),
    m_bHvac_rear_defogger(false),
    m_bHvac_rear_seat_conditioner(false),
    m_bHvac_wiper_deicer(false),
    m_bHvac_dual_mode(false),
    m_bHvac_ac(false),
    m_bHvac_auto_air_conditioner(false),
    m_bHvac_climate_concierge(false),
    m_bHvac_blower_mode(false),
    m_bHvac_blower_step(false),
    m_bHvac_lexus_concierge_screen(false),
    m_bHvac_rear_seat_screen(false),
    m_bHvac_front_seat_screen(false),
    m_bHvac_steering_screen(false),
    m_bHvac_rear_screen(false),
    m_bHvac_front_screen(false),
    m_bHvac_rear_seat_ven_screen(false),
    m_bHvac_front_seat_ven_screen(false),
    m_bHvac_seat_operator_screen(false)
{
    VR_LOGD_FUNC();
}

VR_HVACXMLMessageBuilder::~VR_HVACXMLMessageBuilder()
{
    VR_LOGD_FUNC();
}

VR_HVACXMLMessageBuilder& VR_HVACXMLMessageBuilder::GetMessageBuilder(spVR_DialogEngineIF engineIF)
{
    if (NULL == m_pMessageBuilder) {
        VR_LOGI("NULL == m_pMessageBuilder");
        m_pMessageBuilder = VR_new VR_HVACXMLMessageBuilder(engineIF);
    }
    return *m_pMessageBuilder;
}

void VR_HVACXMLMessageBuilder::ReleaseMessageBuilder()
{
    VR_LOGD_FUNC();

    if (m_pMessageBuilder != NULL) {
        delete m_pMessageBuilder;
        m_pMessageBuilder = NULL;
    }
}

void VR_HVACXMLMessageBuilder::SendMessage() const
{
    VR_LOGD_FUNC();

    VR_ConfigureIF* pConfig = VR_ConfigureIF::Instance();
    if (NULL == pConfig) {
        VR_LOGD("get config instance is null.");
        return;
    }

    bool bAvcConnect = pConfig->getInnerAvcLanConnectStatus();

    if (bAvcConnect) {
        static VR_String xmlStart =  "<event name=\"updateState\">"
                "<item key=\"CLIMATE_FRONT_SCREEN_ACTIVE\" value=\"%1%\"/>"\
                "<item key=\"CLIMATE_FRONT_SEAT_SCREEN_ACTIVE\" value=\"%2%\"/>"\
                "<item key=\"CLIMATE_REAR_SCREEN_ACTIVE\" value=\"%3%\"/>"\
                "<item key=\"CLIMATE_REAR_SEAT_SCREEN_ACTIVE\" value=\"%4%\"/>"\
                "<item key=\"CLIMATE_SEAT_OPERATION_SCREEN_ACTIVE\" value=\"%5%\"/>"\
                "<item key=\"CLIMATE_STEERING_SCREEN_ACTIVE\" value=\"%6%\"/>"\
                "<item key=\"CLIMATE_FRONT_SEAT_VEN_SCREEN_ACTIVE\" value=\"%7%\"/>"\
                "<item key=\"CLIMATE_REAR_SEAT_VEN_SCREEN_ACTIVE\" value=\"%8%\"/>"\
                "<item key=\"CLIMATE_LEXUS_CONCIERGE_SCREEN_ACTIVE\" value=\"%9%\"/>"\
                "<item key=\"CLIMATE_FANSPEED_MIN\" value=\"%10%\"/>"\
                "<item key=\"CLIMATE_FANSPEED_MAX\" value=\"%11%\"/>"\
                "<item key=\"CLIMATE_BASIC_ACTIVE\" value=\"%12%\"/>"\
                "<item key=\"CLIMATE_CONCIERGE_ACTIVE\" value=\"%13%\"/>"\
                "<item key=\"CLIMATE_WIPERDEICE_ACTIVE\" value=\"%14%\"/>"\
                "<item key=\"CLIMATE_REARAIRCON_ACTIVE\" value=\"%15%\"/>"\
                "<item key=\"CLIMATE_DUALMODE_ACTIVE\" value=\"%16%\"/>"\
                "</event>";

        static VR_String xmlResult   =   "";
        VR_String strFrontSrc        =   m_bHvac_front_screen           ?   "true" : "false";
        VR_String strRearSrc         =   m_bHvac_rear_screen            ?   "true" : "false";
        VR_String strFrontSeatSrc    =   m_bHvac_front_seat_screen      ?   "true" : "false";
        VR_String strRearSeatSrc     =   m_bHvac_rear_seat_screen       ?   "true" : "false";
        VR_String strSeatOpt         =   m_bHvac_seat_operator_screen   ?   "true" : "false";
        VR_String strSteering        =   m_bHvac_steering_screen        ?   "true" : "false";
        VR_String strFrontSeatVen    =   m_bHvac_front_seat_ven_screen  ?   "true" : "false";
        VR_String strRearSeatVen     =   m_bHvac_rear_seat_ven_screen   ?   "true" : "false";
        VR_String strLexusConc       =   m_bHvac_lexus_concierge_screen ?   "true" : "false";
        VR_String strSpeedMin        =   "1";
        VR_String strSpeedMax        =   m_bHvac_blower_step            ?   "7"    : "5";
        VR_String strBasic           =   m_bHvac_ac                     ?   "true" : "false";
        VR_String strConcierge       =   m_bHvac_climate_concierge      ?   "true" : "false";
        VR_String strWiperDeice      =   m_bHvac_wiper_deicer           ?   "true" : "false";
        VR_String strRearaircon      =   m_bHvac_rear_seat_conditioner  ?   "true" : "false";
        VR_String strDualMode        =   m_bHvac_dual_mode              ?   "true" : "false";

        xmlResult = (boost::format(xmlStart)
                     % strFrontSrc % strFrontSeatSrc % strRearSrc
                     % strRearSeatSrc % strSeatOpt % strSteering
                     % strFrontSeatVen % strRearSeatVen % strLexusConc
                     % strSpeedMin % strSpeedMax % strBasic % strConcierge
                     % strWiperDeice % strRearaircon % strDualMode).str();

        VR_LOGD("xmlResult = [%s]", xmlResult.c_str());
        m_EngineIF->SendMessage(xmlResult);
    }
    else {
        static VR_String xmlStart =  "<event name=\"updateState\">"
                "<item key=\"CLIMATE_FRONT_SCREEN_ACTIVE\" value=\"%1%\"/>"\
                "<item key=\"CLIMATE_FRONT_SEAT_SCREEN_ACTIVE\" value=\"%2%\"/>"\
                "<item key=\"CLIMATE_REAR_SCREEN_ACTIVE\" value=\"%3%\"/>"\
                "<item key=\"CLIMATE_REAR_SEAT_SCREEN_ACTIVE\" value=\"%4%\"/>"\
                "<item key=\"CLIMATE_SEAT_OPERATION_SCREEN_ACTIVE\" value=\"%5%\"/>"\
                "<item key=\"CLIMATE_STEERING_SCREEN_ACTIVE\" value=\"%6%\"/>"\
                "<item key=\"CLIMATE_FRONT_SEAT_VEN_SCREEN_ACTIVE\" value=\"%7%\"/>"\
                "<item key=\"CLIMATE_REAR_SEAT_VEN_SCREEN_ACTIVE\" value=\"%8%\"/>"\
                "<item key=\"CLIMATE_LEXUS_CONCIERGE_SCREEN_ACTIVE\" value=\"%9%\"/>"\
                "<item key=\"CLIMATE_FANSPEED_MIN\" value=\"%10%\"/>"\
                "<item key=\"CLIMATE_FANSPEED_MAX\" value=\"%11%\"/>"\
                "<item key=\"CLIMATE_BASIC_ACTIVE\" value=\"%12%\"/>"\
                "<item key=\"CLIMATE_CONCIERGE_ACTIVE\" value=\"%13%\"/>"\
                "<item key=\"CLIMATE_WIPERDEICE_ACTIVE\" value=\"%14%\"/>"\
                "<item key=\"CLIMATE_REARAIRCON_ACTIVE\" value=\"%15%\"/>"\
                "<item key=\"CLIMATE_DUALMODE_ACTIVE\" value=\"%16%\"/>"\
                "<item key=\"CLIMATE_TEMPERATURE_MIN\" value=\"%17%\"/>"\
                "<item key=\"CLIMATE_TEMPERATURE_MAX\" value=\"%18%\"/>"\
                "</event>";

        static VR_String xmlResult   =   "";
        VR_String strFrontSrc        =   m_bHvac_front_screen           ?   "true" : "false";
        VR_String strRearSrc         =   m_bHvac_rear_screen            ?   "true" : "false";
        VR_String strFrontSeatSrc    =   m_bHvac_front_seat_screen      ?   "true" : "false";
        VR_String strRearSeatSrc     =   m_bHvac_rear_seat_screen       ?   "true" : "false";
        VR_String strSeatOpt         =   m_bHvac_seat_operator_screen   ?   "true" : "false";
        VR_String strSteering        =   m_bHvac_steering_screen        ?   "true" : "false";
        VR_String strFrontSeatVen    =   m_bHvac_front_seat_ven_screen  ?   "true" : "false";
        VR_String strRearSeatVen     =   m_bHvac_rear_seat_ven_screen   ?   "true" : "false";
        VR_String strLexusConc       =   m_bHvac_lexus_concierge_screen ?   "true" : "false";
        VR_String strSpeedMin        =   "1";
        VR_String strSpeedMax        =   m_bHvac_blower_step            ?   "7"    : "5";
        VR_String strBasic           =   m_bHvac_ac                     ?   "true" : "false";
        VR_String strConcierge       =   m_bHvac_climate_concierge      ?   "true" : "false";
        VR_String strWiperDeice      =   m_bHvac_wiper_deicer           ?   "true" : "false";
        VR_String strRearaircon      =   m_bHvac_rear_seat_conditioner  ?   "true" : "false";
        VR_String strDualMode        =   m_bHvac_dual_mode              ?   "true" : "false";
        VR_String strTempMin         =   (boost::format("%1%") % m_nLowTemp).str();
        VR_String strTempMax         =   (boost::format("%1%") % m_nHighTemp).str();

        if (VR_ConfigureIF::VR_EngineType::VBT == pConfig->getEngineType()) {
            pConfig->setUpdateState(VR_CLIMATE_FANSPEED_MAX, strSpeedMax);
            pConfig->setUpdateState(VR_CLIMATE_TEMPERATURE_MIN, strTempMin);
            pConfig->setUpdateState(VR_CLIMATE_TEMPERATURE_MAX, strTempMax);
            pConfig->setUpdateState(VR_CLIMATE_BASIC_ACTIVE, strBasic);
            pConfig->setUpdateState(VR_CLIMATE_FRONT_SCREEN_ACTIVE, strFrontSrc);
            pConfig->setUpdateState(VR_CLIMATE_REAR_SCREEN_ACTIVE, strRearSrc);
            pConfig->setUpdateState(VR_CLIMATE_STEERING_SCREEN_ACTIVE, strSteering);
            pConfig->setUpdateState(VR_CLIMATE_LEXUS_CONCIERGE_SCREEN_ACTIVE, strLexusConc);
            pConfig->setUpdateState(VR_CLIMATE_CONCIERGE_ACTIVE, strConcierge);
            pConfig->setUpdateState(VR_CLIMATE_FRONT_SEAT_SCREEN_ACTIVE, strFrontSeatSrc);
            pConfig->setUpdateState(VR_CLIMATE_REAR_SEAT_SCREEN_ACTIVE, strRearSeatSrc);
            pConfig->setUpdateState(VR_CLIMATE_SEAT_OPERATION_SCREEN_ACTIVE, strSeatOpt);
        }

        xmlResult = (boost::format(xmlStart)
                     % strFrontSrc % strFrontSeatSrc % strRearSrc
                     % strRearSeatSrc % strSeatOpt % strSteering
                     % strFrontSeatVen % strRearSeatVen % strLexusConc
                     % strSpeedMin % strSpeedMax % strBasic % strConcierge
                     % strWiperDeice % strRearaircon % strDualMode % strTempMin % strTempMax).str();

        VR_LOGD("xmlResult = [%s]", xmlResult.c_str());
        m_EngineIF->SendMessage(xmlResult);
    }
}

int  VR_HVACXMLMessageBuilder::GetLowTemp() const
{
    return m_nLowTemp;
}

int VR_HVACXMLMessageBuilder::GetHighTemp() const
{
    return m_nHighTemp;
}

int VR_HVACXMLMessageBuilder::GetTempUnit() const
{
    return m_nTempUnit;
}

void  VR_HVACXMLMessageBuilder::SetLowTemp(int nLowTemp)
{
    m_nLowTemp = nLowTemp;
}

void VR_HVACXMLMessageBuilder::SetHighTemp(int nHighTemp)
{
    m_nHighTemp = nHighTemp;
}

bool  VR_HVACXMLMessageBuilder::GetBlowStep() const
{
    return m_bHvac_blower_step;
}

void  VR_HVACXMLMessageBuilder::SetBlowStep(bool bBlowStep)
{
    m_bHvac_blower_step = bBlowStep;
}

bool  VR_HVACXMLMessageBuilder::GetFanSpeed()  const
{
    return m_bHvac_fan_speed;
}

bool VR_HVACXMLMessageBuilder::GetChangeTemp() const
{
    return m_bHvac_change_temperature;
}

bool VR_HVACXMLMessageBuilder::GetRearDefogger() const
{
    return m_bHvac_rear_defogger;
}

bool VR_HVACXMLMessageBuilder::GetRearSeatCond() const
{
    return m_bHvac_rear_seat_conditioner;
}

bool VR_HVACXMLMessageBuilder::GetWiperDeicer() const
{
    return m_bHvac_wiper_deicer;
}

bool VR_HVACXMLMessageBuilder::GetDualMode() const
{
    return m_bHvac_dual_mode;
}

bool VR_HVACXMLMessageBuilder::GetAC() const
{
    return m_bHvac_ac;
}

bool VR_HVACXMLMessageBuilder::GetAutoAirCond() const
{
    return m_bHvac_auto_air_conditioner;
}

bool VR_HVACXMLMessageBuilder::GetClimateConci() const
{
    return m_bHvac_climate_concierge;
}

bool VR_HVACXMLMessageBuilder::GetBlowerMode() const
{
    return m_bHvac_blower_mode;
}

bool VR_HVACXMLMessageBuilder::GetLexusScreen() const
{
    return m_bHvac_lexus_concierge_screen;
}

bool VR_HVACXMLMessageBuilder::GetRearSeatScreen() const
{
    return m_bHvac_rear_seat_screen;
}

bool VR_HVACXMLMessageBuilder::GetFrontSeatScreen() const
{
    return m_bHvac_front_seat_screen;
}

bool VR_HVACXMLMessageBuilder::GetSteeringScreen() const
{
    return m_bHvac_steering_screen;
}

bool VR_HVACXMLMessageBuilder::GetRearScreen() const
{
    return m_bHvac_rear_screen;
}

bool VR_HVACXMLMessageBuilder::GetFrontScreen() const
{
    return m_bHvac_front_screen;
}

bool VR_HVACXMLMessageBuilder::GetRearSeatVenScreen() const
{
    return m_bHvac_rear_seat_ven_screen;
}

bool  VR_HVACXMLMessageBuilder::GetFrontSeatVenScreen() const
{
    return m_bHvac_front_seat_ven_screen;
}

bool VR_HVACXMLMessageBuilder::GetSeatOperatorScreen() const
{
    return m_bHvac_seat_operator_screen;
}

void VR_HVACXMLMessageBuilder::HandleMessageData(const std::string &strProperty, const nutshell::NCVariant &var)
{
    VR_LOGD_FUNC();
    VR_LOGD("strProperty = [%s]", strProperty.c_str());

    VR_ConfigureIF* pConfig = VR_ConfigureIF::Instance();
    if (NULL == pConfig) {
        VR_LOGD("get config instance is null.");
        return;
    }
    bool bAvcConnect = pConfig->getInnerAvcLanConnectStatus();

    if (strProperty == vr_get_blower_step) {  // blower step 5 / 7
        int nValue(-1);
        var.value(nValue);
        VR_LOGD("nValue = [%d]", nValue);

        if (nValue == 0) {
            m_bHvac_blower_step = false;
        }
        else if (nValue == 1) {
            m_bHvac_blower_step = true;
        }
        return;
    }
    else if (strProperty == vr_get_telephone_switch) { // add for phone button,
        std::string  strResult = "";
        var.value(strResult);
        VR_LOGD("strResult = [%s]", strResult.c_str());

        const static std::string xmlStart = "<event name=\"updateState\">"
                                                  "<item key=\"STEERING_PHONEKEY_TYPE\" value=\"%1%\"/>"
                                            "</event>";
        std::string xmlValue = "";
        if ("1buttom" == strResult) {
            xmlValue = "TEL";
        }
        else if ("2buttom" == strResult) {
            xmlValue = "OFFHOOK";
        }
        else {
        }

        pConfig->setUpdateState(VR_STEERING_PHONEKEY_TYPE, xmlValue);
        strResult = (boost::format(xmlStart) % xmlValue).str();
        VR_LOGD("strResult = [%s]", strResult.c_str());

        if (NULL != m_EngineIF.get()) {
            m_EngineIF->SendMessage(strResult);
        }
        return;
    }
    else if (strProperty == vr_get_china_hybird) {  // add for cn hybird flag
        std::string  strResult = "";
        var.value(strResult);
        VR_LOGD("strResult = [%s]", strResult.c_str());

        if ("available" == strResult) {
            VR_ConfigureIF::Instance()->setHybridVRFlag(true);
        }
        else if ("unavailable" == strResult) {
            VR_ConfigureIF::Instance()->setHybridVRFlag(false);
        }
        return;
    }
    else if (strProperty == vr_get_temprange) { // temperature range
        std::string  infoTempRange;
        var.value(infoTempRange);
        NDAirconTemperatureRange  tempRange;
        tempRange.ParseFromString(infoTempRange);
        m_nLowTemp  = tempRange.value_lower();
        m_nHighTemp = tempRange.value_upper();
        m_nTempUnit = tempRange.unit();
        VR_LOGD("nLowTemp = [%d], nHighTemp = [%d], nTempUnit = [%d]", m_nLowTemp, m_nHighTemp, m_nTempUnit);
    }
    else if (strProperty == vr_get_carsensor_ig) {  // ig on / off
        bool bValue(false);
        var.value(bValue);
        VR_LOGD("bValue = [%d]", bValue);

        if (bValue) {
            VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_vr_function.c_str());
            return;
        }
        else {
            m_bHvac_fan_speed = false;
            m_bHvac_change_temperature = false;
            m_bHvac_rear_defogger = false;
            m_bHvac_rear_seat_conditioner = false;
            m_bHvac_wiper_deicer = false;
            m_bHvac_dual_mode = false;
            m_bHvac_ac = false;
            m_bHvac_auto_air_conditioner = false;
            m_bHvac_climate_concierge = false;
            m_bHvac_blower_mode = false;
            m_bHvac_lexus_concierge_screen = false;
            m_bHvac_rear_seat_screen = false;
            m_bHvac_front_seat_screen = false;
            m_bHvac_steering_screen = false;
            m_bHvac_rear_screen = false;
            m_bHvac_front_screen = false;
            m_bHvac_rear_seat_ven_screen = false;
            m_bHvac_front_seat_ven_screen = false;
            m_bHvac_seat_operator_screen = false;
        }
    }
    else if (strProperty == vr_get_vr_function) {  // vr aircon func
        std::string strResult = "";
        var.value(strResult);
        VR_LOGD("strResult = [%s]", strResult.c_str());

        if (strResult == "available") {
            VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_ac_screen.c_str());
            // VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_signal_status.c_str());
            return;
        }
        else if (strResult == "unavailable") {
            m_bHvac_fan_speed = false;
            m_bHvac_change_temperature = false;
            m_bHvac_rear_defogger = false;
            m_bHvac_rear_seat_conditioner = false;
            m_bHvac_wiper_deicer = false;
            m_bHvac_dual_mode = false;
            m_bHvac_ac = false;
            m_bHvac_auto_air_conditioner = false;
            m_bHvac_climate_concierge = false;
            m_bHvac_blower_mode = false;
            m_bHvac_lexus_concierge_screen = false;
            m_bHvac_rear_seat_screen = false;
            m_bHvac_front_seat_screen = false;
            m_bHvac_steering_screen = false;
            m_bHvac_rear_screen = false;
            m_bHvac_front_screen = false;
            m_bHvac_rear_seat_ven_screen = false;
            m_bHvac_front_seat_ven_screen = false;
            m_bHvac_seat_operator_screen = false;
        }
    }
    else if (strProperty == vr_get_ac_screen) {  // ac_screen_operation
        std::string strResult = "";
        var.value(strResult);
        VR_LOGD("strResult = [%s]", strResult.c_str());

        if (strResult == "available") {
            // m_bHvac_front_screen = true;
            // VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_vr_version.c_str());
            VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_signal_status.c_str());
            return;
        }
        else if (strResult == "unavailable") {
            m_bHvac_lexus_concierge_screen = false;
            m_bHvac_rear_seat_screen = false;
            m_bHvac_front_seat_screen = false;
            m_bHvac_steering_screen = false;
            m_bHvac_rear_screen = false;
            m_bHvac_front_screen = false;
            m_bHvac_rear_seat_ven_screen = false;
            m_bHvac_front_seat_ven_screen = false;
        }
    }
    else if (strProperty == vr_get_vr_version) {  // vr_version
        std::string strResult = "";
        var.value(strResult);
        VR_LOGD("strResult = [%s]", strResult.c_str());

        if ("1" == strResult || "2" == strResult || "3" == strResult
            || "4" == strResult || "5" == strResult || "6" == strResult
            || "7" == strResult) {
            m_bHvac_fan_speed = true;
            m_bHvac_change_temperature = true;
            m_bHvac_rear_defogger = true;
            m_bHvac_dual_mode = true;
            m_bHvac_ac = true;
            m_bHvac_auto_air_conditioner = true;
            m_bHvac_blower_mode = true;

            VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_wiperdeice_sw.c_str());
            VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_rear_aircon_sw.c_str());
            VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_climateconcierg_sw.c_str());
            VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_dual_status.c_str());
            return;
        }
        else {
            m_bHvac_fan_speed = false;
            m_bHvac_change_temperature = false;
            m_bHvac_rear_defogger = false;
            m_bHvac_rear_seat_conditioner = false;
            m_bHvac_wiper_deicer = false;
            m_bHvac_dual_mode = false;
            m_bHvac_ac = false;
            m_bHvac_auto_air_conditioner = false;
            m_bHvac_climate_concierge = false;
            m_bHvac_blower_mode = false;
        }
    }
    else if (strProperty == vr_get_signal_status) {   // signal
        bool bResult = true;
        var.value(bResult);
        VR_LOGD("bResult = [%d]", bResult);

        if (bResult) {
            m_bHvac_lexus_concierge_screen = false;
            m_bHvac_rear_seat_screen = false;
            m_bHvac_front_seat_screen = false;
            m_bHvac_steering_screen = false;
            m_bHvac_rear_screen = false;
            m_bHvac_front_screen = false;  // if we make it false, we will make it true
            m_bHvac_rear_seat_ven_screen = false;
            m_bHvac_front_seat_ven_screen = false;
        }
        else {
            m_bHvac_front_screen = true;
            VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_vr_version.c_str()); //  if signal vaild , version is vaild
            VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_front_seat_ven_screen.c_str());
            VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_steering_screen.c_str());
            VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_rear_seat_ven_screen.c_str());
            VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_rear_seat_screen.c_str());
            VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_rear_screen.c_str());
            VR_VehicleInfoProxy::CreateInstance().GetShowScreenAsync(vr_get_lexus_con_screen.c_str());
            return;
        }
    }
    else if (strProperty == vr_get_seat_screen) {    // seat screen
        bool bResult = false;
        var.value(bResult);
        VR_LOGD("bResult = %d", bResult);

        m_bHvac_seat_operator_screen = bResult;
    }
    else if (strProperty == vr_get_wiperdeice_sw) {   // wiper deice sw
        bool bResult(false);
        var.value(bResult);
        VR_LOGD("bResult = [%d]", bResult);

        if (bResult) {
            m_bHvac_wiper_deicer = true;
        }
        else {
            m_bHvac_wiper_deicer = false;
        }
    }
    else if (strProperty == vr_get_rear_aircon_sw) {   // rear aircon sw
        bool bResult(false);
        var.value(bResult);
        VR_LOGD("bResult = [%d]", bResult);

        if (bResult) {
            m_bHvac_rear_seat_conditioner = true;
        }
        else {
            m_bHvac_rear_seat_conditioner = false;
        }
    }
    else if (strProperty == vr_get_climateconcierg_sw) {   // concierge sw
        bool bResult(false);
        var.value(bResult);
        VR_LOGD("bResult = [%d]", bResult);

        if (bResult) {
            m_bHvac_climate_concierge = true;
            m_bHvac_lexus_concierge_screen = true;
        }
        else {
            m_bHvac_climate_concierge = false;
            m_bHvac_lexus_concierge_screen = false;
        }
    }
    else if (strProperty == vr_get_dual_status) {  // dual mode status
        bool bResult(false);
        var.value(bResult);
        VR_LOGD("bResult = [%d]", bResult);

        if (bResult) {
            m_bHvac_dual_mode = true;
        }
        else {
            m_bHvac_dual_mode = false;
        }
    }
    else if (strProperty == vr_get_rear_screen) {
        bool bValue(false);
        var.value(bValue);
        VR_LOGD("bValue = [%d]", bValue);

        if (bValue) {  // true support
            m_bHvac_rear_screen = true;
        }
        else {
            m_bHvac_rear_screen = false;
        }
    }
    else if (strProperty == vr_get_steering_screen) {
        int nValue(-1);
        var.value(nValue);
        VR_LOGD("nValue = [%d]", nValue);

        if (nValue == 0x01 || nValue == 0x02) {   // 1 || 2 support
            m_bHvac_steering_screen = true;
        }
        else {
            m_bHvac_steering_screen = false;
        }
    }
    else if (strProperty == vr_get_front_seat_ven_screen) {
        int nValue(-1);
        var.value(nValue);
        VR_LOGD("nValue = [%d]", nValue);

        if (nValue == 0x00) { // both not support
            m_bHvac_front_seat_screen = false;
            m_bHvac_front_seat_ven_screen = false;
        }
        else if (nValue == 0x01) { // heat support, ventilation not support
            m_bHvac_front_seat_screen = true;
            m_bHvac_front_seat_ven_screen = false;
        }
        else if (nValue == 0x02) { // heat not support , ventilation support
            m_bHvac_front_seat_screen = false;
            m_bHvac_front_seat_ven_screen = true;
        }
        else if (nValue == 0x03) { // both support
            m_bHvac_front_seat_screen = true;
            m_bHvac_front_seat_ven_screen = true;
        }
    }
    else if (strProperty == vr_get_rear_seat_screen) {
        bool bValue(false);
        var.value(bValue);
        VR_LOGD("bValue = [%d]", bValue); // true support
        m_bHvac_rear_seat_screen = bValue;
    }
    else if (strProperty == vr_get_rear_seat_ven_screen) {
        bool bValue(false);
        var.value(bValue);
        VR_LOGD("bValue = [%d]", bValue);   // true support

        m_bHvac_rear_seat_ven_screen = bValue;
    }
    // else if (strProperty == vr_get_lexus_con_screen) {
    //    bool bValue(false);
    //    var.value(bValue);
    //    VR_LOGD("bValue = [%d]", bValue);  // true support

    //    m_bHvac_lexus_concierge_screen = bValue;
    // }
    else {
        return;
    }
    if (bAvcConnect) {
        std::unique_ptr<VrAgentVehicleInfo> vehicleInfo(VrAgentVehicleInfo::default_instance().New());
        vehicleInfo->set_ac(m_bHvac_ac);
        vehicleInfo->set_auto_air_conditioner(m_bHvac_auto_air_conditioner);
        vehicleInfo->set_blower_mode(m_bHvac_blower_mode);
        vehicleInfo->set_wiper_deicer(m_bHvac_wiper_deicer);
        vehicleInfo->set_rear_seat_air_conditioner(m_bHvac_rear_seat_conditioner);
        vehicleInfo->set_rear_defogger(m_bHvac_rear_defogger);
        vehicleInfo->set_change_temperature(m_bHvac_change_temperature);
        vehicleInfo->set_fan_speed(m_bHvac_fan_speed);
        vehicleInfo->set_dual_mode(m_bHvac_dual_mode);
        vehicleInfo->set_climate_concierge(m_bHvac_climate_concierge);
        vehicleInfo->set_hvac_front_screen(m_bHvac_front_screen);
        vehicleInfo->set_hvac_rear_screen(m_bHvac_rear_screen);
        vehicleInfo->set_hvac_steering_screen(m_bHvac_steering_screen);
        vehicleInfo->set_hvac_front_seat_screen(m_bHvac_front_seat_screen);
        vehicleInfo->set_hvac_rear_seat_screen(m_bHvac_rear_seat_screen);
        vehicleInfo->set_hvac_lexus_concierge_screen(m_bHvac_lexus_concierge_screen);
        vehicleInfo->set_hvac_seat_operation_screen(m_bHvac_seat_operator_screen);
        vehicleInfo->set_front_seat_ventilation_screen(m_bHvac_front_seat_ven_screen);
        vehicleInfo->set_rear_seat_ventilation_screen(m_bHvac_rear_seat_ven_screen);
        VrVehicleBlowerStep  blowerStep = STEP5;
        if (m_bHvac_blower_step) {
            blowerStep = STEP7;
        }
        vehicleInfo->set_blower_step(blowerStep);
        VrACtempUnit tempUnit = Celsius;
        if (0 == m_nTempUnit) {
            tempUnit = Celsius;
        }
        else {
            tempUnit = Fahrenheit;
        }
        vehicleInfo->set_ac_temperature_unit(tempUnit);
    }
    else {
        SendMessage();
    }
}

void VR_HVACXMLMessageBuilder::OnMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    VR_String strTypeName = msg.GetTypeName();

    if ("navi.VoiceRecog.VrAgentVehicleInfo" == strTypeName) {
        const VrAgentVehicleInfo& vehicleInfo = (const VrAgentVehicleInfo&)msg;

        m_bHvac_ac                       =  vehicleInfo.ac();
        m_bHvac_auto_air_conditioner     =  vehicleInfo.auto_air_conditioner();
        m_bHvac_blower_mode              =  vehicleInfo.blower_mode();
        VrVehicleBlowerStep  blowerStep  =  vehicleInfo.blower_step();
        if (blowerStep == STEP5) {
            m_bHvac_blower_step = false;
        }
        else {
            m_bHvac_blower_step = true;
        }
        m_bHvac_change_temperature       =  vehicleInfo.change_temperature();
        m_bHvac_climate_concierge        =  vehicleInfo.climate_concierge();
        m_bHvac_dual_mode                =  vehicleInfo.dual_mode();
        m_bHvac_fan_speed                =  vehicleInfo.fan_speed();
        m_bHvac_front_seat_ven_screen    =  vehicleInfo.front_seat_ventilation_screen();
        m_bHvac_front_screen             =  vehicleInfo.hvac_front_screen();
        m_bHvac_front_seat_screen        =  vehicleInfo.hvac_front_seat_screen();
        m_bHvac_lexus_concierge_screen   =  vehicleInfo.hvac_lexus_concierge_screen();
        m_bHvac_rear_screen              =  vehicleInfo.hvac_rear_screen();
        m_bHvac_rear_seat_screen         =  vehicleInfo.hvac_rear_seat_screen();
        m_bHvac_seat_operator_screen     =  vehicleInfo.hvac_seat_operation_screen();
        m_bHvac_steering_screen          =  vehicleInfo.hvac_steering_screen();
        m_bHvac_rear_defogger            =  vehicleInfo.rear_defogger();
        m_bHvac_rear_seat_conditioner    =  vehicleInfo.rear_seat_air_conditioner();
        m_bHvac_rear_seat_ven_screen     =  vehicleInfo.rear_seat_ventilation_screen();
        m_bHvac_wiper_deicer             =  vehicleInfo.wiper_deicer();
        VrACtempUnit   tempUnit          =  vehicleInfo.ac_temperature_unit();
        if (Celsius == tempUnit) {
            m_nTempUnit = 0;
        }
        else {
            m_nTempUnit = 1;
        }
    }
    if (m_EngineIF.get() && m_pAvcSenderIF.get()) {
        SendMessage();
    }

    if ("navi.VoiceRecog.VrAgentVehicleInfo" == strTypeName) {
        std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        VrEngineControlMessage*  controlMessage = loopMessage->mutable_m_controlmessage();
        controlMessage->set_type(DMTaskMessageType_VehicleInfoResp);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
    }
}
/* EOF */
















