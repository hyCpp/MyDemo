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
 * @file  MMVR_RecogTask_State.h
 * @brief Declaration file of class MMVR_RecogTaskStateBase and Derived classes.
 *
 * This file includes the declaration of class MMVR_RecogTaskStateBase and Derived classes.
 *
 * @attention used for C++ only.
 */
#ifndef MMVR_RECOGTASKSTATE_H
#define MMVR_RECOGTASKSTATE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_RecogTaskState.h"

/**
 *  @brief The MMVR_RecogTaskStateBase class
 *
 *   class  declaration
 */
class  MMVR_RecogTaskStateBase : public VR_RecogTaskStateBase
{
public:
    MMVR_RecogTaskStateBase(VR_RecogTaskIF* task);
    virtual ~MMVR_RecogTaskStateBase() {}

    virtual void Start(int nType);
    virtual void Cancel();
};

/**
 * @brief The MMVR_RecogTaskStateIDL class
 *
 * class declaration
 */
class  MMVR_RecogTaskStateIDL : public MMVR_RecogTaskStateBase
{
public:
    MMVR_RecogTaskStateIDL(VR_RecogTaskIF* task);
    virtual void Start(int nType);
    virtual void Cancel();
};

/**
 * @brief The MMVR_RecogTaskStateStart class
 *
 * class declaration
 */
class MMVR_RecogTaskStateStart : public MMVR_RecogTaskStateBase
{
public:
    MMVR_RecogTaskStateStart(VR_RecogTaskIF* task);
    virtual void Cancel();
    virtual void OnStarted(bool bResult);

};

/**
 * @brief The MMVR_RecogTaskStateCancel class
 *
 * class declaration
 */
class MMVR_RecogTaskStateCancel : public MMVR_RecogTaskStateBase
{
public:
    MMVR_RecogTaskStateCancel(VR_RecogTaskIF* task);
    virtual void OnCanceled();

};

/**
 * @brief The MMVR_RecogTaskStateWork class
 *
 * class declaration
 */
class MMVR_RecogTaskStateWork : public MMVR_RecogTaskStateBase
{
public:
    MMVR_RecogTaskStateWork(VR_RecogTaskIF* task);
    virtual void Cancel();
    virtual void UserOperation(const VR_String & type);
    virtual void OnUsrOperationEnd(const VR_String & type);
    virtual void OnNotifyResult();
    virtual void OnNotifyComplete();

};

#endif // MMVR_RECOGTASKSTATE_H
/* EOF */
