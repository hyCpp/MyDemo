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
 * @file VR_DeriveDataModel.cpp
 * @brief assign datamodel
 *
 *
 * @attention used for C++ only.
 */
#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "stdafx.h"
#include "VR_DEMessageBuilder.h"
#include <boost/format.hpp>
#include <sstream>
#include "Vr_Asr_Engine.h"
#include "pugixml.hpp"

using namespace N_Vr;
using namespace N_Asr;

#define VR_DISPLAY_VOLUME "<data>"\
    "<display agent=\"Common\" content=\"Volume\">"\
    "<type>asr</type>"\
    "<volume>%1%</volume>"\
    "<volumeStatus>%2%</volumeStatus>"\
    "</display>"\
    "</data>"

#define VR_ACTION_PLAYTTS "<action agent=\"prompt\" op=\"playTts\">"\
    "<language>%1%</language>"\
    "<text>%2%</text>"\
    "<bargein>%3%</bargein>"\
    "</action>"



#define VR_ACTION_PLAYTTSLIST "<action agent=\"prompt\" op=\"playTts\">"\
    "<language>%1%</language>"\
    "<bargein>%2%</bargein>"\
    "</action>"

#define VR_ACTION_PLAYBEEP "<action agent=\"prompt\" op=\"playBeep\">"\
    "<beepFile>%1%</beepFile>"\
    "<bargein>%2%</bargein>"\
    "</action>"

#define VR_ACTION_PLAY_VOICETAG_MIXED_TTS "<action agent=\"prompt\" op=\"playTtsList\">"\
    "<language>%1%</language>"\
    "</action>"

#define VR_ACTION_STOPTTS "<action agent=\"prompt\" op=\"stopTts\"><reqId>123</reqId></action>"
#define VR_ACTION_STOPBEEP "<action agent=\"prompt\" op=\"stopBeep\"></action>"
#define VR_ACTION_STOP_VOICETAG_MIXED_TTS "<action agent=\"prompt\" op=\"stopTtsList\"><reqId>123</reqId></action>"
#define VR_ACTION_CLOSESESSION "<action name=\"closeSession\" />"
#define VR_DISPLAY_QUITVRAPP "<display agent='Common' content='QuitVRApp'></display>"
#define VR_DISPLAY_VRSTATEQUIT "<display agent='Common' content='VRState'><state>quit</state><engineType>local</engineType></display>"
#define VR_DISPLAY_VRSTATEQUIT_SERVER "<display agent='Common' content='VRState'><state>quit</state><engineType>server</engineType></display>"

VR_DEMessageBuilder::~VR_DEMessageBuilder()
{

}

VR_DEMessageBuilder::VR_DEMessageBuilder(VR_DECommonIF* common)
{
    m_pDECommonIF = common;
}


void VR_DEMessageBuilder::buildQuitVRAppDisplay(std::string& outMsg)
{
    outMsg = VR_DISPLAY_QUITVRAPP;
}

void VR_DEMessageBuilder::buildVRStateQuitDisplay(std::string& outMsg, bool HybridVRFlag)
{
    if (HybridVRFlag) {
        outMsg = VR_DISPLAY_VRSTATEQUIT_SERVER;
    }
    else {
        outMsg = VR_DISPLAY_VRSTATEQUIT;
    }
}

void VR_DEMessageBuilder::buildPlayBeepAction(std::string beepPath, std::string& outMsg)
{
    VR_LOGD_FUNC();
    std::stringstream oss;
    oss << std::boolalpha << m_pDECommonIF->getVROverBeep();
    std::string isBargein = oss.str();
    outMsg = (boost::format(VR_ACTION_PLAYBEEP) % beepPath % isBargein).str();
}

void VR_DEMessageBuilder::buildStopBeepAction(std::string& outMsg)
{
    VR_LOGD_FUNC();
    outMsg = VR_ACTION_STOPBEEP;
}

void VR_DEMessageBuilder::buildPlayTTSAction(std::string txt, std::string& outMsg)
{
    VR_LOGD_FUNC();
    std::string language = m_pDECommonIF->getVRLanguage();
    // std::stringstream oss;
    // oss << std::boolalpha << m_pDECommonIF->getVROverPrompt();
    std::string isBargein = "false";

    outMsg = (boost::format(VR_ACTION_PLAYTTS) % language % txt % isBargein).str();
}

void VR_DEMessageBuilder::buildPlayTTSAction(const VoiceList<std::string>::type& txtList, std::string& outMsg)
{
    VR_LOGD_FUNC();
    std::string language = m_pDECommonIF->getVRLanguage();
    // std::stringstream oss;
    // oss << std::boolalpha << m_pDECommonIF->getVROverPrompt();
    std::string isBargein = "false";

    std::string msg = (boost::format(VR_ACTION_PLAYTTSLIST) % language % isBargein).str();
    pugi::xml_document doc;
    if (!doc.load_string(msg.c_str())) {
        return;
    }
    pugi::xml_node actionNode = doc.select_node("//action").node();
    if (!actionNode) {
        return;
    }
    VoiceList<std::string>::const_iterator it = txtList.begin();
    while (it != txtList.end()) {
        std::string txt = *it;
        pugi::xml_node txtNode = actionNode.append_child("text");
        txtNode.text().set(txt.c_str());
        ++it;
    }
    std::stringstream ss;
    doc.print(ss);
    outMsg = ss.str();
}

void VR_DEMessageBuilder::buildStopTTSAction(std::string& outMsg)
{
    VR_LOGD_FUNC();
    outMsg = VR_ACTION_STOPTTS;
}

void VR_DEMessageBuilder::buildPlayVoiceTagMixedTTSAction(std::string voicetagFile, std::string prompt, std::string& outMsg)
{
    VR_LOGD_FUNC();
    std::string language = m_pDECommonIF->getVRLanguage();
    std::string msg = (boost::format(VR_ACTION_PLAY_VOICETAG_MIXED_TTS) % language).str();
    pugi::xml_document doc;
    if (!doc.load_string(msg.c_str())) {
        return;
    }
    pugi::xml_node actionNode = doc.select_node("//action").node();
    if (!actionNode) {
        return;
    }
    if (!voicetagFile.empty()) {
        actionNode.append_child("beepFile").text().set(voicetagFile.c_str());
    }
    if (!prompt.empty()) {
        actionNode.append_child("text").text().set(prompt.c_str());
    }
    std::stringstream ss;
    doc.print(ss);
    outMsg = ss.str();
}

void VR_DEMessageBuilder::buildStopVoiceTagMixedTTSAction(std::string& outMsg)
{
    VR_LOGD_FUNC();
    outMsg = VR_ACTION_STOP_VOICETAG_MIXED_TTS;
}

void VR_DEMessageBuilder::buildCloseSessionAction(std::string& outMsg)
{
    VR_LOGD_FUNC();
    outMsg = VR_ACTION_CLOSESESSION;
}

void VR_DEMessageBuilder::buildVolumeDisplay(C_Event_Notify const& notify, std::string& outMsg)
{
    VR_LOGD_FUNC();
    int volume = notify.Get_Signal_Level_Energy();
    C_Event_Notify::E_Event_Notify_Type volType = notify.Get_Event_Notify_Type();

    std::string volTypeStr = "suitable";
    switch (volType) {
    case C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Too_Quiet:
    case C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_No_Signal:
    {
        volTypeStr = "tooQuiet";
        break;
    }

    case C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Overload:
    case C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Too_Noisy:
    {
        volTypeStr = "tooLoud";
        break;
    }
    default:
        break;
    }

    char buffer[50] = {};
    snprintf (buffer, 50, "%d", volume);
    std::string volumeStr = buffer;
    // if (isNuance) {
    //     double vRel = pow(10, volume / (20.0 * 100.0)) * 100;
    //     volume = (int)(vRel + 0.5);
    //     char buffer[50] = {};
    //     snprintf (buffer, 50, "%d", volume);
    //     volumeStr = buffer;
    // }

    outMsg = (boost::format(VR_DISPLAY_VOLUME) % volumeStr
              % volTypeStr
              ).str();
}

/* EOF */





