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
#include  "VR_ConfigureIF.h"
#include  "VR_NIMusicShareReplier.h"
#include  "VR_NIMusicShareProxy.h"


VR_NIMusicShareReplier::VR_NIMusicShareReplier(VR_NIMusicShareProxy* pMusicProxy)
    : m_pMusicProxy(pMusicProxy)
    , m_reqUsbCallId(0)
{
}


void   VR_NIMusicShareReplier::replyRegisterFunction(uint64_t call_id)
{
    VR_LOGD("call_id: [%d]", call_id);
//    m_pMusicProxy->GetNodeList();
}


void  VR_NIMusicShareReplier::replyRegisterClient(uint64_t call_id)
{
    VR_LOGD("call_id: [%d]", call_id);
    m_pMusicProxy->GetNodeList();
}


void  VR_NIMusicShareReplier::onConnected()
{
    VR_LOGD("VR_NIMusicShareReplier: bind service connected");
//    m_pMusicProxy->RegiserNotifyEvent();
}


void  VR_NIMusicShareReplier::replyGetNodeList(const std::vector<nutshell::media::NIShareNodeInfo>& nodeList, uint64_t call_id)
{
    const std::string usb_media_uri("http://127.0.0.1:5458/CarMultimedia/file/");
    m_reqUsbCallId = m_pMusicProxy->GetCapability(usb_media_uri);
    if (0 == m_reqUsbCallId) {
        VR_LOGD("Ignore get capability");
    }

    VR_LOGD("call_id: [%d]", call_id);
    m_pMusicProxy->SetNodeList(nodeList);
}


void  VR_NIMusicShareReplier::replyGetCapability(const std::string& uriOut, const std::string& capability, uint64_t call_id)
{
    VR_LOGD("uriout: [%s], capability: [%s], call_id: [%d]", uriOut.c_str(), capability.c_str(), call_id);
    const std::string usb_media_uri("http://127.0.0.1:5458/CarMultimedia/file/");
    if (call_id == m_reqUsbCallId && uriOut == usb_media_uri) {
        m_pMusicProxy->OnDectedUDiskStatus(capability);
    }

    m_pMusicProxy->OnConnectMessage(uriOut, capability);
}


void  VR_NIMusicShareReplier::replyGetFileListCount(const std::string &uriOut, const uint32_t count, uint64_t call_id)
{
    VR_LOGD("uriOut: [%s], count: [%d], call_id: [%d]", uriOut.c_str(), count, call_id);
    m_pMusicProxy->OnFileListCountMessage(uriOut, count);
}


void  VR_NIMusicShareReplier::replyGetFileList(const std::string& uriOut,
                                               const std::vector<nutshell::media::NIShareFileDisplay>& fileList,
                                               uint64_t call_id)
{
    VR_LOGD("uriout: [%s], call_id: [%d]", uriOut.c_str(), call_id);
    m_pMusicProxy->OnFileListMessage(uriOut, fileList);
}

void   VR_NIMusicShareReplier::notifyBindSuccess()
{
    VR_LOGD("vr bind mediaproxy success");
    m_pMusicProxy->RegisterFunction();
}

void   VR_NIMusicShareReplier::notifyServiceReady()
{
    VR_LOGD("vr notify service ready");
}


void   VR_NIMusicShareReplier::notifyNodelistChanged(const std::vector<nutshell::media::NIShareNodeInfo>& nodeList)
{
    VR_LOGD_FUNC();
    m_pMusicProxy->SetNodeList(nodeList);
}


void  VR_NIMusicShareReplier::notifyCapabilityChanged(const std::string& uri, const std::string& capability)
{
    VR_LOGD("uriout: [%s], capability: [%s]", uri.c_str(), capability.c_str());
    m_pMusicProxy->OnConnectMessage(uri, capability);
}


void   VR_NIMusicShareReplier::notifyFileListCountChanged(const std::string &uri, const uint32_t count)
{
    VR_LOGD("uri:[%s], count:[%d]", uri.c_str(), count);
    m_pMusicProxy->OnFileListCountMessage(uri, count);
}

/* EOF */
