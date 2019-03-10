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
 * @file  VR_SettingListener.h
 * @brief Declaration file fo class VR_SettingListener
 * This file include declaration of class VR_SettingListener
 * @attention use for C++ only
 */

#ifndef VR_LANGUAGELISTENER_H
#define VR_LANGUAGELISTENER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#    include"VR_Macro.h"
#endif
#include"VR_Def.h"
#include <contentresolver/NCContentListener.h>
#include <ncore/NCTypesDefine.h>
#include <ncore/NCString.h>
#include <ncore/NCUri.h>
#include <nceventsys/NCEventHandler.h>
#include "VR_CommandFactory.h"
#include "VR_DM_CommandHandle.h"

class VR_MessageProcessIF;
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_MessageProcessIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_CommandFactory);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DM_CommandHandle);

const std::string VR_VOICESETTINGLISTENER = "VR_VoiceSettingListener";

/**
 * @brief The class of VR_SettingListener
 *
 *  class declaration
 */
class VR_SettingListener : public nutshell::NCContentListener
{
public:
    explicit VR_SettingListener(const nutshell::NCString& listenerName,
                                const nutshell::NCRunnableLooper &looper,
                                spVR_MessageProcessIF spMessageProcess,
                                spVR_CommandFactory spVrCommandFactory,
                                spVR_DM_CommandHandle spCmdHandle);

    virtual ~VR_SettingListener();

    ////// some notify funcs
    virtual VOID onNotifyInsert(nutshell::NCUri &uri, nutshell::NCVariant &variant);
    virtual VOID onNotifyUpdate(nutshell::NCUri &uri, nutshell::NCVariant &variant);
    virtual VOID onNotifyInsert(nutshell::NCUri &uri, nutshell::ncsp<nutshell::NCCursor>::sp spCursor);
    virtual VOID onNotifyUpdate(nutshell::NCUri &uri, nutshell::ncsp<nutshell::NCCursor>::sp spCursor);
    virtual VOID onNotifyRemove(nutshell::NCUri &uri);

    virtual VOID Regist();
    virtual VOID UnRegist();

protected:
    spVR_MessageProcessIF       m_spMessageProcessIF;
    spVR_CommandFactory         m_spCommandFactory;
    spVR_DM_CommandHandle       m_spCmdHandler;
};


/**
 * @brief  the class of VR_VoiceSettingListener
 *
 * class declaration
 */
class VR_VoiceSettingListener : public VR_SettingListener
{
public:
    explicit  VR_VoiceSettingListener(const nutshell::NCString& ncListenerName,
                                      const nutshell::NCRunnableLooper &looper,
                                      spVR_MessageProcessIF spMessageProcess,
                                      spVR_CommandFactory spVrCommandFactory,
                                      spVR_DM_CommandHandle spCmdHandle);

    virtual   ~VR_VoiceSettingListener();

    virtual  VOID  onNotifyUpdate(nutshell::NCUri &uri, nutshell::NCVariant &variant);

    virtual VOID Regist();
    virtual VOID UnRegist();

private:
    void     SyncSettingItem();
};

#endif /* VR_LANGUAGELISTENER_H */
/* EOF */
