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
 * @file VR_UserManager.cpp
 * @brief Defintion file of class VR_UserManager.
 *
 * This file includes the Defintion of class VR_UserManager and Derived classes.
 *
 * @attention used for C++ only.
 */

#include "VR_Log.h"
#include "VR_Def.h"
#include "VR_UserManager.h"
#include "VR_XmlStrParse.h"

#include "system/NCUser.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"

using namespace nutshell;
using namespace navi::VoiceRecog;

#define VR_USER_DEFAULT_NAME    "default"

VR_UserManager::VR_UserManager()
{
    VR_LOGD_FUNC();
}

VR_UserManager::~VR_UserManager()
{
    VR_LOGD_FUNC();
}

bool VR_UserManager::Initialize()
{
    VR_LOGD_FUNC();
    NCUser userManager;

    NCString UserName;
    UINT32 UserVer;     // what's version means?
    bool bUserDefault = false;  // all error condition load default user

    NCUser::Result Res = userManager.getCurrentUser(UserName, UserVer);
    if (NCUser::Result::Result_Success != Res) {
        VR_LOGD("get user failed. code:%d", Res);
        bUserDefault = true;
    }

    common::NDSystemUserInfo userInfo;
    if (!bUserDefault) {
        Res = userManager.getUserInfo(UserName, userInfo, UserVer);
        if (NCUser::Result::Result_Success != Res) {
            VR_LOGD("get user info failed code:%d", Res);
            m_CurUserId = VR_USER_DEFAULT_NAME;
        }
        else if ("" == userInfo.id()) {
            VR_LOGD("id empty");
            m_CurUserId = VR_USER_DEFAULT_NAME;
        }
        else {
            m_CurUserId = userInfo.id();
        }
    }
    else {
        m_CurUserId = VR_USER_DEFAULT_NAME;
    }

    VR_LOGD("current user: %s", m_CurUserId.c_str());
    for (auto ptrFunc : m_UserFuncList) {
        if (ptrFunc) {
            ptrFunc->ChangeUserContext(m_CurUserId);
        }
    }

    return true;
}

void VR_UserManager::AddUserFunc(VR_UserSpecFuncBase* ptrFunc)
{
    VR_LOGD_FUNC();
    if (nullptr == ptrFunc) {
        VR_LOGD("func invalid");
        return;
    }

    m_UserFuncList.push_back(ptrFunc);
}

void VR_UserManager::HandleUserManual(const VR_ProtoMessageBase* mesg)
{
    VR_LOGD_FUNC();

    const NIVoiceUserManualInfo* proto = dynamic_cast<const NIVoiceUserManualInfo*>(mesg);
    if (nullptr == proto) {
        VR_LOGD("cast failed");
        return;
    }

    UINT32 code = proto->code();
    switch (code) {
    case Vr_UserFunc_AddUser: {
        VR_String id = proto->username();
        VR_LOGD("Add user: %s", id.c_str());
        break;
    }
    case Vr_UserFunc_DelUser: {
        VR_String id = proto->username();
        VR_LOGD("Delete user: %s", id.c_str());
        DeleteVoiceUser(id);
        break;
    }
    case Vr_UserFunc_SwitchUser: {
        VR_String id;
        if ("" != proto->username()) {
            id = proto->username();   // system interface paramters name confuse it'e mean
        }
        else {
            id = VR_USER_DEFAULT_NAME;
        }

        VR_String oldId = proto->oldusername();   // system interface paramters name confuse it'e mean
        VR_LOGD("Switch old: %s to new %s", oldId.c_str(), id.c_str());
        SwitchVoiceUser(id);
        break;
    }
    default: {
        break;
    }
    }
}

void VR_UserManager::SwitchVoiceUser(const VR_String& user_id)
{
    VR_LOGD_FUNC();

    m_CurUserId = user_id;
    VR_LOGD("switch user: %s", m_CurUserId.c_str());

    for (auto ptrFunc : m_UserFuncList) {
        if (ptrFunc) {
            ptrFunc->ChangeUserContext(m_CurUserId);
        }
    }
}

void VR_UserManager::DeleteVoiceUser(const VR_String& user_id)
{
    VR_LOGD_FUNC();

    VR_LOGD("delete user id: %s", user_id.c_str());
    for (auto ptrFunc : m_UserFuncList) {
        if (ptrFunc) {
            ptrFunc->DeleteUserContext(user_id);
        }
    }
}

bool VR_UserManager::QueryUserExistsStatus(const VR_String& vp_id, bool& result)
{
    VR_LOGD_FUNC();
    NCUser userManager;

    common::NDSystemUserInfoList userList;
    UINT32 version;
    int res = userManager.getUserList(userList, version);
    if (NCUser::Result::Result_Success != res) {
        VR_LOGD("get list error ");
        return false;
    }

    int cnt = userList.users().size();
    result = false;
    for (int i = 0; i < cnt; i++) {
        const common::NDSystemUserInfo& user = userList.users(i);
        if (vp_id == user.voiceprint_id()) {
            VR_LOGD("rename user");
            result = true;
        }
    }

    return true;
}

/* EOF */
