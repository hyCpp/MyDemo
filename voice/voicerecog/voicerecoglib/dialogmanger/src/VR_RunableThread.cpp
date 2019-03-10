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
#include "VR_RunableThread.h"

// 构造函数
VR_RunableThread::VR_RunableThread(const nutshell::NCString &threadName, bool bFirstStart)
    : m_runnableThread()
    , m_ncThreadName(threadName)
{
    VR_LOGD_FUNC();

    // 启动这个Thread
    if (bFirstStart) {
        m_runnableThread.startThread(threadName);
    }
}

// 析构函数
VR_RunableThread::~VR_RunableThread()
{
    VR_LOGD_FUNC();
}

// 获取Looper
nutshell::NCRunnableLooper VR_RunableThread::getRunableLooper()
{
    VR_LOGD_FUNC();
    // 返回有效的Looper
    return m_runnableThread.looper();
}

// 停止Listener Thread
void VR_RunableThread::stopThread()
{
        VR_LOGD_FUNC();
        // Thread停止
        m_runnableThread.stopThread();

}

void  VR_RunableThread::startThread()
{
    VR_LOGD_FUNC();
    m_runnableThread.startThread(m_ncThreadName);
}

/* EOF */
