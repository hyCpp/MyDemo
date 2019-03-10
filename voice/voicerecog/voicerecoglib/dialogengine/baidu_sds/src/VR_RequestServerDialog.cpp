/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

// #include "stdafx.h"

#ifndef CXX_VR_REQUESTRSERVERDIALOG_H
#   include "VR_RequestServerDialog.h"
#endif

#include "VR_SDSEvent.h"
#include "VR_SDSServerDEF.h"
#include "VR_PlatformDEF.h"
#include "VR_Log.h"
#include "VR_HttpClient.h"
#include "VR_SDSAbstractState.h"

using namespace std;

VR_RequestServerDialog::VR_RequestServerDialog(std::shared_ptr<VR_SDSStateMachine> listener,
                                               DWORD dwReqID, const BL_String &szUrl)
    :m_pListener(listener)
    ,m_dwReqID(dwReqID)
    ,m_szUrl(szUrl)
    ,m_pcRequest(NULL)
    ,m_bData(NULL)
    ,m_dwSize(0)
    ,m_dwErrCode(0)
{
}

VR_RequestServerDialog::~VR_RequestServerDialog()
{
    if (NULL != m_bData) {
        delete m_bData;
        m_bData = NULL;
    }
}

VOID
VR_RequestServerDialog::DoAction()
{
    string resp;
    shared_ptr<VR_HttpClient> client = make_shared<VR_HttpClient>();
    VR_LOGD("Get data from url [%s]...", m_szUrl.GetString());
    client->Get(m_szUrl.GetString(), resp);

    VR_SDSEvent* pcEvent = nullptr;
    if (!resp.empty() && nullptr != m_pListener) {
        pcEvent = new VR_SDSEvent(VR_SDS_EVENT_TYPE_DIALOG, VR_SDS_EVENT_ID_DIALOG, m_dwReqID);
        pcEvent->SetText(resp);
        m_pListener->PostRequest(pcEvent);
    }
    else {
        VR_LOGE("Can not get resp from remote server!");
        pcEvent = new VR_SDSEvent(VR_SDS_EVENT_TYPE_SYSTEM, VR_SDS_EVENT_ID_BACK, m_dwReqID);
        m_pListener->PostRequest(pcEvent);

        VR_SDSAbstractState* pcState = m_pListener->GetState(VR_SDSSERVER_STATE_TYPE_IDLE);
        m_pListener->ChangeStateTo(pcState);

        std::string respTtsContent = "无法获得服务器响应,请稍后重试";
        string strAction = "<action agent = \"prompt\" op = \"playTts\">";
        strAction += "<language>zh-cn</language>";
        strAction += "<text>" + respTtsContent + "</text>";
        strAction += "<bargein>false</bargein>";
        strAction += "</action>";

        m_pListener->SendMessage(strAction);

        // m_pListener->SendMessage("IDLE");
    }

}

VOID
VR_RequestServerDialog::DoCallBackAction()
{
    string text;
    text.assign((char*)m_bData, m_dwSize);

    VR_SDSEvent* pcEvent = new VR_SDSEvent(VR_SDS_EVENT_TYPE_DIALOG, VR_SDS_EVENT_ID_DIALOG, m_dwReqID);
    pcEvent->SetText(text);
    if(nullptr != m_pListener){
        m_pListener->PostRequest(pcEvent);
    }
}

CL_BOOL
VR_RequestServerDialog::CancelAction()
{
    if (RES_CODE_CANCEL == m_pcRequest->CancelRequset())
    {
        return CL_TRUE;
    }
    else {
        return CL_FALSE;
    }
}

const XCHAR* VR_RequestServerDialog::GetReqName() const
{
    return XTEXT("VR_RequestServerDialog");
}


ServerDialogNetRequest::ServerDialogNetRequest(BL_String szUrl, VR_RequestServerDialog* pcRequest)
:PNetASyncRequest(szUrl.GetString())
{
    m_cServerDialogNetCB.SetRequest(pcRequest);
    this->SetCallback(&m_cServerDialogNetCB);
}

VOID ServerDialogNetRequest::ServerDialogNetCallBack::DoAction(PNetASyncRequest* pcReqeust)
{
    INT nResCode = pcReqeust->GetResCode();
    INT nErrCode = VR_PLATFORM_ERR_OK;
    if(RES_CODE_NO_ERROR == nResCode)
    {
        VR_LOGD("ServerDialogNetRequest::request ok\n");
        nErrCode = VR_PLATFORM_ERR_OK;
    }
    else if(RES_CODE_CONNECT_EXCEPTION == nResCode)
    {
        nErrCode = VR_PLATFORM_ERR_NETWORK;
    }
    else if(RES_CODE_CANCEL == nResCode)
    {
        VR_LOGE("ServerDialogNetRequest::request cancel\n");
        nErrCode = VR_PLATFORM_ERR_UNKNOWN;
    }
    else
    {
        VR_LOGE("ServerDialogNetRequest::request error nResCode=%d\n",nResCode);
        nErrCode = VR_PLATFORM_ERR_NETWORK;
    }
    m_pcRequest->SetErrCode(nErrCode);

    if (NULL != m_pcRequest) {
        m_pcRequest->SetData(pcReqeust->GetReceiveBuf(), pcReqeust->GetReceiveBufSize());
    }

    m_pcRequest->DoCallBackAction();

    // clear buf
    pcReqeust->SetReceiveBuf(NULL, 0);

    // m_pcRequest->m_pPlatform->EraseRequest(m_pcRequest->GetReqID());
}

/* EOF */
