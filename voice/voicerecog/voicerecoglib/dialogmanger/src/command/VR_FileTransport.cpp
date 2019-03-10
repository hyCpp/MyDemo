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
#include "VR_FileTransport.h"
#include "VR_FileTransferContext.h"

#include "navi/Voice/VoiceRecog/Internal/VrFileTransfer.pb.h"
#include "VR_Common_VrUnitAgent.h"

using namespace navi::dataprovider;
using namespace navi::VoiceRecog;

VR_FileTransRightAcquireRequest::VR_FileTransRightAcquireRequest(VrFileUpdateRightAcquisitionRequest* pMsg)
    : VR_Command(NULL)
    , m_pMsg(pMsg)
{
}

VR_FileTransRightAcquireRequest::~VR_FileTransRightAcquireRequest()
{
    if (NULL != m_pMsg) {
        delete m_pMsg;
        m_pMsg = NULL;
    }
}

void VR_FileTransRightAcquireRequest::perform()
{
    VR_LOGD_FUNC();
    VR_FileTransferContextManager* pFileTransManger =
            VR_FileTransferContextManager::GetInstance();
    if ((NULL == pFileTransManger) || (NULL == m_pMsg)) {
        return;
    }

#if defined(VR_DM_TARGET_VRUNIT)
    VR_String filePath = m_pMsg->url();
    int iPos = filePath.find_last_of("/");
    pFileTransManger->RegisterTslContexts(filePath.substr(iPos+1).c_str());
    VR_LOGD("register tsl %s", filePath.substr(iPos+1).c_str());
#endif

    pFileTransManger->RequestRightAcquisition(*m_pMsg);
}

void VR_FileTransRightAcquireRequest::replay()
{
    VR_LOGD_FUNC();
}

VR_FileTransRightAcquireResponse::VR_FileTransRightAcquireResponse(VrFileUpdateRightAcquisitionResponse* pMsg)
    : VR_Command(NULL)
    , m_pMsg(pMsg)
{
    VR_LOGD_FUNC();
}

VR_FileTransRightAcquireResponse::~VR_FileTransRightAcquireResponse()
{
    if (NULL != m_pMsg) {
        delete m_pMsg;
        m_pMsg = NULL;
    }
}

void VR_FileTransRightAcquireResponse::perform()
{
    VR_LOGD_FUNC();
    VR_FileTransferContextManager* pFileTransManger =
            VR_FileTransferContextManager::GetInstance();
    if ((NULL == pFileTransManger) || (NULL == m_pMsg)) {
        return;
    }

    pFileTransManger->ResponseRightAcquisition(*m_pMsg);
}

void VR_FileTransRightAcquireResponse::replay()
{
    VR_LOGD_FUNC();
}

VR_FileTransRightReleaseRequest::VR_FileTransRightReleaseRequest(VrFileUpdateRightReleaseRequest* pMsg)
    : VR_Command(NULL)
    , m_pMsg(pMsg)
{
    VR_LOGD_FUNC();
}

VR_FileTransRightReleaseRequest::~VR_FileTransRightReleaseRequest()
{
    if (NULL != m_pMsg) {
        delete m_pMsg;
        m_pMsg = NULL;
    }
}

void VR_FileTransRightReleaseRequest::perform()
{
    VR_LOGD_FUNC();
    VR_FileTransferContextManager* pFileTransManger =
            VR_FileTransferContextManager::GetInstance();
    if ((NULL == pFileTransManger) || (NULL == m_pMsg)) {
        return;
    }

    pFileTransManger->RequestRightRelease(*m_pMsg);
}

void VR_FileTransRightReleaseRequest::replay()
{
    VR_LOGD_FUNC();
}

VR_FileTransRightReleaseResponse::VR_FileTransRightReleaseResponse(VrFileUpdateRightReleaseResponse* pMsg)
    : VR_Command(NULL)
    , m_pMsg(pMsg)
{
    VR_LOGD_FUNC();
}

VR_FileTransRightReleaseResponse::~VR_FileTransRightReleaseResponse()
{
    if (NULL != m_pMsg) {
        delete m_pMsg;
        m_pMsg = NULL;
    }
}

void VR_FileTransRightReleaseResponse::perform()
{
    VR_LOGD_FUNC();
    VR_FileTransferContextManager* pFileTransManger =
            VR_FileTransferContextManager::GetInstance();
    if ((NULL == pFileTransManger) || (NULL == m_pMsg)) {
        return;
    }

    pFileTransManger->ResponseRightRelease(*m_pMsg);
}

void VR_FileTransRightReleaseResponse::replay()
{
    VR_LOGD_FUNC();
}

VR_FileTransUploadFile::VR_FileTransUploadFile(navi::VoiceRecog::VrFileUploadResponse* pMsg)
    : VR_Command(NULL)
    , m_pMsg(pMsg)
{
    VR_LOGD_FUNC();
}

VR_FileTransUploadFile::~VR_FileTransUploadFile()
{
    if (NULL != m_pMsg) {
        delete m_pMsg;
        m_pMsg = NULL;
    }
}

void VR_FileTransUploadFile::perform()
{
    VR_LOGD_FUNC();
    VR_FileTransferContextManager* pFileTransManger =
            VR_FileTransferContextManager::GetInstance();
    if ((NULL == pFileTransManger) || (NULL == m_pMsg)) {
        return;
    }

    pFileTransManger->ResponseUploadFile(*m_pMsg);
}

void VR_FileTransUploadFile::replay()
{
    VR_LOGD_FUNC();
}

VR_FileTransDownloadFile::VR_FileTransDownloadFile(navi::VoiceRecog::VrFileDownloadResponse* pMsg)
    : VR_Command(NULL)
    , m_pMsg(pMsg)
{
    VR_LOGD_FUNC();
}

VR_FileTransDownloadFile::~VR_FileTransDownloadFile()
{
    if (NULL != m_pMsg) {
        delete m_pMsg;
        m_pMsg = NULL;
    }
}

void VR_FileTransDownloadFile::perform()
{
    VR_LOGD_FUNC();
    VR_FileTransferContextManager* pFileTransManger =
            VR_FileTransferContextManager::GetInstance();
    if ((NULL == pFileTransManger) || (NULL == m_pMsg)) {
        return;
    }

    pFileTransManger->ResponseDownloadFile(*m_pMsg);
}

void VR_FileTransDownloadFile::replay()
{
    VR_LOGD_FUNC();
}

VR_FileTransTimeOutRetry::VR_FileTransTimeOutRetry(navi::VoiceRecog::VrFileTransferTimeOut* pMsg)
    : VR_Command(NULL)
    , m_pMsg(pMsg)
{
    VR_LOGD_FUNC();
}

VR_FileTransTimeOutRetry::~VR_FileTransTimeOutRetry()
{
    if (NULL != m_pMsg) {
        delete m_pMsg;
        m_pMsg = NULL;
    }
}

void VR_FileTransTimeOutRetry::perform()
{
    VR_LOGD_FUNC();
    VR_FileTransferContextManager* pFileTransManger =
            VR_FileTransferContextManager::GetInstance();
    if ((NULL == pFileTransManger) || (NULL == m_pMsg)) {
        return;
    }

    pFileTransManger->NotifyContextsRetry();
}

void VR_FileTransTimeOutRetry::replay()
{
    VR_LOGD_FUNC();
}

/* EOF */
