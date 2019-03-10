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
 * @file VC_AudioSessionAlsa.h                                            
 * @brief Summary for VC_AudioSessionAlsa.
 *
 * This file includes the declaration of class VC_AudioSessionAlsa and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_AUDIOSESSIONALSA_H
#define VC_AUDIOSESSIONALSA_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VC_AudioSession.h"
#include "CL_WaitObj.h"

// using namespace nutshell;

// class SourceFBlock;
// class AVMConnectionShadow;

/**
 * Summary for class VC_AudioSessionAlsa
 * 
 * This is a detail description for class VC_AudioSessionAlsa
 *
 */
class  VC_AudioSessionAlsa: public VC_AudioSession 
{
public:
    explicit VC_AudioSessionAlsa(BYTE instID);
    virtual ~VC_AudioSessionAlsa();

    virtual CL_BOOL ReqAudioOutOn(BYTE mediaType);
    virtual CL_BOOL ReqAudioOutOff(BYTE mediaType);
    virtual CL_BOOL TurnAudioIn(CL_BOOL on);
    
    virtual CL_BOOL SetSessionActive(CL_BOOL on) 
    { 
        return  CL_TRUE; 
    }

    virtual void SetChannelCloseByOSListener(VC_AudioSession::VC_AudioSessionListener *listener);

    // Inherit from NMVmostFBlockCallback
    // virtual INT onRecvSourceInfoGet(CONST BYTE instID, CONST BYTE sourceNr, BOOL& isAvailable);
    // virtual INT onRecvAllocate(CONST BYTE instID, CONST BYTE sourceNr, BOOL& isSuccess);
    // virtual INT onRecvDeallocate(CONST BYTE instID, CONST BYTE sourceNr, BOOL& isSuccess);
    // virtual INT onRecvSourceActivity(CONST BYTE instID, CONST BYTE sourceNr, CONST NMVmostSourceActivity activity, BOOL& isSuccess);

    // // Inherit from NMVmostShadowCallback
    // virtual VOID onRecvAVActivationResult(CONST INT logicalAVChannel, CONST BYTE fBlockID, CONST BYTE instID, CONST BYTE sourceNr, enum NMVmostRequestResult reqResult);

    // virtual VOID onRecvAVDeactivationResult(CONST INT logicalAVChannel, CONST  BYTE fBlockID, CONST BYTE instID, CONST BYTE sourceNr);

    // virtual VOID onRecvBaseVoiceModeStatus(NMVmostVoiceMode voiceMode);

    virtual void SendMsgForSourceActivity();

private:
    VOID WaitReady(INT audioType);

private:
    // struct VC_ACKData       // store channel callback data
    // {
    //     BYTE                    m_insID;
    //     BYTE                    m_sourceNr;
    //     bool                    m_isSuccess;
    //     NMVmostSourceActivity   m_activity;
    // };

    // static const INT PLAYER_NUM = 2;
    // enum State{ ST_NONE = 0, ST_READY, ST_RUNNING };
    // State m_state;
    // NMVmostFBlockNode* m_pSourceFB;
    // NMVmostShadowNode* m_pConnShadow;

    // BYTE m_pInstId;
    // BYTE m_pNaviSourceNr;
    // CL_WaitObj m_waitObj;
    // INT m_iLogicalAVChannel;
    // VC_AudioSession::VC_AudioSessionListener *m_listener;
    // bool m_isPlaying;
    // VC_ACKData  m_ACKData;
};


#endif /* VC_AUDIOSESSIONALSA_H */
/* EOF */
