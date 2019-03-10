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

#include "VR_Log.h"
#include "VR_Macro.h"
#include "VR_XmlStrParse.h"

#include <sstream>
#include <boost/assign/list_of.hpp>

using namespace nutshell;

const std::map<VR_String, emVriAutoProxyType> g_iAutoRouter = boost::assign::map_list_of
        (VR_PROXY_TYPE_HMI_SHOWLIST,             VriAutoProxyType_HMI_ShowList)
        (VR_PROXY_TYPE_HMI_SETVIDEO,             VriAutoProxyType_HMI_SetVideo)
        (VR_PROXY_TYPE_STARTVRDIAG,              VriAutoProxyType_StartVRDiag)
        (VR_PROXY_TYPE_SYS_SETSTATE,             VriAutoProxyType_System_SetState)
        (VR_PROXY_TYPE_SYS_SER_SOUNDLEVEL,       VriAutoProxyType_System_SetSoundLevel)
        (VR_PROXY_TYPE_STOPVRDIAG,               VriAutoProxyType_StopVRDiag)
        (VR_PROXY_TYPE_VUICOMMONACTION,          VriAutoProxyType_VUICommonAction)
        (VR_PROXY_TYPE_QUERY_MICINFO,            VriAutoProxyType_QueryMicInfo)
        (VR_PROXY_TYPE_QUERY_MICSTATUS,          VriAutoProxyType_QueryMicStatus)
        (VR_PROXY_TYPE_QUERY_INT_STATUS,         VriAutoProxyType_QueryInternetStatus)
        (VR_PROXY_TYPE_SYS_GETLANGUAGE,          VriAutoProxyType_System_GetLanguage)
        (VR_PROXY_TYPE_SYS_GETDATAPATH,          VriAutoProxyType_System_GetDataPath)
        (VR_PROXY_TYPE_NAVI_DESTINATION,         VriAutoProxyType_Navi_ToDestination)
        (VR_PROXY_TYPE_NAVI_WUKONGLISTDATA,      VriAutoProxyType_Navi_WukongListData)
        (VR_PROXY_TYPE_SET_ROUTE_OPTION,         VriAutoProxyType_Navi_SetRouteOption)
        (VR_PROXY_TYPE_NAVI_SET_VIAPOINT,        VriAutoProxyType_Navi_SetViaPoint)
        (VR_PROXY_TYPE_NAVI_SRCHCATByAddr,       VriAutoProxyType_Navi_SrchCategoryByAddr)
        (VR_PROXY_TYPE_NAVI_SETMODE,             VriAutoProxyType_Navi_SetMode)
        (VR_PROXY_TYPE_NAVI_NAVISTATUS,          VriAutoProxyType_Navi_GetNaviStatus)
        (VR_PROXY_TYPE_NAVI_TIME_TO_ADDR,        VriAutoProxyType_Navi_DistTimeToAddr)
        (VR_PROXY_TYPE_NAVI_ADDRLOCATION,        VriAutoProxyType_Navi_GetAddrLocation)
        (VR_PROXY_TYPE_NAVI_SET_PRESET_ADDRESS,  VriAutoProxyType_Navi_SetPresetAddress)
        (VR_PROXY_TYPE_MEDIA_GET_SOURCESTATUS,   VriAutoProxyType_Media_GetSourceStatus)
        (VR_PROXY_TYPE_MEDIA_GET_DADATA,         VriAutoProxyType_Media_GetDBData)
        (VR_PROXY_TYPE_MEDIA_PLAY_LOCALMUSIC,    VriAutoProxyType_Media_PlayLocalMusic)
        (VR_PROXY_TYPE_MEDIA_GET_LOCALINFOR,     VriAutoProxyType_Media_GetLocalInfor)
        (VR_PROXY_TYPE_MEDIA_SET_STATUS,         VriAutoProxyType_Media_SetStatus)
        (VR_PROXY_TYPE_MEDIA_PLAY_ONLINEMUSIC,   VriAutoProxyType_Media_PlayOnlineMusic)
        (VR_PROXY_TYPE_MEDIA_PLAY_ONLINEAUDIO,   VriAutoProxyType_Media_PlayOnlineAudio)
        (VR_PROXY_TYPE_TUNER_GET_STATUS,         VriAutoProxyType_Tuner_GetStatus)
        (VR_PROXY_TYPE_TUNER_PLAY_CONTROL,       VriAutoProxyType_Tuner_PlayControl)
        (VR_PROXY_TYPE_SET_AWAKEN_WORD,          VriAutoProxyType_SET_AWAKEN_WORD)
        (VR_PROXY_TYPE_PHONE_GET_STATUS,         VriAutoProxyType_Phone_GetStatus)
        (VR_PROXY_TYPE_PHONE_GET_DBDATA,         VriAutoProxyType_Phone_GetDBData)
        (VR_PROXY_TYPE_PHONE_MAKECALL,           VriAutoProxyType_Phone_MakeCall)
        (VR_PROXY_TYPE_PHONE_CALLOPERATE,        VriAutoProxyType_Phone_CallOperate)
        (VR_PROXY_TYPE_PHONE_GETNUMBER,          VriAutoProxyType_Phone_GetNumber)
        (VR_PROXY_TYPE_NATP_STARTED,             VriAutoProxyType_Natp_Started)
        (VR_PROXY_TYPE_VEHICLE_QUERY_ACSTATUS,   VriAutoProxyType_Vehicle_QueryACStatus)
        (VR_PROXY_TYPE_VEHICLE_TURN_ACONOFF,     VriAutoProxyType_Vehicle_TurnACOnOff)
        (VR_PROXY_TYPE_VEHICLE_SET_TEMPERATURE,  VriAutoProxyType_Vehicle_SetTemperature)
        (VR_PROXY_TYPE_VEHICLE_SET_FANSPEED,     VriAutoProxyType_Vehicle_SetFanSpeed)
        (VR_PROXY_TYPE_VEHICLE_CONTROL,          VriAutoProxyType_Vehicle_Control)
        (VR_PROXY_TYPE_VEHICLE_GETSUNROOFSTATUS, VriAutoProxyType_Vehicle_GetSunroofStatus)
        (VR_PROXY_TYPE_VEHICLE_GETCURSPEED,      VriAutoProxyType_Vehicle_GetCurSpeed)
        (VR_PROXY_TYPE_VEHICLE_CHECKRAINYSTATE,  VriAutoProxyType_Vehicle_CheckRainyState)
        (VR_PROXY_TYPE_VB_STORE_VEHI_SETTING,    VriAutoProxyType_VB_StoreVehiSetting)
        (VR_PROXY_TYPE_VB_CHECK_VEHI_SETTING,    VriAutoProxyType_VB_CheckVehiSetting)
        (VR_PROXY_TYPE_VB_RESET_VEHI_SETTING,    VriAutoProxyType_VB_ResetVehiSetting)
        (VR_PROXY_TYPE_STOCK_GETFAVORITE,        VriAutoProxyType_Stock_GetFavorite)
        (VR_PROXY_TYPE_STOCK_SETFAVORITE,        VriAutoProxyType_Stock_SetFavorite)
        (VR_PROXY_TYPE_SMS_SENDSMS,              VriAutoProxyType_SMS_SendSMS)
        (VR_PROXY_TYPE_SMS_GETMSG,               VriAutoProxyType_SMS_GetMsg)
        (VR_PROXY_TYPE_VBUSER_SAVEUSER,          VriAutoProxyType_VbUser_SaveUser)
        (VR_PROXY_TYPE_VBUSER_VERIFY_VBNAME,     VriAutoProxyType_VbUser_Verify_VBName)
        (VR_PROXY_TYPE_VBUSER_IDENTIFY_USER,     VriAutoProxyType_VbUser_IdentifyUser)
        (VR_PROXY_TYPE_VBUSER_SETCURRENT_USER,   VriAutoProxyType_VbUser_SetCurrentUser)
        (VR_PROXY_TYPE_CM_INQUIRE,               VriAutoProxyType_CM_Inquire)
        (VR_PROXY_GETFUELCONSUMEPTION,           VriAutoProxyType_GetFuelConsumption)
        (VR_PROXY_GETMILEAGELEFT,                VriAutoProxyType_GetMileageLeft)
        (VR_PROXY_GETMILEAGEPERIOD,              VriAutoProxyType_GetMileagePeriod)
        (VR_PROXY_GETNEXTMAINTANCEINFO,          VriAutoProxyType_GetNextMaintenanceInfo)
        (VR_PROXY_GETWINDOWSSTATUS,              VriAutoProxyType_GetWindowsStatus);

VR_XmlStrParse::VR_XmlStrParse()
{
}

VR_XmlStrParse::~VR_XmlStrParse()
{
}

VR_String VR_XmlStrParse::GetXMLValue(const VR_String &tag, const VR_String &attributeName) const
{
    VR_String result = "";
    pugi::xml_node nodelist = m_dom.select_node(("//" + tag).c_str()).node();

    if (attributeName.compare("") == 0) {
        if (nodelist.empty()) {
            VR_LOGD("Parse Exception! -- tag = [%s]", tag.c_str());
        }
        else {
            result = nodelist.text().as_string();
        }
    }
    else {
        if (nodelist.attribute(attributeName.c_str()).empty()) {
            VR_LOGD("Parse Exception! -- tag = [%s], attributeName = [%s]", tag.c_str(), attributeName.c_str());
        }
        else {
            result = nodelist.attribute(attributeName.c_str()).value();
        }
    }

    return result;
}

VR_String VR_XmlStrParse::makeQueryXml(const VRModelXMLNodeData& data)
{
    addTextTo(m_dom, "notification/id", data.id);
    addTextTo(m_dom, "notification/type", data.type);
    addTextTo(m_dom, "notification/domain", data.domain);
    addTextTo(m_dom, "action_result/id", data.id);
    addTextTo(m_dom, "action_result/action_type", data.type);


    return TransferXMLtoString();
}

bool VR_XmlStrParse::LoadString(const VR_String &xmlStr)
{
    pugi::xml_parse_result result = m_dom.load_string(xmlStr.c_str());
    bool bLoadRet = (pugi::status_ok == result.status);
    VR_LOGD("load_string=[%s]", bLoadRet ? "true" : "false");
    return bLoadRet;
}

void VR_XmlStrParse::addTextTo(const VR_String &path, const VR_String &content)
{
    addTextTo(m_dom, path, content);
}

void VR_XmlStrParse::setTextTo(const VR_String &path, const VR_String &content)
{
    pugi::xpath_node_set nodes = m_dom.select_nodes(path.c_str());
    nodes.first().node().text() = content.c_str();
}

void VR_XmlStrParse::setTextListTo(const VR_String &path, const std::vector<VR_String> &strvector)
{
    pugi::xpath_node_set nodes = m_dom.select_nodes(path.c_str());
    std::vector<VR_String>::const_iterator vectorIter = strvector.begin();
    for (; vectorIter != strvector.end(); ++vectorIter) {
        pugi::xml_node child = nodes.first().node().append_child("navi_map_option");
        child.text() = (*vectorIter).c_str();
    }
}

void VR_XmlStrParse::Reset()
{
    m_dom.reset();
}

void VR_XmlStrParse::GetXmlString(VR_String& xml)
{
    std::stringstream ss;
    m_dom.print(ss);
    xml.assign(ss.str());
}

bool VR_XmlStrParse::SaveFile(const VR_String &strPath)
{
    return m_dom.save_file(strPath.c_str());
}

bool VR_XmlStrParse::LoadFile(const VR_String &strPath)
{
    pugi::xml_parse_result result = m_dom.load_file(strPath.c_str());
    bool bLoadRet = (pugi::status_ok == result.status);
    VR_LOGD("LoadFile=[%s]", bLoadRet ? "true" : "false");
    return bLoadRet;
}

bool VR_XmlStrParse::GetNodeBlock(const VR_String& xpath, VR_String& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("null xnode");
        return false;
    }

    std::stringstream ss;
    xnode.node().print(ss);
    content.assign(ss.str());
    return true;
}

bool VR_XmlStrParse::GetNodeText(const VR_String& xpath, VR_String& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("null xnode");
        return false;
    }
    content.assign(xnode.node().text().as_string());
    return true;
}

bool VR_XmlStrParse::GetNodeText(const VR_String& xpath, int& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("null xnode");
        return false;
    }
    content = xnode.node().text().as_int();
    return true;
}

bool VR_XmlStrParse::GetNodeText(const VR_String& xpath, unsigned int& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("null xnode");
        return false;
    }
    content = xnode.node().text().as_uint();
    return true;
}

bool VR_XmlStrParse::GetAttributeValue(const VR_String& xpath, const VR_String& attr, VR_String& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("null xnode");
        return false;
    }
    content = xnode.node().attribute(attr.c_str()).as_string();
    return true;
}

bool VR_XmlStrParse::SetNodeText(const VR_String& xpath, const float& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("null xnode");
        return false;
    }

    bool res = xnode.node().text().set(content);
    if (!res) {
        VR_LOGD("set node value failed");
    }
    return res;
}

bool VR_XmlStrParse::SetNodeText(const VR_String& xpath, const int& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("null xnode");
        return false;
    }

    bool res = xnode.node().text().set(content);
    if (!res) {
        VR_LOGD("set node value failed");
    }
    return res;
}

bool VR_XmlStrParse::SetNodeText(const VR_String& xpath, const unsigned int& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("null xnode");
        return false;
    }

    bool res = xnode.node().text().set(content);
    if (!res) {
        VR_LOGD("set node value failed");
    }
    return res;
}

bool VR_XmlStrParse::SetNodeText(const VR_String& xpath, const VR_String& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("null xnode");
        return false;
    }

    bool res = xnode.node().text().set(content.c_str());
    if (!res) {
        VR_LOGD("set node value failed");
    }
    return res;
}

bool VR_XmlStrParse::AddNode(const VR_String& xpath, const VR_String& xmlNode)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("xnode invalid");
        return false;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result res = doc.load_string(xmlNode.c_str());
    if (!res) {
        VR_LOGD("node invald. res: %s", res.description());
        return false;
    }

    res = xnode.node().append_buffer(xmlNode.c_str(), xmlNode.size());
    if (!res) {
        VR_LOGD("append invalid. res:%s", res.description());
        return false;
    }
    return true;
}

bool VR_XmlStrParse::AddNode(const VR_String& xpath, const VR_String& node, const int& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("xnode invalid");
        return false;
    }

    pugi::xml_node nnode = xnode.node().append_child(node.c_str());
    if (!nnode) {
        VR_LOGD("add node failed");
        return false;
    }

    bool res = nnode.text().set(content);
    if (!res) {
        VR_LOGD("set text failed");
    }
    return res;
}

bool VR_XmlStrParse::AddNode(const VR_String& xpath, const VR_String& node, const unsigned int& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("xnode invalid");
        return false;
    }

    pugi::xml_node nnode = xnode.node().append_child(node.c_str());
    if (!nnode) {
        VR_LOGD("add node failed");
        return false;
    }

    bool res = nnode.text().set(content);
    if (!res) {
        VR_LOGD("set text failed");
    }
    return res;
}

bool VR_XmlStrParse::AddNode(const VR_String& xpath, const VR_String& node, const float& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("xnode invalid");
        return false;
    }

    pugi::xml_node nnode = xnode.node().append_child(node.c_str());
    if (!nnode) {
        VR_LOGD("add node failed");
        return false;
    }

    bool res = nnode.text().set(content);
    if (!res) {
        VR_LOGD("set text failed");
    }
    return res;
}

bool VR_XmlStrParse::AddNode(const VR_String& xpath, const VR_String& node, const VR_String& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("xnode invalid");
        return false;
    }

    pugi::xml_node nnode = xnode.node().append_child(node.c_str());
    if (!nnode) {
        VR_LOGD("add node failed");
        return false;
    }

    bool res = nnode.text().set(content.c_str());
    if (!res) {
        VR_LOGD("set text failed");
    }
    return res;
}

bool VR_XmlStrParse::AddAttr(const VR_String& xpath, const VR_String& attr, const int& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("xnode invalid");
        return false;
    }

    pugi::xml_attribute xattr = xnode.node().append_attribute(attr.c_str());
    if (!xattr) {
        VR_LOGD("attr invalid");
        return false;
    }

    bool res = xattr.set_value(content);
    if (!res) {
        VR_LOGD("set value error");
    }
    return res;
}

bool VR_XmlStrParse::AddAttr(const VR_String& xpath, const VR_String& attr, const unsigned int& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("xnode invalid");
        return false;
    }

    pugi::xml_attribute xattr = xnode.node().append_attribute(attr.c_str());
    if (!xattr) {
        VR_LOGD("attr invalid");
        return false;
    }

    bool res = xattr.set_value(content);
    if (!res) {
        VR_LOGD("set value error");
    }
    return res;
}

bool VR_XmlStrParse::AddAttr(const VR_String& xpath, const VR_String& attr, const VR_String& content)
{
    pugi::xpath_node xnode = m_dom.select_node(xpath.c_str());
    if (!xnode) {
        VR_LOGD("xnode invalid");
        return false;
    }

    pugi::xml_attribute xattr = xnode.node().append_attribute(attr.c_str());
    if (!xattr) {
        VR_LOGD("attr invalid");
        return false;
    }

    bool res = xattr.set_value(content.c_str());
    if (!res) {
        VR_LOGD("set value error");
    }
    return res;
}

XmlMapVector VR_XmlStrParse::GetXMLListValue(const std::string &nodePath) const
{
    pugi::xpath_node_set nodelist = m_dom.select_nodes(nodePath.c_str());
    XmlMapVector xmlList;
    XmlMap result;

    if (!nodelist.empty()) {
        for (pugi::xpath_node_set::const_iterator it = nodelist.begin(); it != nodelist.end(); ++it) {
            pugi::xpath_node node = *it;
            pugi::xml_node itemList = node.node();

            if (!itemList.empty()) {

                for (pugi::xml_node::iterator iter = itemList.begin(); iter != itemList.end(); ++iter) {
                    result[(*iter).name()] = (*iter).text().get();
                }

                xmlList.push_back(result);
                result.clear();
            }
        }
    }
    else {
        VR_LOGE("Parse Exception! -- nodelist is empty!");
    }

    return xmlList;
}

void VR_XmlStrParse::addTextTo(pugi::xml_node node, const VR_String& content)
{
    pugi::xml_node textNode = node.append_child(pugi::xml_node_type::node_pcdata);
    textNode.text() = content.c_str();
}

void VR_XmlStrParse::addTextTo(pugi::xpath_node_set nodes, const VR_String& content)
{
    for (auto xnode : nodes) {
        addTextTo(xnode.node(), content);
    }
}

void VR_XmlStrParse::addTextTo(pugi::xml_node node, const VR_String& xpath, const VR_String& content)
{
    pugi::xpath_node_set nodes = node.select_nodes(xpath.c_str());
    addTextTo(nodes, content);
}

VR_String VR_XmlStrParse::TransferXMLtoString()
{
    std::stringstream ss;
    m_dom.print(ss);
    return ss.str();
}
/* EOF */
