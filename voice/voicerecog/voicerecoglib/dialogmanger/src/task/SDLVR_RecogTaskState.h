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
 * @file SDLVR_RecogTaskStateBase.h
 * @brief Declaration file of class SDLVR_RecogTaskStateBase and Derived classes.
 *
 * This file includes the declaration of class SDLVR_RecogTaskStateBase and Derived classes.
 *
 * @attention used for C++ only.
 */

#ifndef SDLVR_RECOGTASKSTATE_H
#define SDLVR_RECOGTASKSTATE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_RecogTaskState.h"

/**
 *  @brief The SDLVR_RecogTaskStateBase class
 *
 *   class  declaration
 */
class  SDLVR_RecogTaskStateBase : public VR_RecogTaskStateBase
{
public:
    SDLVR_RecogTaskStateBase(VR_RecogTaskIF* task);
    virtual ~SDLVR_RecogTaskStateBase() {}

    virtual void Start(int nType);
    virtual void Cancel();
};

/**
 * @brief The SDLVR_RecogTaskStateIDL class
 *
 * class declaration
 */
class  SDLVR_RecogTaskStateIDL : public SDLVR_RecogTaskStateBase
{
public:
    SDLVR_RecogTaskStateIDL(VR_RecogTaskIF* task);
    virtual void Start(int nType);
    virtual void Cancel();
};

/**
 * @brief The SDLVR_RecogTaskStateStart class
 *
 * class declaration
 */
class SDLVR_RecogTaskStateStart : public SDLVR_RecogTaskStateBase
{
public:
    SDLVR_RecogTaskStateStart(VR_RecogTaskIF* task);
    virtual void Cancel();
    virtual void OnStarted(bool bResult);

};

/**
 * @brief The SDLVR_RecogTaskStateCancel class
 *
 * class declaration
 */
class SDLVR_RecogTaskStateCancel : public SDLVR_RecogTaskStateBase
{
public:
    SDLVR_RecogTaskStateCancel(VR_RecogTaskIF* task);
    virtual void OnCanceled();

};

/**
 * @brief The SDLVR_RecogTaskStateWork class
 *
 * class declaration
 */
class SDLVR_RecogTaskStateWork : public SDLVR_RecogTaskStateBase
{
public:
    SDLVR_RecogTaskStateWork(VR_RecogTaskIF* task);
    virtual void Cancel();
    virtual void UserOperation(const VR_String & type);
    virtual void OnUsrOperationEnd(const VR_String & type);
    virtual void OnNotifyResult();
    virtual void OnNotifyComplete();

};

#endif // SDLVR_RECOGTASKSTATE_H
/* EOF */
