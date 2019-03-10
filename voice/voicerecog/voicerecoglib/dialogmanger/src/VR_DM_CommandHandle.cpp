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
 * @file VR_DM_CommandHandle.h
 * @brief Declaration file of class VR_DM_CommandHandle.
 *
 * This file includes the declaration of class VR_DM_CommandHandle.
 *
 * @attention used for C++ only.
 */

#include "stdafx.h"

#ifndef CXX_BL_AUTOSYNC_H
#   include "BL_AutoSync.h"
#endif

#ifndef VR_DM_COMMANDHANDLE_H
#   include "VR_DM_CommandHandle.h"
#endif

VR_DM_CommandHandle::VR_DM_CommandHandle()
    :BL_Thread()
{

}

VR_DM_CommandHandle::~VR_DM_CommandHandle()
{

}

void VR_DM_CommandHandle::onCommand(spVR_Command pCmd)
{
    {
        BL_AutoSync cAutoSync(m_SyncObject);
        m_qCmds.push(pCmd);
    }
    Notify();
}

void VR_DM_CommandHandle::Run()
{
    while (true) {
        if (m_qCmds.empty()) {
            Wait();
        }

        if (CL_TRUE == CheckQuit()) {
            break;
        }

        while (!m_qCmds.empty()) {
            if (CL_TRUE == CheckQuit()) {
                break;
            }
            spVR_Command pCmd;
            {
                BL_AutoSync cAutoSync(m_SyncObject);
                pCmd = m_qCmds.front();
                m_qCmds.pop();
            }

            if (pCmd) {
                pCmd->perform();
            }

        }
    }
}

void VR_DM_CommandHandle::StartDMThread()
{
    VR_LOGD_FUNC();
    this->RegisterName("VR_DM_MSG_CONTROLLER_THREAD");
    this->StartRegistThread();
}

// thread stop
void VR_DM_CommandHandle::StopDMThread()
{
    VR_LOGD_FUNC();
    BL_Thread::StopThread();
}

/* EOF */
