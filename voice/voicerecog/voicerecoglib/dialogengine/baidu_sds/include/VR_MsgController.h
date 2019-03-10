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

/**
 * @file VR_MsgController.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_MSGCONTROLLER_H
#define VR_MSGCONTROLLER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/* Standard Header */
#include <string>

/* Suntec Header */
#ifndef MEM_LIST_H
#    include "MEM_list.h"
#endif

#ifndef CXX_BL_THREAD_H
#    include "BL_Thread.h"
#endif

#ifndef CXX_BL_SyncObject_H
#    include "BL_SyncObject.h"
#endif

#ifndef VR_DEPROCESSOR_H
#    include "VR_DEProcessor.h"
#endif

#ifndef VR_CONTROLLERBASE_H
#    include "VR_ControllerBase.h"
#endif

/**
 * @brief The VR_MsgController class
 *
 * class declaration
 */
class VR_MsgController : public VR_ControllerBase, public BL_Thread
{
public:
    /**
     * Constructor
     *
     */
    VR_MsgController();

    /**
     * Destructor
     *
     */
    virtual ~VR_MsgController();

    /**
     * Initialize
     *
     * Initialize Message Controller
     *
     * @param pcEngine [IN] : Voice recognized engine
     * @return bool
     * @attention none
     * @see none
     */
    bool Initialize(VR_DEProcessor* processor);

    /**
     * Start
     *
     * Start the Message Controller thread
     *
     * @param none
     * @return none
     * @attention none
     * @see none
     */
    void Start();

    /**
     * Start
     *
     * Stop the Message Controller thread
     *
     * @param none
     * @return none
     * @attention none
     * @see none
     */
    void Stop();

    /**
     * Post Message
     *
     * Post message to the Message Controller
     *
     * @param message [IN] : XML format message
     * @return bool
     * @attention none
     * @see none
     */
    bool PostMessage(const std::string& message, int actionSeqId = -1);

    /**
     * Push Front Message
     *
     * Push message to the front of the message list
     *
     * @param message [IN] : XML format message
     * @return bool
     * @attention none
     * @see none
     */
    bool PushFrontMessage(const std::string& message, int actionSeqId = -1);

    void ClearMessage();

protected:
    /**
     * Run
     *
     * Override base class's Run() function
     *
     * @param none
     * @return none
     * @attention none
     * @see none
     */
    virtual void Run();

private:
    VR_DEProcessor*               m_processor;


    // Used for message list
    struct VR_MsgInfo
    {
        int iMsgId;
        std::string strMsg;
    };

    VoiceList<VR_MsgInfo>::type   m_msglist;      ///< Message list
    static BL_SyncObject          s_cSync;        ///< Sync object

    bool                          m_bDMStartNotify;

    /**
     * Copy Constructor
     *
     */
    VR_MsgController(const VR_MsgController&);

    /**
     * Copy Assignment Operator
     *
     */
    VR_MsgController &operator=(const VR_MsgController&);
};

#endif // VR_MSGCONTROLLER_H
/* EOF */
