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
 * @file VR_ActionBase.h
 * @brief Declaration file of class VR_ActionBase.
 *
 * This file includes the declaration of class VR_ActionBase.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ACTIONBASE_H
#define VR_ACTIONBASE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_INDENTIFIER_HPP
#   include "VR_Indentifier.hpp"
#endif

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ActionEventSender);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_SessionManger);
/**
 * @brief The VR_ActionBase class
 *
 * class declaration
 */
class VR_ActionBase 
{
public:
    const static int INVALID = VR_IdGenerator::INVALID;

public:
    explicit VR_ActionBase(spVR_ActionEventSender sender);
    virtual ~VR_ActionBase();

public:
    int GetActionId() const;
    bool IsActionCompleted() const;
    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase& proto);

    virtual void SetSessionManager(spVR_SessionManger spSessionManager);

protected:
    void SetActionComplete();

protected:
    spVR_ActionEventSender m_sender;
    VR_IdGenerator         m_Id;

private:
    bool m_bActionComplete;

};

#endif /* VR_ACTIONBASE_H */
/* EOF */
