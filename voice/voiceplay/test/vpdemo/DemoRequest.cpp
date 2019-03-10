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

#include "DemoRequest.h"
using namespace ::navi::VoicePlay;

DemoRequest::DemoRequest(VpPlaySoundList *msg) : VP_SoundPlayList_Request(msg, std::string("MNC"))
{
}

DemoRequest::~DemoRequest()
{
}

CL_ERROR DemoRequest::commit()
{
    return CL_TRUE;
}

shared_ptr<VP_Event> DemoRequest::getPlayListEvent(std::string sender)
{
    shared_ptr<VP_PlayListEvent> event = make_shared<VP_PlayListEvent>(m_phraseList, m_ssid, sender, userdata);
    return event;
}

shared_ptr<VP_Event> DemoRequest::getPlayListEvent(std::string sender, VP_SMALL_SIGNAL_ID ssid)
{
   shared_ptr<VP_PlayListEvent> event = make_shared<VP_PlayListEvent>(m_phraseList, ssid, sender, userdata);
   return event;
} 

/* EOF */

