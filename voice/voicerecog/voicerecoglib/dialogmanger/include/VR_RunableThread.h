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
 * @file VR_RunableThread.h
 * @brief Declaration file of class VR_ActionBase.
 *
 * This file includes the declaration of class VR_ActionBase.
 *
 * @attention used for C++ only.
 */

#ifndef VR_RUNABLETHREAD_H
#define VR_RUNABLETHREAD_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include "nceventsys/NCRunnableThread.h"
#include "ncore/NCTypesDefine.h"
#include <ncore/NCString.h>


/**
 * @brief The VR_RunableThread class
 *
 * class declaration
 */
class VR_RunableThread
{
public:
    // 构造函数
    // Thread的Name，不能为空，同时，ThreadName是常量定义方式来做
    // 例如：const std::string VR_VOICEXXXXXXXXXX = "VR_VoiceXXXXXXXX"
    // 创建这个Thread，并Start起来
    VR_RunableThread(const nutshell::NCString& threadName, bool bFirstStart = true);

    // 析构函数
    virtual ~VR_RunableThread();

    // 获取RunableLooper
   nutshell::NCRunnableLooper getRunableLooper();

   // Stop Thread
    void stopThread();

   // Start Thread
    void startThread();

private:
    VR_RunableThread(const VR_RunableThread &);
    VR_RunableThread & operator=(const VR_RunableThread &);

    nutshell::NCRunnableThread   m_runnableThread;
    nutshell::NCString           m_ncThreadName;

};

#endif /* VR_RUNABLETHREAD_H */
/* EOF */
