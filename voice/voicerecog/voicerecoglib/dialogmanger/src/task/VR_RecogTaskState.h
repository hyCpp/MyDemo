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
 * @file VR_RecogTask_State.h
 * @brief Declaration file of class VR_RecogTaskStateBase and Derived classes.
 *
 * This file includes the declaration of class VR_RecogTaskStateBase and Derived classes.
 *
 * @attention used for C++ only.
 */

#ifndef VR_RECOGTASK_STATE
#define VR_RECOGTASK_STATE

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_RecogTask.h"

class VR_RecogTaskIF;

/**
 * @brief The VR_RecogTaskStateBase class
 *
 * class declaration
 */
class VR_RecogTaskStateBase
{
public:
    VR_RecogTaskStateBase(VR_RecogTaskIF* task);
    virtual ~VR_RecogTaskStateBase() {}

public:
    virtual void Start(int nType);
    virtual void Cancel();
    virtual void UserOperation(const VR_String & type);
    virtual void OnUsrOperationEnd(const VR_String & type);
    virtual void OnStarted(bool bResult);
    virtual void OnCanceled();
    virtual void OnNotifyResult();
    virtual void OnNotifyComplete();

    VR_RecogTaskIF* GetRecogTask();

protected:
    VR_RecogTaskIF* m_RecogTask;

private:
    VR_RecogTaskStateBase(VR_RecogTaskStateBase&);
    VR_RecogTaskStateBase& operator = (VR_RecogTaskStateBase&);
};

/**
 * @brief The VR_RecogTaskStateIDL class
 *
 * class declaration
 */
class VR_RecogTaskStateIDL : public VR_RecogTaskStateBase
{
public:
    VR_RecogTaskStateIDL(VR_RecogTaskIF* task);
    virtual void Start(int nType);
    virtual void Cancel();

};

/**
 * @brief The VR_RecogTaskStateStart class
 *
 * class declaration
 */
class VR_RecogTaskStateStart : public VR_RecogTaskStateBase
{
public:
    VR_RecogTaskStateStart(VR_RecogTaskIF* task);
    virtual void Cancel();
    virtual void OnStarted(bool bResult);

};

/**
 * @brief The VR_RecogTaskStateCancel class
 *
 * class declaration
 */
class VR_RecogTaskStateCancel : public VR_RecogTaskStateBase
{
public:
    VR_RecogTaskStateCancel(VR_RecogTaskIF* task);
    virtual void OnCanceled();

};

/**
 * @brief The VR_RecogTaskStateWork class
 *
 * class declaration
 */
class VR_RecogTaskStateWork : public VR_RecogTaskStateBase
{
public:
    VR_RecogTaskStateWork(VR_RecogTaskIF* task);
    virtual void Cancel();
    virtual void UserOperation(const VR_String & type);
    virtual void OnUsrOperationEnd(const VR_String & type);
    virtual void OnNotifyResult();
    virtual void OnNotifyComplete();

};

#endif // RECOGTASK_STATE
/* EOF */
