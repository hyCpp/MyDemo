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
 * @file VR_DP_OB_Action_FileTrans.h
 * @brief Declaration file of class VR_DP_OB_Action_FileTrans.
 *
 * This file includes the declaration of class VR_DP_OB_Action_FileTrans.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DP_OB_ACTIOIN_FILETRANS_H
#define VR_DP_OB_ACTIOIN_FILETRANS_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_DP_OB_Action.h"
#include "VR_FileTransferContext.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_CommandHandleIF);

namespace navi
{
namespace dataprovider
{
#define VR_SERVER_PATH "//Internal/VR/"
#define VR_DOCUNENT_ROOT_PATH "http://192.168.128.129:49153/Internal/VR/"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DP_OB_Action_FileTrans);

/**
 * @brief The VR_DP_OB_Action_Hfp class
 *
 * declare class VR_DP_OB_Action_Hfp
 */

class VR_DP_OB_Action_FileTrans : public VR_DP_OB_Action
{
public:
    VR_DP_OB_Action_FileTrans(VR_ProtoMessageBase*  pMsg,
                              spVR_DialogEngineIF  spEngine,
                              spVR_CommandHandleIF spCommandHandler,
                              spVR_AvcLanSenderIF  spAvcSender);
    ~VR_DP_OB_Action_FileTrans();

public:
    virtual void DoUpdatePrepare();

    virtual bool DownloadFile(const nutshell::NCString& remoteFile,
                              const nutshell::NCString& targetPath,
                              bool IsDownFromRemote = true);

    virtual bool UploadFile(const nutshell::NCString& localPath,
                            const nutshell::NCString& remoteFile,
                            bool IsUpToRemote = false);

private:
    VR_DP_OB_Action_FileTrans(const VR_DP_OB_Action_FileTrans&);
    VR_DP_OB_Action_FileTrans& operator=(const VR_DP_OB_Action_FileTrans&);

    const VR_ProtoMessageBase*      m_pMsg;
    spVR_DialogEngineIF             m_spDialogEngineIF;
    spVR_CommandHandleIF            m_spCommandHandleIF;
};

}
}

#endif // end of __VR_DP_OB_ACTIOIN_FILETRANS_H_
/* EOF */
