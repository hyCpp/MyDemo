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
 * @file VR_NIMusicShareReplier.h
 * @brief Declaration file of class VR_NIMusicShareReplier.
 *
 * This file includes the declaration of class VR_NIMusicShareReplier.
 *
 * @attention used for C++ only.
 */
#ifndef VR_NIMUSICSHAREREPLIER_H
#define VR_NIMUSICSHAREREPLIER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <media/interface/NIShareReplier.h>

#include "VR_Def.h"
#include "VR_Macro.h"
#include "VR_NIMusicShareReplier.h"


class VR_NIMusicShareProxy;
/**
 * @brief The VR_NIMusicShareReplier class
 *
 * class declaration
 */
class  VR_NIMusicShareReplier : public nutshell::media::NIShareReplier
{
public:
    VR_NIMusicShareReplier(VR_NIMusicShareProxy* pMusicProxy);

    // reply
    virtual void replyRegisterClient(uint64_t call_id = 0);
    virtual void replyRegisterFunction(uint64_t call_id = 0);
    virtual void replyGetNodeList(const std::vector<nutshell::media::NIShareNodeInfo>& nodeList, uint64_t call_id = 0);
    virtual void replyGetCapability(const std::string& uriOut, const std::string& capability, uint64_t call_id = 0);
    virtual void replyGetFileListCount(const std::string& uriOut, const uint32_t count, uint64_t call_id = 0);
    virtual void replyGetFileList(const std::string& uriOut,
                                  const std::vector<nutshell::media::NIShareFileDisplay>& fileList,
                                  uint64_t call_id = 0);

    // notify
    virtual void onConnected();
    virtual void notifyBindSuccess();
    virtual void notifyServiceReady();
    virtual void notifyNodelistChanged(const std::vector<nutshell::media::NIShareNodeInfo>& nodeList);
    virtual void notifyCapabilityChanged(const std::string& uri, const std::string& capability);
    virtual void notifyFileListCountChanged(const std::string& uri, const uint32_t count);

private:
    VR_NIMusicShareProxy*       m_pMusicProxy;
    uint64_t                    m_reqUsbCallId;

    VR_NIMusicShareReplier(const VR_NIMusicShareReplier&);
    VR_NIMusicShareReplier& operator =(const VR_NIMusicShareReplier&);
};

#endif // VR_NIMUSICSHAREREPLIER_H
/* EOF */
