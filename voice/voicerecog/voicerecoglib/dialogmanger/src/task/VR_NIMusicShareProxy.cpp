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
 * @file VR_NIMusicShareProxy.cpp
 * @brief Implementation of class VR_NIMusicShareProxy
 */


#include  "stdafx.h"
#include  "VR_CommonTool.h"
#include  "VR_NIMusicShareReplier.h"
#include  "VR_NIMusicShareProxy.h"
#include "navi/Search/ReqUpdateSpotlightData.pb.h"
#include "navi/Voice/VoiceRecog/RequestVriAutoProxy.pb.h"

using namespace nutshell;
using namespace nutshell::media;
using namespace navi::Search;

VR_NIMusicShareProxy::VR_NIMusicShareProxy(const std::string &strName)
    : m_proxyName(strName)
    , m_nodeList()
{
    m_shareProxy = VR_new  NIShareProxy();
    m_shareReplier = VR_new  VR_NIMusicShareReplier(this);
//    Initialize();
}


VR_NIMusicShareProxy::~VR_NIMusicShareProxy()
{
    Distroy();
}


void  VR_NIMusicShareProxy::Clear()
{
    VR_LOGD_FUNC();
    std::unique_ptr<ReqUpdateSpotlightData>  fullUpdate(ReqUpdateSpotlightData::default_instance().New());
    fullUpdate->set_data_type(SearchDefine_SPOTLIGHT_SOURCE_TYPE_MUSIC);
    fullUpdate->set_update_type(ReqUpdateSpotlightData_Source_Update_Type_FULLUPDATE);
    EV_EventSender().SendEvent(EV_EVENT_SEARCH_REQUPDATESPOTLIGHTDATA, fullUpdate.release());
}

void  VR_NIMusicShareProxy::SyncCacheData()
{
    Clear();

    for (VR_NIMusicNodeItem* item : m_nodeList) {
        bool bConnected = item->GetUsbConnect();
        if (bConnected) {
            item->SetFileList();
        }
    }
}

void  VR_NIMusicShareProxy::Distroy()
{
    for (VR_NIMusicNodeItem* item : m_nodeList) {
        if (NULL != item) {
            delete item;
            item = NULL;
        }
    }
}

bool   VR_NIMusicShareProxy::Initialize()
{
    Clear();

    NISP<NIIpcServiceProxy> ipc = NIIpcServiceHelper::findService(m_proxyName.c_str(), -1);
    if (ipc == NULL) {
        VR_LOGD("shareproxy findservice failed.");
        return false;
    }
    m_shareProxy->setReplyReceiver(m_shareReplier);
    bool bRet = m_shareProxy->bindService(ipc);
    if (bRet) {
        VR_LOGD("bind media proxy success.");
        RegiserNotifyEvent();
    }
    return bRet;
}

bool   VR_NIMusicShareProxy::FindNodeItem(const std::string &strUri)
{
    VR_LOGD("m_nodelist:[%d]", m_nodeList.size());
    for (VR_NIMusicNodeItem* item : m_nodeList) {
        std::string nodeUri = item->GetUri();
        if (nodeUri == strUri) {
            return true;
        }
    }
    return false;
}

VR_NIMusicNodeItem*  VR_NIMusicShareProxy::GetNodeItem(const std::string& strUri)
{
    for (VR_NIMusicNodeItem* item : m_nodeList) {
        std::string nodeUri = item->GetUri();
        if (std::string::npos != strUri.find(nodeUri)) {
            return item;
        }

    }
    return NULL;
}

void  VR_NIMusicShareProxy::SetNodeList(const std::vector<NIShareNodeInfo> &nodelist)
{
    VR_LOGD("nodelist size:[%d]", nodelist.size());

    // update list
    std::vector<std::string>  newList;
    for (NIShareNodeInfo node : nodelist) {
        std::string strPort;
        toString(strPort, node.port);
        std::string strUri = "http://" + node.ipAddress + ":" + strPort + "/CarMultimedia/file/";
        newList.push_back(strUri);
    }

    for (std::string uri : newList) {
        bool bFindNode = FindNodeItem(uri);
        if (bFindNode) {
            continue;
        }
        else {
            VR_NIMusicNodeItem* newNode = VR_new  VR_NIMusicNodeItem(uri, this);
            m_nodeList.push_back(newNode);
            newNode->Add();
        }
    }

    std::vector<VR_NIMusicNodeItem*>::iterator iter;
    for (iter = m_nodeList.begin(); iter != m_nodeList.end();) {
        VR_NIMusicNodeItem* item = (*iter);
        std::string uri = item->GetUri();
        std::vector<std::string>::iterator result = find(newList.begin(), newList.end(), uri);
        if (result == newList.end()) {
            item->Remove();
            iter = m_nodeList.erase(iter);
            delete item;
        }
        else {
            ++iter;
        }
    }
}

void  VR_NIMusicShareProxy::OnDectedUDiskStatus(const std::string& capability)
{
    VR_LOGD_FUNC();
    const std::string OK_FLG = "true";
    if (OK_FLG != capability) {
        VR_LOGD("uncare status");
        return;
    }

    using namespace navi::VoiceRecog;
    std::unique_ptr<RequestVriAutoProxy> pRequestVriAutoProxy(new RequestVriAutoProxy);
    pRequestVriAutoProxy->set_sender("ms_proxy");

    EV_EventContext out;
    out.desModuleId = EV_IDENTITY_VOICERECOG;
    out.SetStringData("Vr_UserFunc_AddUser");
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_REQUESTVRIAUTOPROXY, pRequestVriAutoProxy.release(), out);
}

bool  VR_NIMusicShareProxy::IsUsbConnected() const
{
    for (VR_NIMusicNodeItem* item : m_nodeList) {
        bool bConnected = item->GetUsbConnect();
        if (bConnected) {
            return true;
        }
    }
    return false;
}

void  VR_NIMusicShareProxy::OnConnectMessage(const std::string &uri, const std::string &capability)
{
    bool  bConnected(false);
    if ("true" == capability) {
        bConnected = true;
    }

    VR_NIMusicNodeItem* item = GetNodeItem(uri);
    if (NULL != item) {
        item->SetConnected(bConnected);
    }
}


void   VR_NIMusicShareProxy::OnFileListCountMessage(const std::string &uri, const int count)
{
    VR_LOGD("count:[%d]", count);
    VR_NIMusicNodeItem* item = GetNodeItem(uri);
    if (count > 0 && NULL != item && item->GetUsbConnect()) {
        item->SetFileListCount(count);
    }
}


void   VR_NIMusicShareProxy::OnFileListMessage(const std::string &uri, const std::vector<NIShareFileDisplay> &fileList)
{
    VR_LOGD("filelist count:[%d]", fileList.size());
    VR_NIMusicNodeItem* item = GetNodeItem(uri);
    if (NULL != item) {
        item->SetFileList(fileList);
    }
}


void   VR_NIMusicShareProxy::RegisterFunction()
{
    m_shareProxy->registerFunction(NIShareFunctionId_Browser);
    const std::string uri = "http://127.0.0.1:5458/?func=browser&display=ivi";
    m_shareProxy->registerClient(uri);
}


void  VR_NIMusicShareProxy::GetNodeList()
{
    m_shareProxy->getNodeList();
}

uint64_t VR_NIMusicShareProxy::GetCapability(const std::string& uri)
{
    return m_shareProxy->getCapability(uri);
}


void  VR_NIMusicShareProxy::GetFileListCount(const std::string &uri)
{
    m_shareProxy->getFileListCount(uri);
}


void  VR_NIMusicShareProxy::GetFileList(const std::string &uri)
{
    m_shareProxy->getFileList(uri);
}


void  VR_NIMusicShareProxy::RegiserNotifyEvent()
{
    m_shareProxy->registerBindSuccess();
    m_shareProxy->registerServiceReady();
    m_shareProxy->registerNodelistChanged();
    m_shareProxy->registerCapabilityChanged();
    m_shareProxy->registerFileListCountChanged();
}

/* EOF */
