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
 * @file VR_DialogTaskBase.h
 * @brief Declaration file of class VR_DialogTaskBase.
 *
 * This file includes the declaration of class VR_DialogTaskBase.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DIALOGTASKBASE_H
#define VR_DIALOGTASKBASE_H


#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"
#include "VR_Indentifier.hpp"

class EV_EventContext;
class VR_DialogEngineIF;
VR_DECLARE_CLASS_WITH_SMART_PTR(EV_EventContext);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);

/**
 * @brief The VR_DialogTaskBase class
 *
 * class declaration
 */
class VR_DialogTaskBase
{
public:
    VR_DialogTaskBase();
    virtual ~VR_DialogTaskBase();

public:
    int GetTaskID() const;
    bool IsComplete() const;

public:
    virtual void Start () = 0;
    virtual void Start(const EV_EventContext&, const VR_ProtoMessageBase&);
    virtual void Cancel (const EV_EventContext &context) = 0;
    virtual void UserOperation(const EV_EventContext &context, const VR_String &type);
    virtual void OnMessage (const VR_ProtoMessageBase &msg) = 0;

    // tsl use only
    virtual void OnRequestVrTsl(const EV_EventContext& context, const VR_ProtoMessageBase& msg);


public:
    const static int INVALID = 0;

protected:
    bool SetComplete();

private:
    bool                  m_bIsComplete;
    VR_Indentifier<int>   m_taskId;
};


/**
 * @brief The VR_DialogTaskNUll class
 *
 * class declaration
 */
class VR_DialogTaskNull : public VR_DialogTaskBase
{
public:
    VR_DialogTaskNull();
    virtual ~VR_DialogTaskNull();

public:
    virtual void Start ();
    virtual void Cancel (const EV_EventContext &context);
    virtual void UserOperation(const EV_EventContext &context, const VR_String &type);
    virtual void OnMessage (const VR_ProtoMessageBase &msg);

    // tsl use only
    void OnRequestVrTsl(const EV_EventContext& context, const VR_ProtoMessageBase& msg);



};


#endif /* VR_DIALOGTASKBASE_H */
/* EOF */
