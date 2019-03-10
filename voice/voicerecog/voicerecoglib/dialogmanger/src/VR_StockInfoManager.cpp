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
 * @file VR_StockInfoHelper.h
 * @brief Declaration file of class VR_StockInfoHelper.
 *
 * This file includes the declaration of class VR_StockInfoHelper.
 *
 * @attention used for C++ only.
 */

#include "VR_Log.h"
#include "VR_XmlStrParse.h"
#include "VR_Indentifier.hpp"
#include "VR_StockInfoHelper.h"
#include "VR_StockInfoManager.h"

#include "EV_Define.h"
#include "EV_EventSender.h"

#include "boost/format.hpp"

#include "navi/Voice/VoiceRecog/VrNotifyiAutoProxy.pb.h"

VR_StockInfoManager::VR_StockInfoManager()
    : m_StockInfoHelper(VR_new VR_StockInfoHelper())
{
    VR_LOGD_FUNC();
}

VR_StockInfoManager::~VR_StockInfoManager()
{
    VR_LOGD_FUNC();
}

void VR_StockInfoManager::ChangeUserContext(const VR_String& user_id)
{
    VR_LOGD_FUNC();

    m_StockInfoHelper->UpdateCurrUser(user_id);
}

void VR_StockInfoManager::DeleteUserContext(const VR_String& user_id)
{
    VR_LOGD_FUNC();
    m_StockInfoHelper->DeleteVoiceUser(user_id);
}

bool VR_StockInfoManager::GetFavoriteStocksXml(const VR_String& xml)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    bool res = parser.LoadString(xml);
    if (!res) {
        VR_LOGD("load string failed");
        return false;
    }

    VR_String action_id;
    parser.GetNodeText("//action/id", action_id);

    parser.Reset();
    parser.LoadString(VR_STOCK_GETFAVORITESTE_RESP_XML);
    if (!res) {
        VR_LOGD("load string failed");
        return false;
    }

    parser.SetNodeText("//id", VR_IdGenerator().GetID());
    parser.SetNodeText("//action_id", action_id);
    parser.SetNodeText("//error_no", "SYSTEM_RESULT_OK");

    tStockMap mapper;
    m_StockInfoHelper->LoadAllStockInfo(mapper);
    for (tStockPair pair : mapper) {
        parser.AddNode("//stock_favorite_list", "stock_favorite", "");
        parser.AddNode("//stock_favorite_list/stock_favorite[not(stock_id)]", "stock_id", pair.first);
        parser.AddNode("//stock_favorite_list/stock_favorite[not(stock_name)]", "stock_name", pair.second);
    }

    VR_String mesg;
    parser.GetXmlString(mesg);

    VR_LOGD("send mesg: %s", mesg.c_str());
    SendMesgToNATP(mesg);

    return true;
}

bool VR_StockInfoManager::SetFavoriteStockInfo(const VR_String& xml)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    bool res = parser.LoadString(xml);
    if (!res) {
        VR_LOGD("load string failed. xml:%s", xml.c_str());
        return false;
    }

    VR_String StockId;
    parser.GetNodeText("//params/stock_id", StockId);

    VR_String StockName;
    parser.GetNodeText("//params/stock_name", StockName);

    res = m_StockInfoHelper->AddOneStockInfo(StockId, StockName);
    if (!res) {
        VR_LOGD("add failed");
    }

    VR_String action_id;
    parser.GetNodeText("//action/id", action_id);
    VR_String Result = res ? "SYSTEM_RESULT_OK" : "SYSTEM_RESULT_ERROR";

    parser.Reset();
    res = parser.LoadString(VR_STOCK_SETFAVORITESTE_RESP_XML);
    if (!res) {
        VR_LOGD("load failed");
        return false;
    }

    parser.SetNodeText("//id", VR_IdGenerator().GetID());
    parser.SetNodeText("//action_id", action_id);
    parser.SetNodeText("//error_no", Result);

    VR_String mesg;
    parser.GetXmlString(mesg);

    SendMesgToNATP(mesg);
    VR_LOGD("send Mesg: %s", mesg.c_str());

    return res;
}

bool VR_StockInfoManager::DelFavoriteStockInfo(const VR_String& xml)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    bool res = parser.LoadString(xml);
    if (!res) {
        VR_LOGD("load string failed. xml:%s", xml.c_str());
        return false;
    }

    VR_String StockId;
    parser.GetNodeText("//params/stock_id", StockId);

    res = m_StockInfoHelper->DelOneStockInfo(StockId);
    if (!res) {
        VR_LOGD("del failed");
    }
    return res;
}

bool VR_StockInfoManager::AddFavoriteStockInfo(const VR_String& xml)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    bool res = parser.LoadString(xml);
    if (!res) {
        VR_LOGD("load string failed. xml:%s", xml.c_str());
        return false;
    }

    VR_String StockId;
    parser.GetNodeText("//params/stock_id", StockId);

    VR_String StockName;
    parser.GetNodeText("//params/stock_name", StockName);

    res = m_StockInfoHelper->AddOneStockInfo(StockId, StockName);
    if (!res) {
        VR_LOGD("add failed");
    }

    return res;
}

void VR_StockInfoManager::SendMesgToNATP(VR_String& mesg)
{
    VR_LOGD_FUNC();
    using namespace navi::VoiceRecog;
    std::unique_ptr<VrNotifyiAutoProxy> protoMsg(VrNotifyiAutoProxy::default_instance().New());
    protoMsg->set_sender("vr_s");
    protoMsg->set_funccode(VrNotifyiAutoProxy_VrFunc_NotifyRecogMessage);
    protoMsg->mutable_recogmsg()->set_xml_out(mesg.c_str());
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFIAUTOPROXY, protoMsg.release());
}

/* EOF */
