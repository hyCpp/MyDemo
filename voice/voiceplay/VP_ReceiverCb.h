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
 * @file VP_ReceiverCb.h                                            
 * @brief Summary for VP_ReceiverCb.
 *
 * This file includes the declaration of class VP_ReceiverCb and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_RECEIVERCB_H
#define VP_RECEIVERCB_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <string>
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventReceiver.h"
#include "navi/Voice/VoicePlay/VpPlaySoundList.pb.h"
#include "navi/Voice/VoicePlay/VpGetDuration.pb.h"
#include "navi/Setting/SettingChange.pb.h"
#include "VP_Request.h"

class EV_EventReceiver;

/**
 * Summary for class VP_ReceiverCb
 * 
 * This is a detail description for class VP_ReceiverCb
 *
 */
class VP_ReceiverCb
{
public: 

    void Register(EV_EventReceiver* er);

    void OnRecvSoundPlayID(void* msg);

    void OnRecvSoundPlayTTS(void* msg);

    void OnRecvSoundPlayList(void* msg);

    void OnRecvSoundStopPlay(void* msg);

    void OnRecvGetDuration(void* msg);

    void OnRecvTtsPlayEnd(void* msg);

    void OnRecvTtsServiceReady(void *msg); 

    void OnRecvVoiceTest(const EV_EventContext& context, void* msg);

    void OnRecvSetMute(void *msg);

    void OnRecvSettingChange(void *msg);

    void OnRecvSoundPlayFile(void* msg);

private:
    void setTtsLang(BL_APLINFO_REGION region, const std::string& sender, std::string& ttsLang);

};


#endif /* VP_RECEIVERCB_H */
/* EOF */
