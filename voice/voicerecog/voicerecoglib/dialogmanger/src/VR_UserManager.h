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
 * @file VR_UserManager.h
 * @brief Declaration file of class VR_UserManager.
 *
 * This file includes the declaration of class VR_UserManager.
 *
 * @attention used for C++ only.
 */

#ifndef VR_USERMANAGER_H
#define VR_USERMANAGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"

/**
 * @brief The VR_UserSpecFuncBase class
 *
 * class declaration
 */
class VR_UserSpecFuncBase
{
public:
    VR_UserSpecFuncBase() {}
    virtual ~VR_UserSpecFuncBase() {}

public:
    virtual void ChangeUserContext(const VR_String& user_id) = 0;
    virtual void DeleteUserContext(const VR_String& user_id) = 0;
};

/**
 * @brief The VR_UserManager class
 *
 * class declaration
 */
class VR_UserManager
{
public:
    VR_UserManager();
    virtual ~VR_UserManager();

    bool Initialize();
    void AddUserFunc(VR_UserSpecFuncBase* func);
    void HandleUserManual(const VR_ProtoMessageBase *mesg);
    bool QueryUserExistsStatus(const VR_String& vp_id, bool& result);

private:
    void SwitchVoiceUser(const VR_String& user_id);
    void DeleteVoiceUser(const VR_String& user_id);

    std::list<VR_UserSpecFuncBase*> m_UserFuncList;
    VR_String m_CurUserId;
};

#endif // VR_USERMANAGER_H
/* EOF */
