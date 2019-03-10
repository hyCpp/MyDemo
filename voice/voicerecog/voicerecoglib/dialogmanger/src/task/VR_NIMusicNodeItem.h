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
 * @file VR_NIMusicNodeItem.h
 * @brief Declaration file of class VR_NIMusicNodeItem.
 *
 * This file includes the declaration of class VR_NIMusicNodeItem.
 *
 * @attention used for C++ only.
 */
#ifndef VR_NIMUSICNODEITEM_H
#define VR_NIMUSICNODEITEM_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include  "VR_Def.h"
#include  "VR_Macro.h"
#include  "VR_EventSenderIF.h"
#include  "EV_EventSender.h"
#include  "media/proxy/NIShareProxy.h"

class  VR_NIMusicShareProxy;

/**
 *  @brief the VR_NIMusicNodeItem class
 *
 *    class  VR_NIMusicNodeItem
 */
class  VR_NIMusicNodeItem
{
public:
    VR_NIMusicNodeItem(const std::string& strUri, VR_NIMusicShareProxy* proxy);
    ~VR_NIMusicNodeItem();

    void  Add();
    void  Remove();

    void  SetConnected(bool bConnected);
    void  SetFileListCount(int nCount);
    void  SetFileList(const std::vector<nutshell::media::NIShareFileDisplay>& fileList);
    void  SetFileList() const;

    std::string  GetUri() const;
    bool         GetUsbConnect() const;

protected:
    void   GetFileList();
    void   GetFileList(int startIndex, int endIndex);
    void   GetList(int startIndex, int nCount);

protected:
    int    m_nStartIndex;
    int    m_nCurrentIndex;
    bool   m_bUsbConnected;
    std::string m_strUri;
    std::vector<nutshell::media::NIShareFileDisplay>  m_fileList;
    VR_NIMusicShareProxy*   m_shareProxy;

private:
    VR_NIMusicNodeItem& operator = (const VR_NIMusicNodeItem& dat);
    VR_NIMusicNodeItem(VR_NIMusicNodeItem&);
};

#endif // VR_NIMUSICNODEITEM_H
/* EOF */
