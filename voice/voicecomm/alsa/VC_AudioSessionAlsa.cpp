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

#include "stdafx.h"
#include "VC_AudioSessionAlsa.h"
#ifndef CXX_BL_ERROR_H
#	include "BL_Error.h"
#endif
#ifndef CXX_CL_WAITOBJ_H
#	include "CL_WaitObj.h"
#endif

#include "VC_CommonDef.h"

static const INT VC_RETRY_COUNT = 50;
static const INT VC_RETRY_INTERVAL = 10; // in ms
const int VC_WAIT_CHANNELACTIVE_TIME = 1000; // ms

VC_AudioSessionAlsa::VC_AudioSessionAlsa(BYTE instID)
    // :m_state(ST_NONE)
    // , m_pSourceFB(NULL)
    // , m_pConnShadow(NULL)
    // , m_pInstId(instID)
    // , m_iLogicalAVChannel(NM_VMOST_LC_MIX_ALERT_MSG)
    // , m_listener(NULL)
    // , m_isPlaying(false)
{
   //  m_ACKData.m_insID = instID;
   //  m_ACKData.m_sourceNr = NM_VMOST_FBLOCK_ID_NAVI;
   //  m_ACKData.m_isSuccess = true;
   //  m_ACKData.m_activity = NM_VMOST_SA_OFF;
   //  // Create Source FBlock for audio event monitor
   //  m_pSourceFB = new(MEM_Voice) NMVmostFBlockNode(NM_VMOST_FBLOCK_ID_NAVI, m_pInstId, this);
   // if (NULL == m_pSourceFB) {
   //      VC_LOGE("NMVmostFBlockNode create failed...\n");
   //      return;
   // }

   //  // Create AVConnection
   //  m_pConnShadow = new(MEM_Voice) NMVmostShadowNode(NM_VMOST_FBLOCK_ID_NAVI, m_pInstId, this);
   // if (NULL == m_pConnShadow) {
   //      VC_LOGE("NMVmostShadowNode create failed...\n");
   //      return;
   // }
}

VC_AudioSessionAlsa::~VC_AudioSessionAlsa() 
{
    VC_LOGI("~VC_AudioSessionAlsa");
    // delete m_pConnShadow;
    // delete m_pSourceFB;
}

CL_BOOL VC_AudioSessionAlsa::ReqAudioOutOn(BYTE mediaType) 
{
    return true;
 //    m_pNaviSourceNr = SOURCENR_CREATE(NM_VMOST_PHYCHANNEL_NAVI, mediaType);
 //    if (NULL == m_pConnShadow) {
 //        VC_LOGE("m_pConnShadow = NULL...\n");
 //        return CL_FALSE;
 //    }

 //    switch (mediaType) {
 //    case VC_NAVI_LONEPROMPT:
 //        m_iLogicalAVChannel = NM_VMOST_LC_MIX_ALERT_MSG;
 //        break;
 //    case VC_NAVI_SHORTPROMPT:
 //        m_iLogicalAVChannel = NM_VMOST_LC_SHORT_MIX_ALERT;
 //        break;
 //    case VC_NAVI_ALARTTONE:
 //        m_iLogicalAVChannel = NM_VMOST_LC_SYSTEM_PROMPT;
 //        break;
 //    case VC_VR_SPEECHREC:
 //        m_iLogicalAVChannel = NM_VMOST_LC_SPEECH_REC;
 //        break;
 //    case VC_TTS_AUDIO:
 //        m_iLogicalAVChannel = NM_VMOST_LC_SOUND_PROMPT;
 //        break;
 //    default:
 //        m_iLogicalAVChannel = NM_VMOST_LC_MIX_ALERT_MSG;
 //        break;
 //    }

 //    m_pConnShadow->requstAVActivation(m_iLogicalAVChannel, NM_VMOST_FBLOCK_ID_NAVI, m_pInstId, m_pNaviSourceNr);
 //    // 1:logicalChannel   2:FBlockID   3:instID   4:SourceNr(PHYChannel)
 //    // NM_VMOST_LC_MIX_ALERT_MSG, NM_VMOST_FBLOCK_ID_NAVI, NM_VMOST_INSTID_NAVI_NAVI, NAVI_NAVI_SOURCENR
 //    m_isPlaying = true;
    // m_waitObj.Reset();
    // VC_LOGI("req to open channel...");
    // m_state = ST_NONE;
 //    m_waitObj.Wait(VC_WAIT_CHANNELACTIVE_TIME);
    // VC_LOGI("req to open channel end");
 //    return (ST_NONE != m_state);
}

CL_BOOL VC_AudioSessionAlsa::ReqAudioOutOff(BYTE mediaType) 
{
 //    m_pNaviSourceNr = SOURCENR_CREATE(NM_VMOST_PHYCHANNEL_NAVI, mediaType);
 //    if (NULL == m_pConnShadow) {
 //        VC_LOGE("m_pConnShadow = NULL...");
 //        return CL_FALSE;
 //    }
 //    switch (mediaType) {
 //    case VC_NAVI_LONEPROMPT:
 //        m_iLogicalAVChannel = NM_VMOST_LC_MIX_ALERT_MSG;
 //        break;
 //    case VC_NAVI_SHORTPROMPT:
 //        m_iLogicalAVChannel = NM_VMOST_LC_SHORT_MIX_ALERT;
 //        break;
 //    case VC_NAVI_ALARTTONE:
 //        m_iLogicalAVChannel = NM_VMOST_LC_SYSTEM_PROMPT;
 //        break;
 //    case VC_VR_SPEECHREC:
 //        m_iLogicalAVChannel = NM_VMOST_LC_SPEECH_REC;
 //        break;
 //    case VC_TTS_AUDIO:
 //        m_iLogicalAVChannel = NM_VMOST_LC_SOUND_PROMPT;
 //        break;
 //    default:
 //        m_iLogicalAVChannel = NM_VMOST_LC_MIX_ALERT_MSG;
 //        break;
 //    }

    // VC_LOGI("close channel...");
 //    m_isPlaying = false;
    // m_pConnShadow->requstAVDeactivation(m_iLogicalAVChannel, NM_VMOST_FBLOCK_ID_NAVI, m_pInstId, m_pNaviSourceNr);
 //    m_waitObj.Reset();
 //    m_waitObj.Wait(VC_WAIT_CHANNELACTIVE_TIME);
 //    VC_LOGI("close channel end");
    return true;
}

void VC_AudioSessionAlsa::SetChannelCloseByOSListener(VC_AudioSession::VC_AudioSessionListener *listener)
{
    // m_listener = listener;
}

CL_BOOL VC_AudioSessionAlsa::TurnAudioIn(CL_BOOL on) 
{
    return true;
    // VC_LOGI("turn audioin on=%d", on);
    // if (NULL == m_pConnShadow) {
    //     VC_LOGE("m_pConnShadow = NULL...\n");
    //     return CL_FALSE;
    //  }
    
    // INT err = m_pConnShadow->setBaseVoiceMode(on ? NM_VMOST_VOICE_MODE_ON : NM_VMOST_VOICE_MODE_OFF);
    // return (0 == err);
}


// // Handle Tel interrupt event
// INT VC_AudioSessionAlsa::onRecvSourceActivity(CONST BYTE instID, CONST BYTE sourceNr, CONST NMVmostSourceActivity activity, BOOL& isSuccess) {
//     isSuccess = TRUE;
//     VC_LOGD("get result activity=%d, m_isPlaying=%d", activity, m_isPlaying);
//     if (NM_VMOST_SA_OFF == activity && m_isPlaying) {
//         VC_LOGD("Navi Channel off!tickcount=%u", GetTickCount());
//         m_ACKData.m_insID = instID;
//         m_ACKData.m_sourceNr = sourceNr;
//         m_ACKData.m_isSuccess = true;
//         m_ACKData.m_activity = activity;
//         m_listener->OnChannelCloseByOS();
//         return NM_VMOST_SOURCEACTIVITY_ASYNC_ACK;
//     }
//     return 0;
// }

// // Inherit from AVConnectionCallback
// VOID VC_AudioSessionAlsa::onRecvAVActivationResult(CONST INT logicalAVChannel, CONST BYTE fBlockID, CONST BYTE instID, CONST BYTE sourceNr, enum NMVmostRequestResult reqResult) {
//     if (NM_VMOST_FBLOCK_ID_NAVI != fBlockID) {
//         VC_LOGE("fBlockID is wrong, fBlockID=%d", fBlockID);
//         return;
//     }
//     if (m_iLogicalAVChannel != logicalAVChannel) {
//         VC_LOGE("logicalAVChannel is wrong, m_iLogicalAVChannel=%d, logicalAVChannel=%d", m_iLogicalAVChannel, logicalAVChannel);
//         return;
//     }

// 	// NM_VMOST_RS_GRANTED is success
//     m_state = (NM_VMOST_RS_GRANTED == reqResult) ? ST_RUNNING : ST_NONE;
// 	VC_LOGI("Open channel.logiicChannel=%d, sourceNr=%d, res=%d", logicalAVChannel, sourceNr, reqResult, m_state);
// 	if (NM_VMOST_RS_GRANTED != reqResult) {
// 		VC_LOGE("Open channel failed.logiicChannel=%d, sourceNr=%d, res=%d", logicalAVChannel, sourceNr, reqResult, m_state);
// 	}
//     m_waitObj.Notify();
// }

// VOID VC_AudioSessionAlsa::onRecvAVDeactivationResult(CONST INT logicalAVChannel, CONST  BYTE fBlockID, CONST BYTE instID, CONST BYTE sourceNr) {
//     if (NM_VMOST_FBLOCK_ID_NAVI != fBlockID) {
//         VC_LOGE("fBlockID is wrong, fBlockID=%d", fBlockID);
//         return;
//     }
//     if (m_iLogicalAVChannel != logicalAVChannel) {
//         VC_LOGE("logicalAVChannel is wrong, m_iLogicalAVChannel=%d, logicalAVChannel=%d", m_iLogicalAVChannel, logicalAVChannel);
//         return;
//     }

//     VC_LOGI("Deactivate channel success, logiicChannel=%d, sourceNr=%d", logicalAVChannel, sourceNr);
//     m_waitObj.Notify();
// }

// VOID VC_AudioSessionAlsa::onRecvBaseVoiceModeStatus(NMVmostVoiceMode voiceMode) 
// {
// }

// // Inherit from FBlockCallback
// INT VC_AudioSessionAlsa::onRecvSourceInfoGet(CONST BYTE instID, CONST BYTE sourceNr, BOOL& isAvailable) {
//     isAvailable = TRUE;
//     return 0;
// }

// INT VC_AudioSessionAlsa::onRecvAllocate(CONST BYTE instID, CONST BYTE sourceNr, BOOL& isSuccess) {
//     isSuccess = TRUE;
//     return 0;
// }

// INT VC_AudioSessionAlsa::onRecvDeallocate(CONST BYTE instID, CONST BYTE sourceNr, BOOL& isSuccess) {
//     isSuccess = TRUE;
//     return 0;
// }

void VC_AudioSessionAlsa::SendMsgForSourceActivity()
{
    // if (m_pSourceFB) {
    //     VC_LOGI("SendMsgForSourceActivity, tickcount=%u\n", GetTickCount());
    //     m_pSourceFB->sendSourceActivityAckForNavi(m_ACKData.m_insID, m_ACKData.m_sourceNr, m_ACKData.m_activity, m_ACKData.m_isSuccess);
    // }

}
/* EOF */
