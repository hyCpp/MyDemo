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
 * @file VR_FileTransport.h
 * @brief Declaration file of class VR_FileTransport.
 *
 * This file includes the declaration of class VR_FileTransport.
 *
 * @attention used for C++ only.
 */

#ifndef VR_FILETRANSPORT_H
#define VR_FILETRANSPORT_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "VR_Message.h"

#include "navi/Voice/VoiceRecog/Internal/VrFileTransfer.pb.h"

/**
 * @brief The VR_FileTransRightAcquireRequest class
 *
 * declare class VR_FileTransRightAcquireRequest
 */
class VR_FileTransRightAcquireRequest : public VR_Command
{
public:
    VR_FileTransRightAcquireRequest(navi::VoiceRecog::VrFileUpdateRightAcquisitionRequest* pMsg);
    ~VR_FileTransRightAcquireRequest();

    void perform();
    void replay();

private:
    VR_FileTransRightAcquireRequest(const VR_FileTransRightAcquireRequest&);
    VR_FileTransRightAcquireRequest& operator=(const VR_FileTransRightAcquireRequest&);

    navi::VoiceRecog::VrFileUpdateRightAcquisitionRequest* m_pMsg;
};

/**
 * @brief The VR_FileTransRightAcquireResponse class
 *
 * declare class VR_FileTransRightAcquireResponse
 */
class VR_FileTransRightAcquireResponse : public VR_Command
{
public:
    VR_FileTransRightAcquireResponse(navi::VoiceRecog::VrFileUpdateRightAcquisitionResponse* pMsg);
    ~VR_FileTransRightAcquireResponse();

    void perform();
    void replay();

private:
    VR_FileTransRightAcquireResponse(const VR_FileTransRightAcquireResponse&);
    VR_FileTransRightAcquireResponse& operator=(const VR_FileTransRightAcquireResponse&);

    navi::VoiceRecog::VrFileUpdateRightAcquisitionResponse* m_pMsg;
};

/**
 * @brief The VR_FileTransRightReleaseRequest class
 *
 * declare class VR_FileTransRightReleaseRequest
 */
class VR_FileTransRightReleaseRequest : public VR_Command
{
public:
    VR_FileTransRightReleaseRequest(navi::VoiceRecog::VrFileUpdateRightReleaseRequest* pMsg);
    ~VR_FileTransRightReleaseRequest();

    void perform();
    void replay();

private:
    VR_FileTransRightReleaseRequest(const VR_FileTransRightReleaseRequest&);
    VR_FileTransRightReleaseRequest& operator=(const VR_FileTransRightReleaseRequest&);

    navi::VoiceRecog::VrFileUpdateRightReleaseRequest* m_pMsg;
};

/**
 * @brief The VR_FileTransRightReleaseResponse class
 *
 * declare class VR_FileTransRightReleaseResponse
 */
class VR_FileTransRightReleaseResponse : public VR_Command
{
public:
    VR_FileTransRightReleaseResponse(navi::VoiceRecog::VrFileUpdateRightReleaseResponse* pMsg);
    ~VR_FileTransRightReleaseResponse();

    void perform();
    void replay();

private:
    VR_FileTransRightReleaseResponse(const VR_FileTransRightReleaseResponse&);
    VR_FileTransRightReleaseResponse& operator=(const VR_FileTransRightReleaseResponse&);

    navi::VoiceRecog::VrFileUpdateRightReleaseResponse* m_pMsg;
};

/**
 * @brief The VR_FileTransUploadFile class
 *
 * declare class VR_FileTransUploadFile
 */
class VR_FileTransUploadFile : public VR_Command
{
public:
    VR_FileTransUploadFile(navi::VoiceRecog::VrFileUploadResponse* pMsg);
    ~VR_FileTransUploadFile();

    void perform();
    void replay();

private:
    VR_FileTransUploadFile(const VR_FileTransUploadFile&);
    VR_FileTransUploadFile& operator=(const VR_FileTransUploadFile&);

    navi::VoiceRecog::VrFileUploadResponse* m_pMsg;
};

/**
 * @brief The VR_FileTransDownloadFile class
 *
 * declare class VR_FileTransDownloadFile
 */
class VR_FileTransDownloadFile : public VR_Command
{
public:
    VR_FileTransDownloadFile(navi::VoiceRecog::VrFileDownloadResponse* pMsg);
    ~VR_FileTransDownloadFile();

    void perform();
    void replay();

private:
    VR_FileTransDownloadFile(const VR_FileTransDownloadFile&);
    VR_FileTransDownloadFile& operator=(const VR_FileTransDownloadFile&);

    navi::VoiceRecog::VrFileDownloadResponse* m_pMsg;
};

/**
 * @brief The VR_FileTransTimeOutRetry class
 *
 * declare class VR_FileTransTimeOutRetry
 */
class VR_FileTransTimeOutRetry : public VR_Command
{
public:
    VR_FileTransTimeOutRetry(navi::VoiceRecog::VrFileTransferTimeOut* pMsg);
    ~VR_FileTransTimeOutRetry();

    void perform();
    void replay();

private:
    VR_FileTransTimeOutRetry(const VR_FileTransTimeOutRetry&);
    VR_FileTransTimeOutRetry& operator=(const VR_FileTransTimeOutRetry&);

    navi::VoiceRecog::VrFileTransferTimeOut* m_pMsg;
};

#endif // End of __VR_FILETRANSPORT_H__
/* EOF */
