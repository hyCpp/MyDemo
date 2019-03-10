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
#include <boost/assign.hpp>
#include "VR_Log.h"
#include "VR_NCTutorialManager.h"
#include "VR_NCTutorialXMLBuilder.h"
#include "VR_ConfigureIF.h"
#include "contentresolver/NCContentResolver.h"
#include "avmanager/NCSubAudioDef.h"
#include "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCommonPropertyResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"
#include "navi/Voice/TTS/TtsPlayFile.pb.h"
#include "navi/Voice/TTS/TtsStop.pb.h"
#include "navi/Voice/TTS/TtsRespPlayState.pb.h"
#include "system/NCEnvironment.h"
#include "ncore/NCDir.h"
#include "ncore/NCFile.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

using namespace navi::VoiceRecog;
using namespace navi::TTS;
namespace nutshell
{
namespace tutorial
{

#define VR_NCTUTORIAL_INFO_RESOURCE_FILE_COUNT      10

VR_NCTutorialManager::TutorialErrorRelateMap VR_NCTutorialManager::m_ErrorRelate = boost::assign::map_list_of
        ((INT)TUTORIAL_ERROR_OF_OK,                       "operator is successful")
        ((INT)TUTORIAL_ERROR_OF_STATE_ISNOT_EXIST,        "query current product country failed")
        ((INT)TUTORIAL_ERROR_OF_LANGUAGE_ISNOT_EXIST,     "query current system language failed")
        ((INT)TUTORIAL_ERROR_OF_MACHINE_TYPE_ISNOT_EXIST, "query current product name failed")
        ((INT)TUTORIAL_ERROR_OF_RESPATH_ISNOT_EXIST,      "resource path is not exist")
        ((INT)TUTORIAL_ERROR_OF_RESPATH_HASNOT_RESOURCE,  "resource path has not resource")
        ((INT)TUTORIAL_ERROR_OF_RESFILE_FORMAT_ERROR,     "resource file content format error")
        ((INT)TUTORIAL_ERROR_OF_IMGFILE_ISNOT_EXIST,      "image resource file is not exist")
        ((INT)TUTORIAL_ERROR_OF_TUTORIAL_PROVIDER_ACCESS_FAILED,  "tutorialinfo provider access failed");


const NCString VR_NCTutorialManager::m_categoryUri   = "content://tutorialinfo/tutorial_category/data";
const NCString VR_NCTutorialManager::m_infoUri       = "content://tutorialinfo/tutorial_info/data";
const NCString VR_NCTutorialManager::m_screenInfoUri = "content://tutorialinfo/tutorial_screen_info/data";

VR_NCTutorialManager::VR_NCTutorialManager()
    : m_xmlBuilder(VR_new VR_NCTutorialXMLBuilder)
    , m_resolver(VR_new NCContentResolver)
    , m_sequenceId(0)
    , m_curPlayStatus(TUTORIAL_PLAY_STATUS_STOP)
{
    // 1.initialize info
    bool result = Initialize();
    if (!result) {
        VR_LOGD("tutorial manager initialize failed");
    }

}

VR_NCTutorialManager::~VR_NCTutorialManager()
{
    if (NULL != m_resolver) {
        delete m_resolver;
    }
}

bool VR_NCTutorialManager::Initialize()
{
    // 1.load config root path
    bool result = LoadConfigRootPath();
    if (!result) {
        VR_LOGD("load config root path failed");
        return false;
    }

    return true;
}

void VR_NCTutorialManager::StartTutorialsMode(VR_ProtoMessageBase* requestMsg, VR_ProtoMessageBase* replyMsg)
{
    VR_LOGD_FUNC();

    VrReqStartTutorialsMode* startModeRequestMsg = dynamic_cast<VrReqStartTutorialsMode*>(requestMsg);
    VrReqStartTutorialsModeResult* startModeReplyMsg = dynamic_cast<VrReqStartTutorialsModeResult*>(replyMsg);
    if ((NULL == startModeRequestMsg) || (NULL == startModeReplyMsg)) {
        VR_LOGD("VrReqStartTutorialsMode or VrReqStartTutorialsModeResult object is null");
        return;
    }

    // 1.get start mode sender
    NCString sender = startModeRequestMsg->sender().c_str();
    VR_LOGD("start tutorial mode sender:%s", sender.getString());

    // 2.reply message
    startModeReplyMsg->set_starttype(0);
}

void VR_NCTutorialManager::CancelTutorialsMode(VR_ProtoMessageBase* requestMsg, VR_ProtoMessageBase* replyMsg)
{
    VR_LOGD_FUNC();

    VrReqCancelTutorialsMode* cancelTutorialModeRequestMsg = dynamic_cast<VrReqCancelTutorialsMode*>(requestMsg);
    VrReqCancelTutorialsModeResult* cancelTutorialModeReplyMsg = dynamic_cast<VrReqCancelTutorialsModeResult*>(replyMsg);
    if ((NULL == cancelTutorialModeRequestMsg) || (NULL == cancelTutorialModeReplyMsg)) {
        VR_LOGD("VrReqCancelTutorialsMode or VrReqCancelTutorialsModeResult object is null");
        return;
    }
    // 1.get cancel mode sender
    NCString sender = cancelTutorialModeRequestMsg->sender().c_str();
    VR_LOGD("cancel tutorial mode sender:%s", sender.getString());

    // 2.check media play is stop
    if (TUTORIAL_PLAY_STATUS_STOP == m_curPlayStatus) {
        VR_LOGD("current tutorial media already stop");
        // notify session manager close
        std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        loopMessage->mutable_m_controlmessage()->set_type(DMTaskMessageType_CloseSession);
        loopMessage->mutable_m_controlmessage()->mutable_sessiontype()->set_sessiontype(Session_Tutorial);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());

        cancelTutorialModeReplyMsg->set_canceltype(0);
        return;
    }

    // 3.check is already force stop
    if ((TUTORIAL_PLAY_STATUS_UNKNOW == m_curPlayStatus)
        && (0 == m_sequenceId)) {
        VR_LOGD("current tutorial media already force stop");
        cancelTutorialModeReplyMsg->set_canceltype(0);
        return;
    }

    // 4.if not stop or will stop, force stop it
    std::unique_ptr<TtsStop> ttsStopMsg(TtsStop::default_instance().New());
    ttsStopMsg->mutable_base()->set_sender(VR_NCTUTORIAL_TTS_PLAY_SENDER_NAME);
    ttsStopMsg->mutable_base()->set_onresponse(true);
    ttsStopMsg->set_reqid(m_sequenceId);

    VR_LOGD("msgName=[%s] result=[true]", EV_EVENT_TTS_STOP);
    EV_EventSender().SendEvent(EV_EVENT_TTS_STOP, ttsStopMsg.release());

    // 5.update current media play status
    m_curPlayStatus = TUTORIAL_PLAY_STATUS_UNKNOW;
    m_sequenceId = 0;

    // 6.reply message
    cancelTutorialModeReplyMsg->set_canceltype(0);
}

void VR_NCTutorialManager::LoadTutorialItemsList(VR_ProtoMessageBase* replyMsg)
{
    VR_LOGD_FUNC();

    VrReqMediaListDataResult* listDataReplyMsg = dynamic_cast<VrReqMediaListDataResult*>(replyMsg);
    if (NULL == listDataReplyMsg) {
        VR_LOGD("VrReqMediaListDataResult object is null");
        return;
    }

    // 1.load current state\language\machineType
    NCString curState, curLanguage, curMachineType;
    bool result = LoadCurrentState(curState);
    if (!result) {
        ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST, listDataReplyMsg, TUTORIAL_ERROR_OF_STATE_ISNOT_EXIST);
        return;
    }
    
    result = LoadCurrentLanguage(curLanguage);
    if (!result) {
        ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST, listDataReplyMsg, TUTORIAL_ERROR_OF_LANGUAGE_ISNOT_EXIST);
        return;
    }
    
    result = LoadMachineType(curMachineType);
    if (!result) {
        ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST, listDataReplyMsg, TUTORIAL_ERROR_OF_MACHINE_TYPE_ISNOT_EXIST);
        return;
    }

    // 2.builed resource file path
    result = BuildResourceFilePath(curState, curLanguage, curMachineType);
    if (!result) {
        ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST, listDataReplyMsg, TUTORIAL_ERROR_OF_RESPATH_ISNOT_EXIST);
        return;
    }

    // 3.check tutorial itemslist is load
    INT categoryId = 0;
    INT checkStatus = CheckTutorialItemsListIsLoad(curState, curLanguage, curMachineType, categoryId);
    if (TUTORIAL_INFO_STATUS_ERROR == checkStatus) {
        VR_LOGD("tutorial provider access failed");
        ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST, listDataReplyMsg, TUTORIAL_ERROR_OF_TUTORIAL_PROVIDER_ACCESS_FAILED);
        return;
    }
    
    if (TUTORIAL_INFO_STATUS_NOT_EXIST == checkStatus) {
        VR_LOGD("tutorial items list info load from xml resource file");
        // clear tutorial provider data
        (VOID)ClearTutorialCategoryData();
        (VOID)ClearTutorialInfoData();
        (VOID)ClearTutorialScreenInfoData();

        // parse xml to provider or other format
        result = InsertTutorialCategoryData(curState, curLanguage, curMachineType, categoryId);
        if (!result) {
            VR_LOGD("tutorial provider access failed");
            ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST, listDataReplyMsg, TUTORIAL_ERROR_OF_TUTORIAL_PROVIDER_ACCESS_FAILED);
            return;
        }
        
        result = ParseItemsListXML(listDataReplyMsg, categoryId);
        if (!result) {
            VR_LOGD("parse items list xml resource file failed");
            (VOID)ClearTutorialCategoryData();
            (VOID)ClearTutorialInfoData();
        }
        return;
    }

    // 4.query tutorial itemslist info
    VR_LOGD("tutorial items list info load from tutorial provider");
    (VOID)GetItemsListInfo(listDataReplyMsg, categoryId);
}

void VR_NCTutorialManager::LoadTutorialSingleItemInfo(VR_ProtoMessageBase* requestMsg, VR_ProtoMessageBase* replyMsg)
{
    VR_LOGD_FUNC();

    VrReqMediaPlayInfo* singleItemInfoRequestMsg = dynamic_cast<VrReqMediaPlayInfo*>(requestMsg);
    VrReqMediaPlayInfoResult* singleItemInfoReplyMsg = dynamic_cast<VrReqMediaPlayInfoResult*>(replyMsg);
    if ((NULL == singleItemInfoRequestMsg) || (NULL == singleItemInfoReplyMsg)) {
        VR_LOGD("VrReqMediaPlayInfo object or VrReqMediaPlayInfoResult object is null");
        return;
    }

    // 1.get tutorialId from pMsg
    INT tutorialId = singleItemInfoRequestMsg->index();

    // 2.check tutorial single item info is load
    INT locationIndex = 0;
    INT checkStatus = CheckTutorialSingleItemInfoIsLoad(tutorialId, locationIndex);
    if (TUTORIAL_INFO_STATUS_ERROR == checkStatus) {
        VR_LOGD("tutorial provider access failed");
        ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO, singleItemInfoReplyMsg, TUTORIAL_ERROR_OF_TUTORIAL_PROVIDER_ACCESS_FAILED);
        return;
    }

    if (TUTORIAL_INFO_STATUS_NOT_EXIST == checkStatus) {
        ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO, singleItemInfoReplyMsg, TUTORIAL_ERROR_OF_RESPATH_HASNOT_RESOURCE);
        return;
    }

    // 3.parse xml to provider or other format
    if (TUTORIAL_INFO_STATUS_PART_EXIST == checkStatus) {
        bool result = ParseSingleItemInfoXML(singleItemInfoReplyMsg, tutorialId, locationIndex);
        if (!result) {
            VR_LOGD("parse single item xml resource file failed");
            (VOID)ClearTutorialScreenInfoData(tutorialId);
        }
        return;
    }

    // 4.query tutorial single item info
    (VOID)GetSingleItemInfo(singleItemInfoReplyMsg, tutorialId);
}

void VR_NCTutorialManager::StartPlayMedia(VR_ProtoMessageBase* requestMsg, VR_ProtoMessageBase* replyMsg)
{
    VR_LOGD_FUNC();

    VrReqPlayMedia* playMediaRequestMsg = dynamic_cast<VrReqPlayMedia*>(requestMsg);
    VrReqPlayMediaResult* playMediaReplyMsg = dynamic_cast<VrReqPlayMediaResult*>(replyMsg);
    if ((NULL == playMediaRequestMsg) || (NULL == playMediaReplyMsg)) {
        VR_LOGD("VrReqPlayMedia object or VrReqPlayMediaResult object is null");
        return;
    }

    // 1.get audio file name
    NCString audioFileName = playMediaRequestMsg->mediapath().c_str();

    // 2.get audio file path
    NCString audioFilePath;
    NCString fileNameSuffix = audioFileName.right(3);
    if ("wav" == fileNameSuffix) {
        audioFilePath = m_curAudioResPath + "/" + audioFileName;
    }
    else {
        VR_LOGD("%s file name format only support wav", audioFileName.getString());
        return;
    }

    bool result = (NC_TRUE == NCFile::exists(audioFilePath));
    if (!result) {
        VR_LOGD("%s resouce file is not exist", audioFilePath.getString());
        return;
    }

    // 3.check current media play status
    if (TUTORIAL_PLAY_STATUS_STOP != m_curPlayStatus) {
        VR_LOGD("current tutorial media already playing or will play");
        playMediaReplyMsg->set_playflag(false);
        playMediaReplyMsg->set_taskid(m_sequenceId);
        return;
    }
    
    // 4.request play media
    m_sequenceId++;
    std::unique_ptr<TtsPlayFile> ttsPlayFileMsg(TtsPlayFile::default_instance().New());
    ttsPlayFileMsg->set_filepath(audioFilePath.getString());

    TtsBasePlay* ttsBasePlayMsg = ttsPlayFileMsg->mutable_play();
    ttsBasePlayMsg->mutable_base()->set_sender(VR_NCTUTORIAL_TTS_PLAY_SENDER_NAME);
    ttsBasePlayMsg->mutable_base()->set_onresponse(true);
    ttsBasePlayMsg->set_priority(TtsBasePlay_OP_PRIORITY_OP_PRI_VR);
    ttsBasePlayMsg->set_reqid(m_sequenceId);
    ttsBasePlayMsg->set_tokenid(NCSubAudioHmi_VoiceRecognitionDisplay);
    ttsBasePlayMsg->set_streamtype(TtsBasePlay_TTS_STREAM_TYPE_TTS_STREAM_NORMAL);

    VR_LOGD("msgName=[%s] result=[true]", EV_EVENT_TTS_PLAYFILE);
    EV_EventSender().SendEvent(EV_EVENT_TTS_PLAYFILE, ttsPlayFileMsg.release());

    // 5.update current media play status. set unkown status
    m_curPlayStatus = TUTORIAL_PLAY_STATUS_UNKNOW;

    // 6.reply message
    playMediaReplyMsg->set_playflag(true);
    playMediaReplyMsg->set_taskid(m_sequenceId);
}

void VR_NCTutorialManager::StopPlayMedia(VR_ProtoMessageBase* requestMsg, VR_ProtoMessageBase* replyMsg)
{
    VR_LOGD_FUNC();

    VrReqStopMedia* stopMediaRequestMsg = dynamic_cast<VrReqStopMedia*>(requestMsg);
    VrReqStopMediaResult* stopMediaReplyMsg = dynamic_cast<VrReqStopMediaResult*>(replyMsg);
    if ((NULL == stopMediaRequestMsg) || (NULL == stopMediaReplyMsg)) {
        VR_LOGD("VrReqStopMedia object or VrReqStopMediaResult object is null");
        return;
    }

    // 1.get taskid
    INT taskId = stopMediaRequestMsg->taskid();

    // 2.check taskId is Invalid
    if (m_sequenceId != taskId) {
        VR_LOGD("current will stop taksId not same to playing taskId");
        VR_LOGD("playing taskId:%d", m_sequenceId);
        VR_LOGD("will stop taskId:%d", taskId);
        stopMediaReplyMsg->set_stopflag(false);
        stopMediaReplyMsg->set_taskid(taskId);
        return;
    }

    // 3.check current media play status
    if (TUTORIAL_PLAY_STATUS_START != m_curPlayStatus) {
        VR_LOGD("current tutorial media already stopped or will stop");
        stopMediaReplyMsg->set_stopflag(false);
        stopMediaReplyMsg->set_taskid(taskId);
        return;
    }

    // 4.request stop media
    std::unique_ptr<TtsStop> ttsStopMsg(TtsStop::default_instance().New());
    ttsStopMsg->mutable_base()->set_sender(VR_NCTUTORIAL_TTS_PLAY_SENDER_NAME);
    ttsStopMsg->mutable_base()->set_onresponse(true);
    ttsStopMsg->set_reqid(taskId);

    VR_LOGD("msgName=[%s] result=[true]", EV_EVENT_TTS_STOP);
    EV_EventSender().SendEvent(EV_EVENT_TTS_STOP, ttsStopMsg.release());

    // 5.update current media play status. set unkown status
    m_curPlayStatus = TUTORIAL_PLAY_STATUS_UNKNOW;

    // 6.reply message
    stopMediaReplyMsg->set_stopflag(true);
    stopMediaReplyMsg->set_taskid(taskId);
}

void VR_NCTutorialManager::NotifyPlayMediaEnd(VR_ProtoMessageBase* notifyMsg)
{
    VR_LOGD_FUNC();

    TtsRespPlayState* ttsNotifyMsg = dynamic_cast<TtsRespPlayState*>(notifyMsg);
    if (NULL == ttsNotifyMsg) {
        VR_LOGD("TtsRespPlayState object is null");
        return;
    }

    INT requestId = ttsNotifyMsg->reqid();
    TtsRespPlayState_TTS_PLAY_STATE playStatus = ttsNotifyMsg->state();
    VR_LOGD("requestId=[%d] sequenceId=[%d] playstate=[%d]", requestId, m_sequenceId, playStatus);
    switch (playStatus) {
    case TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_START: {
        m_curPlayStatus = TUTORIAL_PLAY_STATUS_START;
        return;
    }
    case TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_STOPED:
    case TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_ABORT:
    case TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_ERROR_ID:
    case TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_ERROR_LANG:
    case TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_ERROR: {
        m_curPlayStatus = TUTORIAL_PLAY_STATUS_STOP;
        break;
    }
    default:
        return;
    }

    // 1.check is force stop
    if (0 == m_sequenceId) {
        // notify session manager close
        std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        loopMessage->mutable_m_controlmessage()->set_type(DMTaskMessageType_CloseSession);
        loopMessage->mutable_m_controlmessage()->mutable_sessiontype()->set_sessiontype(Session_Tutorial);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
        return;
    }

    // 2.notfy message
    std::unique_ptr<VrNotifyCommonProperty> notifyCommonProperty(VrNotifyCommonProperty::default_instance().New());
    notifyCommonProperty->set_sender(EV_MODULE_VOICERECOG);
    notifyCommonProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_MeidaPlayResult);

    NCString playEndDesc;
    playEndDesc.format("media play end.Sender:%s TaskId:%d PlayStatus:%d",
                       VR_NCTUTORIAL_TTS_PLAY_SENDER_NAME, requestId, playStatus);
    VrMeidaPlayResult* mediaPlayEnd = notifyCommonProperty->mutable_mediaplayresult();
    mediaPlayEnd->set_taskid(requestId);
    mediaPlayEnd->set_playend(playEndDesc.getString());

    VR_LOGD("msgName=[%s] desc=[%s]", EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, playEndDesc.getString());
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, notifyCommonProperty.release());
}

bool VR_NCTutorialManager::LoadConfigRootPath()
{
    // 1.get config root path
    VR_ConfigureIF* config = VR_ConfigureIF::Instance();
    std::string configUserPath = config->getTutorialPath();
    m_configRootPath.format("%s", configUserPath.c_str());

    // 2.build tutorial path
    NCDir rootPath;
    bool result = (NC_TRUE == rootPath.exists(m_configRootPath));
    if (!result) {
        VR_LOGD("VR_Configure user path is not exist:%s", m_configRootPath.getString());
        m_configRootPath = "/var/user";
        return false;
    }

    VR_LOGD("tutorial root path:%s", m_configRootPath.getString());
    return true;
}

bool VR_NCTutorialManager::LoadCurrentState(NCString& curState)
{
    NCString strCountry;
    NCEnvironment env;
    env.getProductCountry(strCountry);
    if (0 >= strCountry.getLength()) {
        VR_LOGD("current state value is null");
        return false;
    }

    if ("America" != strCountry) {
        VR_LOGD("current state value is not America");
        return false;
    }

    env.getSubCountry(curState);
    if (0 >= curState.getLength()) {
        VR_LOGD("current state value is null");
        curState = "USA"; // when get nothing then setting usa for default
    }

    VR_LOGD("current state:%s", curState.getString());
    return true;
}

bool VR_NCTutorialManager::LoadCurrentLanguage(NCString& curLanguage)
{
    NCCursor* cursor = m_resolver->query(NCUri(NCString("content://settings/system?vr_language")),
                                         NCList<NCString>(), NCString(), NCList<NCString>(),
                                         NCString());

    if (NULL == cursor) {
        VR_LOGD("content://system/info/sys/language query failed");
        return false;
    }

    INT count = cursor->getCount();
    if (0 == count) {
        VR_LOGD("current language info is not exist");
        delete cursor;
        return false;
    }

    bool result = (NC_TRUE == cursor->moveToFirst());
    if (!result) {
        VR_LOGD("current language info is not exist");
        delete cursor;
        return false;
    }

    curLanguage = cursor->getString(0);
    curLanguage.upperCase();
    delete cursor;

    if (0 >= curLanguage.getLength()) {
        VR_LOGD("current language info value is null");
        return false;
    }

    VR_LOGD("current language:%s", curLanguage.getString());
    return true;
    
}

bool VR_NCTutorialManager::LoadMachineType(NCString& curMachineType)
{
    NCEnvironment env;
    env.getProductName(curMachineType);
    if (0 >= curMachineType.getLength()) {
        VR_LOGD("current machine type value is null");
        return false;
    }

    VR_LOGD("current machine type:%s", curMachineType.getString());
    return true;
}

bool VR_NCTutorialManager::InsertTutorialCategoryData(const NCString& curState, const NCString& curLanguage,
                                                      const NCString& curMachineType, INT& categoryId)
{
    NCContentValues values;
    values.put("category_index", "1");
    values.put("state", curState);
    values.put("language", curLanguage);
    values.put("machine_type", curMachineType);
    INT64 rowId = m_resolver->insert(NCUri(m_categoryUri), values);
    if (-1 == rowId) {
        VR_LOGD("insert tutorial category data failed");
        return false;
    }

    categoryId = 1;
    return true;
}

bool VR_NCTutorialManager::ClearTutorialCategoryData()
{
    INT32 numChanges = m_resolver->remove(NCUri(m_categoryUri), NCString(), NCList<NCString>());
    if (0 == numChanges) {
        VR_LOGD("tutorial category not data is cleared");
    }

    return true;
}

bool VR_NCTutorialManager::ClearTutorialInfoData()
{
    INT32 numChanges = m_resolver->remove(NCUri(m_infoUri), NCString(), NCList<NCString>());
    if (0 == numChanges) {
        VR_LOGD("tutorial info not data is cleared");
    }

    return true;
}

bool VR_NCTutorialManager::ClearTutorialScreenInfoData(INT tutorialId)
{
    NCString where;
    NCString ArgOne;
    NCList<NCString> whereArgs;
    if (0 == tutorialId) {
        where = NCString();
    }
    else {
        ArgOne.format("%d", tutorialId);
        whereArgs << &ArgOne;
        where = NCString(" tutorial_index = ? ");
    }
    
    INT32 numChanges = m_resolver->remove(NCUri(m_screenInfoUri), where, whereArgs);
    if (0 == numChanges) {
        VR_LOGD("tutorial screen info not data is cleared");
    }

    return true;
}

bool VR_NCTutorialManager::BuildResourceFilePath(const NCString& curState, const NCString& curLanguage,
                                                 const NCString& curMachineType)
{
    VR_LOGD_FUNC();
    bool IsFind = false;
    m_curResourcePath = m_configRootPath;

    // 1.check state sub directory is Exist
    NCString state;
    if ("USA" ==  curState) {
        state = "US Destination";
    }
    else {
        state = "Canada-Mexico";
    }
    m_curResourcePath = m_curResourcePath + "/" + state;
    VR_LOGD("%s resource path is find", m_curResourcePath.getString());

    // 2.check language sub directory is Exist
    NCDir languageSubDir(m_curResourcePath);
    NCDirIterator* dirIterator = languageSubDir.EntryIterator(NC_FFT_Dirs | NC_FFT_NoDotAndDotDot);
    if (NULL == dirIterator) {
        VR_LOGD("dirIterator point is NULL");
        return false;
    }
    while (!(dirIterator->End())) {
        NCString language = dirIterator->CurrentFileName();
        VR_LOGD("%s find language folder %s", m_curResourcePath.getString(), language.getString());

        INT Result = language.find(curLanguage.mid(0, 2));
        if (-1 != Result) {
            m_curResourcePath = m_curResourcePath + "/" + language;
            IsFind = true;
            break;
        }

        ++(*dirIterator);
    }
    delete dirIterator;
    dirIterator = NULL;

    if (!IsFind) {
        VR_LOGD("%s path has not resource", m_curResourcePath.getString());
        return false;
    }
    VR_LOGD("%s resource path is find", m_curResourcePath.getString());

    m_curImageResPath = m_configRootPath + "/Images";
    m_curImageResPath = m_curImageResPath + "/" + curLanguage.mid(0, 2);
    m_curAudioResPath = m_configRootPath + "/Audio";
    m_curAudioResPath = m_curAudioResPath + "/" + curLanguage.mid(0, 2);

    // 3.check machine type sub directory is Exist
    IsFind = false;
    NCDir machineTypeSubDir(m_curResourcePath);
    NCString machineType;
    dirIterator = machineTypeSubDir.EntryIterator(NC_FFT_Dirs | NC_FFT_NoDotAndDotDot);
    if (NULL == dirIterator) {
        VR_LOGD("dirIterator point is NULL");
        return false;
    }
    while (!(dirIterator->End())) {
        machineType = dirIterator->CurrentFileName();
        VR_LOGD("%s find machine type folder %s", m_curResourcePath.getString(), machineType.getString());

        INT Result = machineType.find(curMachineType);
        if (-1 != Result) {
            m_curResourcePath = m_curResourcePath + "/" + machineType;
            IsFind = true;
            break;
        }

        ++(*dirIterator);
    }
    delete dirIterator;
    if (!IsFind) {
        VR_LOGD("%s path has not resource", m_curResourcePath.getString());
        return false;
    }
    VR_LOGD("%s resource path is find", m_curResourcePath.getString());

    // build other path
    m_curImageResPath = m_curImageResPath + "/" + machineType;
    m_curAudioResPath = m_curAudioResPath + "/" + machineType;
    IsFind = NCFile::exists(m_curImageResPath);
    if (!IsFind) {
        VR_LOGD("%s path is not exist", m_curImageResPath.getString());
        return false;
    }

    IsFind = NCFile::exists(m_curAudioResPath);
    if (!IsFind) {
        VR_LOGD("%s path is not exist", m_curAudioResPath.getString());
        return false;
    }

    VR_LOGD("%s image resource path is find", m_curImageResPath.getString());
    VR_LOGD("%s audio resource path is find", m_curAudioResPath.getString());
    return true;
}

INT VR_NCTutorialManager::CheckTutorialItemsListIsLoad(const NCString& curState, const NCString& curLanguage,
                                                       const NCString& curMachineType, INT& categoryId)
{
    VR_LOGD_FUNC();
    NCCursor* cursor = m_resolver->query(NCUri(m_categoryUri), NCList<NCString>(), NCString(),
                                         NCList<NCString>(), NCString(" category_index "));
    if (NULL == cursor) {
        VR_LOGD("%s query failed", m_categoryUri.getString());
        return TUTORIAL_INFO_STATUS_ERROR;
    }

    INT count = cursor->getCount();
    if (0 == count) {
        delete cursor;
        return TUTORIAL_INFO_STATUS_NOT_EXIST;
    }

    bool result = (NC_TRUE == cursor->moveToFirst());
    if (!result) {
        VR_LOGD("%s query failed", m_categoryUri.getString());
        delete cursor;
        return TUTORIAL_INFO_STATUS_ERROR;
    }

    categoryId = cursor->getInt64(0);
    NCString state = cursor->getString(1);
    NCString language = cursor->getString(2);
    NCString machineType = cursor->getString(3);
    delete cursor;

    if ((state != curState) || (language != curLanguage) || (machineType != curMachineType)) {
        return TUTORIAL_INFO_STATUS_NOT_EXIST;
    }

    return TUTORIAL_INFO_STATUS_EXIST;
}

INT VR_NCTutorialManager::CheckTutorialSingleItemInfoIsLoad(const INT& tutorialId, INT& locationIndex)
{
    VR_LOGD_FUNC();
    NCObjList<NCString> columns; 
    columns << VR_new NCString("location_index");

    NCString ArgOne;
    NCList<NCString> whereArgs;
    ArgOne.format("%d", tutorialId);
    whereArgs << &ArgOne;
    
    NCCursor* cursor = m_resolver->query(NCUri(m_infoUri), columns, NCString(" tutorial_index = ? "),
                                         whereArgs, NCString());
    if (NULL == cursor) {
        VR_LOGD("%s query failed", m_infoUri.getString());
        return TUTORIAL_INFO_STATUS_ERROR;
    }

    INT count = cursor->getCount();
    if (0 == count) {
        delete cursor;
        return TUTORIAL_INFO_STATUS_NOT_EXIST;
    }

    bool result = (NC_TRUE == cursor->moveToFirst());
    if (!result) {
        VR_LOGD("%s query failed", m_infoUri.getString());
        delete cursor;
        return TUTORIAL_INFO_STATUS_ERROR;
    }

    locationIndex = cursor->getInt64(0);
    delete cursor;
    VR_LOGD("uri:%s tutorial_index = %d locationIndex:%d has records.%d",
            m_infoUri.getString(), tutorialId, locationIndex, count);

    columns.clearData();
    columns << VR_new NCString("tutorial_index");
    cursor = m_resolver->query(NCUri(m_screenInfoUri), columns, NCString(" tutorial_index = ? "),
                               whereArgs, NCString());
    if (NULL == cursor) {
        VR_LOGD("%s query failed", m_screenInfoUri.getString());
        return TUTORIAL_INFO_STATUS_ERROR;
    }

    count = cursor->getCount();
    if (0 == count) {
        delete cursor;
        return TUTORIAL_INFO_STATUS_PART_EXIST;
    }
    
    delete cursor;
    return TUTORIAL_INFO_STATUS_EXIST;
}

bool VR_NCTutorialManager::ParseItemsListXML(VR_ProtoMessageBase* replyMsg, const INT& categoryId)
{
    VR_LOGD_FUNC();

    VrReqMediaListDataResult* listDataReplyMsg = dynamic_cast<VrReqMediaListDataResult*>(replyMsg);
    if (NULL == listDataReplyMsg) {
        VR_LOGD("VrReqMediaListDataResult object is null");
        return false;
    }

    // 1.begin parse xml resource file
    INT curRecordNum = 0;
    m_xmlBuilder->AddTutorialNode("listdata");
    for (INT i = 1; i <= VR_NCTUTORIAL_INFO_RESOURCE_FILE_COUNT; i++) {
        NCString srcFilePath;
        srcFilePath.format("%s/Tutorial%02d-XML File.xml", m_curResourcePath.getString(), i);

        bool result = (NC_TRUE == NCFile::exists(srcFilePath));
        if (!result) {
            continue;
        }

        VR_LOGD("%s xml resource file load begin.", srcFilePath.getString());
        pugi::xml_document m_xmlDocument;
        m_xmlDocument.load_file(srcFilePath.getString());

        pugi::xml_node tutorialNode = m_xmlDocument.child("Tutorial");
        if (pugi::xml_node() == tutorialNode) {
            VR_LOGD("%s xml resource file format error", srcFilePath.getString());
            ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST, listDataReplyMsg, TUTORIAL_ERROR_OF_RESFILE_FORMAT_ERROR);
            return false;
        }

        // 1.1 get current xml value
        NCString name = tutorialNode.child_value("Name");
        NCString version = tutorialNode.child_value("Version");
        INT screenNum = tutorialNode.child("Screens").attribute("count").as_int();

        // 1.2 insert item abstract info
        NCContentValues values;
        values.put("tutorial_index", curRecordNum + 1);
        values.put("name", name);
        values.put("version", version);
        values.put("screen_number", screenNum);
        values.put("location_index", i);
        values.put("category_index", categoryId);
        INT64 rowId = m_resolver->insert(NCUri(m_infoUri), values);
        if (-1 == rowId) {
            VR_LOGD("insert tutorial info data failed");
            ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST, listDataReplyMsg, TUTORIAL_ERROR_OF_TUTORIAL_PROVIDER_ACCESS_FAILED);
            return false;
        }

        // 1.3 iterator build xml
        m_xmlBuilder->AddItemNode(values);
        VR_LOGD("%s xml resource file load end.", srcFilePath.getString());

        curRecordNum++;
    }

    if (0 == curRecordNum) {
        ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST, listDataReplyMsg, TUTORIAL_ERROR_OF_RESPATH_HASNOT_RESOURCE);
        return false;
    }

    // 2.reply message
    std::string xmlStr;
    m_xmlBuilder->GetXML(xmlStr);
    ReplyDataInfo(REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST, listDataReplyMsg, xmlStr);
    return true;
}

bool VR_NCTutorialManager::ParseSingleItemInfoXML(VR_ProtoMessageBase* replyMsg, const INT& tutorialId, const INT& locationIndex)
{
    VR_LOGD_FUNC();

    VrReqMediaPlayInfoResult* singleItemInfoReplyMsg = dynamic_cast<VrReqMediaPlayInfoResult*>(replyMsg);
    if (NULL == singleItemInfoReplyMsg) {
        VR_LOGD("VrReqMediaListDataResult object is null");
        return false;
    }

    // 1.get single item xml resource file path    
    m_xmlBuilder->AddTutorialNode("screeninfo", tutorialId);
    NCString srcFilePath;
    srcFilePath.format("%s/Tutorial%02d-XML File.xml", m_curResourcePath.getString(), locationIndex);

    VR_LOGD("%s xml resource file load begin.", srcFilePath.getString());
    bool result = (NC_TRUE == NCFile::exists(srcFilePath));
    if (!result) {
        ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO, singleItemInfoReplyMsg, TUTORIAL_ERROR_OF_RESPATH_HASNOT_RESOURCE);
        return false;
    }

     pugi::xml_document m_xmlDocument;
     m_xmlDocument.load_file(srcFilePath.getString());

     pugi::xml_node tutorialNode = m_xmlDocument.child("Tutorial");
     if (pugi::xml_node() == tutorialNode) {
         VR_LOGD("%s xml resource file format error", srcFilePath.getString());
         ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO, singleItemInfoReplyMsg, TUTORIAL_ERROR_OF_RESFILE_FORMAT_ERROR);
         return false;
     }

     // 2 get current xml value
     pugi::xml_node screensNode = tutorialNode.child("Screens");
     pugi::xml_node screenChildNode = screensNode.first_child();
     INT screenNum = screensNode.attribute("count").as_int();
     INT curRecordNum = 0;
     for (INT i = 0; i < screenNum; i++) {
         if (screenChildNode.empty()) {
             VR_LOGD("%s xml resource file format error", srcFilePath.getString());
             ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO, singleItemInfoReplyMsg, TUTORIAL_ERROR_OF_RESFILE_FORMAT_ERROR);
             return false;
         }

         INT sequenceNum = screenChildNode.attribute("index").as_int();
         NCString imageFileName = screenChildNode.child_value("Image");
         NCString audioFileName = screenChildNode.child_value("Audio");

         // 2.1 get image binary data
         NCString curImageFilePath;
         curImageFilePath.format("%s/%s", m_curImageResPath.getString(), imageFileName.getString());
         VR_LOGD("image path:%s", curImageFilePath.getString());

         BYTE* imageData = NULL;
         INT32 imageSize = 0;
         result = NCFile::exists(curImageFilePath);
         if (!result) {
            VR_LOGD("%s resource file is not exist", curImageFilePath.getString());
            ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO, singleItemInfoReplyMsg, TUTORIAL_ERROR_OF_IMGFILE_ISNOT_EXIST);
            return false;
         }
         else {
            FILE* imageFd = NULL;
            imageFd = fopen(curImageFilePath.getString(), "rb");
            if (NULL == imageFd) {
                VR_LOGD("%s resource file open failed", curImageFilePath.getString());
                ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO, singleItemInfoReplyMsg, TUTORIAL_ERROR_OF_IMGFILE_ISNOT_EXIST);
                return false;
            }

            fseek(imageFd, 0, SEEK_END);
            imageSize = (INT32)(ftell(imageFd) + 4);
            rewind(imageFd);
            VR_LOGD("images size:%d", imageSize);

            // read binary data
            imageData = VR_new BYTE[sizeof(BYTE) * imageSize];
            if (NULL == imageData) {
                VR_LOGD("%s alloc memory failed", curImageFilePath.getString());
                fclose(imageFd);
                return false;
            }
            memset(imageData, 0, imageSize);
            fread(imageData, 1, imageSize, imageFd);
            fclose(imageFd);
         }

         // 2.2 insert item detail info
         NCContentValues values;
         values.put("image", imageData, imageSize);
         values.put("audio", audioFileName);
         values.put("sequence_number", sequenceNum);
         values.put("tutorial_index", tutorialId);
         INT64 rowId = m_resolver->insert(NCUri(m_screenInfoUri), values);
         if (-1 == rowId) {
             VR_LOGD("insert tutorial screen info data failed");
             delete[] imageData;
             ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO, singleItemInfoReplyMsg, TUTORIAL_ERROR_OF_TUTORIAL_PROVIDER_ACCESS_FAILED);
             return false;
         }

         // 2.3 build xml
         values.put("screen_index", curRecordNum + 1);
         m_xmlBuilder->AddScreenNode(values);
         delete[] imageData;

         // 2.4 iterator
         screenChildNode = screenChildNode.next_sibling();
         curRecordNum++;
     }   
     VR_LOGD("%s xml resource file load end.", srcFilePath.getString());

     // 3.reply message
     std::string xmlStr;
     m_xmlBuilder->GetXML(xmlStr);
     ReplyDataInfo(REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO, singleItemInfoReplyMsg, xmlStr);
     return true;
}

bool VR_NCTutorialManager::GetItemsListInfo(VR_ProtoMessageBase* replyMsg, const INT& categoryId)
{
    VR_LOGD_FUNC();

    VrReqMediaListDataResult* listDataReplyMsg = dynamic_cast<VrReqMediaListDataResult*>(replyMsg);
    if (NULL == listDataReplyMsg) {
        VR_LOGD("VrReqMediaListDataResult object is null");
        return false;
    }

    // 1.query items list data from provider
    NCString ArgOne;
    NCList<NCString> whereArgs;
    ArgOne.format("%d", categoryId);
    whereArgs << &ArgOne;
    m_xmlBuilder->AddTutorialNode("listdata");
    
    NCCursor* cursor = m_resolver->query(NCUri(m_infoUri), NCList<NCString>(), NCString(" category_index = ? "),
                                         whereArgs, NCString());
    if (NULL == cursor) {
        VR_LOGD("%s query failed", m_infoUri.getString());
        ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST, listDataReplyMsg, TUTORIAL_ERROR_OF_TUTORIAL_PROVIDER_ACCESS_FAILED);
        return false;
    }

    INT rowNum = cursor->getCount();
    if (0 == rowNum) {
        ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST, listDataReplyMsg, TUTORIAL_ERROR_OF_RESPATH_HASNOT_RESOURCE);
        delete cursor;
        return false;
    }

    bool result = (NC_TRUE == cursor->moveToFirst());
    if (!result) {
        VR_LOGD("%s moveToFirst failed", m_infoUri.getString());
        delete cursor;
        return false;
    }

    VR_LOGD("%s provider query begin", m_infoUri.getString());
    NCContentValues values;
    for (INT i = 0; i < rowNum; i++) {
        values.put("tutorial_index", cursor->getInt64(0));
        values.put("name", cursor->getString(1));
        values.put("version", cursor->getString(2));
        values.put("screen_number", cursor->getInt64(3));
        values.put("location_index", cursor->getInt64(4));
        values.put("category_index", cursor->getInt64(5));
        m_xmlBuilder->AddItemNode(values);
        values.clear();

        // iterator
        if (NC_TRUE == cursor->isLast()) {
            break;
        }
        cursor->moveToNext();
    }
    delete cursor;
    VR_LOGD("%s provider query end", m_infoUri.getString());

    // 2.reply message
    std::string xmlStr;
    m_xmlBuilder->GetXML(xmlStr);
    ReplyDataInfo(REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST, listDataReplyMsg, xmlStr);
    return true;
}

bool VR_NCTutorialManager::GetSingleItemInfo(VR_ProtoMessageBase* replyMsg, const INT& tutorialId)
{
    VR_LOGD_FUNC();

    VrReqMediaPlayInfoResult* singleItemInfoReplyMsg = dynamic_cast<VrReqMediaPlayInfoResult*>(replyMsg);
    if (NULL == singleItemInfoReplyMsg) {
        VR_LOGD("VrReqMediaPlayInfoResult object is null");
        return false;
    }

    // 1.query single item info
    NCList<NCString> ncProjection;
    NCString  ncAudio("audio");
    ncProjection << &ncAudio;
    NCString  ncSequenceNum("sequence_number");
    ncProjection << &ncSequenceNum;
    NCString  ncIndex("tutorial_index");
    ncProjection << &ncIndex;

    NCString ArgOne;
    NCList<NCString> whereArgs;
    ArgOne.format("%d", tutorialId);
    whereArgs << &ArgOne;
    m_xmlBuilder->AddTutorialNode("screeninfo", tutorialId);

    NCCursor* cursor = m_resolver->query(NCUri(m_screenInfoUri), ncProjection, NCString(" tutorial_index = ? "),
                                         whereArgs, NCString());
    if (NULL == cursor) {
        VR_LOGD("%s query failed", m_screenInfoUri.getString());
        ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO, singleItemInfoReplyMsg, TUTORIAL_ERROR_OF_TUTORIAL_PROVIDER_ACCESS_FAILED);
        return false;
    }

    INT rowNum = cursor->getCount();
    if (0 == rowNum) {
        ReplyErrorDataInfo(REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO, singleItemInfoReplyMsg, TUTORIAL_ERROR_OF_RESPATH_HASNOT_RESOURCE);
        delete cursor;
        return false;
    }

    bool result = (NC_TRUE == cursor->moveToFirst());
    if (!result) {
        VR_LOGD("%s moveToFirst failed", m_screenInfoUri.getString());
        delete cursor;
        return false;
    }

    VR_LOGD("%s provider query begin", m_screenInfoUri.getString());
    INT curRecordNum = 0;
    NCContentValues values;
    for (INT i = 0; i < rowNum; i++) {
        values.put("screen_index", curRecordNum + 1);
        values.put("audio", cursor->getString(0));
        values.put("sequence_number", cursor->getInt64(1));
        values.put("tutorial_index", cursor->getInt64(2));
        m_xmlBuilder->AddScreenNode(values);
        values.clear();
        
        // iterator
        if (NC_TRUE == cursor->isLast()) {
            break;
        }
        cursor->moveToNext();
        curRecordNum++;
    }
    delete cursor;
    VR_LOGD("%s provider query end", m_screenInfoUri.getString());

    // 2.reply message
    std::string xmlStr;
    m_xmlBuilder->GetXML(xmlStr);
    ReplyDataInfo(REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO, singleItemInfoReplyMsg, xmlStr);
    return true;
}

void VR_NCTutorialManager::ReplyDataInfo(TutorialInfoReplyMsgType type, VR_ProtoMessageBase* replyMsg, const std::string& xmlData)
{
    VR_LOGD("xmlSize:%d  content:%s", xmlData.size(), xmlData.c_str());

    // type: load items list
    if (REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST == type) {
        VrReqMediaListDataResult* listDataReplyMsg = dynamic_cast<VrReqMediaListDataResult*>(replyMsg);
        if (NULL == listDataReplyMsg) {
            VR_LOGD("VrReqMediaListDataResult object is null");
            return;
        }
        listDataReplyMsg->set_listinfo(xmlData);
        return;
    }

    // type: load single item info
    if (REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO == type) {
        VrReqMediaPlayInfoResult* singleItemInfoReplyMsg = dynamic_cast<VrReqMediaPlayInfoResult*>(replyMsg);
        if (NULL == singleItemInfoReplyMsg) {
            VR_LOGD("VrReqMediaPlayInfoResult object is null");
            return;
        }
        singleItemInfoReplyMsg->set_screeninfo(xmlData);
        VR_LOGD("message cache size:%d", singleItemInfoReplyMsg->ByteSize());
        return;
    }

    VR_LOGD("TutorialInfoReplyMsgType is not support");
}

void VR_NCTutorialManager::ReplyErrorDataInfo(TutorialInfoReplyMsgType type, VR_ProtoMessageBase* replyMsg, TutorialErrorStatus errorCode)
{
    std::string errorXML;
    m_xmlBuilder->BuildErrorXML(errorCode, m_ErrorRelate[errorCode].getString(), errorXML);
    ReplyDataInfo(type, replyMsg, errorXML);
}
    
}
}
/* EOF */
