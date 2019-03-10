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
 * @file VR_DailogEngineMessageBuilder.cpp
 * @brief Declaration file of class VR_DailogEngineMessageBuilder.
 *
 * This file includes the declaration of class VR_DailogEngineMessageBuilder.
 *
 * @attention used for C++ only.
 */

#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>

#include "VR_Log.h"
#include "VR_DailogEngineMessageBuilder.h"
#include "navi/Voice/VoiceRecog/RequestVrStartRecognition.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDictation.pb.h"

using namespace navi::VoiceRecog;

VR_DialogEngineMessageBuilder::VR_DialogEngineMessageBuilder()
{
    VR_LOGD_FUNC();
}

VR_String VR_DialogEngineMessageBuilder::CreateStartDialogMsg(const int& type)
{
    VR_String state = MsgTypeEnumToXML(type);

    const VR_String startXml =   "<event name=\"startAgent\">"
                                            "<agent>%1%</agent>"
                                        "</event>";
    VR_String retv = (boost::format(startXml) % state).str();
    VR_LOGI("startAgent xml=[%s]", retv.c_str());
    return retv;
}

VR_String  VR_DialogEngineMessageBuilder::CreateSDLStartDialogMsg(const int &type,
                                                                  int timeout,
                                                                  const std::string &prompt)
{
    VR_String state = MsgTypeEnumToXML(type);

    const VR_String startXml =     "<event name=\"startAgent\" vrmode = \"sdlvr\">"
                                       "<agent>%1%</agent>"
                                       "<timeout>%2%</timeout>"
                                       "<prompt>%3%</prompt>"
                                   "</event>";

    VR_String retv = (boost::format(startXml) % state % timeout % prompt).str();
    return retv;
}

VR_String VR_DialogEngineMessageBuilder::CreateMMVRStartDialogMsg(const int &type,
                                                                  const std::string &strprompt,
                                                                  const int screentype
                                                                  )
{
    VR_String state = MsgTypeEnumToXML(type);

    const VR_String  startXml =  "<event name=\"startAgent\" vrmode = \"mmvr\">"
                                     "<agent>%1%</agent>"
                                     "<prompt>%2%</prompt>"
                                     "<screentype>%3%</screentype>"
                                 "</event>";

    VR_String retv = (boost::format(startXml) % state % strprompt % screentype).str();
    return retv;
}

VR_String VR_DialogEngineMessageBuilder::CreatePrepareDialogMsg(const int& type)
{
    VR_String state = MsgTypeEnumToXML(type);

    const VR_String startXml =   "<event name=\"prepare\">"
                                                        "<agent>%1%</agent>"
                                                    "</event>";
    VR_String retv = (boost::format(startXml) % state).str();
    VR_LOGI("prepare xml=[%s]", retv.c_str());
    return retv;
}

VR_String VR_DialogEngineMessageBuilder::CreateCancelDialogMsg()
{
    const VR_String startXml =   "<event name=\"cancel\"></event>";
    VR_LOGI("cancel xml=[%s]", startXml.c_str());
    return startXml;
}

VR_String VR_DialogEngineMessageBuilder::CreateGetHintsDialogMsg(const int& type)
{
    VR_String state = MsgTypeEnumToXML(type);
    const VR_String hintXml =   "<event name=\"getHints\">"
                                            "<agentName>%1%</agentName>"
                                            "<pageSize>5</pageSize>"
                                        "</event>";
    VR_String retv = (boost::format(hintXml) % state).str();
    VR_LOGI("getHints xml=[%s]", retv.c_str());
    return retv;
}

VR_String VR_DialogEngineMessageBuilder::CreateStopEngineDialogMsg(const int &type)
{
    VR_String state = MsgTypeEnumToXML(type);

    const VR_String stopXml =   "<event op=\"Agent.stop\">"
                                            "<AgentName>%1%</AgentName>"
                                        "</event>";
    VR_String retv = (boost::format(stopXml) % state).str();
    VR_LOGI("Agent.stop xml=[%s]", retv.c_str());
    return retv;
}

VR_String VR_DialogEngineMessageBuilder::CreateInitEngineDialogMsg(const int &type)
{
    VR_String state = MsgTypeEnumToXML(type);

    const VR_String initXml =   "<event op=\"Agent.init\">"
                                            "<AgentName>%1%</AgentName>"
                                        "</event>";
    VR_String retv = (boost::format(initXml) % state).str();
    VR_LOGI("Agent.init xml=[%s]", retv.c_str());
    return retv;
}

VR_String VR_DialogEngineMessageBuilder::CreateUpdateGrammerDialogMsg(const int &type)
{
    VR_String state = MsgTypeEnumToXML(type);

    const VR_String updateXml =   "<event op=\"Agent.update\">"
                                            "<AgentName>%1%</AgentName>"
                                        "</event>";
    VR_String retv = (boost::format(updateXml) % state).str();
    VR_LOGI("Agent.update xml=[%s]", retv.c_str());
    return retv;
}

VR_String VR_DialogEngineMessageBuilder::CreateDebugMenuMsg(const std::string &option, const int &value)
{
    const VR_String debugMenuXml = "<event name=\"debugMenu\">"
                                              "<option>%1%</option>"
                                              "<value>%2%</value>"
                                          "</event>";
    VR_String retv = (boost::format(debugMenuXml) % option % value).str();
    VR_LOGI("debugMenu xml=[%s]", retv.c_str());
    return retv;
}

VR_String VR_DialogEngineMessageBuilder::CreateTtsPlayResultMsg()
{
    const VR_String TtsPlayResultXml = "<action-result agent=\"prompt\" op=\"playTts\" errcode=\"0\">"
                                        "</action-result>";
    VR_LOGI("playTts xml=[%s]", TtsPlayResultXml.c_str());
    return TtsPlayResultXml;
}

VR_String VR_DialogEngineMessageBuilder::CreateShowSubMenuMsg()
{
    const VR_String strMsg = "<event name=\"showSubMenu\">"
                             "</event>";
    VR_LOGI("strMsg=[%s]", strMsg.c_str());
    return strMsg;
}

VR_String VR_DialogEngineMessageBuilder::CreateAwakenResultMsg(const bool bRet)
{
    VR_String strMsg = "<action-result agent=\"system\" op=\"awake\""
                            "errcode=\"%1%\">"
                        "</action-result>";
    VR_INT32 value = bRet ? 0 : 1;
    VR_String retv = (boost::format(strMsg) % value).str();
    VR_LOGI("xml=[%s]", retv.c_str());
    return retv;
}

VR_String VR_DialogEngineMessageBuilder::CreateQuitResultMsg(const bool bRet)
{
    VR_String strMsg = "<action-result agent=\"system\" op=\"quit\""
                            "errcode=\"%1%\">"
                        "</action-result>";
    VR_INT32 value = bRet ? 0 : 1;
    VR_String retv = (boost::format(strMsg) % value).str();
    VR_LOGI("xml=[%s]", retv.c_str());
    return retv;
}

VR_String VR_DialogEngineMessageBuilder::CreateAudioInOpeMsg(const bool bRet)
{
    VR_String strMsg = "<event name=\"AudioInOpe\">"
                           "<operate>%1%</operate>"
                        "</event>";
    VR_String value = bRet ? "open" : "close";
    VR_String retv = (boost::format(strMsg) % value).str();
    VR_LOGI("xml=[%s]", retv.c_str());
    return retv;
}

VR_String VR_DialogEngineMessageBuilder::PvrTTSRequestMsg(const std::string &status)
{
    const VR_String PvrTtsPlayResultXml =   "<event name=\"PvrTTS\">"
                                                      "<StatusName>%1%</StatusName>"
                                                   "</event>";
    VR_String retv = (boost::format(PvrTtsPlayResultXml) % status).str();
    VR_LOGI("PvrTTSRequestMsg xml=[%s]", retv.c_str());
    return retv;
}

const VR_String &VR_DialogEngineMessageBuilder::MsgTypeEnumToXML(const int& type)
{
    VR_LOGI("type=[%d]", type);
    static const VR_String& FAILED_STR = "";
    static const VoiceMap<int, VR_String>::type recogVrDomainTypeMap = boost::assign::map_list_of
            (RequestVrStartRecognition_VrDomainType_VrDomainType_Golbal,         "topmenu")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_Phone,          "phone")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_Navigation,     "navi")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_Apps,           "apps")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_Audio,          "music")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_Info,           "information")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_HVAC,           "climate")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_InCalling,      "in_call")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_InMessage,      "in_message")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_Training,       "adaptation")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_SendTone,       "sendtone")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_VoiceTag,       "voicetag")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_POI,            "poi")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_Radio,          "radio")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_DictUndefine,   "DictUndefine")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_DictMusic,      "music_key_dict")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_DictHFD,        "hfd_key_dict")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_DictMessage,    "msg_dict")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_DictNavigation, "navi_key_dict")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_DictState,      "states_key_dict")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_DictCity,       "cities_key_dict")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_DictStreet,     "streets_key_dict")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_DictPOI,        "poi_key_dict")
            (RequestVrStartRecognition_VrDomainType_VrDomainType_DictOthers,     "generic_key_dict");

    VoiceMap<int, VR_String>::const_iterator itr = recogVrDomainTypeMap.find(type);
    if (itr != recogVrDomainTypeMap.end()) {
        VR_LOGI("type=[%s]", (itr->second).c_str());
        return itr->second;
    }
    else {
        VR_LOGI("type is not find");
        return FAILED_STR;
    }
}

VR_String VR_DialogEngineMessageBuilder::UsrOperationMsg(const std::string &type, int arg1)
{
    VR_LOGI("type=[%s],arg1=[%d]", type.c_str(), arg1);

    const VoiceMap<VR_String, VR_String>::type operationTypeMap1 = boost::assign::map_list_of
            ("ptt",             "<event name=\"hardKey\">""<keycode value=\"ptt\"/>""</event>")
            // ("phone",           "<event name=\"hardKey\">""<keycode value=\"phone\"/>""</event>")
            ("startOver",       "<event name=\"startover\">""</event>")
            ("pause",           "<event name=\"buttonPressed\">""<keycode value=\"pause\"/>""</event>")
            ("resume",          "<event name=\"buttonPressed\">""<keycode value=\"resume\"/>""</event>")
            ("nextPage",        "<event name=\"buttonPressed\">""<keycode value=\"nextPage\"/>""</event>")
            ("prevPage",        "<event name=\"buttonPressed\">""<keycode value=\"prevPage\"/>""</event>")
            ("lastPage",        "<event name=\"buttonPressed\">""<keycode value=\"lastPage\"/>""</event>")
            ("firstPage",       "<event name=\"buttonPressed\">""<keycode value=\"firstPage\"/>""</event>")
            ("confirmYes",      "<event name=\"buttonPressed\">""<keycode value=\"confirmYes\"/>""</event>")
            ("confirmNo",       "<event name=\"buttonPressed\">""<keycode value=\"confirmNo\"/>""</event>")
            ("back",            "<event name=\"buttonPressed\">""<keycode value=\"back\"/>""</event>")
            ("alongRoute",      "<event name=\"buttonPressed\">""<keycode value=\"alongRoute\"/>""</event>")
            ("nearDestination", "<event name=\"buttonPressed\">""<keycode value=\"nearDestination\"/>""</event>")
            ("help",            "<event name=\"buttonPressed\">""<keycode value=\"help\"/>""</event>")
            ("endTask",     "<event name=\"buttonPressed\">""<keycode value=\"ptt_hard_key_press_special_in_endtaskstatus\"/>""</event>")
            ("hardKeyCanceVR",     "<event name=\"cancel\"></event>");

    const VoiceMap<VR_String, VR_String>::type operationTypeMap2 = boost::assign::map_list_of
            ("select",          "<event name=\"buttonPressed\">""<keycode value=\"select\"/>""<index>%1%</index>""</keycode>""</event>")
            ("getHints",        "<event name=\"getHints\">""<agentName>Music</agentName>""<pageSize>%1%</pageSize>""</event>");

    const VoiceMap<VR_String, VR_String>::type operationTypeMapDefault = boost::assign::map_list_of
            ("",          "<event name=\"buttonPressed\">""<keycode value=\"%1%\"/>""</event>");

    VR_String retv;
    VoiceMap<VR_String, VR_String>::const_iterator itr = operationTypeMap1.find(type);
    if (itr != operationTypeMap1.end()) {
        retv = itr->second;
        VR_LOGI("type is finded in operationTypeMap1");
    }
    else {
        itr = operationTypeMap2.find(type);
        if (itr != operationTypeMap2.end()) {
            retv = (boost::format(itr->second) % arg1).str();
            VR_LOGI("type is finded in operationTypeMap2");
        }
        else {
            retv = (boost::format(operationTypeMapDefault.begin()->second) % type).str();
            VR_LOGI("type is finded in operationTypeMapDefault");
        }
    }

    VR_LOGI("xml=[%s]", retv.c_str());
    return retv;
}
/* EOF */
