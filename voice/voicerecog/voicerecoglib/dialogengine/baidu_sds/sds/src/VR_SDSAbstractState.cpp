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

#include "VR_SDSAbstractState.h"
#include "VR_SDSStateMachine.h"

using namespace std;

VR_SDSAbstractState::VR_SDSAbstractState()
    : m_dwReqID(VR_INVALID_REQUET_ID)
{

}

VR_SDSAbstractState::VR_SDSAbstractState(const string &rcName)
    :m_dwReqID(VR_INVALID_REQUET_ID)
    ,m_cStateName(rcName)
{

}

VOID VR_SDSAbstractState::SetVRState(VR_SDSSERVER_STATE eStateSDS)
{
    string message = TranslateVrStateToUI(eStateSDS);
    m_pStateMachine->SendMessage(message);
}

VR_SDSSERVER_STATE VR_SDSAbstractState::GetVRState()
{
    return m_eVRState;
}

// FIXME: translate this state to iAuto3.0
// and notify it to DM
string VR_SDSAbstractState::TranslateVrStateToUI(VR_SDSSERVER_STATE eStateSDS)
{
    switch (eStateSDS) {
    case VR_SDS_STATE_SPEACH:
        return "LISTENING";
    case VR_SDS_STATE_BUSY:
        return "THINKING";
    case VR_SDS_STATE_TTS:
        return "SPEAKING";
    case VR_SDS_STATE_IDLE:
        return "IDLE";
    default:
        break;
    }

    return "IDLE";
}

VOID VR_SDSAbstractState::SessionAdd(const BL_String& rcContent, CL_BOOL bUser,
                                BL_String rcAudioFileName)
{
    VRCP_ConversationInfo cInfo;

    cInfo.szRoleType = (bUser ? "U" : "S");
    cInfo.szContent = rcContent;
    if (bUser && (rcAudioFileName.GetLength() > 0)) {
        cInfo.szAudioFileName = rcAudioFileName;
        m_szCurSessionAudioFile = rcAudioFileName;
    }

    m_cSessionVector.push_back(cInfo);
}

VOID VR_SDSAbstractState::SessionCommit()
{
//    if (0 < cSessionVector.size()) {
//        GetPlatformIF()->SendConversation(m_cSessionVector);

//        m_cSessionVector.clear();
//    }
}

void VR_SDSAbstractState::SetPlatformIF(std::shared_ptr<VR_Platform> spcPlatformIF)
{
    m_pPlatformIF = spcPlatformIF;
}

std::shared_ptr<VR_Platform> VR_SDSAbstractState::GetPlatformIF()
{
    return m_pPlatformIF;
}
