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
 * @file VR_NIMusicShareProxy.h
 * @brief Declaration file of class VR_NIMusicShareProxy.
 *
 * This file includes the declaration of class VR_NIMusicShareProxy.
 *
 * @attention used for C++ only.
 */
#ifndef VR_NIMUSICSHAREPROXY_H
#define VR_NIMUSICSHAREPROXY_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"

#include "VR_NIMusicNodeItem.h"
#include <nievip/NIIpcServiceHelper.h>
#include <media/proxy/NIShareProxy.h>

const std::string  PROXY_NAME = "nutshell.media.share";

/**
 * @brief The VR_NIMusicShareProxy class
 *
 * class declaration
 */
class   VR_NIMusicShareProxy
{
public:
    VR_NIMusicShareProxy(const std::string& strName);
    ~VR_NIMusicShareProxy();

    void  RegisterFunction();
    void  GetNodeList();
    uint64_t  GetCapability(const std::string& uri);
    void  GetFileListCount(const std::string& uri);
    void  GetFileList(const std::string& uri);
    void  RegiserNotifyEvent();

    void  OnConnectMessage(const std::string& uri, const std::string& capability);
    void  OnFileListCountMessage(const std::string &uri, const int count);
    void  OnFileListMessage(const std::string& uri,
                            const std::vector<nutshell::media::NIShareFileDisplay>& fileList);

    bool  IsUsbConnected() const;
    void  SetNodeList(const std::vector<nutshell::media::NIShareNodeInfo> &nodelist);
    void  OnDectedUDiskStatus(const std::string& capability);

public:
    void   SyncCacheData();
    void   Clear();
    void   Distroy();
    bool   Initialize();
    bool   FindNodeItem(const std::string& strUri);
    VR_NIMusicNodeItem* GetNodeItem(const std::string& strUri);
#ifdef HAVE_NUTSHELL_OS

protected:
    std::string                      m_proxyName;
    nutshell::NISP<nutshell::media::NIShareProxy>     m_shareProxy;
    nutshell::NISP<nutshell::NIReplyReceiver>  m_shareReplier;
    std::vector<VR_NIMusicNodeItem*> m_nodeList;

private:
    VR_NIMusicShareProxy(const VR_NIMusicShareProxy&);
    VR_NIMusicShareProxy& operator=(const VR_NIMusicShareProxy&);
#else

private:
    std::string           m_proxyName;
#endif
};

#endif // VR_NIMUSICSHAREPROXY_H
/* EOF */













