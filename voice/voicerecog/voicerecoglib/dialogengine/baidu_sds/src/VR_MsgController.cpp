/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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

/* Suntec Header */
#ifndef VR_LOG_H
#    include "VR_Log.h"
#endif

#ifndef CXX_BL_AUTOSYNC_H
#   include "BL_AutoSync.h"
#endif

#ifndef VR_XMLPARSER_H
#    include "VR_XmlParser.h"
#endif

#ifndef VR_MSGCONTROLLER_H
#    include "VR_MsgController.h"
#endif

#include "VR_Log.h"

BL_SyncObject   VR_MsgController::s_cSync;   ///< Sync object

// Constructor
VR_MsgController::VR_MsgController()
: m_processor(nullptr)
, m_msglist()
, m_bDMStartNotify(false)
{
}

// Destructor
VR_MsgController::~VR_MsgController()
{

}

// Initialize the Message Controller
bool
VR_MsgController::Initialize(VR_DEProcessor* processor)
{
    if (!processor) {
        return false;
    }

    m_processor = processor;

    RegisterName("VR_VBT_AGENT_DATA_THREAD");
    return true;
}

// Start the Message Controller thread
void
VR_MsgController::Start()
{
    StartRegistThread();
}

// Stop the Message Controller thread
void
VR_MsgController::Stop()
{
    StopThread();
}

// Post message to the Message Controller
bool
VR_MsgController::PostMessage(const std::string& message, int actionSeqId)
{
    VR_MsgInfo msgInfo;
    msgInfo.iMsgId = actionSeqId;
    msgInfo.strMsg = message;

    // if (m_bDMStartNotify) {
        // Add new messages to the list
        BL_AutoSync cAutoSync(s_cSync);
        m_msglist.push_back(msgInfo);
        Notify();
    // }
    // else {
    //     VR_XmlParser parser(message);
    //     if ("startedNotify" == parser.getXmlKey()) {
    //         m_bDMStartNotify = true;
    //         // insert this message to the head
    //         BL_AutoSync cAutoSync(s_cSync);
    //         m_msglist.push_front(msgInfo);
    //         Notify();
    //     }
    //     else {
    //         // Add new messages to the list, wait for start notify
    //         BL_AutoSync cAutoSync(s_cSync);
    //         m_msglist.push_back(msgInfo);
    //     }
    // }

    return true;
}

// Push message to the front of message list
bool
VR_MsgController::PushFrontMessage(const std::string& message, int actionSeqId)
{
    // Add new messages to the front of list
    {
        VR_MsgInfo msgInfo;
        msgInfo.iMsgId = actionSeqId;
        msgInfo.strMsg = message;
        BL_AutoSync cAutoSync(s_cSync);
        m_msglist.push_front(msgInfo);
    }

    Notify();

    return true;
}

// Process the messages
void
VR_MsgController::Run()
{
    VR_LOGD_FUNC();
    while (true) {
        if (m_msglist.empty()) {
            Wait();
        }

        if (TRUE == CheckQuit()) {
            break;
        }

        while (!m_msglist.empty()) {
            if (TRUE == CheckQuit()) {
                break;
            }

            VR_MsgInfo msgInfo;
            {
                BL_AutoSync cAutoSync(s_cSync);
                if (!m_msglist.empty()) {
                    msgInfo = m_msglist.front();
                    m_msglist.pop_front();
                }
                else {
                    continue;
                }
            }

            VR_LOGD("VR_MsgController ProcessMessage msgInfo: %s", msgInfo.strMsg.c_str());
            m_processor->ProcessMessage(msgInfo.strMsg, msgInfo.iMsgId);
        }
    }
}

void
VR_MsgController::ClearMessage()
{
    BL_AutoSync cAutoSync(s_cSync);
    m_msglist.clear();
}

/* EOF */
