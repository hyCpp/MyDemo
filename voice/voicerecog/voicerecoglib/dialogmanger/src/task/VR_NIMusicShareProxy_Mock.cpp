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
#include  "VR_NIMusicShareProxy.h"

using namespace nutshell::media;

VR_NIMusicShareProxy::VR_NIMusicShareProxy(const std::string &strName)
    : m_proxyName(strName)
{

}

VR_NIMusicShareProxy::~VR_NIMusicShareProxy()
{

}

void VR_NIMusicShareProxy::RegisterFunction()
{
    VR_LOGD_FUNC();
}

void VR_NIMusicShareProxy::GetNodeList()
{
    VR_LOGD_FUNC();
}

void VR_NIMusicShareProxy::GetCapability(const std::string& uri)
{
    VR_LOGD_FUNC();
}

void VR_NIMusicShareProxy::GetFileListCount(const std::string &uri)
{
    VR_LOGD_FUNC();
}

void VR_NIMusicShareProxy::GetFileList(const std::string &uri)
{
    VR_LOGD_FUNC();
}

void VR_NIMusicShareProxy::RegiserNotifyEvent()
{
    VR_LOGD_FUNC();
}

void VR_NIMusicShareProxy::OnConnectMessage(const std::string &uri, const std::string &capability)
{
    VR_LOGD_FUNC();
}

void VR_NIMusicShareProxy::OnFileListCountMessage(const std::string &uri, const int count)
{
    VR_LOGD_FUNC();
}

void   VR_NIMusicShareProxy::OnFileListMessage(const std::string &uri, const std::vector<NIShareFileDisplay> &fileList)
{
    VR_LOGD_FUNC();
}

bool  VR_NIMusicShareProxy::IsUsbConnected() const
{
    VR_LOGD_FUNC();
    return true;
}

void  VR_NIMusicShareProxy::SetNodeList(const std::vector<NIShareNodeInfo> &nodelist)
{
    VR_LOGD_FUNC();
}

void VR_NIMusicShareProxy::SyncCacheData()
{
    VR_LOGD_FUNC();
}

void VR_NIMusicShareProxy::Clear()
{
    VR_LOGD_FUNC();
}

void VR_NIMusicShareProxy::Distroy()
{
    VR_LOGD_FUNC();
}

bool VR_NIMusicShareProxy::Initialize()
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_NIMusicShareProxy::FindNodeItem(const std::string& strUri)
{
    VR_LOGD_FUNC();
    return true;
}

/* EOF */
