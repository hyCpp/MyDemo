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
 * @file VR_NCDrvInfoProxy.cpp
 * @brief Implementation of class VR_NCDrvInfoProxy
 */

#include "VR_NCDrvInfoProxy.h"
#include "VR_NCDrvInfoReplier.h"
#include "VR_Log.h"
#include <ctime>
#include <chrono>
#include <iomanip>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "EV_EventSender.h"
#include "EV_Define.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"

using namespace nutshell;
using namespace nutshell::drvasst;
using namespace nutshell::drvinfoservice;
using namespace navi::VoiceRecog;
static nutshell::NCSyncObj syncObj;

const VR_String sendToFCDrvInfoXml("<display agent=\"Common\" content=\"DrvInfo\">"\
                                       "<content>DrvInfo</content>"\
                                       "<runredlights>%1%</runredlights>"\
                                       "<fatiguedrive>%2%</fatiguedrive>"\
                                       "<speeding>%3%</speeding>"\
                                       "<changelane>%4%</changelane>"\
                                       "<hardaccelerate>%5%</hardaccelerate>"\
                                       "<harddecelerate>%6%</harddecelerate>"\
                                       "<hardcornering>%7%</hardcornering>"\
                                       "<turnwithnosignal>%8%</turnwithnosignal>"\
                                       "<total>%9%</total>"\
                                   "</display>");

VR_NCDrvInfoProxy::VR_NCDrvInfoProxy(const NCString& serviceName)
    : m_serviceName(serviceName)
    , m_spDrvInfoProxy(VR_new NCDrvInfoProxy)
    , m_spDrvInfoReplier(VR_new VR_NCDrvInfoReplier(this))
    , m_DrvInfoState(State_Engine_Init)
{
    VR_LOGD_FUNC();
    m_spDrvInfoProxy->setReplyReceiver(m_spDrvInfoReplier);
    onBind();
}

VR_NCDrvInfoProxy::~VR_NCDrvInfoProxy()
{
    onUnBind();
}

bool VR_NCDrvInfoProxy::onBind()
{
    VR_LOGD("DrvInfo_onBind");

    auto ipc = NEIpcServiceHelper::findService(m_serviceName, 0);

    if (ipc.get() == nullptr) {
        VR_LOGD("DrvInfo_findService error !");
        return false;
    }

    if (m_spDrvInfoProxy.get() == nullptr) {
        VR_LOGD("DrvInfo_m_spDrvInfoProxy is nullptr !");
        return false;
    }

    NC_BOOL result = m_spDrvInfoProxy->bindService(ipc);

    if (!result) {
        VR_LOGD("DrvInfo_bindService error !");
        return false;
    }

    return true;
}

bool VR_NCDrvInfoProxy::getDrivingAssistantTime(VR_String &intervalTime, int time)
{
    VR_LOGD("DrvInfo_getDrivingAssistantTime");

    intervalTime.clear();
    std::time_t currentSystemTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() - std::chrono::hours(time));
    char mbstr[1024] = {};

    if (std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", std::localtime(&currentSystemTime))) {
        intervalTime.append(mbstr);
        intervalTime.append(".000");
        return true;
    }

    return false;
}

bool VR_NCDrvInfoProxy::reqDrivingAssistantByUser(int time)
{
    VR_LOGD("DrvInfo_reqDrivingAssistantByUser time = [%d]", time);

    nutshell::drvinfoservice::NDDrvinfoTime reqProto;
    VR_String startTime("");
    VR_String endTime("");

    if (!getDrivingAssistantTime(startTime)) {
        VR_LOGD("getDrivingAssistantTime error !!!");
        return false;
    }

    if (!getDrivingAssistantTime(endTime, time)) {
        VR_LOGD("getDrivingAssistantTime error !!!");
        return false;
    }

    reqProto.set_starttime(startTime);
    reqProto.set_endtime(endTime);

    if (0 == m_spDrvInfoProxy->getDrvEvaluate(reqProto)) {
        VR_LOGD("DrvInfo getDrvEvaluate fail !!!");
        return false;
    }

    return true;
}

void VR_NCDrvInfoProxy::reqDrivingAssistant()
{
    VR_LOGD("DrvInfo_reqDrivingAssistant");

    setDrvInfoState(State_Engine_Init);
    m_DrvInfo.Clear();

    if (!reqDrivingAssistantByUser(7 * 24)) {
        VR_LOGD("reqDrivingAssistantByUser error !!!");
    }

    if (!reqDrivingAssistantByUser(30 * 24)) {
        VR_LOGD("reqDrivingAssistantByUser error !!!");
    }

    if (!reqDrivingAssistantByUser(365 * 24)) {
        VR_LOGD("reqDrivingAssistantByUser error !!!");
    }
}

void VR_NCDrvInfoProxy::replyDrvInfo(const NDDrvinfoEvaluate &evaluateInfo)
{
    VR_LOGD("DrvInfo_replyDrvInfo");

    VR_String resultXml("");
    VR_String startTime = evaluateInfo.starttime();
    VR_String endTime = evaluateInfo.endtime();

    int32_t runredlights = evaluateInfo.runredlights();
    int32_t fatiguedrive = evaluateInfo.fatiguedrive();
    int32_t speeding = evaluateInfo.speeding();
    int32_t changelane = evaluateInfo.changelane();
    int32_t hardaccelerate = evaluateInfo.hardaccelerate();
    int32_t harddecelerate = evaluateInfo.harddecelerate();
    int32_t hardcornering = evaluateInfo.hardcornering();
    int32_t turnwithnosignal = evaluateInfo.turnwithnosignal();
    int reqTimeInterval = DateTimeInterval(startTime, endTime);
    int32_t total = runredlights + fatiguedrive + speeding + changelane + hardaccelerate + harddecelerate + hardcornering + turnwithnosignal;

    VR_LOGD("DrvInfo runredlights=[%d], fatiguedrive=[%d]", runredlights, fatiguedrive);
    VR_LOGD("DrvInfo speeding=[%d], changelane=[%d]", speeding, changelane);
    VR_LOGD("DrvInfo hardaccelerate=[%d], harddecelerate=[%d]", hardaccelerate, harddecelerate);
    VR_LOGD("DrvInfo hardcornering=[%d], turnwithnosignal=[%d]", hardcornering, turnwithnosignal);
    VR_LOGD("DrvInfo reqTimeInterval=[%d]", reqTimeInterval);

    if (reqTimeInterval == 7) {
        m_DrvInfo.set_recentlyweek(total);
    }
    else if (reqTimeInterval == 30) {
        m_DrvInfo.set_recentlymouth(total);
    }
    else if (reqTimeInterval == 365) {
        m_DrvInfo.set_recentlyyear(total);
    }

    switch (m_DrvInfoState) {
    case State_User_Command:
        resultXml = (boost::format(sendToFCDrvInfoXml) % runredlights % fatiguedrive % speeding % changelane
                         % hardaccelerate % harddecelerate %  hardcornering % turnwithnosignal % total).str();
        sendDrvInfoToFC(resultXml);
        break;
    case State_Engine_Init:
        sendDrvInfoToiAutoLink();
        break;
    default:
        break;
    }
}

void VR_NCDrvInfoProxy::sendDrvInfoToFC(const VR_String& evaluateInfo)
{
    VR_LOGD("DrvInfo_sendDrvInfoToFC = [%s]", evaluateInfo.c_str());

    std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
    noitfyProperty->set_sender("fc");
    noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyCommonAction);
    noitfyProperty->mutable_vuicommonaction()->set_action(evaluateInfo);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
}

void VR_NCDrvInfoProxy::sendDrvInfoToiAutoLink()
{
    VR_LOGD("DrvInfo_sendDrvInfoToiAutoLink");

    std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
    noitfyProperty->set_sender("iautolink");
    noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyAlexaDrvInfo);

    VrNotifyAlexaDrvInfo* pDrvInfo = noitfyProperty->mutable_alexadrvinfo();
    if (m_DrvInfo.has_recentlymouth() && m_DrvInfo.has_recentlyweek() && m_DrvInfo.has_recentlyyear()) {
        pDrvInfo->CopyFrom(m_DrvInfo);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
        VR_LOGD("DrvInfo_sendDrvInfoToiAutoLink success !!!");
    }
}

time_t VR_NCDrvInfoProxy::StringToDatetime(const VR_String& str)
{
    tm tm_;
    std::vector<std::string> tokens;
    boost::split(tokens, str, boost::is_any_of(" -:."));

    if (tokens.size() >= 6) {
        tm_.tm_year = atoi(tokens.at(0).c_str()) - 1900;
        tm_.tm_mon = atoi(tokens.at(1).c_str()) - 1;
        tm_.tm_mday = atoi(tokens.at(2).c_str());
        tm_.tm_hour = atoi(tokens.at(3).c_str());
        tm_.tm_min = atoi(tokens.at(4).c_str());
        tm_.tm_sec = atoi(tokens.at(5).c_str());
        tm_.tm_isdst = 0;
    }

    time_t t_ = mktime(&tm_);
    return t_;
}

int VR_NCDrvInfoProxy::DateTimeInterval(const VR_String& beginTime, const VR_String& endTime)
{
    int time = difftime(StringToDatetime(beginTime), StringToDatetime(endTime)) / 3600.0 / 24.0;
    return time;
}

void VR_NCDrvInfoProxy::setDrvInfoState(DrvInfoState state)
{
    VR_LOGD("DrvInfo_setDrvInfoState [%d]", state);
    nutshell::NCAutoSync  lock(syncObj);
    m_DrvInfoState = state;
}

void VR_NCDrvInfoProxy::onUnBind()
{
    VR_LOGD("DrvInfo_onUnBind");
    m_spDrvInfoProxy->unbindService();
}

void VR_NCDrvInfoProxy::Register()
{
    VR_LOGD("DrvInfo_Register");
}

void VR_NCDrvInfoProxy::unRegister()
{
    VR_LOGD("DrvInfo_unRegister");
}

/* EOF */
