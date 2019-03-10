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

#include "stdafx.h"
#include <stdlib.h>
#include <sstream>
#include <boost/assign/list_of.hpp>

#include "VR_TransformXmlToXml.h"
#include "VR_ConfigureIF.h"

// amazon
const VoiceMap<VR_String, VR_String>::type alexaVrState = boost::assign::map_list_of
        ("processing", "SYSTEM_STATE_SPEECH")
        ("listening",  "SYSTEM_STATE_RECOGNITION")
        ("playing",    "SYSTEM_STATE_PROMPTING")
        ("idle",       "SYSTEM_STATE_BASIC_INIT");

VR_TransformXmlToXml::VR_TransformXmlToXml()
{
    VR_LOGD_FUNC();
}

VR_TransformXmlToXml::~VR_TransformXmlToXml()
{
    VR_LOGD_FUNC();
}

VR_String VR_TransformXmlToXml::Transform(int id, const std::string &msg, emVriAutoProxyType &emType)
{
    VR_LOGI("id=[%d],msg=[%s]", id, msg.c_str());

    VR_String strXml;
    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(msg)) {
        VR_LOGE("load_string error");
        return strXml;
    }
    strXml = msg;
    VR_String strText;
    VR_String strAgent;
    VR_String strState;
    VR_String strStateAction;
    bool bRet = xmlStrParse.GetAttributeValue("//display", "content", strText);
    VR_LOGI("strText=[%s]", strText.c_str());
    if (bRet) {
        if ("VRState" == strText) {
            xmlStrParse.GetNodeText("//state", strState);
            emType = VriAutoProxyType_System_SetState;
            if (alexaVrState.end() != alexaVrState.find(strState)) {
                strStateAction = "SYSTEM_STATE_BEGIN";
                if ("idle" == strState) {
                    strStateAction = "SYSTEM_STATE_END";
                }
                strText = alexaVrState.at(strState);
                strXml = CreateStateXML(id, strText, strStateAction);
            }
        }
        else if ("VRDictationState" == strText) {
            xmlStrParse.GetNodeText("//state", strState);
            emType = VriAutoProxyType_Dictation_SetState;
            strXml = strState;
            VR_LOGI("strXml=[%s]", strXml.c_str());
        }
        else if ("dictationResult" == strText) {
            emType = VriAutoProxyType_Dictation_RecResult;
            strXml = msg;
            VR_LOGI("strXml=[%s]", strXml.c_str());
        }
        else if ("Volume" == strText) {
            VR_String strVolume;
            xmlStrParse.GetNodeText("//volume", strVolume);
            emType = VriAutoProxyType_System_SetState;
            strXml = CreateVolumeXML(id, strVolume);
        }
        else if ("QuitVRApp" == strText) {
            emType = VriAutoProxyType_System_SetState;
            strState = "SYSTEM_STATE_DIALOG";
            strStateAction = "SYSTEM_STATE_END";
            strXml = CreateStateXML(id, strState, strStateAction);
        }
        else if ("ScreenDisplay" == strText) {
            xmlStrParse.GetNodeText("//content", strText);
            if ("FindPOIList" == strText) {
                emType = VriAutoProxyType_Vehicle_Control;
                strXml = GetPoiListXml(strXml);
            }
        }
    }

    bRet = xmlStrParse.GetAttributeValue("//event-result", "name", strText);
    VR_LOGI("strText=[%s]", strText.c_str());
    if (bRet) {
        if ("startAgent" == strText) {
            emType = VriAutoProxyType_System_SetState;
            strState = "SYSTEM_STATE_DIALOG";
            strStateAction = "SYSTEM_STATE_BEGIN";
            strXml = CreateStateXML(id, strState, strStateAction);
        }
    }

    bRet = xmlStrParse.GetAttributeValue("//action", "op", strText);
    bool bAgent = xmlStrParse.GetAttributeValue("//action", "agent", strAgent);
    VR_LOGI("strText=[%s]", strText.c_str());

    if (bAgent && bRet) {
        if (strAgent.compare("suggestion") == 0 && strText.compare("quitScreen") == 0) {
            emType = VriAutoProxyType_Suggestion_QuitScreen;
        }
    }

    if (bRet) {
        if ("sunroofControl" == strText) {
            emType = VriAutoProxyType_Vehicle_Control;
            xmlStrParse.GetNodeText("//state", strState);
            strXml = CreateSunroofXML(id, strState);
        }
        else if ("showPhoneCall" == strText) {
            emType = VriAutoProxyType_HMI_ShowList;
            VR_String strName,strNumber,strType;
            xmlStrParse.GetNodeText("//phonename", strName);
            xmlStrParse.GetNodeText("//phonenumber", strNumber);
            xmlStrParse.GetNodeText("//phonetype", strType);
            strXml = CreateShowPhoneCallXML(id, strName, strNumber, strType);
        }
        else if ("open" == strText) {
            emType = VriAutoProxyType_Vehicle_Control;
            strXml = CreatePlayMusicXML(id);
        }
        else if ("close" == strText) {
            emType = VriAutoProxyType_Vehicle_Control;
            strXml = CreateQuitMusicXML(id);
        }
        else if ("dial" == strText) {
            emType = VriAutoProxyType_Vehicle_Control;
            VR_String strName,strNumber;
            xmlStrParse.GetNodeText("//phoneInfo/contactName", strName);
            xmlStrParse.GetNodeText("//phoneInfo/number", strNumber);
            strXml = CreatePhoneCallXML(id, strName, strNumber);
        }
        else if ("calculateRoute" == strText) {
            emType = VriAutoProxyType_Vehicle_Control;
            VR_String strLon,strLat;
            xmlStrParse.GetNodeText("//guidePoint/longitude", strLon);
            xmlStrParse.GetNodeText("//guidePoint/latitude", strLat);
            strLon = TransferLonAndLat(strLon);
            strLat = TransferLonAndLat(strLat);
            strXml = CreateCalculateRouteXML(id, strLon, strLat);
        }
    }

    bRet = xmlStrParse.GetNodeText("//type", strText);
    VR_LOGI("strText=[%s]", strText.c_str());
    if ("HMI_ShowList" == strText) {
        emType = VriAutoProxyType_HMI_ShowList;
    }

    VR_LOGI("iAuto3 strXml=[%s]", strXml.c_str());
    return strXml;
}

VR_String VR_TransformXmlToXml::CreateStateXML(int id, VR_String &strState, VR_String &strStateAction)
{
    VR_String strXml;

    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(VR_ALEXA_STATE)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(id));
        xmlStrParse.setTextTo("//params/system_state", strState);
        xmlStrParse.setTextTo("//params/system_state_action", strStateAction);
        strXml = xmlStrParse.TransferXMLtoString();
    }

    VR_LOGI("strXml=[%s]", strXml.c_str());
    return strXml;
}

VR_String VR_TransformXmlToXml::CreateSunroofXML(int id, VR_String &strState)
{
    VR_String strXml;

    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(VR_VEHICLE_CONTROL)) {
        if ("open" == strState) {
            strState = "VEH_SUNROOF_FULL_OPEN";
        }
        else {
            strState = "VEH_SUNROOF_CLOSE";
        }
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(id));
        xmlStrParse.setTextTo("//params/veh_action", strState);
        strXml = xmlStrParse.TransferXMLtoString();
    }

    VR_LOGI("strXml=[%s]", strXml.c_str());
    return strXml;
}

VR_String VR_TransformXmlToXml::CreatePlayMusicXML(int id)
{
    VR_String strXml;

    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(VR_MEDIA_PLAY_LOCAL_MUSIC_XML)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(id));
        xmlStrParse.setTextTo("//params/source_id", "100");
        strXml = xmlStrParse.TransferXMLtoString();
    }

    VR_LOGI("strXml=[%s]", strXml.c_str());
    return strXml;
}

VR_String VR_TransformXmlToXml::CreateQuitMusicXML(int id)
{
    VR_String strXml;

    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(VR_MEDIA_SET_STATUS_ACTION_XML)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(id));
        xmlStrParse.setTextTo("//params/media_control_type", "MEDIA_CONTROL_TYPE_EXIT");
        strXml = xmlStrParse.TransferXMLtoString();
    }

    VR_LOGI("strXml=[%s]", strXml.c_str());
    return strXml;
}

VR_String VR_TransformXmlToXml::CreatePhoneCallXML(int id, VR_String &strName, VR_String &strNumber)
{
    VR_String strXml;

    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(VR_PHONE_MAKECALL)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(id));
        xmlStrParse.setTextTo("//params/contact_name", strName);
        xmlStrParse.setTextTo("//params/phone_number", strNumber);
        strXml = xmlStrParse.TransferXMLtoString();
    }

    VR_LOGI("strXml=[%s]", strXml.c_str());
    return strXml;
}

VR_String VR_TransformXmlToXml::CreateShowPhoneCallXML(int id, VR_String &strName, VR_String &strNumber, VR_String& strType)
{
    VR_String strXml;

    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(VR_SHOWLIST_PHONE)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(id));
        xmlStrParse.setTextTo("//params/scenario", "PHONE_SMART_LISTENING");
        xmlStrParse.setTextTo("//params/HMI_NAME", strName);
        xmlStrParse.setTextTo("//params/HMI_NUMBER", strNumber);
        xmlStrParse.setTextTo("//params/HMI_TYPE", strType);
        strXml = xmlStrParse.TransferXMLtoString();
    }

    VR_LOGI("strXml=[%s]", strXml.c_str());
    return strXml;
}

VR_String VR_TransformXmlToXml::CreateCalculateRouteXML(int id, VR_String &strLon, VR_String &strLat)
{
    VR_String strXml;

    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(VR_TO_DESTINATION_XML)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(id));
        xmlStrParse.setTextTo("//params/addr_longitude", strLon);
        xmlStrParse.setTextTo("//params/addr_latitude", strLat);
        strXml = xmlStrParse.TransferXMLtoString();
    }

    VR_LOGI("strXml=[%s]", strXml.c_str());
    return strXml;
}

VR_String VR_TransformXmlToXml::TransferLonAndLat(VR_String &strValue)
{
    return std::to_string(atof(strValue.c_str()) / (60 * 60 * 256));
}

VR_String VR_TransformXmlToXml::GetPoiListXml(VR_String &strXml)
{
    pugi::xml_document doc;
    doc.load_string(strXml.c_str());
    pugi::xml_node nodeAgentList = doc.select_node("//display/guidePoint").node();
    pugi::xml_node firstItem = nodeAgentList.first_child();
    while (firstItem) {
        VR_String strLon;
        VR_String strLat;
        pugi::xml_node parmNode = firstItem.first_child();
        while (parmNode) {
            std::string name = parmNode.name();
            if ("lon" == name) {
                strLon = parmNode.text().as_string();
            }
            else if ("lat" == name) {
                strLat = parmNode.text().as_string();
            }
            parmNode = parmNode.next_sibling();
        }
        if (!strLon.empty() && !strLat.empty()) {
            ULONG distance;
            WORD direction;
            GetDirection(strLon, strLat, distance, direction);
            pugi::xml_node nnode = firstItem.append_child("direction");
            if (!nnode) {
                VR_LOGD("add node failed");
            }
            else {
                nnode.text().set(direction);
            }
        }
        firstItem = firstItem.next_sibling();
    }
    std::stringstream ss;
    doc.print(ss);
    printf("ss=[%s]\n", ss.str().c_str());

    return ss.str();
}

VR_INT32 VR_TransformXmlToXml::GetDirection(VR_String &strlon, VR_String &strlat, ULONG &distance, WORD &direction)
{
    nutshell::INT32  nLon(0);
    nutshell::INT32  nLat(0);
    nutshell::INT32  nRoadKind(0);
    bool  bDemonOn(false);
    VR_ConfigureIF::Instance()->getLocateInfo(nRoadKind, nLon, nLat, bDemonOn);

    sMapLonLat _org, _dst;
    _org.Longitude = nLon;
    _org.Latitude = nLat;
    _dst.Longitude = atol(strlon.c_str());
    _dst.Latitude = atol(strlat.c_str());
    VR_INT32 ret = CalDistance(&_org, &_dst, &distance, &direction);
    VR_LOGD("ret=[%d], direction=[%d],strlon=[%s],strlat=[%s]", ret, direction, strlon.c_str(), strlat.c_str());

    return ret;
}

VR_String VR_TransformXmlToXml::CreateVolumeXML(int id, VR_String &strVolume)
{
    VR_String strXml;

    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(VR_ALEXA_VOLUME)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(id));
        xmlStrParse.setTextTo("//params/sound_level", strVolume);
        strXml = xmlStrParse.TransferXMLtoString();
    }

    VR_LOGI("strXml=[%s]", strXml.c_str());
    return strXml;
}


/* EOF */



