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
 * @brief Declaration file of class VR_DP_ThreadWorking.
 *
 * This file includes the declaration of class VR_DP_ThreadWorking.
 *
 * @attention used for C++ only.
 */

#include "stdafx.h"
#include "VR_MusicListener.h"
#include "VR_DP_ThreadWorking.h"
#include "VR_AudioSourceListener.h"
#include "VR_PhoneBookListener.h"
#include "VR_PhoneBookStateListener.h"
#include "VR_RadioListener.h"
#include "VR_TslStateListener.h"
#include "BL_AutoSync.h"

namespace navi
{
namespace dataprovider
{

VR_DP_ThreadWorking::VR_DP_ThreadWorking()
    :BL_Thread()
{
    VR_LOGD_FUNC();
}

VR_DP_ThreadWorking::~VR_DP_ThreadWorking()
{
    VR_LOGD_FUNC();
}

void VR_DP_ThreadWorking::onUpdataPreare(spVR_DP_OB_Action pObAction)
{
    VR_LOGD_FUNC();
    {
        BL_AutoSync cAutoSync(m_SyncObject);
        m_qObActions.push(pObAction);
    }
    Notify();
}

void VR_DP_ThreadWorking::Run()
{
    VR_LOGD_FUNC();
    while (true) {
        VR_LOGI("VR_DM_DP_CommandHandle Run");
        if (m_qObActions.empty()) {
            Wait();
        }

        if (CL_TRUE == CheckQuit()) {
            break;
        }

        while (!m_qObActions.empty()) {
            if (CL_TRUE == CheckQuit()) {
                break;
            }
            spVR_DP_OB_Action pObAction;
            {
                BL_AutoSync cAutoSync(m_SyncObject);
                pObAction = m_qObActions.front();
                m_qObActions.pop();
            }

            if (pObAction) {
                pObAction->DoUpdatePrepare();
            }

        }
    }
}

void VR_DP_ThreadWorking::StartThread()
{
    VR_LOGD_FUNC();
    // DataProvider数据更新处理用
    this->RegisterName("VR_DM_DP_CONTROLLER_THREAD");
    this->StartRegistThread();
}

// thread stop
void VR_DP_ThreadWorking::StopThread()
{
    VR_LOGD_FUNC();
    BL_Thread::StopThread();
    VR_MusicListener::Destory();
    VR_AudioSourceListener::Destory();
    VR_PhoneBookListener::Dispose();
    VR_PhoneBookStateListener::Dispose();
    VR_RadioListener::Dispose();
    VR_TslStateListener::Dispose();
}

}
}

/* EOF */
