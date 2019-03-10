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


#include "VR_SDSStateConcrete.h"
#include "VR_SDSStateBase.h"
#include "VR_SDSRequestServer.h"
#include "VR_Platform.h"
#include "VR_SDSStateMachine.h"
#include "VR_PlatformDEF.h"
#include "VR_Log.h"
#include "VR_ConfigureIF.h"

using namespace std;
using namespace nutshell;

VR_SDSParserDLG* VR_SDSStateBase::s_pcDLGParser = NULL;
DWORD VR_SDSStateBase::s_dwBeepID = 0;

VOID VR_SDSStateServerIdle::Enter()
{
    VR_LOGD("|VR_SDSStateServerIdle| |Enter|\n");

    SessionCommit();

    SetVRState(VR_SDS_STATE_IDLE);
    // GetPlatformIF()->ChangeState(SESSION_STATE_END);
    DeleteParser();
}

CL_BOOL VR_SDSStateServerIdle::OnEvent(VR_SDSEvent& rcEvent)
{
    if (VR_SDSStateBase::OnEvent(rcEvent)) {
        return CL_TRUE;
    }

    const VR_SDSEvent& rcSDSEvent = reinterpret_cast<VR_SDSEvent&>(rcEvent);
    DWORD dwID = rcSDSEvent.GetID();

    if (VR_SDS_EVENT_ID_MIC == dwID) {
        VR_SDSAbstractState* pcState = GetStateMachine()->GetState(VR_SDSSERVER_STATE_TYPE_PRESTART);
        VR_LOGD("Change state to pre start state");
        GetStateMachine()->ChangeStateTo(pcState);
        return CL_TRUE;
    }

    return CL_FALSE;
}

VOID VR_SDSStateServerPreStart::Enter()
{
    VR_LOGD("|VR_SDSStateServerPreStart| |Enter|\n");

    // if (GetPlatformIF()->IsNetworkConnected()) {
        VR_LOGD("Network is connected, ready to change to start state!");
        // GetPlatformIF()->ChangeState(SESSION_STATE_BEGIN, VR_SDSListenerFactory::GetContactSyncListener());
        VR_SDSAbstractState* pcState = GetStateMachine()->GetState(VR_SDSSERVER_STATE_TYPE_START);
        GetStateMachine()->ChangeStateTo(pcState);

        GetStateMachine()->SendMessage("CONNECTED");
    // }
    // else {
    //     VR_LOGE("Network is unavailable...");
    //     GetStateMachine()->SendMessage("DISCONNECTED");
    //     DoActionError(VR_SDSSERVER_ERROR_NETWORK_DISCONNECT);
    // }
}

VOID VR_SDSStateServerStart::Enter()
{
    VR_LOGD("|VR_SDSStateServerStart| |Enter|\n");

    string cUID = "vr_service";
    SetVRState(VR_SDS_STATE_TTS);
    // if (GetPlatformIF()->GetUserID(cUID)) {
    VR_SDSMessageServerStart cReq(cUID);

    DoActionRequestDialog(&cReq);
    // }
    // else {
    //     VR_SDSMessageServerStart cReq;

    //     DoActionRequestDialog(&cReq);
    // }
}

CL_BOOL VR_SDSStateServerStart::OnEvent(VR_SDSEvent& rcEvent)
{
    VR_LOGD_FUNC();
    if (VR_SDSStateBase::OnEvent(rcEvent)) {
        return CL_TRUE;
    }

    const VR_SDSEvent& rcSDSEvent = reinterpret_cast<VR_SDSEvent&>(rcEvent);
    DWORD dwID = rcSDSEvent.GetID();

    if (VR_SDS_EVENT_ID_DIALOG == dwID) {
        // Check ReqID
        if (GetReqID() != rcSDSEvent.GetReqID()) {
            VR_LOGE("|VR_SDSStateServerStart| |OnEvent| [ReqID_A:%lu ReqID_B:%lu]\n", GetReqID(), rcSDSEvent.GetReqID());
            return CL_FALSE;
        }

        SetReqID(VR_INVALID_REQUET_ID);
        // Check error
        if (VR_PLATFORM_ERR_OK != rcSDSEvent.GetParam()) {
            DoActionError(VR_SDSSERVER_ERROR_NETWORK_PROBLEM);
            return CL_TRUE;
        }
        // Create parser
        VR_SDSParserDLG* pcParser = CreateParser();
        if (NULL == pcParser) {
            VR_LOGE("|VR_SDSStateServerStart| |OnEvent| [NEW Fails]\n");
            return CL_FALSE;
        }
        // Check xml
        if (!pcParser->ParseResult(rcSDSEvent.GetText())) {
            VR_LOGE("|VR_SDSStateServerStart| |OnEvent| [Error XML]\n");
            DoActionError(VR_SDSSERVER_ERROR_SC_UNKNOWN);
            return CL_FALSE;
        }
        // Check error
        if (pcParser->HasError()) {
            DoActionError(pcParser->GetOperationVector()[0]);
            return CL_TRUE;
        }

        // Set SID
        VR_SDSRequestServerBase::SetSID(pcParser->GetSID());

        // ChangeStateTo VR_SDSSERVER_STATE_TYPE_DIALOG
        VR_SDSAbstractState* pcState = GetStateMachine()->GetState(VR_SDSSERVER_STATE_TYPE_DIALOG);
        GetStateMachine()->ChangeStateTo(pcState);

        return CL_TRUE;
    }

    return CL_FALSE;
}

VR_SDSStateServerDialog::VR_SDSStateServerDialog(const string& name)
    : VR_SDSStateBase(name)
    , m_bSelect(CL_FALSE)
{
    m_pfActionMap[OPERATION_TYPE_DISPLAY_CONTACT_LIST_ON] = &VR_SDSStateServerDialog::DoActionDisplayContactListOn;
    m_pfActionMap[OPERATION_TYPE_DISPLAY_NEWS_LIST_ON] = &VR_SDSStateServerDialog::DoActionDisplayNewsListOn;
    m_pfActionMap[OPERATION_TYPE_DISPLAY_HELP_LIST_ON] = &VR_SDSStateServerDialog::DoActionDisplayHelpListOn;
    m_pfActionMap[OPERATION_TYPE_DISPLAY_POI_LIST_ON] = &VR_SDSStateServerDialog::DoActionDisplayPOIListOn;
    m_pfActionMap[OPERATION_TYPE_DISPLAY_COM_LIST_ON] = &VR_SDSStateServerDialog::DoActionDisplayComListOn;
    m_pfActionMap[OPERATION_TYPE_DISPLAY_LIST_OFF] = &VR_SDSStateServerDialog::DoActionDisplayListOff;

    m_pfActionMap[OPERATION_TYPE_DESTINATION_NORMAL] = &VR_SDSStateServerDialog::DoActionDestinationNormal;
    m_pfActionMap[OPERATION_TYPE_DISPLAY_WEATHERINFO_LIST_ON] = &VR_SDSStateServerDialog::DoActionDisplayWeatherInfoListOn;

    m_pfActionMap[OPERATION_TYPE_ERROR] = &VR_SDSStateServerDialog::DoActionError;
    m_pfActionMap[OPERATION_TYPE_CALL] = &VR_SDSStateServerDialog::DoActionCall;
    m_pfActionMap[OPERATION_TYPE_STOP] = &VR_SDSStateServerDialog::DoActionStop;
    m_pfActionMap[OPERATION_TYPE_VR] = &VR_SDSStateServerDialog::DoActionVR;
    m_pfActionMap[OPERATION_TYPE_HINTS] = &VR_SDSStateServerDialog::DoActionHints;
    m_pfActionMap[OPERATION_TYPE_PROMPT] = &VR_SDSStateServerDialog::DoActionPrompt;
}

VOID
VR_SDSStateServerDialog::Enter()
{
    VR_LOGD("|VR_SDSStateServerDialog| |Enter|\n");

    m_bSelect = CL_FALSE;

    m_cOperationVector = VR_SDSStateBase::GetParser()->GetOperationVector();

    DoOperation();
}

CL_BOOL
VR_SDSStateServerDialog::OnEvent(VR_SDSEvent& rcEvent)
{
    VR_LOGD_FUNC();
//    if (VR_SDSStateBase::OnEvent(rcEvent)) {
//        return CL_TRUE;
//    }

    const VR_SDSEvent& rcSDSEvent = reinterpret_cast<VR_SDSEvent&>(rcEvent);
    DWORD dwID = rcSDSEvent.GetID();

    if (VR_SDS_EVENT_ID_SELECT == dwID) {
        OnSelect(rcSDSEvent);
        return CL_TRUE;
    }

    if (GetReqID() != rcSDSEvent.GetReqID()) {
        VR_LOGE("|VR_SDSStateServerDialog| |OnEvent| [ReqID_A:%lu ReqID_B:%lu]\n", GetReqID(), rcSDSEvent.GetReqID());
        return CL_FALSE;
    }

    SetReqID(VR_INVALID_REQUET_ID);
    m_bSelect = CL_FALSE;

    switch (dwID) {
        case VR_SDS_EVENT_ID_DIALOG:
            OnDialog(rcSDSEvent);
            break;

        case VR_SDS_EVENT_ID_TTS_END:
            DoOperation();
            break;

        case VR_SDS_EVENT_ID_VR_END:
            OnVR(rcSDSEvent);
            break;

        case VR_SDS_EVENT_ID_REPORT_END:
            OnReport(rcSDSEvent);
            break;

        case VR_SDS_EVENT_ID_WEIBO_END:
            OnWeibo(rcSDSEvent);
            break;

        case VR_SDS_EVENT_ID_PICTURE_END:
            OnPicture(rcSDSEvent);
            break;

        case VR_SDS_EVENT_ID_PLAYMUSIC_REQ_END:
                OnPlayMusic(rcSDSEvent);
                break;

        case VR_SDS_EVENT_ID_START_APP_END:
                OnAppControl(rcSDSEvent);
                break;

        default:
            break;
    }

    return CL_TRUE;
}

CL_BOOL
VR_SDSStateServerDialog::OnDialog(const VR_SDSEvent& rcEvent)
{
    VR_LOGD("|VR_SDSStateServerDialog| |OnDialog|\n");

    if (VR_PLATFORM_ERR_OK != rcEvent.GetParam()) {
        DoActionError(VR_SDSSERVER_ERROR_NETWORK_PROBLEM);
        return CL_TRUE;
    }

    // Parse error
    if (!GetParser()->ParseResult(rcEvent.GetText())) {
        VR_LOGE("|VR_SDSStateServerDialog| |OnActionDialog| [Error XML]\n");
        DoActionError(VR_SDSSERVER_ERROR_SC_INVALID_FORMAT);
        return CL_FALSE;
    }

    m_cOperationVector = GetParser()->GetOperationVector();
    return DoOperation();
}

CL_BOOL
VR_SDSStateServerDialog::DoOperation()
{
    VR_LOGD("|VR_SDSStateServerDialog| |DoOperation|\n");

    for (VR_SDSParserDLG::OperationIt it = m_cOperationVector.begin(); it != m_cOperationVector.end(); ) {
        VR_SDSSERVER_OPERATION_TYPE eType = OPERATION_TYPE_UNKNOWN;
        CL_BOOL bBreak = CL_FALSE;

        if (nullptr == *it) {
            VR_LOGD("continue , it is null\n");
            m_cOperationVector.erase(it);
            continue;
        }

        eType = (*it)->GetOperationType();
        VR_LOGD("Operation type is: %d", eType);
        VR_LOGD("Is action null? [%s]", (NULL == m_pfActionMap[eType] ? "TRUE" : "FALSE"));

        if (NULL != m_pfActionMap[eType]) {
            bBreak = !(this->*m_pfActionMap[eType])(*it);

            m_cOperationVector.erase(it);
        }
        else {
            bBreak = CL_TRUE;

            ++it;

            VR_LOGE("|VR_SDSStateServerDialog| |OnOperation| [Error Type:%d]\n", eType);
        }

        if (bBreak) {
            VR_LOGE("continue , action fail");
            continue;
        }
    }

    return CL_TRUE;
}

CL_BOOL
VR_SDSStateServerDialog::OnVR(const VR_SDSEvent& rcEvent)
{
    VR_LOGD("|VR_SDSStateServerDialog| |OnVR| [Ercd:%lx Result:%s]\n", rcEvent.GetParam(), rcEvent.GetText().c_str());

    SetVRState(VR_SDS_STATE_BUSY);
    VR_SDSSERVER_ERROR eError = VR_SDSSERVER_ERROR_NO_ERROR;

    switch (rcEvent.GetParam()) {
        case VR_PLATFORM_ERR_OK:
            eError = VR_SDSSERVER_ERROR_NO_ERROR;
            break;

        case VR_PLATFORM_ERR_NETWORK:
            eError = VR_SDSSERVER_ERROR_CS_VR_NETWORK;
            break;

        case VR_PLATFORM_ERR_VR_SILENCE:
            eError = VR_SDSSERVER_ERROR_CS_VR_NOSOUND;
            break;

        case VR_PLATFORM_ERR_VR_TIMEOUT:
            eError = VR_SDSSERVER_ERROR_CS_VR_TIMEOUT;
            break;

        case VR_PLATFORM_ERR_VR_NOMATCH:
            eError = VR_SDSSERVER_ERROR_CS_VR_MISMATCH;
            break;

        default:
            eError = VR_SDSSERVER_ERROR_CS_UNKNOWN;
            break;
    }

    if (VR_SDSSERVER_ERROR_NO_ERROR == eError) {
        DoActionDisplayVR(rcEvent.GetText());

        long lLon = 0, lLat = 0;
        // GetPlatformIF()->GetLocation(lLon, lLat);

        nutshell::INT32  nLon(0);
        nutshell::INT32  nLat(0);
        nutshell::INT32  nRoadKind(0);
        bool  bDemonOn(false);
        VR_ConfigureIF::Instance()->getLocateInfo(nRoadKind, nLon, nLat, bDemonOn);

        lLon = static_cast<long>(nLon);
        lLat = static_cast<long>(nLat);

        VR_SDSResponseServerVR cReq(rcEvent.GetText(), lLon, lLat);

        DoActionRequestDialog(&cReq);
    }
    else {
        VR_SDSResponseServerVR cReq(eError);
        DoActionRequestDialog(&cReq);
    }

    return CL_TRUE;
}

CL_BOOL
VR_SDSStateServerDialog::OnSelect(const VR_SDSEvent& rcEvent)
{
    if (!m_bSelect) {
        if (VR_SDS_STATE_SPEACH == GetVRState()) {
            VR_LOGD("|VR_SDSStateServerDialog| |OnSelect|\n");

            CancelRequest();

            m_bSelect = CL_TRUE;

            VR_SDSMessageServerSelect cReq(rcEvent.GetParam());
            DoActionRequestDialog(&cReq);
        }
    }

    return CL_TRUE;
}

CL_BOOL
VR_SDSStateServerDialog::OnPicture(const VR_SDSEvent& rcEvent)
{
    VR_LOGD("|VR_SDSStateServerDialog| |OnPicture|\n");

    SetVRState(VR_SDS_STATE_IDLE);

    VR_SDSSERVER_ERROR eError = ((VR_PLATFORM_ERR_OK == rcEvent.GetParam()) ? VR_SDSSERVER_ERROR_NO_ERROR : VR_SDSSERVER_ERROR_CS_PIC);

    if (VR_SDSSERVER_ERROR_NO_ERROR == eError) {
        DoActionDisplayPicture(rcEvent.GetText());
    }

    VR_SDSResponseServerPIC cReq(eError);
    DoActionRequestDialog(&cReq);

    return CL_TRUE;
}

CL_BOOL
VR_SDSStateServerDialog::OnReport(const VR_SDSEvent& rcEvent)
{
    VR_LOGD("|VR_SDSStateServerDialog| |OnReport|\n");

    SetVRState(VR_SDS_STATE_IDLE);

    VR_SDSSERVER_ERROR eError = ((VR_PLATFORM_ERR_OK == rcEvent.GetParam()) ? VR_SDSSERVER_ERROR_NO_ERROR : VR_SDSSERVER_ERROR_CS_RPT);

    VR_SDSResponseServerRPT cReq(eError);
    DoActionRequestDialog(&cReq);

    return CL_TRUE;
}

CL_BOOL
VR_SDSStateServerDialog::OnWeibo(const VR_SDSEvent& rcEvent)
{
    VR_LOGD("|VR_SDSStateServerDialog| |OnWeibo|\n");

    SetVRState(VR_SDS_STATE_IDLE);

    VR_SDSSERVER_ERROR eError = ((VR_PLATFORM_ERR_OK == rcEvent.GetParam()) ? VR_SDSSERVER_ERROR_NO_ERROR : VR_SDSSERVER_ERROR_CS_WB);

    VR_SDSResponseServerWB cReq(eError);
    DoActionRequestDialog(&cReq);

    return CL_TRUE;
}

VR_SDSStateServerError::VR_SDSStateServerError()
    : m_eError(VR_SDSSERVER_ERROR_NO_ERROR)
{

}

CL_BOOL
VR_SDSStateServerDialog::OnPlayMusic(const VR_SDSEvent& rcEvent)
{
    VR_LOGD("|VR_SDSStateServerDialog| |OnPlayMusic|\n");

    DWORD dwResult =  rcEvent.GetParam();

    VR_SDSResponseServerPlayMusic cReq(dwResult);
    DoActionRequestDialog(&cReq);

    return CL_TRUE;
}

CL_BOOL
VR_SDSStateServerDialog::OnAppControl(const VR_SDSEvent& rcEvent)
{
    VR_LOGD("|VR_SDSStateServerDialog| |OnAppControl|\n");

    DWORD dwResult =  rcEvent.GetParam();
    string cAppType("startapp");

    if (VR_PLATFORM_ERR_OK != dwResult) {
        dwResult = 1;
    }

    if (rcEvent.GetText() == "startradio") {
        cAppType = rcEvent.GetText();
    }

    VR_SDSResponseServerAppControl cReq(dwResult, cAppType);
    DoActionRequestDialog(&cReq);

    return CL_TRUE;
}

VOID
VR_SDSStateServerError::Enter()
{
    VR_LOGD("|VR_SDSStateServerError| |Enter| [Ercd:%d]\n", GetError());

    switch (GetError()) {
        case VR_SDSSERVER_ERROR_NETWORK_DISCONNECT:
        case VR_SDSSERVER_ERROR_NETWORK_PROBLEM:
        case VR_SDSSERVER_ERROR_SC_INSUFFICIENT_SID:
        case VR_SDSSERVER_ERROR_SC_SEARCH:
        case VR_SDSSERVER_ERROR_SC_INVALID_UID:
        case VR_SDSSERVER_ERROR_SC_INVALID_SID:
        case VR_SDSSERVER_ERROR_SC_INVALID_MSG:
        case VR_SDSSERVER_ERROR_SC_INVALID_RSP:
        case VR_SDSSERVER_ERROR_SC_INVALID_INDEX:
        case VR_SDSSERVER_ERROR_SC_UNKNOWN:
            DoActionDisplayPrompt(m_pfErrorMap[GetError()]);
            DoActionTTS(m_pfErrorMap[GetError()]);
            break;

        default:
            break;
    }

    SetVRState(VR_SDS_STATE_IDLE);
}

CL_BOOL
VR_SDSStateServerError::OnEvent(VR_SDSEvent& rcEvent)
{
    if (VR_SDSStateBase::OnEvent(rcEvent)) {
        return CL_TRUE;
    }

    VR_SDSStateMachine* pcStateMachine = static_cast<VR_SDSStateMachine*>(GetStateMachine().get());
    if (NULL == pcStateMachine) {
        VR_LOGE("Error OnEvent pcStateMachine is NULL\n");
        return CL_FALSE;
    }

    if (GetReqID() != rcEvent.GetReqID()) {
        VR_LOGE("|VR_SDSStateServerError| |OnEvent| [ReqID_A:%lu ReqID_B:%lu]\n", GetReqID(), rcEvent.GetReqID());
        return CL_FALSE;
    }

    SetReqID(VR_INVALID_REQUET_ID);

    if (VR_SDS_EVENT_ID_TTS_END == rcEvent.GetID()) {
        VR_SDSAbstractState* pcState = pcStateMachine->GetState(VR_SDSSERVER_STATE_TYPE_IDLE);
        pcStateMachine->ChangeStateTo(pcState);
    }

    return CL_TRUE;
}

VOID
VR_SDSStateServerError::BuildErrorMap(VR_SDSSERVER_ERROR eError, const BL_String& rcError)
{
    m_pfErrorMap[eError] = rcError;
}

