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
 * @file VR_TunerStatusManager.cpp
 * @brief Declaration file of class VR_TunerStatusManager.
 *
 * This file includes the declaration of class VR_TunerStatusManager and Derived classes.
 *
 * @attention used for C++ only.
 */

#include "VR_TunerStatusManager.h"
#include "VR_Log.h"
#include "EV_EventSender.h"
#include "BL_File.h"
#include "BL_DataStorage.h"
#include "CL_AbstractFile.h"
#include "VR_Configure.h"

#include "navi/Voice/VoiceRecog/VrNotifyiAutoProxy.pb.h"
#include "navi/Voice/VoiceRecog/RequestVriAutoProxy.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"

using namespace navi::VoiceRecog;

#define        TUNER_LAST_STATUS_FILE           "tuner_last_status.txt"
#define        TUNER_AM_LAST_STATUS_FILE        "tuner_AM_last_status.txt"
#define        TUNER_FM_LAST_STATUS_FILE        "tuner_FM_last_status.txt"


VR_TunerStatusManager::VR_TunerStatusManager()
{
    VR_LOGD_FUNC();
}

VR_TunerStatusManager::~VR_TunerStatusManager()
{
    VR_LOGD_FUNC();
}

bool VR_TunerStatusManager::CheckTunerStationFile()
{
    VR_LOGD_FUNC();
    bool checkResult = true;
    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadFile(GetTunerStationFilePath(TUNER_LAST_STATUS_FILE))) {
        if (xmlStrParse.LoadString(VR_TUNER_PLAY_CONTROL_REQUEST_FM_XML)) {
            checkResult = xmlStrParse.SaveFile(GetTunerStationFilePath(TUNER_LAST_STATUS_FILE));
        }
        else {
            VR_LOGE("load_string error!");
            checkResult = false;
        }
    }
    else {
        VR_LOGE("tuner_last_status.txt has been exist!");
    }

    if (!xmlStrParse.LoadFile(GetTunerStationFilePath(TUNER_AM_LAST_STATUS_FILE))) {
        if (xmlStrParse.LoadString(VR_TUNER_PLAY_CONTROL_REQUEST_AM_XML)) {
            checkResult = xmlStrParse.SaveFile(GetTunerStationFilePath(TUNER_AM_LAST_STATUS_FILE));
        }
        else {
            VR_LOGE("load_string error!");
            checkResult = false;
        }
    }
    else {
        VR_LOGE("tuner_AM_last_status.txt has been exist!");
    }

    if (!xmlStrParse.LoadFile(GetTunerStationFilePath(TUNER_FM_LAST_STATUS_FILE))) {
        if (xmlStrParse.LoadString(VR_TUNER_PLAY_CONTROL_REQUEST_FM_XML)) {
            checkResult = xmlStrParse.SaveFile(GetTunerStationFilePath(TUNER_FM_LAST_STATUS_FILE));
        }
        else {
            VR_LOGE("load_string error!");
            checkResult = false;
        }
    }
    else {
        VR_LOGE("tuner_FM_last_status.txt has been exist!");
    }

    return checkResult;
}

void VR_TunerStatusManager::ReplyTunerGetStatus(VR_String &strId)
{
    VR_LOGD_FUNC();
    VR_String strXml = VR_TUNER_GET_STATUS_RESP_XML;
    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
        return;
    }

    VR_String strError = VR_TUNER_RESULT_OK;
    VR_String strIsRadioOn = "fasle";
    VR_String strStationName = "";
    VR_String strStationFreq = "";
    VR_String strStationWaveband = VR_TUNER_MODE_AM;
    if (VR_ConfigureIF::Instance()->getBRadioStatus()) {
        strIsRadioOn = "true";
        VR_XmlStrParse xmlStrParseFromFile;
        if (xmlStrParseFromFile.LoadFile(GetTunerStationFilePath(TUNER_LAST_STATUS_FILE))) {
            strStationName = xmlStrParseFromFile.GetXMLValue(VR_TUNER_ACTION_STATION_NAME);
            strStationFreq = xmlStrParseFromFile.GetXMLValue(VR_TUNER_TUNER_STATION_FREQ);
            strStationWaveband = xmlStrParseFromFile.GetXMLValue(VR_TUNER_ACTION_STATION_WAVEBAND);
        }
    }

    xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
    xmlStrParse.setTextTo(VR_XPATH_ACTION_ID, strId);
    xmlStrParse.setTextTo(VR_TUNER_ERROR_NO, strError);
    xmlStrParse.setTextTo(VR_TUNER_IS_RADIO_ON, strIsRadioOn);
    xmlStrParse.setTextTo(VR_TUNER_STATION_NAME, strStationName);
    xmlStrParse.setTextTo(VR_TUNER_STATION_FREQ, strStationFreq);
    xmlStrParse.setTextTo(VR_TUNER_STATION_WAVEBAND, strStationWaveband);

    VR_String strMsg = xmlStrParse.TransferXMLtoString();
    SendReqToNATP(strMsg);
}

void VR_TunerStatusManager::QueryTunerPlayControl(const VR_String &strXml)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGE("load_string error");
        return;
    }
    VR_String strId = xmlStrParse.GetXMLValue(VR_XPATH_QUERY_ID);
    VR_String strType = xmlStrParse.GetXMLValue(VR_TUNER_TUNER_ACTION_TYPE);

    VR_String strMsg = strXml;
    if (VR_TUNER_TURN_ON == strType) {
        if (!xmlStrParse.LoadFile(GetTunerStationFilePath(TUNER_LAST_STATUS_FILE))) {
            ReplyTunerPlayControl(strId);
            return;
        }
        else {
            xmlStrParse.setTextTo(VR_XPATH_ID, strId);
            strMsg = xmlStrParse.TransferXMLtoString();
        }
    }
    else if (VR_TUNER_SET_LATEST_AM == strType) {
        if (!xmlStrParse.LoadFile(GetTunerStationFilePath(TUNER_AM_LAST_STATUS_FILE))) {
            ReplyTunerPlayControl(strId);
            return;
        }
        else {
            xmlStrParse.setTextTo(VR_XPATH_ID, strId);
            strMsg = xmlStrParse.TransferXMLtoString();
            xmlStrParse.SaveFile(GetTunerStationFilePath(TUNER_LAST_STATUS_FILE));
        }
    }
    else if (VR_TUNER_SET_LATEST_FM == strType) {
        if (!xmlStrParse.LoadFile(GetTunerStationFilePath(TUNER_FM_LAST_STATUS_FILE))) {
            ReplyTunerPlayControl(strId);
            return;
        }
        else {
            xmlStrParse.setTextTo(VR_XPATH_ID, strId);
            strMsg = xmlStrParse.TransferXMLtoString();
            xmlStrParse.SaveFile(GetTunerStationFilePath(TUNER_LAST_STATUS_FILE));
        }
    }
    else if (VR_TUNER_SET_FREQUENCY == strType) {
        VR_INT32 nStationFreq = atoi(xmlStrParse.GetXMLValue(VR_TUNER_TUNER_STATION_FREQ).c_str());
        if (VR_TUNER_MODE_AM_MAX >= nStationFreq) {
            xmlStrParse.AddNode(VR_TUNER_PARAMS_PATH, VR_TUNER_WAVEBAND, VR_TUNER_MODE_AM);
            strMsg = xmlStrParse.TransferXMLtoString();
            xmlStrParse.SaveFile(GetTunerStationFilePath(TUNER_AM_LAST_STATUS_FILE));
            xmlStrParse.SaveFile(GetTunerStationFilePath(TUNER_LAST_STATUS_FILE));
        }
        else {
            xmlStrParse.AddNode(VR_TUNER_PARAMS_PATH, VR_TUNER_WAVEBAND, VR_TUNER_MODE_FM);
            strMsg = xmlStrParse.TransferXMLtoString();
            xmlStrParse.SaveFile(GetTunerStationFilePath(TUNER_FM_LAST_STATUS_FILE));
            xmlStrParse.SaveFile(GetTunerStationFilePath(TUNER_LAST_STATUS_FILE));
        }
    }
    std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
    noitfyProperty->set_sender("NATP");
    noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyCommonAction);
    noitfyProperty->mutable_vuicommonaction()->set_action(strMsg);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
}

void VR_TunerStatusManager::ReplyTunerPlayControl(VR_String &strId)
{
    VR_LOGD_FUNC();
    VR_String strXml = VR_TUNER_PLAY_CONTROL_RESP_XML;
    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
        return;
    }

    xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
    xmlStrParse.setTextTo(VR_XPATH_ACTION_ID, strId);
    xmlStrParse.setTextTo(VR_TUNER_ERROR_NO, VR_TUNER_RESULT_EMPTY);

    VR_String strMsg = xmlStrParse.TransferXMLtoString();
    SendReqToNATP(strMsg);
}

VR_String VR_TunerStatusManager::GetTunerStationFilePath(VR_String strTunerFileName)
{
    // 1.if not exist vr dir and create it
    BL_Dir vrDir(BL_FILE_PREFIX_RW);
    bool result = (CL_TRUE == vrDir.MakeDir(VR_VR_WORK_FILE_RWDISK_FOLDERNAME,
                                             CL_FALSE));
    if (!result) {
        VR_LOGD("create %s directory failed", VR_VR_WORK_FILE_RWDISK_FOLDERNAME);
        return("");
    }

    // 2.load work vr path
    BL_String vrFilePath;
    BL_File file(BL_FILE_PREFIX_RW);
    result = (CL_TRUE == file.GetLogicalPath(VR_VR_WORK_FILE_RWDISK_FOLDERNAME,
                                                   vrFilePath));
    if (!result) {
        VR_LOGD("get %s absolute path failed", VR_VR_WORK_FILE_RWDISK_FOLDERNAME);
        return("");
    }

    vrFilePath = vrFilePath + "/" + strTunerFileName.c_str();

    VR_LOGD("get vui root path:%s", vrFilePath.GetString());
    return(vrFilePath.GetString());
}

void VR_TunerStatusManager::SendReqToNATP(VR_String &strXml)
{
    VR_LOGD("strXml=[%s]", strXml.c_str());
    std::unique_ptr<VrNotifyiAutoProxy> protoMsg(VrNotifyiAutoProxy::default_instance().New());
    protoMsg->set_sender("vr_s");
    protoMsg->set_funccode(VrNotifyiAutoProxy_VrFunc_NotifyRecogMessage);
    protoMsg->mutable_recogmsg()->set_xml_out(strXml);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFIAUTOPROXY, protoMsg.release());
}

VR_UINT32 VR_TunerStatusManager::GetActionId()
{
    VR_IdGenerator nId;
    return nId.GetID();
}

/* EOF */
