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
 * @file DemoRequest.h                                            
 * @brief Summary for DemoRequest.
 *
 * This file includes the declaration of class DemoRequest and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef DEMO_REQUEST_H
#define DEMO_REQUEST_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>

#include "VP_ModuleIF.h"
#include "VP_Factory.h"
#include "VP_VoicePlayControl.h"
#include "VP_ReceiverCb.h"
#include "VP_Request.h"
#include "VP_Event.h"
#include "navi/Voice/VoicePlay/VpPlaySoundList.pb.h"


/**
 * Summary for class DemoRequest
 *
 * There should be a detail description for
 * class DemoRequest
 */
class DemoRequest : public VP_SoundPlayList_Request
{
public:
    explicit DemoRequest(navi::VoicePlay::VpPlaySoundList* msg);

    ~DemoRequest();

    CL_ERROR commit();

    shared_ptr<VP_Event> getPlayListEvent(std::string sender);
    shared_ptr<VP_Event> getPlayListEvent(std::string sender, VP_SMALL_SIGNAL_ID ssid);

private:
    std::shared_ptr<VP_Channel> channel;
    std::shared_ptr<VP_DataAccessorList> pAccessorList;
    std::shared_ptr<VP_ChannelOpenEvent> event;
};

#endif
/* EOF */
