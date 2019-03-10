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
 * @file VR_NCTutorialManager.h
 * @brief Declaration file of class VR_NCTutorialManager.
 *
 * This file includes the declaration of class VR_NCTutorialManager.
 *
 * @attention used for C++ only.
 */

#ifndef VR_NC_TUTORIAL_MANAGER_H
#define VR_NC_TUTORIAL_MANAGER_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include "ncore/NCTypesDefine.h"
#include "ncore/NCString.h"

namespace nutshell
{
class NCContentResolver;
namespace tutorial
{

#define VR_NCTUTORIAL_TTS_PLAY_SENDER_NAME          "VoiceRecogTutorial"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_NCTutorialXMLBuilder);
typedef enum tagTutorialErrorStatus
{
    TUTORIAL_ERROR_OF_OK = 0,
    TUTORIAL_ERROR_OF_STATE_ISNOT_EXIST,
    TUTORIAL_ERROR_OF_LANGUAGE_ISNOT_EXIST,
    TUTORIAL_ERROR_OF_MACHINE_TYPE_ISNOT_EXIST,
    TUTORIAL_ERROR_OF_RESPATH_ISNOT_EXIST,
    TUTORIAL_ERROR_OF_RESPATH_HASNOT_RESOURCE,
    TUTORIAL_ERROR_OF_RESFILE_FORMAT_ERROR,
    TUTORIAL_ERROR_OF_IMGFILE_ISNOT_EXIST,
    TUTORIAL_ERROR_OF_TUTORIAL_PROVIDER_ACCESS_FAILED
    
}TutorialErrorStatus;

typedef enum tagTutorialInfoCheckStatus
{
    TUTORIAL_INFO_STATUS_EXIST = 0,
    TUTORIAL_INFO_STATUS_PART_EXIST,
    TUTORIAL_INFO_STATUS_NOT_EXIST,
    TUTORIAL_INFO_STATUS_ERROR
    
}TutorialInfoCheckStatus;

typedef enum tagTutorialInfoReplyMsgType
{
    REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST = 0,
    REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO,
    REPLY_MSG_TYPE_OF_PLAY_MEDIA,
    REPLY_MSG_TYPE_OF_STOP_MEDIA

}TutorialInfoReplyMsgType;

typedef enum tagTutorialPlayStatus
{
    TUTORIAL_PLAY_STATUS_UNKNOW = 0,
    TUTORIAL_PLAY_STATUS_START,
    TUTORIAL_PLAY_STATUS_STOP

}TutorialPlayStatus;

/**
 * @brief The VR_NCTutorialManager class
 *
 * class declaration.
 */
class VR_NCTutorialManager
{
public:
    VR_NCTutorialManager();
    ~VR_NCTutorialManager();
    bool Initialize();

    void StartTutorialsMode(VR_ProtoMessageBase* requestMsg, VR_ProtoMessageBase* replyMsg);
    void CancelTutorialsMode(VR_ProtoMessageBase* requestMsg, VR_ProtoMessageBase* replyMsg);

    void LoadTutorialItemsList(VR_ProtoMessageBase* replyMsg);
    void LoadTutorialSingleItemInfo(VR_ProtoMessageBase* requestMsg, VR_ProtoMessageBase* replyMsg);
    void StartPlayMedia(VR_ProtoMessageBase* requestMsg, VR_ProtoMessageBase* replyMsg);
    void StopPlayMedia(VR_ProtoMessageBase* requestMsg, VR_ProtoMessageBase* replyMsg);
    void NotifyPlayMediaEnd(VR_ProtoMessageBase* notifyMsg);

protected:
    // load environment params
    bool LoadConfigRootPath();
    bool LoadCurrentState(NCString& curState);
    bool LoadCurrentLanguage(NCString& curLanguage);
    bool LoadMachineType(NCString& curMachineType);
    
    // save current state\language\machineType load flag. [YES/NO]
    bool InsertTutorialCategoryData(const NCString& curState, const NCString& curLanguage,
                                    const NCString& curMachineType, INT& categoryId);

    // clear provider data info
    bool ClearTutorialCategoryData();
    bool ClearTutorialInfoData();
    bool ClearTutorialScreenInfoData(INT tutorialId = 0);

    // fuzzy matching path
    bool BuildResourceFilePath(const NCString& curState, const NCString& curLanguage,
                               const NCString& curMachineType);

    // check data info is load
    INT CheckTutorialItemsListIsLoad(const NCString& curState, const NCString& curLanguage,
                                     const NCString& curMachineType, INT& categoryId);
    INT CheckTutorialSingleItemInfoIsLoad(const INT& tutorialId, INT& locationIndex);
    

    // parse xml and insert provider
    bool ParseItemsListXML(VR_ProtoMessageBase* replyMsg, const INT& categoryId);
    bool ParseSingleItemInfoXML(VR_ProtoMessageBase* replyMsg, const INT& tutorialId, const INT& locationIndex);

    // get data from provider
    bool GetItemsListInfo(VR_ProtoMessageBase* replyMsg, const INT& categoryId);
    bool GetSingleItemInfo(VR_ProtoMessageBase* replyMsg, const INT& tutorialId);

    // message reply
    void ReplyDataInfo(TutorialInfoReplyMsgType type, VR_ProtoMessageBase* replyMsg, const std::string& xmlData);
    void ReplyErrorDataInfo(TutorialInfoReplyMsgType type, VR_ProtoMessageBase* replyMsg, TutorialErrorStatus errorCode);

private:
    VR_NCTutorialManager(const VR_NCTutorialManager&);
    VR_NCTutorialManager& operator=(const VR_NCTutorialManager&);
    
    NCString             m_configRootPath;
    NCString             m_curResourcePath;
    NCString             m_curImageResPath;
    NCString             m_curAudioResPath;

    spVR_NCTutorialXMLBuilder m_xmlBuilder;
    NCContentResolver*        m_resolver;
    INT                       m_sequenceId;
    TutorialPlayStatus        m_curPlayStatus;

    typedef VoiceMap<INT, NCString>::type TutorialErrorRelateMap;
    static TutorialErrorRelateMap m_ErrorRelate;
    static const NCString m_categoryUri;
    static const NCString m_infoUri;
    static const NCString m_screenInfoUri;
};

}
}
#endif /* VR_NC_TUTORIAL_MANAGER_H */
/* EOF */
