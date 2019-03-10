/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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

/* Standard Header */
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include "VR_Log.h"

/* VBT Header */
#ifndef VR_VOICEBOXSOURCE_H
#    include "VR_VoiceBoxSource.h"
#endif

/* Suntec Header */
#ifndef VR_VOICEBOXENGINE_H
#   include "VR_VoiceBoxEngine.h"
#endif

#ifndef VR_VOICEBOXEVENTSINK_H
#    include "VR_VoiceBoxEventSink.h"
#endif

#ifndef VR_VOICEBOXXMLPARSER_H
#    include "VR_VoiceBoxXmlParser.h"
#endif

#ifndef VR_DIALOGENGINELISTENER_H
#    include "VR_DialogEngineListener.h"
#endif

#ifndef VR_VOICEBOXCONTROLLER_H
#    include "VR_VoiceBoxController.h"
#endif

#ifndef VR_VOICEBOXAGENTAPPS_H
#    include "VR_VoiceBoxAgentApps.h"
#endif

#ifndef VR_VOICEBOXAGENTAPPS_AU_H
#    include "VR_VoiceBoxAgentApps_AU.h"
#endif

#ifndef VR_VOICEBOXAGENTAUDIO_H
#    include "VR_VoiceBoxAgentAudio.h"
#endif

#ifndef VR_VOICEBOXAGENTAUDIO_AU_H
#    include "VR_VoiceBoxAgentAudio_AU.h"
#endif

#ifndef VR_VOICEBOXAGENTCLIMATE_H
#    include "VR_VoiceBoxAgentClimate.h"
#endif

#ifndef VR_VOICEBOXAGENTGLOBAL_H
#    include "VR_VoiceBoxAgentGlobal.h"
#endif

#ifndef VR_VOICEBOXAGENTINFO_H
#    include "VR_VoiceBoxAgentInfo.h"
#endif

#ifndef VR_VOICEBOXAGENTNAVI_H
#    include "VR_VoiceBoxAgentNavi.h"
#endif

#ifndef VR_VOICEBOXAGENTPHONE_AU_H
#    include "VR_VoiceBoxAgentPhone_AU.h"
#endif

#ifndef VR_VOICEBOXXMLBUILDER_H
#    include "VR_VoiceBoxXmlBuilder.h"
#endif

#ifndef VR_VOICEBOXLOG_H
#    include "VR_VoiceBoxLog.h"
#endif

#ifndef VR_VOICEBOXCATALOGMANAGER_H
#    include "VR_VoiceBoxCatalogManager.h"
#endif

#ifndef VR_VOICEBOXXMLNODESTRING_H
#    include "VR_VoiceBoxXmlNodeString.h"
#endif

#ifndef VR_CONFIGUREIF_H
#    include "VR_ConfigureIF.h"
#endif

#ifndef VR_AUDIOBUFFER_H
#    include "VR_AudioBuffer.h"
#endif

#ifndef VR_VOICETAGIDMANAGER_H
#include "VR_VoiceTagIDManager.h"
#endif

#ifndef VR_VOICEBOXAPPSXML_H
#    include "VR_VoiceBoxAppsXml.h"
#endif

#ifndef VR_AUDIOSTREAMIF_H
#    include "VR_AudioStreamIF.h"
#endif

#ifndef VR_VOICEBOXDATASTORAGE_H
#    include "VR_VoiceBoxDataStorage.h"
#endif

#ifndef VR_VOICEBOXPHONEDATA_H
#    include "VR_VoiceBoxPhoneData.h"
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

#ifndef VR_VOICEBOXVOICETAG_H
#    include "VR_VoiceBoxVoiceTag.h"
#endif

using namespace nutshell;

#define VR_VOICETAG_ID_PREFIX       "VoiceTagID"
#define HIGH_VERBOSITY      "0"
#define SILENT_VERBOSITY    "2"
#define LOW_VERBOSITY       "4"
#define PROMPTLEVEL     "level"
#define VOICETAGFILE    "voiceTag.wav"

#define VR_ACTION_STOPBEEP "<action agent=\"prompt\" op=\"stopBeep\"></action>"
#define VR_ACTION_CLOSESESSION "<action name=\"closeSession\" />"

const int VR_INVALID_ACTION_ID = -1;

VR_VoiceBoxFrontEndShare VR_VoiceBoxEngine::s_frontEndShare;    ///< FrontEndShare
bool VR_VoiceBoxEngine::m_bUpdateState = false;

// Create VoiceBox Engine Instance
VR_VoiceBoxEngineIF* VR_VoiceBoxEngineIF::CreateInstance()
{
    return VR_new VR_VoiceBoxEngine;
}

// Constructor
VR_VoiceBoxEngine::VR_VoiceBoxEngine()
    : m_pcExternalCallback(NULL)
    , m_pcMsgController(NULL)
    , m_pcCatalogController(NULL)
    , m_pcCatalogPhone(NULL)
    , m_pcCatalogManager(NULL)
    , m_pcCatalogAudio(NULL)
    , m_pcPlayTransation(NULL)
    , m_sessionState(VR_SessionStateNone)
    , m_bTaskCompelete(false)
    , m_bPlayTTS(false)
    , m_iCurReqId(0)
    , m_iPlayVoiceTagBeep(VR_INVALID_ACTION_ID)
    , m_iCurTTSReqId(0)
    , m_iCurrActionId(0)
    , m_messageAvailable(false)
    , m_bUpdatingMapData(false)
    , m_isActiveFM(false)
    , m_isActiveAM(false)
    , m_isSatellite(false)
    , m_isActiveFMHD(false)
    , m_isActiveSatellite(false)
    , m_bBosDetected(false)
    , m_iPromptLevel(VR_PROMPTLEVEL_HIGH)
    , m_bDoCancelVoiceTag(false)
    , m_bEngineStarting(false)
    , m_bUsb1(false)
    , m_bUsb2(false)
    , m_bIpod1(false)
    , m_bIpod2(false)
    , m_bBtAudio(false)
    , m_bHVACBasicActive(false)
    , m_bHVACFrontScrnActive(false)
    , m_bHVACRearScrnActive(false)
    , m_bHVACSteeringScrnActive(false)
    , m_bHVACConcModeActive(false)
    , m_bHVACSeatActive(false)
    , m_bHVACLexusConciergeScreenActive(false)
    , m_bHVACConciergeActive(false)
    , m_bHVACFrontSeatScreenActive(false)
    , m_bHVACRearSeatScreenActive(false)
    , m_bHVACSeatOperationScreenActive(false)
    , m_isNameNull(false)
    , m_bTTSNull(false)
    , m_iStartBeepID(VR_INVALID_ACTION_ID)
    , m_iDoneBeepID(VR_INVALID_ACTION_ID)
    , m_bGrammarErrorCode(true)
    , m_bEngineReady(false)
    , m_isIncomingMessage(false)
    , m_country(VR_REGION_US)
    , m_strUsbOrIpodConnected("False")
    , m_userId("")
    , m_stopVoiceTagTTs(VR_INVALID_ACTION_ID)
    , m_stopTraining(VR_INVALID_ACTION_ID)
    , m_bSongInfoAvailable(false)
    , m_iToturialBeepId(VR_INVALID_ACTION_ID)
    , m_iVoiceTrainingBeepId(VR_INVALID_ACTION_ID)
    , m_iInstallVecIndex(0)
    , m_bInstallingAgent(false)
    , m_PVRStateCurrent(VR_PVRState_None)
    , m_bWaitConfirmation(false)
    , m_bSpeakOverBeep(false)
    , m_bTslNetworkAvailable(false)
    , m_bAppRecoState(VR_AppRecoState_Invalid)
    , m_bWaitGreetingEnd(false)
    , m_bQuitWaitForPrompt(false)
    , m_bTslDialog(false)
    , m_bTslAppsAvailable(false)
    , m_bPVRScreen(false)
    , m_bSettingToStartScreen(false)
    , m_bNBestFlg(false)
    , m_bWaitForDoneBeep(false)
    , m_strCurResourceState("1")
    , m_bDoCancelTslVR(false)
    , m_bBackInterupted(false)
    , m_bStartSessionWithBargeIn(false)
    , m_bSendMsgAvailable(false)
    , m_bHavePVR(false)
    , m_bPauseReturnAfterVR(false)
    , m_bWaitResumeGrammar(false)
    , m_bCanceled(false)
    , m_bRouteStatus(false)
    , m_bDoCancelSession(false)
    , m_bChangeLanguage(false)
    , m_bNeedPromptPlaying(false)
    , m_bPrepare(false)
    , m_ulInstallErrorCode(S_OK)
    , m_bPhoneConnected(false)
    , m_bDiscAvailable(false)
    , m_bIgnoreTslPtt(true)
    , m_bSession(false)
    , m_bPhoneReloadData(false)
    , m_bNeedGrammarResult(true)
    , m_bHandleTTSCrush(false)
    , m_bInEndTask(false)
    , m_bAvcLan(false)
    , m_bHVACFahrenheit(false)
    , m_enBtnProcessState(VR_Button_Null)
    , m_bReInstallTSL(false)
    , m_enVDataStatus(enVDataStatus_Default)
    , m_iPlayVoiceTagTts(VR_INVALID_ACTION_ID)
    , m_bVoiceTagTtsCrash(false)
    , m_stopVoiceTagBeep(VR_INVALID_ACTION_ID)
    , m_enVoiceTagRecState(VR_VoiceTag_NoRec)
    , m_bSpeakAdaptPrompt(false)
{
    int nAgentNum = AgentType_Num;
    for (int i = 0; i < nAgentNum; ++i) {
        m_agents[i] = NULL;
    }

    m_iIndex = 0;
    m_iHintSize = 0;

}

// Destructor
VR_VoiceBoxEngine::~VR_VoiceBoxEngine()
{
    for (int i = 0; i < AgentType_Num; ++i) {
        if (NULL != m_agents[i]) {
            delete m_agents[i];
        }
    }

    m_pcExternalCallback = NULL;
    m_pcMsgController = NULL;
    m_pcCatalogController = NULL;
    delete m_pcCatalogManager;
    m_pcCatalogManager = NULL;
    m_pcCatalogPhone = NULL;
    m_pcCatalogAudio = NULL;
    m_pcPlayTransation = NULL;
}

// Create the VoiceBox engine related instances and initialize them
bool
VR_VoiceBoxEngine::Initialize(
    VR_DialogEngineListener*     pcExternalCallback,
    VR_VoiceBoxController*  pcMsgController,
    VR_VoiceBoxController*  pcCatalogController
    )
{
    VR_LOGD_FUNC();
    VR_ConfigureIF * pcConfig = VR_ConfigureIF::Instance();
    if (NULL != pcConfig) {
        m_country = pcConfig->getVRContry();

        // the type of L2/T2 is team work, the other is not.
        if (VR_PRODUCT_TYPE_L2 == pcConfig->getVRProduct()
            || VR_PRODUCT_TYPE_T2 == pcConfig->getVRProduct()) {
            m_bAvcLan = true;
        }
        else {
            m_bAvcLan = false;
        }
    }

    m_bEngineStarting = false;

    // Used for notifing the voice recognition result and request to the user
    m_pcExternalCallback = pcExternalCallback;
    // Used for processing the voice recognition related XML messages
    m_pcMsgController = pcMsgController;
    m_pcCatalogController = pcCatalogController;

    VR_AudioStreamIF* pcAudioStream = VR_AudioStreamIF::Instance();
    if (NULL != pcAudioStream) {
        pcAudioStream->Initialize(this);
    }

    m_lstTransaction.clear();
    m_lstUCAppQuitVROp.clear();
    m_lstNeedReactiveAction.clear();
    m_lstCancelOption.clear();
    m_lstGrammarNewName.clear();

    // Create the aciton-function map
    m_mapMsgHandler.insert(std::make_pair("startAgent", &VR_VoiceBoxEngine::SendRecognitionState));
    m_mapMsgHandler.insert(std::make_pair("startCurrentAgent", &VR_VoiceBoxEngine::SendRecognitionState));
    m_mapMsgHandler.insert(std::make_pair("startNextAgent", &VR_VoiceBoxEngine::SaveNextRecognitionState));
    m_mapMsgHandler.insert(std::make_pair("showHintScreen", &VR_VoiceBoxEngine::ShowHintScreen));
    m_mapMsgHandler.insert(std::make_pair("saveTutoAgent", &VR_VoiceBoxEngine::SaveTuToRecognitionState));
    m_mapMsgHandler.insert(std::make_pair("startDictation", &VR_VoiceBoxEngine::StartDictation));
    m_mapMsgHandler.insert(std::make_pair("buttonPressed", &VR_VoiceBoxEngine::ButtonPressed));
    m_mapMsgHandler.insert(std::make_pair("updateState", &VR_VoiceBoxEngine::UpdateState));
    m_mapMsgHandler.insert(std::make_pair("changeSettings", &VR_VoiceBoxEngine::ChangeSettings));
    m_mapMsgHandler.insert(std::make_pair("changeLanguage", &VR_VoiceBoxEngine::ChangeLanguage));
    m_mapMsgHandler.insert(std::make_pair("fullupdateNotify", &VR_VoiceBoxEngine::FullUpdateNotify));
    m_mapMsgHandler.insert(std::make_pair("getResourceState", &VR_VoiceBoxEngine::GetResourceState));
    m_mapMsgHandler.insert(std::make_pair("stop", &VR_VoiceBoxEngine::Cancel));
    m_mapMsgHandler.insert(std::make_pair("cancel", &VR_VoiceBoxEngine::Cancel));
    m_mapMsgHandler.insert(std::make_pair("back", &VR_VoiceBoxEngine::Back));
    m_mapMsgHandler.insert(std::make_pair("repeat-internal", &VR_VoiceBoxEngine::Repeat));
    m_mapMsgHandler.insert(std::make_pair("help", &VR_VoiceBoxEngine::Help));
    m_mapMsgHandler.insert(std::make_pair("playTts", &VR_VoiceBoxEngine::SpeakDone));
    m_mapMsgHandler.insert(std::make_pair("stopTts", &VR_VoiceBoxEngine::StopSpeakDone));
    m_mapMsgHandler.insert(std::make_pair("playTtsList", &VR_VoiceBoxEngine::SpeakDone));
    m_mapMsgHandler.insert(std::make_pair("stopTtsList", &VR_VoiceBoxEngine::StopSpeakDone));
    m_mapMsgHandler.insert(std::make_pair("playBeep", &VR_VoiceBoxEngine::BeepDone));
    m_mapMsgHandler.insert(std::make_pair("stopBeep", &VR_VoiceBoxEngine::StopBeepDone));
    m_mapMsgHandler.insert(std::make_pair("start-internal", &VR_VoiceBoxEngine::StartRecoSession));
    m_mapMsgHandler.insert(std::make_pair("start-bargein", &VR_VoiceBoxEngine::StartRecoSessionWithBargeIn));
    m_mapMsgHandler.insert(std::make_pair("startover", &VR_VoiceBoxEngine::StartOver));
    m_mapMsgHandler.insert(std::make_pair("startover-internal", &VR_VoiceBoxEngine::StartOver));
    m_mapMsgHandler.insert(std::make_pair("commandcomplete-internal", &VR_VoiceBoxEngine::OnCommandComplete));
    m_mapMsgHandler.insert(std::make_pair("pause-internal", &VR_VoiceBoxEngine::Pause));
    m_mapMsgHandler.insert(std::make_pair("getHints", &VR_VoiceBoxEngine::OnGetHints));
    m_mapMsgHandler.insert(std::make_pair("waitStart", &VR_VoiceBoxEngine::SetRecoSessionFlag));
    m_mapMsgHandler.insert(std::make_pair("morehint-internal", &VR_VoiceBoxEngine::MoreHints));
    m_mapMsgHandler.insert(std::make_pair("actionMessage", &VR_VoiceBoxEngine::HandleActionMsg));
    m_mapMsgHandler.insert(std::make_pair("sendVRState", &VR_VoiceBoxEngine::HandleVRState));
    m_mapMsgHandler.insert(std::make_pair("PvrTTS", &VR_VoiceBoxEngine::PVRAction));
    m_mapMsgHandler.insert(std::make_pair("agentHelp", &VR_VoiceBoxEngine::AgentHelp));
    m_mapMsgHandler.insert(std::make_pair("startedNotify", &VR_VoiceBoxEngine::OnDMStartedNotify));
    m_mapMsgHandler.insert(std::make_pair("IncomingMessageInfo", &VR_VoiceBoxEngine::IncomingMessageInfo));
    m_mapMsgHandler.insert(std::make_pair("hintPage", &VR_VoiceBoxEngine::HintPage));
    m_mapMsgHandler.insert(std::make_pair("changeCountry", &VR_VoiceBoxEngine::UpdateMapData));
    m_mapMsgHandler.insert(std::make_pair("help-internal", &VR_VoiceBoxEngine::OnHelpRecognized));
    m_mapMsgHandler.insert(std::make_pair("escalating-error", &VR_VoiceBoxEngine::OnEscalatingError));
    m_mapMsgHandler.insert(std::make_pair("initialpersondata", &VR_VoiceBoxEngine::InitialPersonData));
    m_mapMsgHandler.insert(std::make_pair("prepare", &VR_VoiceBoxEngine::Prepare));
    m_mapMsgHandler.insert(std::make_pair("changeLanguage-internal", &VR_VoiceBoxEngine::ChangeLanguageInner));

    m_mapMsgHandler.insert(std::make_pair("back-internal", &VR_VoiceBoxEngine::Back));
    m_mapMsgHandler.insert(std::make_pair("install-agent-notification", &VR_VoiceBoxEngine::OnInstallAgentNotification));
    m_mapMsgHandler.insert(std::make_pair("onBosDetected", &VR_VoiceBoxEngine::HandleBosDetected));
    m_mapMsgHandler.insert(std::make_pair("confirm-rendering", &VR_VoiceBoxEngine::ConfirmRendering));

    m_mapMsgBtnHandler.insert(std::make_pair("ptt_hard_key_short_press", &VR_VoiceBoxEngine::PttShort));
    m_mapMsgBtnHandler.insert(std::make_pair("ptt_hard_key_long_press", &VR_VoiceBoxEngine::PttLong));
    m_mapMsgBtnHandler.insert(std::make_pair("hard_key_enter_normal_press", &VR_VoiceBoxEngine::EntryNormalPress));
    m_mapMsgBtnHandler.insert(std::make_pair("meter_hard_key_back_normal_press", &VR_VoiceBoxEngine::BackNormalPress));
    m_mapMsgBtnHandler.insert(std::make_pair("select_one", &VR_VoiceBoxEngine::SelectOne));
    m_mapMsgBtnHandler.insert(std::make_pair("select_two", &VR_VoiceBoxEngine::SelectTwo));
    m_mapMsgBtnHandler.insert(std::make_pair("select_three", &VR_VoiceBoxEngine::SelectThree));
    m_mapMsgBtnHandler.insert(std::make_pair("select_four", &VR_VoiceBoxEngine::SelectFour));
    m_mapMsgBtnHandler.insert(std::make_pair("select_five", &VR_VoiceBoxEngine::SelectFive));
    m_mapMsgBtnHandler.insert(std::make_pair("phone", &VR_VoiceBoxEngine::Phone));
    m_mapMsgBtnHandler.insert(std::make_pair("navi", &VR_VoiceBoxEngine::Navigation));
    m_mapMsgBtnHandler.insert(std::make_pair("apps", &VR_VoiceBoxEngine::Apps));
    m_mapMsgBtnHandler.insert(std::make_pair("audio", &VR_VoiceBoxEngine::Audio));
    m_mapMsgBtnHandler.insert(std::make_pair("info", &VR_VoiceBoxEngine::Info));
    m_mapMsgBtnHandler.insert(std::make_pair("climate", &VR_VoiceBoxEngine::Climate));
    m_mapMsgBtnHandler.insert(std::make_pair("start", &VR_VoiceBoxEngine::SpeakAdaptation));
    m_mapMsgBtnHandler.insert(std::make_pair("start_over", &VR_VoiceBoxEngine::StartOver));
    m_mapMsgBtnHandler.insert(std::make_pair("pause", &VR_VoiceBoxEngine::HandlePause));
    m_mapMsgBtnHandler.insert(std::make_pair("resume", &VR_VoiceBoxEngine::Resume));
    m_mapMsgBtnHandler.insert(std::make_pair("next_page", &VR_VoiceBoxEngine::NextPage));
    m_mapMsgBtnHandler.insert(std::make_pair("previous_page", &VR_VoiceBoxEngine::PrevPage));
    m_mapMsgBtnHandler.insert(std::make_pair("last_page", &VR_VoiceBoxEngine::LastPage));
    m_mapMsgBtnHandler.insert(std::make_pair("first_page", &VR_VoiceBoxEngine::FirstPage));
    m_mapMsgBtnHandler.insert(std::make_pair("yes", &VR_VoiceBoxEngine::ConfirmYes));
    m_mapMsgBtnHandler.insert(std::make_pair("no", &VR_VoiceBoxEngine::ConfirmNo));
    m_mapMsgBtnHandler.insert(std::make_pair("go_directly", &VR_VoiceBoxEngine::GoDirectly));
    m_mapMsgBtnHandler.insert(std::make_pair("add_to_route", &VR_VoiceBoxEngine::AddToRoute));
    m_mapMsgBtnHandler.insert(std::make_pair("go_back", &VR_VoiceBoxEngine::Back));
    m_mapMsgBtnHandler.insert(std::make_pair("along_route", &VR_VoiceBoxEngine::AlongRoute));
    m_mapMsgBtnHandler.insert(std::make_pair("near_destination", &VR_VoiceBoxEngine::NearDestination));
    m_mapMsgBtnHandler.insert(std::make_pair("help", &VR_VoiceBoxEngine::Help));
    m_mapMsgBtnHandler.insert(std::make_pair("more_hints", &VR_VoiceBoxEngine::HandleMoreHints));
    m_mapMsgBtnHandler.insert(std::make_pair("call", &VR_VoiceBoxEngine::Call));
    m_mapMsgBtnHandler.insert(std::make_pair("send_message", &VR_VoiceBoxEngine::SendMessage));
    m_mapMsgBtnHandler.insert(std::make_pair("album", &VR_VoiceBoxEngine::BrowseAlbums));
    m_mapMsgBtnHandler.insert(std::make_pair("artist", &VR_VoiceBoxEngine::BrowseArtists));
    m_mapMsgBtnHandler.insert(std::make_pair("song", &VR_VoiceBoxEngine::BrowseSongs));
    m_mapMsgBtnHandler.insert(std::make_pair("composer", &VR_VoiceBoxEngine::BrowseComposers));
    m_mapMsgBtnHandler.insert(std::make_pair("genre", &VR_VoiceBoxEngine::BrowseGenres));
    m_mapMsgBtnHandler.insert(std::make_pair("podcast", &VR_VoiceBoxEngine::BrowsePodcasts));
    m_mapMsgBtnHandler.insert(std::make_pair("playlist", &VR_VoiceBoxEngine::BrowsePlaylists));
    m_mapMsgBtnHandler.insert(std::make_pair("audiobook", &VR_VoiceBoxEngine::BrowseAudiobooks));
    m_mapMsgBtnHandler.insert(std::make_pair("reply", &VR_VoiceBoxEngine::Reply));
    m_mapMsgBtnHandler.insert(std::make_pair("next", &VR_VoiceBoxEngine::ReadNext));
    m_mapMsgBtnHandler.insert(std::make_pair("previous", &VR_VoiceBoxEngine::ReadPrevious));
    m_mapMsgBtnHandler.insert(std::make_pair("tutorials", &VR_VoiceBoxEngine::Tutorials));
    m_mapMsgBtnHandler.insert(std::make_pair("voice_training", &VR_VoiceBoxEngine::VoiceTraining));
    m_mapMsgBtnHandler.insert(std::make_pair("ptt_hard_key_short_press_special", &VR_VoiceBoxEngine::InternalStartVR));
    m_mapMsgBtnHandler.insert(std::make_pair("setting_voice_training_start", &VR_VoiceBoxEngine::SettingToTraning));

    m_mapAgenttoRecoState.insert(std::make_pair("topmenu", VBT_RECO_STATE_GLOBAL));
    m_mapAgenttoRecoState.insert(std::make_pair("apps", VBT_RECO_STATE_APPS_HOME));
    m_mapAgenttoRecoState.insert(std::make_pair("phone", VBT_RECO_STATE_HFD_HOME));
    m_mapAgenttoRecoState.insert(std::make_pair("in_call", VBT_RECO_STATE_IN_CALL));
    m_mapAgenttoRecoState.insert(std::make_pair("in_message", VBT_RECO_STATE_INCOMING_MESSAGE));
    m_mapAgenttoRecoState.insert(std::make_pair("climate", VBT_RECO_STATE_HVAC_HOME));
    m_mapAgenttoRecoState.insert(std::make_pair("information", VBT_RECO_STATE_INFORMATION_HOME));
    if (VR_REGION_US == m_country) {
        m_mapAgenttoRecoState.insert(std::make_pair("music", VBT_RECO_STATE_MUSIC_HOME));
        m_mapAgenttoRecoState.insert(std::make_pair("media", VBT_RECO_STATE_MUSIC_HOME));
        m_mapAgenttoRecoState.insert(std::make_pair("radio", VBT_RECO_STATE_RADIO_HOME));
    }
    else if (VR_REGION_OC == m_country) {
        m_mapAgenttoRecoState.insert(std::make_pair("music", "Audio Home"));
        m_mapAgenttoRecoState.insert(std::make_pair("media", "Audio Home"));
        m_mapAgenttoRecoState.insert(std::make_pair("radio", "Audio Home"));
    }
    else {
    }

    m_mapAgenttoRecoState.insert(std::make_pair("navi", VBT_RECO_STATE_NAVIGATION_HOME));
    m_mapAgenttoRecoState.insert(std::make_pair("poi", VBT_RECO_STATE_POI_HOME));
    m_mapAgenttoRecoState.insert(std::make_pair("cities_key_dict", VBT_RECO_STATE_CITIES_KEYBOARD_DICTATION));
    m_mapAgenttoRecoState.insert(std::make_pair("generic_key_dict", VBT_RECO_STATE_GENERIC_KEYBOARD_DICTATION));
    m_mapAgenttoRecoState.insert(std::make_pair("hfd_key_dict", VBT_RECO_STATE_HFD_KEYBOARD_DICTATION));
    m_mapAgenttoRecoState.insert(std::make_pair("msg_dict", VBT_RECO_STATE_MESSAGE_DICTATION));
    m_mapAgenttoRecoState.insert(std::make_pair("music_key_dict", VBT_RECO_STATE_MUSIC_KEYBOARD_DICTATION));
    m_mapAgenttoRecoState.insert(std::make_pair("navi_key_dict", VBT_RECO_STATE_NAVIGATION_KEYBOARD_DICTATION));
    m_mapAgenttoRecoState.insert(std::make_pair("poi_key_dict", VBT_RECO_STATE_POI_KEYBOARD_DICTATION));
    m_mapAgenttoRecoState.insert(std::make_pair("states_key_dict", VBT_RECO_STATE_STATES_KEYBOARD_DICTATION));
    m_mapAgenttoRecoState.insert(std::make_pair("streets_key_dict", VBT_RECO_STATE_STREETS_KEYBOARD_DICTATION));
    m_mapAgenttoRecoState.insert(std::make_pair("adaptation", VBT_RECO_STATE_SPEAKER_ADAPTATION));
    m_mapAgenttoRecoState.insert(std::make_pair("voice_tag", VBT_RECO_STATE_VOICE_TAG));

    m_mapHintsScreenID.insert(std::make_pair("none", VBT_HINT_SCREEN_ID_NONE));
    m_mapHintsScreenID.insert(std::make_pair("main", VBT_HINT_SCREEN_ID_MAIN));
    m_mapHintsScreenID.insert(std::make_pair("help", VBT_HINT_SCREEN_ID_HELP));
    m_mapHintsScreenID.insert(std::make_pair("phone", VBT_HINT_SCREEN_ID_PHONE));
    m_mapHintsScreenID.insert(std::make_pair("apps", VBT_HINT_SCREEN_ID_APPS));
    m_mapHintsScreenID.insert(std::make_pair("audio", VBT_HINT_SCREEN_ID_AUDIO));
    m_mapHintsScreenID.insert(std::make_pair("info", VBT_HINT_SCREEN_ID_INFO));
    m_mapHintsScreenID.insert(std::make_pair("climate", VBT_HINT_SCREEN_ID_CLIMATE));
    m_mapHintsScreenID.insert(std::make_pair("navigation", VBT_HINT_SCREEN_ID_NAVIGATION));

    m_lstNeedReactiveAction.push_back(VBT_ACTION_TYPE_APPS_GO_TO_APPS);
    m_lstNeedReactiveAction.push_back(VBT_ACTION_TYPE_HFD_HFD_HOME);
    m_lstNeedReactiveAction.push_back(VBT_ACTION_TYPE_HVAC_GO_TO_HVAC);
    m_lstNeedReactiveAction.push_back(VBT_ACTION_TYPE_HVAC_AC_ALREADY_ON);
    m_lstNeedReactiveAction.push_back(VBT_ACTION_TYPE_HVAC_AC_ALREADY_OFF);
    m_lstNeedReactiveAction.push_back(VBT_ACTION_TYPE_HVAC_CHANGE_TEMPERATURE_ALREADY_AT_REQUESTED_TEMPERATURE);
    m_lstNeedReactiveAction.push_back(VBT_ACTION_TYPE_HVAC_ALREADY_AT_MAX_TEMPERATURE);
    m_lstNeedReactiveAction.push_back(VBT_ACTION_TYPE_HVAC_ALREADY_AT_MIN_TEMPERATURE);
    m_lstNeedReactiveAction.push_back("Decrease Temperature - Already At Min Temperature");
    m_lstNeedReactiveAction.push_back(VBT_ACTION_TYPE_MUSIC_MUSIC_MENU);
    m_lstNeedReactiveAction.push_back(VBT_ACTION_TYPE_RADIO_GO_TO_RADIO);
    m_lstNeedReactiveAction.push_back(VBT_ACTION_TYPE_RADIO_TUNE_FREQUENCY_BAND_AM);
    m_lstNeedReactiveAction.push_back(VBT_ACTION_TYPE_RADIO_TUNE_FREQUENCY_BAND_FM);
    m_lstNeedReactiveAction.push_back(VBT_ACTION_TYPE_RADIO_TUNE_FREQUENCY_CATCH);
    m_lstNeedReactiveAction.push_back(VBT_ACTION_TYPE_RADIO_TUNE_RADIO_CATCH);
    m_lstNeedReactiveAction.push_back(VBT_ACTION_TYPE_RADIO_TUNE_FREQUENCY_BAND_SATELLITE);
    m_lstNeedReactiveAction.push_back("Confirm Tutorial - Reject");
    m_lstNeedReactiveAction.push_back("Start Over");
    m_lstNeedReactiveAction.push_back("Apps Home");
    m_lstNeedReactiveAction.push_back("Audio Home");
    m_lstNeedReactiveAction.push_back("Navigation Home");

    m_lstUCAppQuitVROp.push_back("changeSourceByName");
    m_lstUCAppQuitVROp.push_back("startRadio");
    m_lstUCAppQuitVROp.push_back("dial");
    m_lstUCAppQuitVROp.push_back("sendMessage");
    m_lstUCAppQuitVROp.push_back("showAddVoiceTagUI");
    m_lstUCAppQuitVROp.push_back("launchApp");
    m_lstUCAppQuitVROp.push_back("showTrainingEndMsg");

    m_lstGrammarNewName.push_back("quickreplymessage");
    m_lstGrammarNewName.push_back("phonetype");
    m_lstGrammarNewName.push_back("messagetype");
    m_lstGrammarNewName.push_back("audiosource");
    m_lstGrammarNewName.push_back("fmgenre");
    m_lstGrammarNewName.push_back("satchannelname");
    m_lstGrammarNewName.push_back("satchannelnumber");
    m_lstGrammarNewName.push_back("satgenre");

    InitAutoUpdateStateGrammar();

    // Before start vbt engine, rename the existed vbt log files
    RenameVbtLog();

    // Creates an Engine Client object and initializes it.
    if (!CreateEngineClient()) {
        VR_LOGE("CreateEngineClient Failed");
        return false;
    }

    // Create an engine command dispatcher object.
    if (!CreateEngineCommand()) {
        VR_LOGE("CreateEngineCommand Failed");
        return false;
    }

    // Creates Event Sink object instance and register it.
    if (!CreateEventSink()) {
        VR_LOGE("CreateEventSink Failed");
        return false;
    }

    // Create an catalog manager object
    if (!CreateCatalogManager()) {
        VR_LOGE("CreateCatalogManager Failed");
        return false;
    }

    InitPromptOfScreen();

    if (NULL != pcConfig) {
        VR_LOGI("The target's model is: %d", pcConfig->getVRProduct());
    }
    return true;
}

void
VR_VoiceBoxEngine::InitAutoUpdateStateGrammar()
{
    m_mapUpdateStateHandler.clear();
    m_lstUpdateGrammar.clear();

    VR_ConfigureIF * pcConfig = VR_ConfigureIF::Instance();
    if (NULL == pcConfig) {
        return;
    }

    VR_PRODUCT_TYPE productType = pcConfig->getVRProduct();

    m_mapUpdateStateHandler.insert(std::make_pair("PHONE_STATE_CONNECTED", &VR_VoiceBoxEngine::UpdatePhoneConnState));
    m_mapUpdateStateHandler.insert(std::make_pair("PHONE_DEVICE_ID", &VR_VoiceBoxEngine::UpdatePhoneDeviceId));
    m_mapUpdateStateHandler.insert(std::make_pair("USB_1_CONNECTED", &VR_VoiceBoxEngine::UpdateMusicConnState));
    m_mapUpdateStateHandler.insert(std::make_pair("USB_2_CONNECTED", &VR_VoiceBoxEngine::UpdateMusicConnState));
    m_mapUpdateStateHandler.insert(std::make_pair("IPOD_1_CONNECTED", &VR_VoiceBoxEngine::UpdateMusicConnState));
    m_mapUpdateStateHandler.insert(std::make_pair("IPOD_2_CONNECTED", &VR_VoiceBoxEngine::UpdateMusicConnState));
    m_mapUpdateStateHandler.insert(std::make_pair("TSL_STATE_AVAILABLE", &VR_VoiceBoxEngine::UpdateTSLAvailable));
    m_mapUpdateStateHandler.insert(std::make_pair("TSL_APPS_AVAILABLE", &VR_VoiceBoxEngine::UpdateTSLAvailable));
    m_mapUpdateStateHandler.insert(std::make_pair("TSL_APPSLIST", &VR_VoiceBoxEngine::UpdateTSLList));
    if (VR_REGION_US == m_country) {
        m_mapUpdateStateHandler.insert(std::make_pair("PHONE_MAPPROTOCOL", &VR_VoiceBoxEngine::UpdateMessageState));
        m_mapUpdateStateHandler.insert(std::make_pair("PHONE_GETMESSAGESTATE", &VR_VoiceBoxEngine::UpdateMessageState));
        m_mapUpdateStateHandler.insert(std::make_pair("OPERESTRICT", &VR_VoiceBoxEngine::UpdateOperestrict));
        m_mapUpdateStateHandler.insert(std::make_pair("MUSIC_SONG_INFORMATION_AVAILABLE", &VR_VoiceBoxEngine::UpdateMusicState));
        m_mapUpdateStateHandler.insert(std::make_pair("MEDIAINFOKEY_MAGICSTRING", &VR_VoiceBoxEngine::UpdateGraceNoteMagicNum));
        m_mapUpdateStateHandler.insert(std::make_pair("RADIO_STATE_FM_ACTIVE", &VR_VoiceBoxEngine::UpdateRadioState));
        m_mapUpdateStateHandler.insert(std::make_pair("RADIO_STATE_AM_ACTIVE", &VR_VoiceBoxEngine::UpdateRadioState));

        if (VR_PRODUCT_TYPE_T1 == productType || VR_PRODUCT_TYPE_L1 == productType) {
            m_mapUpdateStateHandler.insert(std::make_pair("RADIO_XM_CONNECTED", &VR_VoiceBoxEngine::UpdateRadioState));
            m_mapUpdateStateHandler.insert(std::make_pair("RADIO_STATE_FM_HD_ACTIVE", &VR_VoiceBoxEngine::UpdateRadioState));
            m_mapUpdateStateHandler.insert(std::make_pair("RADIO_STATE_XM_ACTIVE", &VR_VoiceBoxEngine::UpdateRadioState));
        }

        if (VR_PRODUCT_TYPE_L1 == productType) {
            m_mapUpdateStateHandler.insert(std::make_pair("CLIMATE_FANSPEED_MAX", &VR_VoiceBoxEngine::UpdateHVACFanTempRange));
            m_mapUpdateStateHandler.insert(std::make_pair("CLIMATE_TEMPERATURE_MIN", &VR_VoiceBoxEngine::UpdateHVACFanTempRange));
            m_mapUpdateStateHandler.insert(std::make_pair("CLIMATE_TEMPERATURE_MAX", &VR_VoiceBoxEngine::UpdateHVACFanTempRange));
            m_mapUpdateStateHandler.insert(std::make_pair("CLIMATE_BASIC_ACTIVE", &VR_VoiceBoxEngine::UpdateHVACState));
            m_mapUpdateStateHandler.insert(std::make_pair("CLIMATE_FRONT_SCREEN_ACTIVE", &VR_VoiceBoxEngine::UpdateHVACState));
            m_mapUpdateStateHandler.insert(std::make_pair("CLIMATE_REAR_SCREEN_ACTIVE", &VR_VoiceBoxEngine::UpdateHVACState));
            m_mapUpdateStateHandler.insert(std::make_pair("CLIMATE_STEERING_SCREEN_ACTIVE", &VR_VoiceBoxEngine::UpdateHVACState));
            m_mapUpdateStateHandler.insert(std::make_pair("CLIMATE_LEXUS_CONCIERGE_SCREEN_ACTIVE", &VR_VoiceBoxEngine::UpdateHVACState));
            m_mapUpdateStateHandler.insert(std::make_pair("CLIMATE_CONCIERGE_ACTIVE", &VR_VoiceBoxEngine::UpdateHVACState));
            m_mapUpdateStateHandler.insert(std::make_pair("CLIMATE_FRONT_SEAT_SCREEN_ACTIVE", &VR_VoiceBoxEngine::UpdateHVACState));
            m_mapUpdateStateHandler.insert(std::make_pair("CLIMATE_REAR_SEAT_SCREEN_ACTIVE", &VR_VoiceBoxEngine::UpdateHVACState));
            m_mapUpdateStateHandler.insert(std::make_pair("CLIMATE_SEAT_OPERATION_SCREEN_ACTIVE", &VR_VoiceBoxEngine::UpdateHVACState));
        }
    }

    if (VR_REGION_OC == m_country) {
        m_mapUpdateStateHandler.insert(std::make_pair("PHONE_STATE_REGISTERED", &VR_VoiceBoxEngine::UpdatePhoneRegistered));
        m_mapUpdateStateHandler.insert(std::make_pair("STEERING_PHONEKEY_TYPE", &VR_VoiceBoxEngine::UpdatePhoneSteeringType));
        m_mapUpdateStateHandler.insert(std::make_pair("BTAUDIO_CONNECTED", &VR_VoiceBoxEngine::UpdateMusicConnState));
    }

    if (VR_REGION_US == m_country) {
        m_lstUpdateGrammar.push_back(VR_RADIO_GRAMMAR_NEW_FMGENRE);
        if (VR_PRODUCT_TYPE_T1 == productType || VR_PRODUCT_TYPE_L1 == productType) {
            m_lstUpdateGrammar.push_back(VR_RADIO_GRAMMAR_NEW_SATCHANNEL);
            m_lstUpdateGrammar.push_back(VR_RADIO_GRAMMAR_NEW_SATGENRE);
        }
    }

    m_lstUpdateGrammar.push_back(VR_MUSIC_GRAMMAR_NEW_AUDIOSOURCE);

    m_lstUpdateGrammar.push_back(VR_PHONE_GRAMMAR_DISACTIVE);
    m_lstUpdateGrammar.push_back(VR_PHONE_GRAMMAR_INIT);
    m_lstUpdateGrammar.push_back(VR_PHONE_GRAMMAR_ACTIVE);
    m_lstUpdateGrammar.push_back(VR_PHONE_GRAMMAR_NEW_PHONETYPE);

    if (VR_REGION_US == m_country) {
        m_lstUpdateGrammar.push_back(VR_PHONE_GRAMMAR_NEW_MESSAGETYPE);
        m_lstUpdateGrammar.push_back(VR_PHONE_GRAMMAR_NEW_QUICKREPLAYMESSAGE);
    }

    m_lstUpdateGrammar.push_back(VR_MUSIC_GRAMMAR_DISACTIVE);
    m_lstUpdateGrammar.push_back(VR_MUSIC_GRAMMAR_INIT_1);
    m_lstUpdateGrammar.push_back(VR_MUSIC_GRAMMAR_DIFF_1);
    m_lstUpdateGrammar.push_back(VR_MUSIC_GRAMMAR_INIT_2);
    m_lstUpdateGrammar.push_back(VR_MUSIC_GRAMMAR_DIFF_2);
    m_lstUpdateGrammar.push_back(VR_MUSIC_GRAMMAR_ACTIVE);

}

void
VR_VoiceBoxEngine::InitPromptOfScreen()
{
    std::string strVRStatePath = VR_ConfigureIF::Instance()->getDataPath();
    strVRStatePath.append("config/VRState.dat");

    FILE * fd = fopen(strVRStatePath.c_str(), "r");
    char buf[256] = { 0 };

    if (NULL == fd) {
        VR_LOGI("the fd of the file is null");
        return;
    }

    if (VR_REGION_OC == m_country) {
        while (!feof(fd)) {
            memset(buf, 0, sizeof(buf));
            fgets(buf, sizeof(buf), fd);

            std::string strScreenPrompt = std::string(buf);
            size_t iPos = strScreenPrompt.find("|");

            std::string strScreenId;
            std::string strPromptOC;
            if (std::string::npos != iPos) {
                strScreenId = strScreenPrompt.substr(0, iPos);
            }

            size_t iPos1 = strScreenPrompt.find("|", iPos + 1);
            if (std::string::npos != iPos1) {
                strPromptOC = strScreenPrompt.substr(iPos + 1, iPos1 - iPos - 1);
            }

            if (!strScreenId.empty() && !strPromptOC.empty()) {
                m_mapScreenPromptOC.insert(std::make_pair(strScreenId, strPromptOC));
            }
        }
    }
    else if (VR_REGION_US == m_country) {
        while (!feof(fd)) {
            memset(buf, 0, sizeof(buf));
            fgets(buf, sizeof(buf), fd);

            std::string strScreenPrompt = std::string(buf);
            size_t iPos = strScreenPrompt.find("|");

            std::string strScreenId;
            std::string strPromptMX;
            std::string strPromptCA;
            std::string strPromptUS;

            if (std::string::npos != iPos) {
                strScreenId = strScreenPrompt.substr(0, iPos);
            }
            VoiceVector<std::string>::type vecPrompt;
            size_t iPos1 = strScreenPrompt.find("|", iPos + 1);
            if (std::string::npos != iPos1) { // en-MX
                strPromptMX = strScreenPrompt.substr(iPos + 1, iPos1 - iPos - 1);
                vecPrompt.push_back(strPromptMX);
            }

            size_t iPos2 = strScreenPrompt.find("|", iPos1 + 1);
            if (std::string::npos != iPos2) { // fr-CA
                strPromptCA = strScreenPrompt.substr(iPos1 + 1, iPos2 - iPos1 - 1);
                vecPrompt.push_back(strPromptCA);
            }

            size_t iPos3 = strScreenPrompt.find("|", iPos2 + 1);
            if (std::string::npos != iPos3) { // en-US
                strPromptUS = strScreenPrompt.substr(iPos2 + 1, iPos3 - iPos2 - 1);
                vecPrompt.push_back(strPromptUS);
            }

            m_mapScreenPrompt.insert(std::make_pair(strScreenId, vecPrompt));
        }
    }

    fclose(fd);

    return;
}

// Start the VoiceBox engine with the specified culture
bool
VR_VoiceBoxEngine::Start(const std::string &strCultureName)
{
    VR_LOGD_FUNC();

    VR_LOGI("current culture : %s", strCultureName.c_str());

    if (NULL == m_engineCommand.ptr()) {
        VR_LOGE("The engine have not been initialized");
        return false;
    }
    m_strCultureName = strCultureName;
    m_strInstallCulture = strCultureName;
    m_bEngineStarting = false;

    // Start the dialog engine with the specified culture
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->StartEngine(
                     &transaction,
                     strCultureName.c_str()
                     );
    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "Start Engine Failed, result: %lx, transaction: %p", result, transaction.ptr());
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnStartEngine
            )
        );

    // Make it a sync operation, wait until the engine is started
    VBT_BOOL bTimeout = VBT_FALSE;
    result = transaction->WaitForCompletion(INFINITE, &bTimeout);
    if (FAILED(result) || bTimeout) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "Waiting Engine Start Failed, result: %lx, timeout: %n", result, bTimeout);
        return false;
    }

    if ("en-AU" == strCultureName) {
        UpdateMapGrammar();
    }

    if (VR_REGION_US == m_country) {
        std::string relativePath;
        if ("" == m_strDevice) {
            m_strAdaptationPath = GetAdaptationProfilePath(strCultureName, "default");
            relativePath = strCultureName + "/default" + ".spk";
        }
        else {
            m_strAdaptationPath = GetAdaptationProfilePath(strCultureName, m_strDevice);
            relativePath = strCultureName + "/" + m_strDevice + ".spk";
        }

        VR_LOGI("relativePath : %s", relativePath.c_str());

        if (AdaptationProfileCanbeUsed(relativePath, m_strAdaptationPath)) {
            result = m_engineCommand->LoadSpeakerProfile(
                     &transaction,
                     m_strAdaptationPath.c_str());
            if (FAILED(result) || (NULL == transaction.ptr())) {
                VR_LOGI("error :SpeakAdaptation error");
            }
        }
        else {
            VR_LOGI("the training file is not be loaded");
        }
    }

    if (NULL == m_pcCatalogPhone) {
        // m_pcCatalogPhone = VR_new VR_VoiceBoxCatalogPhone(*m_client, *this);
        m_pcCatalogPhone = m_pcCatalogManager->GetCatalogPhone();
        VR_LOGI("Get m_pcCatalogPhone");
    }

    m_bEngineReady = true;

    return true;
}

// Stop the VoiceBox engine
void
VR_VoiceBoxEngine::Stop()
{
    VR_LOGD_FUNC();
    m_bEngineStarting = false;
    SendDEStatus("engine", "1");

    if (NULL == m_engineCommand) {
        VR_LOGE("The engine have not been initialized");
        return;
    }
    auto pairTransaction = GetCurrentTransaction();
    if (NULL != m_pcCatalogManager && !pairTransaction.first.empty() && !pairTransaction.second.empty()) {
        m_pcCatalogManager->CancelGrammarUpdate(pairTransaction.first, true);
    }

    if (VR_REGION_US == m_country) {
        CVECIPtr<IVECITransaction> saveTrans;
        HRESULT saveRet = m_engineCommand->SaveSpeakerProfile(
                          &saveTrans,
                          m_strAdaptationPath.c_str());
        if (FAILED(saveRet) || (NULL == saveTrans.ptr())) {
            VR_LOGE("Save speaker profile failed: %lx", saveRet);
        }
        else {
            VBT_BOOL bSaveTimeout = VBT_FALSE;
            saveRet = saveTrans->WaitForCompletion(INFINITE, &bSaveTimeout);
            if (FAILED(saveRet) || bSaveTimeout) {
                VR_LOGE("Save speaker profile waiting failed: %lx", saveRet);
            }
        }
    }

    // Stop the dialog engine immediately.
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->StopEngine(
                     &transaction,
                     VBT_TRUE         // Stop immediately
                     );
    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_LOGE("Stop Engine Failed, result: %lx, transaction: %p", result, transaction.ptr());
        return;
    }

    // Wait until the engine is stopped
    VBT_BOOL bTimeout = VBT_FALSE;
    result = transaction->WaitForCompletion(INFINITE, &bTimeout);
    if (FAILED(result) || bTimeout) {
        VR_LOGE("Waiting Engine Stop Failed, result: %lx, timeout: %n", result, bTimeout);
        return;
    }
}

// Uninitialize the dialog engine
void
VR_VoiceBoxEngine::UnInitialize()
{
    VR_LOGD_FUNC();

    // Set the external instance pointers to NULL
    m_pcExternalCallback = NULL;
    m_pcMsgController = NULL;
    m_pcPlayTransation = NULL;

    if (NULL == m_client) {
        return;
    }

    int nAgentNum = AgentType_Num;
    for (int i = 0; i < nAgentNum; ++i) {
        if (NULL == m_agents[i]) {
            continue;
        }

        m_agents[i]->UnInitialize();

        // Unregister event sink and shutdown the engine
        HRESULT ret = m_client->UnregisterEventSink(m_agents[i]);
        if (FAILED(ret)) {
            VR_LOGE("UnregisterEventSink Failed, ret: %lx", ret);
        }

        // This instance will be deleted in UnregisterEventSink(),
        // so we need not delete it.
        m_agents[i] = NULL;
    }

    HRESULT result = m_client->Shutdown();
    if (FAILED(result)) {
        VR_LOGE("Shutdown Failed, result: %lx", result);
    }

    m_mapMsgHandler.clear();
    m_mapMsgBtnHandler.clear();
    m_mapCmdResultHandler.clear();
    m_mapAgenttoRecoState.clear();
    m_mapHintsScreenID.clear();
    m_mapActiveSouceTrans.clear();
    m_lstUCAppQuitVROp.clear();
    m_lstNeedReactiveAction.clear();
    m_lstGrammarNewName.clear();
    m_mapUpdateStateHandler.clear();
    m_lstUpdateGrammar.clear();

    VR_AudioStreamIF* pcAudioStream = VR_AudioStreamIF::Instance();
    if (NULL != pcAudioStream) {
        pcAudioStream->UnInitialize();
    }
}

// Creates an Engine Client object and initializes it.
bool
VR_VoiceBoxEngine::CreateEngineClient()
{
    VR_LOGD_FUNC();

    // Creates an Engine Client object instance
    HRESULT result = CreateVBTEngineClient(&m_client);
    if (FAILED(result) || (NULL == m_client.ptr())) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "Create CreateVBTEngineClient Failed, result: %lx, client: %p", result, m_client.ptr());
        return false;
    }

    VARIANT value;
    value.vt = VT_BOOL;
    value.boolVal = VARIANT_TRUE;

    // Set property of PropAudioLevelThread
    result = m_client->SetProperty(PropAudioLevelThread, &value);
    if (FAILED(result)) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "SetProperty PropAudioLevelThread Failed, result: %lx", result);
        return false;
    }

    value.vt = VT_BOOL;
    value.boolVal = VARIANT_TRUE;
    result = m_client->SetProperty(PropClientManagedTTS, &value);
    if (FAILED(result)) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "SetProperty PropClientManagedTTS Failed, result: %lx", result);
        return false;
    }

    CVECIPtr<IVECIInitParams> initParams;
    result = CreateVECIInitParams(&initParams);
    if (FAILED(result) || (NULL == initParams.ptr())) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "CreateVECIInitParams Failed, result: %lx, initParams: %p", result, initParams.ptr());
        return false;
    }

    // Set the client name setting
    result = initParams->SetName(_T(""));
    if (FAILED(result)) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "Set Client Name Failed, result: %lx", result);
        return false;
    }

    // Set the VBT runtime component dir to empty string
    // for the default dir
    result = initParams->SetRuntimeDir(_T("/data/vr/config"));
    if (FAILED(result)) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "Set Runtime Dir Failed, result: %lx", result);
        return false;
    }

    // Set the VBT Engine config file
    VBT_CSTR szCfgFileName = _T("./VoiceBox.cfg");
    FILE * fd = fopen("/data/vr/vbtflg/on.flg", "r");
    if (NULL != fd) {
        fclose(fd);

        VR_LOGI("Use VoiceBox_On.cfg");
        szCfgFileName = "VoiceBox_On.cfg";
    }

    result = initParams->SetConfigFileName(szCfgFileName);
    if (FAILED(result)) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "SetConfigFileName Failed, result: %lx", result);
        return false;
    }

    result = initParams->SetFrontEndSharingFactoryParams(
        VBT_ULONG(&CreateVocon4FrontEndShare), 1);
    if (S_OK != result) {
        FAILED(result);
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "SetFrontEndSharingFactoryParams Failed, result: %lx", result);
        return false;
    }

    // Initializes the Engine Client object using an IVECIInitParams object.
    result = m_client->InitEx(initParams);
    if (FAILED(result)) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "Initialize engine Failed, result: %lx", result);
        return false;
    }

    return true;
}

void
VR_VoiceBoxEngine::RenameVbtLog()
{
    char buffer[256] = { 0 };
    time_t now;
    time(&now);
    struct tm* pTime = NULL;
    pTime = localtime(&now);
    if (NULL == pTime) {
        return;
    }
    (void)snprintf(buffer, sizeof(buffer), "%04d%02d%02d%02d%02d%02d",
        1900 + pTime->tm_year, 1 + pTime->tm_mon, pTime->tm_mday,
        pTime->tm_hour, pTime->tm_min, pTime->tm_sec);

    FILE * pVBTLog = fopen("/data/vr/vbtlog/Engine.vbtlog", "r");
    if (NULL != pVBTLog) {
        fclose(pVBTLog);

        char bufnewname[256] = { 0 };
        (void)snprintf(bufnewname, sizeof(bufnewname), "/data/vr/vbtlog/Engine_%s.vbtlog", buffer);
        (void)rename("/data/vr/vbtlog/Engine.vbtlog", bufnewname);
    }

    FILE *pVECILog = fopen("/data/vr/vbtlog/VECILog_1.vbtlog", "r");
    if (NULL != pVECILog) {
        fclose(pVECILog);

        char bufnewname[256] = { 0 };
        (void)snprintf(bufnewname, sizeof(bufnewname), "/data/vr/vbtlog/VECILog_1_%s.vbtlog", buffer);
        (void)rename("/data/vr/vbtlog/VECILog_1.vbtlog", bufnewname);
    }
}

// Creates Event Sink object instance and register it.
bool VR_VoiceBoxEngine::CreateEventSink()
{
    VR_LOGD_FUNC();

    // If the Engine Client object was NOT created,
    // we could not register the Event Sink object.
    if (NULL == m_client) {
        return false;
    }

        // Create Event Sink object instance
    m_agents[AgentType_Global] = VR_new VR_VoiceBoxAgentGlobal(*m_client, *m_engineCommand, *this);
    if (NULL == m_agents[AgentType_Global]) {
        VR_LOGE("Create VR_VoiceBoxEventSink Failed");
        return false;
    }

        // Create Event Sink object instance
    m_agents[AgentType_Climate] = VR_new VR_VoiceBoxAgentClimate(*m_client, *m_engineCommand, *this);
    if (NULL == m_agents[AgentType_Climate]) {
        VR_LOGE("Create VR_VoiceBoxEventSink Failed");
        return false;
    }

        // Create Event Sink object instance
    m_agents[AgentType_Info] = VR_new VR_VoiceBoxAgentInfo(*m_client, *m_engineCommand, *this);
    if (NULL == m_agents[AgentType_Info]) {
        VR_LOGE("Create VR_VoiceBoxEventSink Failed");
        return false;
    }

        // Create Event Sink object instance
    m_agents[AgentType_Navi] = VR_new VR_VoiceBoxAgentNavi(*m_client, *m_engineCommand, *this);
    if (NULL == m_agents[AgentType_Navi]) {
        VR_LOGE("Create VR_VoiceBoxEventSink Failed");
        return false;
    }

    // Create Event Sink object instance
    if (VR_REGION_US == m_country) {
        VR_LOGI("US VR_VoiceBoxAgentAudio&VR_VoiceBoxAgentPhone new");
        // Create Event Sink object instance
        m_agents[AgentType_Audio] = VR_new VR_VoiceBoxAgentAudio(*m_client, *m_engineCommand, *this);
        if (NULL == m_agents[AgentType_Audio]) {
            VR_LOGE("Create VR_VoiceBoxEventSink Failed");
            return false;
        }

        m_agents[AgentType_Phone] = VR_new VR_VoiceBoxAgentPhone(*m_client, *m_engineCommand, *this);
        if (NULL == m_agents[AgentType_Phone]) {
            VR_LOGE("Create VR_VoiceBoxEventSink Failed");
            return false;
        }

        // Create Event Sink object instance
        m_agents[AgentType_Apps] = VR_new VR_VoiceBoxAgentApps(*m_client, *m_engineCommand, *this);
        if (NULL == m_agents[AgentType_Apps]) {
            VR_LOGE("Create VR_VoiceBoxEventSink Failed");
            return false;
        }
    }
    else if (VR_REGION_OC == m_country) {
        VR_LOGI("OC VR_VoiceBoxAgentPhone_AU&VR_VoiceBoxAgentAudio_AU new");
        m_agents[AgentType_Phone] = VR_new VR_VoiceBoxAgentPhone_AU(*m_client, *m_engineCommand, *this);
        if (NULL == m_agents[AgentType_Phone]) {
            VR_LOGE("Create VR_VoiceBoxEventSink Failed");
            return false;
        }

        m_agents[AgentType_Audio] = VR_new VR_VoiceBoxAgentAudio_AU(*m_client, *m_engineCommand, *this);
        if (NULL == m_agents[AgentType_Audio]) {
            VR_LOGE("Create VR_VoiceBoxEventSink Failed");
            return false;
        }

        // Create Event Sink object instance
        m_agents[AgentType_Apps] = VR_new VR_VoiceBoxAgentApps_AU(*m_client, *m_engineCommand, *this);
        if (NULL == m_agents[AgentType_Apps]) {
            VR_LOGE("Create VR_VoiceBoxEventSink Failed");
            return false;
        }
    }
    else {
    }

    int nAgentNum = AgentType_Num;
    for (int i = 0; i < nAgentNum; ++i) {
        if (NULL == m_agents[i]) {
            continue;
        }

        if (!m_agents[i]->Initialize()) {
            VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, i, "Initialize failed");
            return false;
        }

        VBT_INT nMask = EventAgentResult;
        if (AgentType_Global == i) {
            nMask = EventAll;
        }
        // Register Event Sink object instance to the Engine Client object
        HRESULT result = m_client->RegisterEventSink(
                         nMask,
                         VBT_FALSE,
                         m_agents[i]
                         );
        if (FAILED(result)) {
            VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "RegisterEventSink Failed, result: %lx", result);
            return false;
        }
    }

    return true;
}

// Create an engine command dispatcher object.
bool VR_VoiceBoxEngine::CreateEngineCommand()
{
    // If the Engine Client object was NOT created,
    // we could not create an engine command dispatcher object.
    if (NULL == m_client) {
        return false;
    }

    // Create an engine command dispatcher instance.
    // This instance builds and sends SMURF messages to the VBT Engine
    HRESULT result = m_client->CreateEngineDispatcher(&m_engineCommand);
    if (FAILED(result) || (NULL == m_engineCommand.ptr())) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "Create CreateEngineDispatcher Failed, result: %lx, engine command: %p", result, m_engineCommand.ptr());
        return false;
    }

    return true;
}

bool VR_VoiceBoxEngine::CreateCatalogManager()
{
    VR_LOGD_FUNC();

    if (NULL == m_client) {
        return false;
    }

    m_pcCatalogManager = VR_new VR_VoiceBoxCatalogManager(*m_client, *m_engineCommand, *this);
    if (NULL == m_pcCatalogManager) {
        return false;
    }

    return m_pcCatalogManager->Initialize();
}

void
VR_VoiceBoxEngine::ProcessGrammarMSg(const std::string& message)
{
    VR_LOGI("Post Message to Catalog controller");

    if (std::string::npos != message.find("category")) {
        if ((VR_REGION_OC == m_country) && std::string::npos != message.find("audiosource")) {
            if (std::string::npos != message.find("Disc")) {
                m_bDiscAvailable = true;
                m_pcCatalogController->PostMessage("<grammar_audiosource_oc agent=\"media\" withdisc=\"true\"></grammar_audiosource_oc>");
            }
            else {
                m_bDiscAvailable = false;
                m_pcCatalogController->PostMessage("<grammar_audiosource_oc agent=\"media\" withdisc=\"false\"></grammar_audiosource_oc>");
            }
            return;
        }

        m_pcCatalogController->PostMessage(message);
    }
    else if (std::string::npos != message.find("requestGrammar")) {
        VR_VoiceBoxXmlParser parser(message);
        std::string strAgent = parser.getValueByKey("agent");
        std::string strGrammarID = parser.getValueByKey("grammarID");
        if (NULL != m_pcCatalogManager && !m_pcCatalogManager->UpdateGrammarFinishedStatus(strAgent, strGrammarID)) {
            VR_LOGI("UpdateGrammarFinishedStatus failed");
        }
    }
    else {
        VR_VoiceBoxXmlParser parser(message);
        std::string strXmlKey = parser.getXmlKey();
        std::string strAgent = parser.getValueByKey("agent");
        if ("phone" == strAgent) {
            if (("grammar_init" == strXmlKey) || ("grammar_active" == strXmlKey)) {
                m_strPath = parser.getValueByKey("path");
                SendDEStatus("voicetag", "1");
            }
            if ("grammar_disactive" == strXmlKey) {
                SendDEStatus("voicetag", "1");
            }
        }
        m_pcCatalogController->PostMessage(message);
    }
}

// Process the messages that came from the external service
void
VR_VoiceBoxEngine::ProcessMessage(const std::string& message, int actionSeqId)
{
    VR_LOGD_FUNC();

    m_iCurrActionId = actionSeqId;

    if (HandleQuitVRApp(message)) {
        return;
    }

    if (message.empty()) {
        return;
    }

    VR_LOGI("msg : %s ; id : %i", message.c_str(), actionSeqId);

    if ((m_iToturialBeepId == actionSeqId) && (m_iToturialBeepId != VR_INVALID_ACTION_ID)) {
        m_iToturialBeepId = VR_INVALID_ACTION_ID;
        OnRecognized(m_strToturialResult);
        m_strToturialResult = "";
    }
    if ((m_iVoiceTrainingBeepId == actionSeqId) && (m_iVoiceTrainingBeepId != VR_INVALID_ACTION_ID)) {
        m_iVoiceTrainingBeepId = VR_INVALID_ACTION_ID;
        OnRecognized(m_strVoiceTrainingResult);
        m_strVoiceTrainingResult = "";
    }

    // Check the header contents
    const int HEADER_LEN = 150;
    std::string header = message.substr(0, HEADER_LEN);

    if (std::string::npos != header.find("queryVehicleinMotion")) {
        VR_LOGI("queryVehicleinMotion");
        HandleVehicleDriving(message);
        return;
    }
    if (std::string::npos != header.find("getMessageDetail")) {
        if (VBT_RECO_STATE_INCOMING_MESSAGE == m_strCurrentStateName) {
            VR_LOGI("do not solve messagebody");
            return;
        }
    }
    if (std::string::npos != header.find("getMsgDetailStatusNoChange")) {
        if (m_isIncomingMessage) {
            HandleIncomingMessage(message);
            return;
        }
    }
    if (std::string::npos != header.find("queryHintFirstPage")) {
        VR_LOGI("queryHintFirstPage");
        QueryHintFirstPage();
        return;
    }
    if (std::string::npos != header.find("queryHintLastPage")) {
        VR_LOGI("queryHintLastPage");
        QueryHintLastPage();
        return;
    }
    if (std::string::npos != header.find("smartagent")) {
        SmartAgent(message);
        return;
    }

    if (std::string::npos != header.find("<action ")) {
        HandleAction(message);
        return;
    }

    if (std::string::npos != header.find("queryRouteExist")) {
        SetRoute(message);
        return;
    }

    if (std::string::npos != header.find("RouteStatusNotify")) {
        NotifyRouteStatus(message);
        return;
    }

    if ((std::string::npos == header.find("grammar_new") && std::string::npos != header.find("<category"))
        || (std::string::npos != header.find("requestGrammar") && std::string::npos != header.find("action-result"))) {
        ProcessGrammarMSg(message);
        return;
    }

    // If the header contains "grammar", it is for catalog
    if (std::string::npos != header.find("grammar")) {
        // if (std::string::npos != header.find("grammar_diff")) {
        //     GrammarDiffDetail(message);
        // }
        if (std::string::npos != header.find("grammar_new")) {
            GrammarNewDetail(message);
        }
        else {
            if (std::string::npos != header.find("<grammar_active agent=\"media\"")) {
                VR_VoiceBoxXmlParser parser(message);
                m_strDMActiveSourceId = parser.getValueByKey("grammarid");
                if (m_strDMActiveSourceId == m_strGrammarInitSourceId) {
                    if (NULL != m_pcCatalogManager) {
                        m_pcCatalogAudio = m_pcCatalogManager->GetCatalogAudio();
                        if (NULL != m_pcCatalogAudio) {
                            m_pcCatalogAudio->SetDataActiveSource(m_strGrammarInitSourceId);
                        }
                    }
                }
            }
            else if (std::string::npos != header.find("<grammar_disactive agent=\"media\"")) {
                m_strDMActiveSourceId = "5";
            }
            else {

            }

            ProcessGrammarMSg(message);
            return;
        }
    }
    if (std::string::npos != header.find("VoiceTag")) {
        HandleVoiceTag(message);
        return;
    }

    if (ProcessTslMessage(message, actionSeqId)) {
        return;
    }

    if (std::string::npos != header.find("getSupportedLanguage")) {
        std::string strSupportedLanguage;
        if (VR_REGION_US == m_country) {
            strSupportedLanguage = "<event-result name=\"getSupportedLanguage\" errcode=\"0\">"
                                      "<TSLINFO>"
                                        "<Action type=\"Get Supported Language\"/>"
                                        "<LanguageList>"
                                          "<Item>"
                                            "<Parameter name=\"Language\" value=\"en\"/>"
                                          "</Item>"
                                          "<Item>"
                                            "<Parameter name=\"Language\" value=\"fr\"/>"
                                          "</Item>"
                                          "<Item>"
                                            "<Parameter name=\"Language\" value=\"es\"/>"
                                          "</Item>"
                                        "</LanguageList>"
                                      "</TSLINFO>"
                                    "</event-result>";
        }
        else if (VR_REGION_OC == m_country) {
            strSupportedLanguage = "<event-result name=\"getSupportedLanguage\" errcode=\"0\">"
                                      "<TSLINFO>"
                                        "<Action type=\"Get Supported Language\"/>"
                                        "<LanguageList>"
                                          "<Item>"
                                            "<Parameter name=\"Language\" value=\"en\"/>"
                                          "</Item>"
                                        "</LanguageList>"
                                      "</TSLINFO>"
                                    "</event-result>";
        }
        else {
            return;
        }

        OnRecognized(strSupportedLanguage);
        return;
    }

    VR_VoiceBoxXmlParser parser(message);
    std::string strActionKey = parser.getXmlKey();

    VR_LOGI("strActionKey : %s", strActionKey.c_str());
    // Dispatch the messages
    VoiceMap<std::string, MessageHandler>::const_iterator iterMap = m_mapMsgHandler.find(strActionKey);
    if (m_mapMsgHandler.cend() != iterMap) {
        if (NULL != iterMap->second) {
            (this->*(iterMap->second))(parser);
        }
    }
    else {
        std::string strAgent = parser.getValueByKey("agent");
        VR_LOGI("ProcessMessage: %s", strAgent.c_str());
        if ("phone" == strAgent) {
            std::string op = parser.getValueByKey("op");
            VR_LOGI("op: %s", op.c_str());
            if ("MAPProtocol" == op) {
                MessageAvaiableState(message);
                return;
            }
            else if ("getMessageState" == op) {
                SendMessageAvaiableState(message);
                return;
            }
            else {
                m_agents[AgentType_Phone]->ReplyQueryInfo(parser);
            }
        }
        else if ("climate" == strAgent) {
            m_agents[AgentType_Climate]->ReplyQueryInfo(parser);
        }
        else if ("apps" == strAgent) {
            m_agents[AgentType_Apps]->ReplyQueryInfo(parser);
        }
        else if ("media" == strAgent) {
            m_agents[AgentType_Audio]->ReplyQueryInfo(parser);
        }
        else if ("navi" == strAgent) {
            m_agents[AgentType_Navi]->ReplyQueryInfo(parser);
        }
        else if ("help" == strAgent) {
            m_agents[AgentType_Global]->ReplyQueryInfo(parser);
        }
        else {

        }
    }
}

bool
VR_VoiceBoxEngine::ProcessTslMessage(const std::string& message, int actionSeqId)
{
    // Check the header contents
    const int HEADER_LEN = 150;
    std::string header = message.substr(0, HEADER_LEN);

    if (std::string::npos != header.find("Install Agent Start")) {
        return true;
    }

    if (std::string::npos != header.find("Install Agent End")) {
        return true;
    }

    if (std::string::npos != header.find("Install Agent")) {
        // Cancel the curent session
        CancelRecoSession();
        InstallDownloadableAgents(message);
        return true;
    }

    if (std::string::npos != header.find("TSL_Token")) {
        VR_LOGI("Set TSL Token");
        m_agents[AgentType_Apps]->ProcessMessage(message);
        std::string strEventName = "";
        if (m_bAvcLan) {
            strEventName = "SendAvcLanAppXmlMsg";
        }
        else {
            strEventName = "SendAppXmlMessage";
        }
        std::string eventResult = "<event-result name=\"" + strEventName + "\" errcode=\"0\"/>";
        OnRecognized(eventResult);
        return true;
    }

    if (std::string::npos != header.find("TSLINFO")) {
        // Cancel the curent session
        CancelRecoSession();
        m_agents[AgentType_Apps]->ProcessMessage(message);
        return true;
    }

    if (!m_bAvcLan && std::string::npos != header.find("StartAppRecognition")) {
        VR_LOGI("DCU Start App Recognition");
        switch (m_bAppRecoState) {
        case VR_AppRecoState_Valid:
            {
                StartAppRecognition(message);
                std::string eventResult = "<event-result name=\"StartAppRecognition\" errcode=\"0\"/>";
                OnRecognized(eventResult);
            }
            break;
        case VR_AppRecoState_Sending:
        case VR_AppRecoState_SendCaching:
            {
                VR_MsgInfo msgInfo;
                msgInfo.iMsgId = actionSeqId;
                msgInfo.strMsg = message;
                m_listAppMessages.push_back(msgInfo);
            }
            break;
        case VR_AppRecoState_Invalid:
            {
                std::string eventResult = "<event-result name=\"StartAppRecognition\" errcode=\"1\"/>";
                OnRecognized(eventResult);
            }
            break;
        default:
            break;
        }

        return true;
    }

    if (std::string::npos != header.find("CancelAppRecognition")) {

        if (!m_bHandleTTSCrush) {
            VR_VoiceBoxXmlParser parser(message);
            std::string strOption = parser.getValueByKey("option");
            if ("ttscrash" == strOption) {
                VR_LOGI("when running tsl, the app of TTS crush!");
                m_bHandleTTSCrush = true;
            }
        }

        switch (m_bAppRecoState) {
        case VR_AppRecoState_Valid:
        case VR_AppRecoState_SendCaching:
            {
                if (!CancelAppRecognition(message)) {

                    std::string eventResult = "<event-result name=\"CancelAppRecognition\" ";
                    if (m_bHandleTTSCrush) {
                        VR_LOGI("the cancel event of ttscrash is handled over");
                        m_bHandleTTSCrush = false;
                        eventResult.append("option=\"ttscrash\" ");
                    }
                    eventResult.append("errcode=\"1\" />");
                    OnRecognized(eventResult);

                    if (m_bUpdateState) {
                        VR_LOGI("log on/off -> stop UnInitialize initialize start");
                        SendCancelResult();
                    }

                    if (!m_lstCancelOption.empty()) {
                        VR_LOGI("clear cancel result");
                        SendCancelResult();
                        NotifyResourceState();
                        HandleQuitVR();
                    }
                }
            }
            break;
        case VR_AppRecoState_Sending:
            {
                VR_LOGI("Cancel during send APP recognition state");
                CVECIPtr<IVECITransaction> transaction;
                HRESULT result = m_engineCommand->CancelSession(&transaction);
                if (FAILED(result) || (NULL == transaction.ptr())) {
                    VR_LOGE("Cancel failed: 0x%lx", result);
                }

                VR_MsgInfo msgInfo;
                msgInfo.iMsgId = actionSeqId;
                msgInfo.strMsg = message;
                m_listAppMessages.push_back(msgInfo);
            }
            break;
        case VR_AppRecoState_Invalid:
            {
                std::string eventResult = "<event-result name=\"CancelAppRecognition\" ";
                if (m_bHandleTTSCrush) {
                    VR_LOGI("the cancel event of ttscrash is handled over");
                    m_bHandleTTSCrush = false;
                    eventResult.append("option=\"ttscrash\" ");
                }
                eventResult.append("errcode=\"1\" />");
                OnRecognized(eventResult);

                if (m_bUpdateState) {
                    VR_LOGI("log on/off -> stop UnInitialize initialize start");
                    SendCancelResult();
                }

                if (!m_lstCancelOption.empty()) {
                    VR_LOGI("clear cancel result");
                    SendCancelResult();
                    NotifyResourceState();
                    HandleQuitVR();
                }

                HandleGrammarQuitVR();
            }
            break;
        default:
            break;
        }

        return true;
    }

    if (m_bAvcLan) {
        if (std::string::npos != header.find("SendAvcLanAppXmlMsg")) {
            SendAvcLanAppXmlMsg(message, actionSeqId);
            std::string eventResult = "<event-result name=\"SendAvcLanAppXmlMsg\" errcode=\"0\"/>";
            OnRecognized(eventResult);
            return true;
        }
    }
    else {
        if (std::string::npos != header.find("SendAppXmlMessage")) {
            SendAppXmlMessage(message);
            std::string eventResult = "<event-result name=\"SendAppXmlMessage\" errcode=\"0\"/>";
            OnRecognized(eventResult);
            return true;
        }
    }

    if (std::string::npos != header.find("updateAppList")) {
        SendAppXmlMessage(message);

        // Update Apps Grammar
        std::string reload = "<event name=\"SendAppXmlMessage\">"
            "<CSVR>"
                "<Message source=\"Client\" class=\"System\">"
                    "<Parameter name=\"Agent\" value=\"Apps\"/>"
                    "<Action type=\"VBT Agent Grammar Update\">"
                        "<Parameter name=\"Table Name\" value=\"AppsAgentApps\"/>"
                        "<Parameter name=\"Operation\" value=\"Reload\"/>"
                    "</Action>"
                "</Message>"
            "</CSVR>"
        "</event>";
        SendAppXmlMessage(reload);

        std::string eventResult = "<event-result name=\"updateAppList\" errcode=\"0\"/>";
        OnRecognized(eventResult);

        // get appname for hints display when OC
        if (VR_REGION_OC == m_country) {
            VR_VoiceBoxAppsXml appsXml;
            if (!appsXml.GetAppNameList(message, m_vecAppName)) {
                VR_LOGI("cannot get apps name.");
            }
        }
        return true;
    }

    return false;
}

std::string
VR_VoiceBoxEngine::GetPhonBookDBPath()
{
    VR_LOGD_FUNC();

    return m_strPath;
}

bool
VR_VoiceBoxEngine::GetSendMessageAvailableState()
{
    VR_LOGD_FUNC();

    return m_bSendMsgAvailable;
}

bool VR_VoiceBoxEngine::GetRouteStatus()
{
    VR_LOGD_FUNC();
    return m_bRouteStatus;
}

void
VR_VoiceBoxEngine::HandleVoiceTag(const std::string& message)
{
    VR_LOGD_FUNC();

    m_bVoiceTagTtsCrash = false;

    pugi::xml_document doc;
    doc.load_string(message.c_str());
    pugi::xml_node eventNode = doc.select_node("//event").node();
    std::string eventName;
    if (eventNode) {
        eventName = eventNode.attribute("name").as_string();
    }
    VR_LOGI("eventName: %s", eventName.c_str());
    if ("addRecordVoiceTag" == eventName) {
        m_bDoCancelVoiceTag = false;
        m_enVoiceTagRecState = VR_VoiceTag_Recording;
        m_strVoiceTagMessage = message;
        RecordVoiceTagEvent(eventNode, false);
        return;
    }
    else if ("editRecordVoiceTag" == eventName) {
        m_bDoCancelVoiceTag = false;
        m_enVoiceTagRecState = VR_VoiceTag_Recording;
        m_strVoiceTagMessage = message;
        RecordVoiceTagEvent(eventNode, true);
        return;
    }
    else if ("saveVoiceTag" == eventName) {
        SaveVoiceTagEvent(eventNode);
        return;
    }
    else if ("deleteVoiceTag" == eventName) {
        DeleteVoiceTagEvent(eventNode);
        return;
    }
    else if ("playVoiceTag" == eventName) {
        m_strVoiceTagMessage = message;
        PlayVoiceTagEvent(eventNode);
        return;
    }
    else if ("stopVoiceTag" == eventName) {
        m_strVoiceTagMessage = message;
        StopVoiceTagEvent(eventNode);
        return;
    }
    else if ("cancelRecordVoiceTag" == eventName) {
        CancelRecordVoiceTag(eventNode);
    }
    else if ("cancelSaveVoiceTag" == eventName) {
        CancelSaveVoiceTag(eventNode);
    }
    else if ("CancelVoiceTag" == eventName) {
        CancelVoiceTag(eventNode);
    }
    else if (0 == strcmp(eventName.c_str(), "syncVoiceTag")) {
        SyncVoiceTagEvent(eventNode);
    }
    else {
    }
    return;
}

void
VR_VoiceBoxEngine::SmartAgent(const std::string& message)
{
    VR_LOGD_FUNC();

    pugi::xml_document doc;
    doc.load_string(message.c_str());
    pugi::xml_node eventNode = doc.select_node("//event").node();
    if (eventNode) {
        HandleSmartAgent(eventNode);
        return;
    }

}

void
VR_VoiceBoxEngine::HandleSmartAgent(pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();

    pugi::xml_node paramNode = eventNode.child("param");
    if (!paramNode) {
        VR_LOGI("param is null");
        return;
    }
    std::string paramName = paramNode.attribute("name").as_string();
    std::string userId = paramNode.attribute("userid").as_string();
    VR_LOGI("paramName: %s", paramName.c_str());
    VR_LOGI("userId: %s", userId.c_str());

    if ("adduser" == paramName) {
        m_userId = userId;
    }
    else if ("deleteuser" == paramName) {
        DeleteSmartAgent(userId);
        m_userId = "";
    }
    else if ("changeuser" == paramName) {
        if ("0" == userId) {
            m_userId = "";
            SetSpeakerProfile("default");
        }
        else {
            m_userId = userId;
            HandlePromptLevel(userId);
            SetSpeakerProfile(userId);
        }
    }
    else {
    }

    std::string strEventResult = "<event-result name=\"smartagent\">";
    strEventResult.append("<param name=\"");
    strEventResult.append(paramName);
    strEventResult.append("\" userid=\"");
    strEventResult.append(userId);
    strEventResult.append("\"/>");
    strEventResult.append("</event-result>");
    OnRecognized(strEventResult);
}

void
VR_VoiceBoxEngine::DeleteSmartAgent(const std::string& userId)
{
    VR_LOGD_FUNC();

    VR_VoiceBoxDataStorage storage;
    std::string promptLevelValueStr;

    storage.GetValue(promptLevelValueStr);
    pugi::xml_document promptLevelValueDoc;
    promptLevelValueDoc.load_string(promptLevelValueStr.c_str());

    pugi::xml_node promptLevelalueNode = promptLevelValueDoc.select_node((std::string("//") + PROMPTLEVEL + userId).c_str()).node();

    if (promptLevelalueNode) {
        std::string nodeName = promptLevelalueNode.name();
        VR_LOGI("nodeName: %s", nodeName.c_str());
        promptLevelValueDoc.remove_child(nodeName.c_str());
        std::ostringstream oss;
        promptLevelValueDoc.print(oss);
        promptLevelValueStr = oss.str();

        storage.PutValue(promptLevelValueStr);

        promptLevelValueStr.clear();
        storage.GetValue(promptLevelValueStr);
        VR_LOGI("prompt: %s", promptLevelValueStr.c_str());
    }
    else {
        VR_LOGI("there is no information for this id in db");
    }
}

void
VR_VoiceBoxEngine::DeleteALLSmartAgent()
{
    VR_LOGD_FUNC();

    VR_VoiceBoxDataStorage storage;
    storage.DeleteValue();
}

void
VR_VoiceBoxEngine::HandlePromptLevel(const std::string& userId)
{
    VR_LOGD_FUNC();

    VR_VoiceBoxDataStorage storage;
    std::string promptLevelValueStr;

    storage.GetValue(promptLevelValueStr);
    pugi::xml_document promptLevelValueDoc;
    promptLevelValueDoc.load_string(promptLevelValueStr.c_str());

    pugi::xml_node promptLevelalueNode = promptLevelValueDoc.select_node((std::string("//") + PROMPTLEVEL + userId).c_str()).node();

    std::string nodeName = promptLevelalueNode.name();
    VR_LOGI("nodeName: %s", nodeName.c_str());
    std::string level;
    if (promptLevelalueNode) {
        level = promptLevelalueNode.child("level").text().as_string();
        std::string promptLevel;
        VR_LOGI("level: %s", level.c_str());
        if ("0" == level) {
            promptLevel = "off";
        }
        else if ("2" == level) {
            promptLevel = "low";
        }
        else if ("3" == level) {
            promptLevel = "high";
        }
        else {
        }
        std::string strActionResult = "<action agent=\"smartagent\" op=\"setPromptLevel\" >"
                                      "<level>";
        strActionResult.append(promptLevel);
        strActionResult.append("</level></action>");
        OnRequestAction(strActionResult);
    }
    else {
        SavePromptLevel(userId, promptLevelValueDoc);
    }

}

void
VR_VoiceBoxEngine::SavePromptLevel(const std::string& strKeyValue)
{
    VR_LOGD_FUNC();

    VR_LOGI("strKeyValue: %s", strKeyValue.c_str());
    int level = VR_PROMPTLEVEL_HIGH;
    if ("" != m_userId) {
        if ("OFF" == strKeyValue) {
            level = VR_PROMPTLEVEL_OFF;
        }
        else if ("SIMPLE" == strKeyValue) {
            level = VR_PROMPTLEVEL_LOW;
        }
        else if ("FULL" == strKeyValue) {
            level = VR_PROMPTLEVEL_HIGH;
        }
        std::string promptLevel;
        std::stringstream ss;
        ss << level;
        ss >> promptLevel;

        VR_VoiceBoxDataStorage storage;
        std::string promptLevelValueStr;
        storage.GetValue(promptLevelValueStr);
        pugi::xml_document promptLevelValueDoc;
        promptLevelValueDoc.load_string(promptLevelValueStr.c_str());
        promptLevelValueDoc.remove_child((std::string(PROMPTLEVEL) + m_userId).c_str());

        pugi::xml_document levelValueDoc;
        levelValueDoc.load_string("");
        pugi::xml_node levelValueNode = levelValueDoc.append_child((std::string(PROMPTLEVEL) + m_userId).c_str());
        levelValueNode.append_child("level").text().set(promptLevel.c_str());
        std::ostringstream osss;
        levelValueNode.print(osss);
        VR_LOGI("levelValueNode Value: %s", osss.str().c_str());
        promptLevelValueDoc.append_copy(levelValueNode);

        promptLevelValueStr.clear();
        std::ostringstream oss;
        promptLevelValueDoc.print(oss);
        promptLevelValueStr = oss.str();
        VR_LOGI("promptLevelValueStr Value: %s", promptLevelValueStr.c_str());

        storage.PutValue(promptLevelValueStr);
    }

}

void
VR_VoiceBoxEngine::SavePromptLevel(const std::string& userId, pugi::xml_document& promptLevelValueDoc)
{
    VR_LOGD_FUNC();

    pugi::xml_document levelValueDoc;
    levelValueDoc.load_string("");
    pugi::xml_node levelValueNode = levelValueDoc.append_child((std::string(PROMPTLEVEL) + userId).c_str());

    std::string promptLevel;
    std::stringstream ss;
    ss << m_iPromptLevel;
    ss >> promptLevel;
    VR_LOGI("promptLevel: %s", promptLevel.c_str());
    levelValueNode.append_child("level").text().set(promptLevel.c_str());
    std::ostringstream osss;
    levelValueNode.print(osss);
    VR_LOGI("levelValueNode Value: %s", osss.str().c_str());

    VR_VoiceBoxDataStorage storage;
    std::string promptLevelValueStr;

    promptLevelValueDoc.append_copy(levelValueNode);

    // promptLevelValueStr.clear();
    std::ostringstream oss;
    promptLevelValueDoc.print(oss);
    promptLevelValueStr = oss.str();
    VR_LOGI("promptLevelValueStr Value: %s", promptLevelValueStr.c_str());

    storage.PutValue(promptLevelValueStr);

}

void
VR_VoiceBoxEngine::QueryHintFirstPage()
{
    VR_LOGD_FUNC();

    std::string strValue = "false";
    if (m_iIndex <= 5) {
        strValue = "true";
    }

    std::string strActionResult = "<action-result agent=\"help\" op=\"queryHintFirstPage\" >"
                                      "<value>";
    strActionResult.append(strValue);
    strActionResult.append("</value></action-result>");
    VR_VoiceBoxXmlParser parserTmp(strActionResult);
    m_agents[AgentType_Global]->ReplyQueryInfo(parserTmp);
}

void
VR_VoiceBoxEngine::QueryHintLastPage()
{
    VR_LOGD_FUNC();

    std::string strValue = "false";
    if (m_iHintSize == m_iIndex) {
        strValue = "true";
    }
    std::string strActionResult = "<action-result agent=\"help\" op=\"queryHintLastPage\" >"
                                      "<value>";
    strActionResult.append(strValue);
    strActionResult.append("</value></action-result>");
    VR_VoiceBoxXmlParser parserTmp(strActionResult);
    m_agents[AgentType_Global]->ReplyQueryInfo(parserTmp);
}

void
VR_VoiceBoxEngine::HandleVehicleDriving(const std::string& strMessage)
{
    VR_LOGD_FUNC();

    VR_VoiceBoxXmlParser parser(strMessage);
    std::string strAgent = parser.getValueByKey("agent");
    std::string strMotion = parser.getValueByKey("motion");
    VR_LOGI("HandleVehicleDriving: %s", strAgent.c_str());
    VR_VoiceBoxEventSink* agentTmp = NULL;
    if ("phone" == strAgent) {
        agentTmp = m_agents[AgentType_Phone];
    }
    else if ("climate" == strAgent) {
        agentTmp = m_agents[AgentType_Climate];
    }
    else if ("apps" == strAgent) {
        agentTmp = m_agents[AgentType_Apps];
    }
    else if ("media" == strAgent) {
        agentTmp = m_agents[AgentType_Audio];
    }
    else if ("navi" == strAgent) {
        agentTmp = m_agents[AgentType_Navi];
    }
    else if ("help" == strAgent) {
        agentTmp = m_agents[AgentType_Global];
    }
    else {
        return;
    }

    std::string strVehicleDriving = "<action-result agent=\"";
    strVehicleDriving.append(strAgent);
    strVehicleDriving.append("\" op=\"queryVehicleinMotion\"><motion>");
    strVehicleDriving.append(strMotion);
    strVehicleDriving.append("</motion></action-result>");
    VR_LOGI("strVehicleDriving : %s", strVehicleDriving.c_str());
    VR_VoiceBoxXmlParser parserTmp(strVehicleDriving);

    agentTmp->ReplyQueryInfo(parserTmp);
}

void
VR_VoiceBoxEngine::SetRoute(const std::string& strMessage)
{
    VR_LOGD_FUNC();

    VR_VoiceBoxXmlParser parser(strMessage);
    std::string strExist = parser.getValueByKey("exist");
    SetVBTPrefRouteActive(strExist);

}

void
VR_VoiceBoxEngine::NotifyRouteStatus(const std::string& strMessage)
{
    VR_LOGD_FUNC();

    VR_VoiceBoxXmlParser parser(strMessage);
    std::string strStatus = parser.getValueByKey("status");
    SetVBTPrefRouteActive(strStatus);
}

void
VR_VoiceBoxEngine::SetVBTPrefRouteActive(const std::string& strStatus)
{
    std::string strExist;
    if ("true" == strStatus) {
        VR_LOGI("set route true");
        strExist = "True";
        m_bRouteStatus = true;
    }
    else {
        VR_LOGI("set route false");
        strExist = "False";
        m_bRouteStatus = false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SetPreference(
                     &transaction,
                     VBT_AGENT_SYSTEM,
                     VBT_USR_PREF_SYSTEM_ROUTEACTIVE,
                     strExist.c_str());
    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_LOGI("set route error");
    }
}

void
VR_VoiceBoxEngine::PlayVoiceTagEvent(pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();

    VR_LOGI("m_stopVoiceTagBeep %d", m_stopVoiceTagBeep);

    m_stopVoiceTagBeep = VR_INVALID_ACTION_ID;

    std::string voiceTagID(eventNode.child("voiceTagId").text().as_string());
    std::string voiceTagPCMPath;
    if (VR_ConfigureIF::Instance()->isFileExist(VOICETAGFILE)) {
        voiceTagPCMPath.assign(VR_ConfigureIF::Instance()->getUsrPath() + VOICETAGFILE);
    }
    else {
        VR_VoiceBoxVoiceTag voiceTag;
        voiceTagPCMPath = voiceTag.GetVoiceTagPCMPath(voiceTagID, m_deviceAddress);
    }

    if (voiceTagPCMPath.empty()) {
        eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_FAILURE);
        eventNode.set_name("event-result");
        m_iOss.str("");
        eventNode.print(m_iOss);
        OnRecognized(m_iOss.str());
        HandleQuitVoiceTag();
    }
    else {
        OnBeep(voiceTagPCMPath);
        eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_OK);
    }
    eventNode.set_name("event-result");
    m_iOss.str("");
    eventNode.print(m_iOss);
    VR_LOGI("m_iOss: %s", m_iOss.str().c_str());
}

void
VR_VoiceBoxEngine::StopVoiceTagEvent(pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();

    eventNode.set_name("event-result");
    eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_OK);

    std::string strPlayvoiceTagResult = m_iOss.str();
    m_iOss.str("");
    eventNode.print(m_iOss);
    VR_LOGI("m_iOss: %s", m_iOss.str().c_str());
    if ((VR_INVALID_ACTION_ID == m_iPlayVoiceTagBeep) || (VR_INVALID_ACTION_ID != m_stopVoiceTagBeep)) {
        VR_LOGI("m_stopVoiceTagBeep : %d", m_stopVoiceTagBeep);
        VR_LOGI("voicetag has played over or voicetag has been stop");
        OnRequestAction(m_iOss.str());
    }
    else {
        pugi::xml_document doc;
        if (doc.load_string(strPlayvoiceTagResult.c_str())) {
            pugi::xml_node eventNode = doc.select_node("//event-result").node();
            std::string strActionId = eventNode.child("actionId").text().as_string();
            std::string strVoiceTagId = eventNode.child("voiceTagId").text().as_string();
            strPlayvoiceTagResult = "<event-result name=\"playVoiceTag\" errcode=\"4\"><actionId>";
            strPlayvoiceTagResult.append(strActionId);
            strPlayvoiceTagResult.append("</actionId><voiceTagId>");
            strPlayvoiceTagResult.append(strVoiceTagId);
            strPlayvoiceTagResult.append("</voiceTagId></event-result>");
            OnRequestAction(strPlayvoiceTagResult);
        }
        else {
            VR_LOGI("load message failed");
        }

        m_stopVoiceTagBeep = OnRequestAction(VR_ACTION_STOPBEEP);
    }

}

void
VR_VoiceBoxEngine::CancelRecordVoiceTag(pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();

    if (m_bDoCancelVoiceTag) {
        VR_LOGI("record has been canceled");
        eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_OK);
        eventNode.set_name("event-result");
        std::stringstream oss;
        eventNode.print(oss);
        VR_LOGI("oss: %s", oss.str().c_str());
        OnRecognized(oss.str());
        return;
    }
    m_bDoCancelVoiceTag = true;
    bool removeOK = false;
    if (VR_ConfigureIF::Instance()->isFileExist(VOICETAGFILE)) {
        removeOK = VR_ConfigureIF::Instance()->removeFile(VOICETAGFILE);
        VR_LOGI("removeOK :%d", removeOK);
    }
    else {
        removeOK = true;
    }
    if (removeOK) {
        eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_OK);
    }
    else {
        eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_FAILURE);
    }
    eventNode.set_name("event-result");

    std::stringstream oss;
    eventNode.print(oss);
    VR_LOGI("oss: %s", oss.str().c_str());
    if ((VR_VoiceTag_FinishRec != m_enVoiceTagRecState) && (VR_VoiceTag_NoRec != m_enVoiceTagRecState)) {
        std::string strReult;
        // reply dm record failed
        m_iOss.str("");
        ReplyDMVoiceTagResult(strReult);
        OnRequestAction(strReult);
        CancelVoiceTagRecoSession();
        if (m_bPlayTTS) {
           HandleVoiceTagTTs();
        }
    }
    else {
        VR_LOGI("record has been succeeded");
    }
    // reply dm cancelvicetag result
    OnRequestAction(oss.str());

}

void
VR_VoiceBoxEngine::HandleVoiceTagTTs()
{
    VR_LOGD_FUNC();

    std::string strStopTTS;
    if (VR_VoiceTag_RecordSucceed == m_enVoiceTagRecState) {
        strStopTTS = "<action agent=\"prompt\" op=\"stopTtsList\"><reqId>";
    }
    else {
        strStopTTS = "<action agent=\"prompt\" op=\"stopTts\"><reqId>";
    }
    strStopTTS.append(std::to_string(m_iCurTTSReqId));
    strStopTTS.append("</reqId></action>");
    m_stopVoiceTagTTs = OnRequestAction(strStopTTS);
    VR_LOGI("stop current speak id, %d", m_iCurTTSReqId);
    m_mapStopTTSTransation.insert(std::make_pair(m_stopVoiceTagTTs, m_iCurTTSReqId));
}

void
VR_VoiceBoxEngine::CancelSaveVoiceTag(pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();

    bool removeOK = false;
    if (VR_ConfigureIF::Instance()->isFileExist(VOICETAGFILE)) {
        removeOK = VR_ConfigureIF::Instance()->removeFile(VOICETAGFILE);
        VR_LOGI("removeOK :%d", removeOK);
    }
    else {
        removeOK = true;
    }
    if (removeOK) {
        eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_OK);
    }
    else {
        eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_FAILURE);
    }
    eventNode.set_name("event-result");

    std::stringstream oss;
    eventNode.print(oss);
    VR_LOGI("oss: %s", oss.str().c_str());
    OnRequestAction(oss.str());
}

void VR_VoiceBoxEngine::CancelVoiceTag(pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();

    std::string strOption = eventNode.attribute("option").as_string();
    if ("ttscrash" == strOption) {
        HandleVoiceTagTtsCrashMessage();
        eventNode.set_name("event-result");
        std::ostringstream oss;
        eventNode.print(oss);
        VR_LOGI("oss: %s", oss.str().c_str());
        OnRequestAction(oss.str());
    }
    else {
        if (VR_INVALID_ACTION_ID != m_iPlayVoiceTagBeep) {
            if (VR_INVALID_ACTION_ID != m_stopVoiceTagBeep) {
                eventNode.set_name("event-result");
                std::ostringstream oss;
                eventNode.print(oss);
                VR_LOGI("oss: %s", oss.str().c_str());
                OnRequestAction(oss.str());
            }
            else {
                m_iOss.str("");
                std::string strResult;
                ReplyDMVoiceTagResult(strResult);
                OnRequestAction(strResult);
                VR_LOGI("need to stop play voicetag");
                m_stopVoiceTagBeep = OnRequestAction(VR_ACTION_STOPBEEP);
                eventNode.set_name("event-result");
                std::ostringstream oss;
                eventNode.print(oss);
                VR_LOGI("oss: %s", oss.str().c_str());
                OnRequestAction(oss.str());
            }
        }
        else {
            CancelRecordVoiceTag(eventNode);
        }
    }
}

void
VR_VoiceBoxEngine::HandleVoiceTagTtsCrashMessage()
{
    VR_LOGD_FUNC();

    m_bVoiceTagTtsCrash = true;

    if (m_iStartBeepID != VR_INVALID_ACTION_ID) {
        VR_LOGI("start beep");
        m_iStartBeepID = VR_INVALID_ACTION_ID;
        HRESULT result = m_engineCommand->BeepDone();
        if (FAILED(result)) {
            VR_LOGI("beepdone failed");
        }
    }

    StopAllTTS();

    std::string strReult;
    ReplyDMVoiceTagResult(strReult);
    CancelVoiceTagRecoSession();

    OnRequestAction(strReult);
    m_iOss.str("");
    m_enVoiceTagRecState = VR_VoiceTag_FinishRec;
    HandleQuitVoiceTag();
    m_stopVoiceTagTTs = VR_INVALID_ACTION_ID;
    m_iPlayVoiceTagBeep = VR_INVALID_ACTION_ID;
    m_stopVoiceTagBeep = VR_INVALID_ACTION_ID;
    m_iPlayVoiceTagTts = VR_INVALID_ACTION_ID;
}

void
VR_VoiceBoxEngine::ReplyDMVoiceTagResult(std::string& strResult)
{
    VR_LOGD_FUNC();

    pugi::xml_document doc;
    if (!doc.load_string(m_strVoiceTagMessage.c_str())) {
        VR_LOGI("load message failed");
        return;
    }
    doc.load_string(m_strVoiceTagMessage.c_str());
    pugi::xml_node eventNode = doc.select_node("//event").node();
    eventNode.set_name("event-result");

    VR_VoiceBoxXmlParser parser(m_strVoiceTagMessage);
    std::string strName = parser.getValueByKey("name");
    if ("stopVoiceTag" == strName) {
        eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_OK);
    }
    else {
        eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_FAILURE);
    }
    std::ostringstream oss;
    eventNode.print(oss);
    VR_LOGI("oss: %s", oss.str().c_str());
    strResult = oss.str();
}

void VR_VoiceBoxEngine::SyncVoiceTagEvent(pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();
    std::string deviceAddress = eventNode.child("deviceAddress").text().as_string();
    std::string btVoiceTagIDListStr = eventNode.child("voiceTagIds").text().as_string();
    VR_LOGI("btVoiceTagIDListStr: %s", btVoiceTagIDListStr.c_str());
    VoiceList<std::string>::type btVoiceTagIDList;
    if (!btVoiceTagIDListStr.empty()) {
        size_t startPos = 0;
        size_t pos = btVoiceTagIDListStr.find(",");
        while (std::string::npos != pos) {
            btVoiceTagIDList.push_back(btVoiceTagIDListStr.substr(startPos, pos - startPos));
            startPos = pos + 1;
            pos = btVoiceTagIDListStr.find(",", startPos);
        }
        btVoiceTagIDList.push_back(btVoiceTagIDListStr.substr(startPos));

    }
    std::string strGrammar;
    SyncVoiceTag(strGrammar, deviceAddress, btVoiceTagIDList);
    VR_LOGI("strGrammar : %s", strGrammar.c_str());
    if ("" != strGrammar) {
        SendDEStatus("voicetag", "1");
        m_pcMsgController->PostMessage(strGrammar);
    }

    btVoiceTagIDListStr.clear();
    VoiceList<std::string>::iterator it = btVoiceTagIDList.begin();
    while (btVoiceTagIDList.end() != it) {
        btVoiceTagIDListStr += (*it + ",");
        ++it;
    }
    if (!btVoiceTagIDListStr.empty()) {
        btVoiceTagIDListStr.pop_back();
    }
    eventNode.child("voiceTagIds").text().set(btVoiceTagIDListStr.c_str());
    eventNode.set_name("event-result");
    std::stringstream oss;
    std::string strOss;
    eventNode.print(oss);
    oss >> strOss;
    VR_LOGI("strOss: %s", strOss.c_str());
    OnRecognized(oss.str());
}

bool VR_VoiceBoxEngine::SyncVoiceTag(std::string &strGrammar, const std::string &deviceAddress, VoiceList<std::string>::type &btVoiceTagIDList)
{
    VR_LOGD_FUNC();

    VR_VoiceBoxVoiceTag voiceTag;
    if (btVoiceTagIDList.empty()) {
        VR_LOGI("Delete ALL");
        return voiceTag.DeleteAllVoiceTag(strGrammar, deviceAddress);
    }

    int key = VR_VoiceTagIDManager::getInstance()->getVoiceTagStorageKey(deviceAddress);
    std::string deviceVoiceTagValueStr;
    VR_VoiceBoxDataStorage storage;
    VR_LOGI("Delete VoiceTag Begin");
    storage.GetValue(key, deviceVoiceTagValueStr);
    VR_LOGI("VoiceTag Value: %s", deviceVoiceTagValueStr.c_str());

    pugi::xml_document deviceVoiceTagValueDoc;
    deviceVoiceTagValueDoc.load_string(deviceVoiceTagValueStr.c_str());
    pugi::xml_node tempNode = deviceVoiceTagValueDoc.first_child();
    VoiceList<std::string>::type voiceTagIDList;
    while (!tempNode.empty()) {
        std::string voiceTagID = tempNode.name();
        VR_LOGI("voiceTagID: %s", voiceTagID.c_str());
        if (std::string::npos != voiceTagID.find(VR_VOICETAG_ID_PREFIX)) {
            voiceTagIDList.push_back(std::string(tempNode.name()).substr(std::string(VR_VOICETAG_ID_PREFIX).size()));
        }
        else {
            deviceVoiceTagValueDoc.remove_child(voiceTagID.c_str());
        }
        tempNode = tempNode.next_sibling();
    }
    std::ostringstream oss;
    deviceVoiceTagValueDoc.print(oss);
    deviceVoiceTagValueStr = oss.str();
    VR_LOGI("new voicetag: %s", deviceVoiceTagValueStr.c_str());
    storage.PutValue(key, deviceVoiceTagValueStr);

    btVoiceTagIDList.sort();
    voiceTagIDList.sort();

    VoiceList<std::string>::iterator btIt = btVoiceTagIDList.begin();
    VoiceList<std::string>::iterator it = voiceTagIDList.begin();
    for (; btIt != btVoiceTagIDList.end() && it != voiceTagIDList.end();) {
        if (*btIt < *it) {
            ++btIt;
        }
        else if (*it < *btIt) {
            ++it;
        }
        else {
            btIt = btVoiceTagIDList.erase(btIt);
            it = voiceTagIDList.erase(it);
        }
    }

    it = voiceTagIDList.begin();

    while (it != voiceTagIDList.end()) {
        std::string strTempGrammar;
        VoiceList<std::string>::type voiceTagIDListDelete;
        voiceTagIDListDelete.push_back(*it);
        voiceTag.DeleteVoiceTag(strTempGrammar, deviceAddress, voiceTagIDListDelete);
        VR_LOGI("strTempGrammar : %s", strTempGrammar.c_str());
        if ("" != strTempGrammar) {
            SendDEStatus("voicetag", "1");
            m_pcMsgController->PostMessage(strTempGrammar);
        }
        ++it;
    }
    return true;
}


void
VR_VoiceBoxEngine::CancelVoiceTagRecoSession()
{
    VR_LOGD_FUNC();

    // Cancel any ASR or TTS session that might be currently running
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->CancelSession(&transaction);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_LOGI("CancelSession Failed, result: %lx", result);
        return;
    }

    // Wait until the engine is stopped
    VBT_BOOL bTimeout = VBT_FALSE;
    result = transaction->WaitForCompletion(INFINITE, &bTimeout);
    if (FAILED(result) || bTimeout) {
        VR_LOGI("Waiting Session Cancel Failed, result: %lx", result);
        return;
    }
}

void
VR_VoiceBoxEngine::DeleteVoiceTagEvent(pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();

    VR_LOGI("DeleteVoiceTagEvent!");
    VoiceList<std::string>::type voiceTagIDList;
    std::string deviceAddress(eventNode.child("deviceAddress").text().as_string());
    VR_LOGI("deviceAddress: %s", deviceAddress.c_str());
    pugi::xml_node tempNode = eventNode.first_child();
    while (!tempNode.empty()) {
        VR_LOGI("tempNode");
        std::string name = tempNode.name();
        VR_LOGI("name: %s", name.c_str());
        if ("voiceTagId" == std::string(tempNode.name())) {
            std::string voiceTagID(tempNode.text().as_string());
            VR_LOGI("voiceTagID: %s", voiceTagID.c_str());
            if (!voiceTagID.empty()) {
                voiceTagIDList.push_back(voiceTagID);
            }
        }
        tempNode = tempNode.next_sibling();
    }
    VR_VoiceBoxVoiceTag voiceTag;
    bool result = false;
    std::string strGrammar;
    if (voiceTagIDList.empty()) {
        result = voiceTag.DeleteAllVoiceTag(strGrammar, deviceAddress);
    }
    else {
        result = voiceTag.DeleteVoiceTag(strGrammar, deviceAddress, voiceTagIDList);
    }
    VR_LOGI("strGrammar : %s", strGrammar.c_str());
    if ("" != strGrammar) {
        SendDEStatus("voicetag", "1");
        m_pcMsgController->PostMessage(strGrammar);
    }

    eventNode.remove_child("deviceAddress");
    eventNode.set_name("event-result");
    if (result) {
        eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_OK);
    }
    else {
        eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_FAILURE);
    }

    std::stringstream oss;
    std::string strOss;
    eventNode.print(oss);
    VR_LOGI("oss: %s", oss.str().c_str());
    oss >> strOss;
    VR_LOGI("strOss: %s", strOss.c_str());
    OnRecognized(oss.str());

}

void
VR_VoiceBoxEngine::SaveVoiceTagEvent(pugi::xml_node& eventNode)
{
    VR_LOGD_FUNC();

    std::string voiceTagID(eventNode.child("voiceTagId").text().as_string());
    VR_VoiceBoxVoiceTag voiceTag;
    VoiceTagInfo voiceTagPara;
    voiceTagPara.voiceTagID = voiceTagID;
    voiceTagPara.pcmPath = m_tempPcmFile;
    voiceTagPara.phoneme = m_tempPronunc;
    voiceTagPara.contactMsg = m_voiceTagContactMsg;
    std::string strGrammar;
    bool result = voiceTag.SaveVoiceTag(strGrammar, m_deviceAddress, voiceTagPara, "false", m_isVoiceTagUpdate);
    VR_LOGI("strGrammar : %s", strGrammar.c_str());
    if ("" != strGrammar) {
        SendDEStatus("voicetag", "1");
        m_pcMsgController->PostMessage(strGrammar);
    }

    eventNode.set_name("event-result");
    if (result) {
        eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_OK);
    }
    else {
        if (VR_ConfigureIF::Instance()->isFileExist(VOICETAGFILE)) {
            bool removeOK = VR_ConfigureIF::Instance()->removeFile(VOICETAGFILE);
            VR_LOGI("removeOK : %d", removeOK);
        }
        eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_FAILURE);
    }

    std::stringstream oss;
    std::string strOss;
    eventNode.print(oss);
    VR_LOGI("oss: %s", oss.str().c_str());
    oss >> strOss;
    VR_LOGI("strOss: %s", strOss.c_str());
    OnRecognized(oss.str());
}

void
VR_VoiceBoxEngine::NotifyPhoneGrammarFinished()
{
    VR_LOGD_FUNC();

    if (NULL != m_pcCatalogController) {
        m_pcCatalogController->PostStatusMsg("<op name=\"nofiyphonegrammarfinished\"></op>");
    }
}

void
VR_VoiceBoxEngine::RecordVoiceTagEvent(pugi::xml_node& eventNode, bool isUpdate)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand) {
        return;
    }

    VR_AudioStreamIF* pcAudioStream = VR_AudioStreamIF::Instance();
    if (NULL != pcAudioStream) {
        VR_LOGI("SetVoiceTag");
        pcAudioStream->SetVoiceTag(true);
        VR_LOGI("SetVoiceTag");
    }
    if (VR_ConfigureIF::Instance()->isFileExist(VOICETAGFILE)) {
        bool removeOK = VR_ConfigureIF::Instance()->removeFile(VOICETAGFILE);
        VR_LOGI("removeOK : %d", removeOK);
    }
    std::string deviceAddress(eventNode.child("deviceAddress").text().as_string());
    std::string strActionID(eventNode.child("actionId").text().as_string());
    m_strActionID = strActionID;
    m_deviceAddress = deviceAddress;
    m_tempVoiceTagID = eventNode.child("voiceTagId").text().as_string();
    pugi::xml_node contactNode = eventNode.child("contact");
    std::stringstream oss;
    contactNode.print(oss);
    VR_LOGI("contactNode : [%s]", oss.str().c_str());
    if (!isUpdate) {
        m_tempVoiceTagID = "-1";
    }
    VR_LOGI("Record voiceTagID: [%s]", m_tempVoiceTagID.c_str());
    m_voiceTagContactMsg = oss.str();
    m_isVoiceTagUpdate = isUpdate;
    // Cancel the curent session
    VR_LOGI("m_tempVoiceTagID: %s", m_tempVoiceTagID.c_str());
    if (!m_isVoiceTagUpdate) {
        VR_LOGI("create voiceTagId");
        m_tempVoiceTagID = VR_VoiceTagIDManager::getInstance()->getVoiceTagID(m_tempVoiceTagID);
    }

    VR_LOGI("m_tempVoiceTagID: %s", m_tempVoiceTagID.c_str());
    std::string strStartAgent = "<event name=\"startCurrentAgent\">";
    strStartAgent.append("<agent>voice_tag</agent>");
    strStartAgent.append("</event>");

    OnRequest(strStartAgent);
    VR_LOGI("RecordVoiceTagEvent - voice tag: %s", strStartAgent.c_str());
}

void
VR_VoiceBoxEngine::RecordVoiceTagResult(const std::string& tempPronunc)
{
    m_tempPronunc = tempPronunc;

    VR_LOGI("m_tempPronunc: %s", m_tempPronunc.c_str());

    m_tempPcmFile = VR_ConfigureIF::Instance()->getUsrPath() + VOICETAGFILE;
    VR_LOGI("m_tempPcmFile: %s", m_tempPcmFile.c_str());
    VR_LOGI("m_deviceAddress: %s", m_deviceAddress.c_str());
    VR_LOGI("m_voiceTagContactMsg: %s", m_voiceTagContactMsg.c_str());
    VR_LOGI("m_isVoiceTagUpdate: %d", m_isVoiceTagUpdate);

    pugi::xml_document eventDoc;
    eventDoc.load_string("");
    pugi::xml_node eventNode = eventDoc.append_child("event-result");
    if (!m_isVoiceTagUpdate) {
        eventNode.append_attribute("name").set_value("addRecordVoiceTag");
    }
    else {
        eventNode.append_attribute("name").set_value("editRecordVoiceTag");
    }
    bool recordResult = true;
    if ("" == m_tempPronunc
        || "voice tag too similar" == m_tempPronunc
        || "no speech detected" == m_tempPronunc
        || "voice tag too short" == m_tempPronunc) {
        eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_FAILURE);
        recordResult = false;
        m_enVoiceTagRecState = VR_VoiceTag_RecordFailed;
    }
    else {
        m_enVoiceTagRecState = VR_VoiceTag_RecordSucceed;
        VoiceTagInfo voiceTagPara;
        voiceTagPara.voiceTagID = "-2";
        voiceTagPara.pcmPath = m_tempPcmFile;
        voiceTagPara.phoneme = m_tempPronunc;
        voiceTagPara.contactMsg = m_voiceTagContactMsg;
        VR_VoiceBoxVoiceTag voiceTag;
        std::string strGrammar;
        bool result = voiceTag.SaveVoiceTag(strGrammar, m_deviceAddress, voiceTagPara, "true", m_isVoiceTagUpdate);
        if (result) {
            eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_OK);
        }
        else {
            eventNode.append_attribute("errcode").set_value((int)VR_VoiceTagResult::VOICETAG_FAILURE);
            recordResult = false;
        }
    }

    if (!recordResult) {
        if (VR_ConfigureIF::Instance()->isFileExist(VOICETAGFILE)) {
            bool removeOK = VR_ConfigureIF::Instance()->removeFile(VOICETAGFILE);
            VR_LOGI("removeOK : %d", removeOK);
        }
    }

    eventNode.append_child("actionId").text().set(m_strActionID.c_str());
    eventNode.append_child("voiceTagId").text().set(m_tempVoiceTagID.c_str());
    m_iOss.str("");
    eventNode.print(m_iOss);
    VR_LOGI("m_iOss: %s", m_iOss.str().c_str());
}

void
VR_VoiceBoxEngine::HandleIncomingMessage(const std::string& message)
{
    std::string strMessage = "<action-result agent=\"phone\" op=\"setIncomingMessageInfo\"><instanceId>";
    strMessage.append(m_messageinfo.instanceId);
    strMessage.append("</instanceId><messageId>");
    strMessage.append(m_messageinfo.messageId);
    strMessage.append("</messageId><messageType>");
    strMessage.append(m_messageinfo.messageType);
    strMessage.append("</messageType><subject>");
    strMessage.append(m_messageinfo.subject);
    strMessage.append("</subject><phoneNumber>");
    strMessage.append(m_messageinfo.phoneNumber);
    strMessage.append("</phoneNumber><phoneType>");
    strMessage.append(m_messageinfo.phoneType);
    strMessage.append("</phoneType><contactID>");
    strMessage.append(m_messageinfo.contactID);
    strMessage.append("</contactID></action-result>");
    VR_VoiceBoxXmlParser parserTemp(strMessage);
    m_agents[AgentType_Phone]->ReplyQueryInfo(parserTemp);
    VR_VoiceBoxXmlParser parserBody(message);
    std::string strBody = parserBody.getValueByKey("messagebody");
    m_client->CreateParameterSet(&m_pVariables);
    if (NULL != m_pVariables.ptr()) {
        m_pVariables->AddParameter(_T("body"), strBody.c_str(), NULL, NULL);
        m_pVariables->AddParameter(_T("clienttts"), _T("False"), NULL, NULL);

        if ("" != m_messageinfo.sender) {
            m_pVariables->AddParameter(_T("entry"), m_messageinfo.sender.c_str(), NULL, NULL);
        }
        else if ("" != m_messageinfo.phoneNumber) {
            m_pVariables->AddParameter(_T("entry"), m_messageinfo.phoneNumber.c_str(), NULL, NULL);
        }
        else {
            m_pVariables->AddParameter(_T("entry"), "unknown", NULL, NULL);
        }
        if ("1" == m_messageinfo.messageType) {
            m_pVariables->AddParameter(_T("number"), "", NULL, NULL);
            m_pVariables->AddParameter(_T("sender"), m_messageinfo.phoneNumber.c_str(), NULL, NULL);
        }
        else {
            m_pVariables->AddParameter(_T("number"), m_messageinfo.phoneNumber.c_str(), NULL, NULL);
            m_pVariables->AddParameter(_T("sender"), m_messageinfo.sender.c_str(), NULL, NULL);
        }
    }

    std::string strStartAgent = "<event name=\"startCurrentAgent\">";
    strStartAgent.append("<agent>in_message</agent>");
    strStartAgent.append("</event>");

    OnRequest(strStartAgent);
    VR_LOGI("HandleIncomingMessage: %s", strStartAgent.c_str());
}

std::string
VR_VoiceBoxEngine::GetVoiceTagId(pugi::xml_node& contactNode)
{
    VR_LOGI("GetVoiceTagId");

    StructNode structNode;
    VoiceVector<StructNode>::type tempVector;
    std::string FullName = contactNode.attribute("name").as_string();
    VR_LOGI("FullName: %s", FullName.c_str());
    pugi::xml_node phoneItemNode = contactNode.first_child();

    while (!phoneItemNode.empty()) {
        structNode.Name = FullName;
        std::string number = phoneItemNode.attribute("number").as_string();
        VR_LOGI("number: %s", number.c_str());
        structNode.Value = number;
        tempVector.push_back(structNode);
        phoneItemNode = phoneItemNode.next_sibling();
    }
    std::string strVoiceTagId;
    // VoiceVector<PersonInfo>::type m_vecContact = m_pcCatalogPhone->GetContactInfo();
    /* if (0 == m_vecPersonInfo.size()) {
        VR_LOGI("m_vecPersonInfo is null");
    } */
    for (size_t i = 0; i < m_vecPersonInfo.size(); ++i) {
        CVECIPtr<IVECIParameterSet> spContact;
        CVECICStr cFullName(&(*m_client));
        PersonInfo perInfo = m_vecPersonInfo[i];

        // cFullName.Format(_T("%s %s"), perInfo.firstName.c_str(), perInfo.lastName.c_str());
        std::string contactName = perInfo.firstName + " " + perInfo.lastName;
        VR_LOGI("contactName :%s", contactName.c_str());
        // std::string strFullName = cFullName.Get();
        // VR_LOGI("strFullName :%s", strFullName.c_str());
        for (size_t index = 0; index < perInfo.phoneItemVector.size(); ++index) {
            if (!perInfo.phoneItemVector[index].Value.empty()) {
                VR_LOGI("perInfo.phoneItemVector[index].Value :%s", perInfo.phoneItemVector[index].Value.c_str());
                for (size_t item = 0; item < tempVector.size(); ++item) {
                    StructNode tempStuct = tempVector[item];
                    VR_LOGI("contactName :%s", contactName.c_str());
                    VR_LOGI("Name: %s", tempStuct.Name.c_str());
                    VR_LOGI("Value: %s", tempStuct.Value.c_str());
                    if (tempStuct.Value == perInfo.phoneItemVector[index].Value) {
                        VR_LOGI("number is IsEqual");
                    }
                    VR_LOGI("contactName size: %d", contactName.size());
                    VR_LOGI("tempStuct.Name size: %d", tempStuct.Name.size());

                    if (contactName == tempStuct.Name) {
                        VR_LOGI("name is IsEqual");
                    }
                    if ((contactName == tempStuct.Name) && (tempStuct.Value == perInfo.phoneItemVector[index].Value)) {
                        VR_LOGI("finded");
                        VR_LOGI("perInfo.strId: %s", perInfo.strId.c_str());
                        strVoiceTagId = perInfo.strId;
                        VR_LOGI("strVoiceTagId : %s", strVoiceTagId.c_str());
                        break;
                    }
                }
            }
        }
    }

    return strVoiceTagId;
}

VR_VoiceBoxCatalogManager*
VR_VoiceBoxEngine::GetCatalogManager()
{
    return m_pcCatalogManager;
}

bool
VR_VoiceBoxEngine::OnRecordingVoiceTag(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    VR_LOGI("voice traning completed");

    std::string strStartAgent = "<event name=\"startCurrentAgent\">";
    strStartAgent.append("<agent>voice_tag</agent>");
    strStartAgent.append("</event>");

    OnRequest(strStartAgent);
    VR_LOGI("voice tag: %s", strStartAgent.c_str());

    return true;
}

void
VR_VoiceBoxEngine::OnRecognized(const std::string& message)
{
    VR_LOGD_FUNC();

    if (NULL == m_pcExternalCallback) {
        // VR_LOGE("The external callback is NULL");
        return;
    }

    VR_LOGI("->DM msg : %s", message.c_str());

    pugi::xml_document doc;
    if (!doc.load(message.c_str())) {
        VR_LOGI("Error format message !!");
        return;
    }

    if (NULL != doc.select_node("/action[@name='notifyAppXmlMessage']")) {
        OnRequestAction(message);
        return;
    }

    if (NULL != doc.select_node("//actionId")) {
        OnRequestAction(message);
        return;
    }

    if (NULL != doc.select_node("/action")) {
        VR_LOGI("send the action to queue");
        OnRequest(message);
        return;
    }

    if (NULL != doc.select_node("/display[@content='ShowPopupMessage']")) {
        m_strMsgPrompt = "";
        m_strMsgPrompt = doc.select_node("//prompt").node().child_value();
        VR_LOGI("prompt:%s", m_strMsgPrompt.c_str());
        OnRequestAction(message);
        return;
    }

    if (NULL != doc.select_node("/display[@content='ScreenDisplay']")) {
        std::string strContent;
        m_strScreenState.clear();
        m_strStep = "";
        m_strScreenState = doc.select_node("//content").node().child_value();
        if (InScreen("phone_one_call_contact_message_selected")) {
            std::string name = doc.select_node("//screenType").node().child_value();
            VR_LOGI("name: %s", name.c_str());
            if ("Selected Recent Call No Contact Info" == name) {
                m_isNameNull = true;
            }
            else if ("One Contact Selected" == name) {
                m_isNameNull = false;
            }
            else {
            }
        }
        else if (InScreen("adaptation")) {
            m_strStep = doc.select_node("//step").node().child_value();
        }
        else if ("na_topmenu_idle" == m_strScreenState) {
            std::string strMessage = "<action-result agent=\"phone\" op=\"QuitVRInit\"/>";
            VR_VoiceBoxXmlParser parserTemp(strMessage);
            m_agents[AgentType_Phone]->ReplyQueryInfo(parserTemp);
        }
        else {
        }


        if (m_bMoreHint && (m_strScreenState != m_strMoreHints)) {
            VR_LOGI("this is not more hints state, m_strMoreHints(%s), strContent(%s)", m_strMoreHints.c_str(), strContent.c_str());
            m_bMoreHint = false;
        }

        if ("" != m_strMsgPrompt) {
            m_strDisplayScreen = message;
            return;
        }
        m_strDisplayScreen = "";
    }

    if (0 == strcmp("processing", doc.select_node("//state").node().child_value())) {
        if (VR_SessionStatePaused == m_sessionState
            || VR_SessionStateTemporarilyCanceled == m_sessionState
            || VR_SessionStateRestartSessionAfterTTS == m_sessionState
            || VR_SessionStateCanceled == m_sessionState
            || VR_SessionStatePttLong == m_sessionState) {
            return;
        }
        if (VBT_RECO_STATE_VOICE_TAG == m_strCurrentStateName) {
            std::string tempPronunc;
            tempPronunc = doc.select_node("//result").node().child_value();
            VR_LOGI("OnRecognized-tempPronunc : %s", tempPronunc.c_str());
            RecordVoiceTagResult(tempPronunc);

            std::string strMsgVoiceTag = "<display agent=\"Common\" content=\"VRState\">\
            <engineType>local</engineType>\
            <state>processing</state>\
            <tag>Voice Control</tag>\
            <prompt />\
            <nbest>false</nbest>\
            <result />\
            <meter>on</meter>\
            </display>";
            OnRequestAction(strMsgVoiceTag);
            return;
        }
    }

    if (InScreen("help_train_voice_recognition")) {
        m_sessionState = VR_SessionStateSpeakAdaptaion;
    }

    // Notify the recognized result to the external service
    OnRequestAction(message);
}
// On Info Query Message
void
VR_VoiceBoxEngine::OnInfoQueryMessage(const std::string& message)
{
    VR_LOGD_FUNC();

    if (NULL == m_pcExternalCallback) {
        VR_LOGE("The external callback is NULL");
        return;
    }

    // Query some information to the external service
    OnRequestAction(message);
}

// Play beep request
void
VR_VoiceBoxEngine::OnBeep(const std::string& message)
{
    VR_LOGD_FUNC();

    if (m_bHandleTTSCrush) {
        VR_LOGI("tts is crushed, we don't need to play the done beep");
        return;
    }

    if (NULL == m_pcExternalCallback) {
        VR_LOGE("The external callback is NULL");
        return;
    }

    VR_LOGI("message : %s", message.c_str());
    VR_VoiceBoxXmlBuilder  xmlBuilder;
    pugi::xml_node node = xmlBuilder.buildStartActionElement("prompt", "playBeep");
    xmlBuilder.buildGivenElement(node, "beepFile", message, "", "");
    std::string strBeepXml = xmlBuilder.getXmlString();

    VR_LOGI("strBeepXml : %s", strBeepXml.c_str());
    // Play voicetag beep request
    m_iPlayVoiceTagBeep = OnRequestAction(strBeepXml);
    VR_LOGI("m_iPlayVoiceTagBeep: %d", m_iPlayVoiceTagBeep);
}

void
VR_VoiceBoxEngine::OnBeep(const VR_BeepType& beepType)
{
    VR_LOGD_FUNC();

    if (VR_BeepType_TSL_Done == beepType) {
        VR_LOGI("TSL state is over");
        if (NULL != m_agents[AgentType_Global]) {
            std::string startNativeVR = "<send event=\"startNativeVR\" target=\"de\"/>";
            m_agents[AgentType_Global]->ProcessMessage(startNativeVR);
        }
    }

    if (m_bHandleTTSCrush) {
        VR_LOGI("tts is crushed, we don't need to play beep");

        if ("" != m_strMsgPrompt) {
            VR_LOGI("close popup message");
            ClosePopupMsg();
            m_strMsgPrompt = "";
        }

        if (m_bTaskCompelete && ("" != m_strAction)) {
            if (std::string::npos != m_strAction.find("launchApp")) {
                VR_LOGI("tts crash, don't run launching app");
                m_strAction = "";
            }
            else {
                OnRequestAction(m_strAction);
                m_strAction = "";
            }
        }

        if (VR_BeepType_Done == beepType) {
            VR_LOGI("native VR! TTS is crushed, we don't need to play done beep");

            m_strMsgPrompt = "";
            m_bPlayTTS = false;
            SendCancelResult();
            NotifyResourceState();
            HandleQuitVR();

            VR_LOGI("tts is crushed, we quit VR finished");
        }
        else if (VR_BeepType_TSL_Done == beepType) {
            VR_LOGI("TSL VR! TTS is crushed, we don't need to play done beep");

            // Notify end task state to UI
            NotifyEndTaskState(false);
            m_bInEndTask = false;

            m_bHandleTTSCrush = false;
            m_bQuitWaitForPrompt = false;
            m_bWaitForDoneBeep = false;
            m_strTslAppRecoStateMsg = "";
            m_bAppRecoState = VR_AppRecoState_Invalid;
            VR_LOGI("the cancel event of ttscrash is handled over");
            std::string eventResult = "<event-result name=\"CancelAppRecognition\" option=\"ttscrash\" errcode=\"0\" />";
            OnRecognized(eventResult);

            if (m_bUpdateState) {
                VR_LOGI("log on/off -> stop UnInitialize initialize start");
                SendCancelResult();
            }

            if (!m_lstCancelOption.empty()) {
                VR_LOGI("clear cancel result");
                SendCancelResult();
                NotifyResourceState();
                HandleQuitVR();
            }

            VR_LOGI("tts is crushed, we quit tsl VR finished");
        }
        else if (VR_BeepType_Listening == beepType) {
            VR_LOGI("TTS is crushed, we don't need to play start beep, run beepdone!");
            HRESULT result = m_engineCommand->BeepDone();
            if (FAILED(result)) {
                VR_LOGI("beepdone is failed.");
            }
            VR_LOGI("TTS is crushed, beepdone finished");
        }
        else {
            VR_LOGI("TTS is crushed, we don't need to play beep!");
        }
        return;
    }

    // when cancelvoicetag or voice tts crash do not need to play voicetag beep
    if ((VR_BeepType_Done != beepType) && (VBT_RECO_STATE_VOICE_TAG == m_strCurrentStateName)) {
        if (m_bVoiceTagTtsCrash || m_bDoCancelVoiceTag) {
            VR_LOGI("Don't play beep, when voicetag tts crash or cancelvoicetag");
            if (VR_BeepType_Listening == beepType) {
                if (NULL != m_engineCommand.ptr()) {
                    m_engineCommand->BeepDone();
                }
            }
            return;
        }
    }

    if (NULL == m_pcExternalCallback) {
        VR_LOGE("The external callback is NULL");
        return;
    }

    if ((VR_BeepType_Done != beepType)
        && (!m_lstCancelOption.empty() || m_bDoCancelSession)) {
        VR_LOGI("Don't play return beep, when canceled");
        return;
    }

    if ((VR_BeepType_TSL_Done != beepType) && m_bDoCancelTslVR) {
        VR_LOGI("Don't play return beep, when TSL VR canceled");
        return;
    }

    std::string strBeepFileName;
    switch (beepType) {
    case VR_BeepType_Listening:
    {
        m_bWaitConfirmation = true;
        if (VR_SessionStateStartWithiBargeIn == m_sessionState) {
            VR_LOGI("This is barge in. Don't play start beep.");
            m_sessionState = VR_SessionStateStarting;
            m_iStartBeepID = VR_INVALID_ACTION_ID;
            if (NULL == m_engineCommand.ptr()) {
                VR_LOGI("Point is NULL.");
                return;
            }

            HRESULT result = m_engineCommand->BeepDone();
            if (FAILED(result)) {
                VR_LOGI("Result is failed.");
                return;
            }
            return;
        }
        strBeepFileName = "startVR.wav";
        m_sessionState = VR_SessionStateStarting;

        if (m_bSpeakOverBeep) {
            VR_AudioStreamIF* pcAudioStream = VR_AudioStreamIF::Instance();
            if (NULL != pcAudioStream) {
                pcAudioStream->StartAudioInWithCache();
            }
        }

        break;
    }

    case VR_BeepType_Confirmation:
    {
        if ((VR_SessionStateStartWithiBargeIn != m_sessionState)
            && (!m_bWaitConfirmation)) {
            VR_LOGI("In this case, this beep don't play");
            return;
        }
        VR_LOGP("DE: VBT beep done after recognition... case : 215-3-00 215-4-00");
        m_bWaitConfirmation = false;
        strBeepFileName = "returnVR.wav";
        break;
    }

    case VR_BeepType_Done:
    case VR_BeepType_TSL_Done:
    {
        strBeepFileName = "endVR.wav";
        break;
    }

    default:
    {
        VR_LOGE("No beep type");
        return;
    }
    }

    VR_VoiceBoxXmlBuilder  xmlBuilder;
    pugi::xml_node node = xmlBuilder.buildStartActionElement("prompt", "playBeep");
    std::string strBeepPath = VR_ConfigureIF::Instance()->getDataPath() + "beep/" + strBeepFileName;
    xmlBuilder.buildGivenElement(node, "beepFile", strBeepPath, "", "");
    std::string strBeepXml = xmlBuilder.getXmlString();

    VR_LOGI("beepFile : %s", strBeepXml.c_str());

    // Play beep request
    VR_LOGP("request beep : %s", strBeepFileName.c_str());
    int iBeepID = OnRequestAction(strBeepXml);

    if (VR_BeepType_Listening == beepType) {
        m_iStartBeepID = iBeepID;
        VR_LOGI("start beep : %d", m_iStartBeepID);
    }
    else if (VR_BeepType_Done == beepType) {
        m_iDoneBeepID = iBeepID;
        VR_LOGI("done beep : %d", m_iDoneBeepID);
    }
    else {
    }
}

void
VR_VoiceBoxEngine::PlayPrompt(IVECITransaction* pcTrans, const std::string& message)
{
    VR_LOGD_FUNC();

    // it is voicetag tts
    std::string tempMessage = message;
    if ((VBT_RECO_STATE_VOICE_TAG == m_strCurrentStateName) && (VR_VoiceTag_RecordSucceed == m_enVoiceTagRecState)) {
        BuilderPlayVoiceTagXml(tempMessage);
    }

    // setting VRState
    OnRequest("<send event=\"sendVRState\"/>");

    VR_LOGI("waiting play tts: speak size, %d", m_mapPlayTTSTransation.size());
    // Play guidance request
    m_pcPlayTransation = pcTrans;
    m_pcPlayTransation->AddRef();

    m_iCurTTSReqId = OnRequestAction(tempMessage);
    VR_LOGI("current speak id, %d", m_iCurTTSReqId);

    if ((VBT_RECO_STATE_VOICE_TAG == m_strCurrentStateName) && (VR_VoiceTag_RecordSucceed == m_enVoiceTagRecState)) {
        m_iPlayVoiceTagTts = m_iCurTTSReqId;
    }
    m_mapPlayTTSTransation.insert(std::make_pair(m_iCurTTSReqId, m_pcPlayTransation));
    VR_LOGI("speak size, %d", m_mapPlayTTSTransation.size());

    m_bPlayTTS = true;
}

void
VR_VoiceBoxEngine::BuilderPlayVoiceTagXml(std::string& message)
{
    VR_LOGD_FUNC();

    pugi::xml_document doc;
    if (!doc.load_string(message.c_str())) {
        VR_LOGI("load xml failed");
        return;
    }
    pugi::xml_node actionNode = doc.select_node("//action").node();
    std::string strLanguage = actionNode.child("language").text().as_string();
    std::string strText = actionNode.child("text").text().as_string();
    std::string strphoneme = actionNode.child("phoneme").text().as_string();
    pugi::xml_document voiceTagValueDoc;
    voiceTagValueDoc.load_string("");
    pugi::xml_node voiceTagValueNode = voiceTagValueDoc.append_child("action");

    std::string voiceTagPCMPath;
    if (VR_ConfigureIF::Instance()->isFileExist(VOICETAGFILE)) {
        voiceTagPCMPath.assign(VR_ConfigureIF::Instance()->getUsrPath() + VOICETAGFILE);
    }
    voiceTagValueNode.append_attribute("agent").set_value("prompt");
    voiceTagValueNode.append_attribute("op").set_value("playTtsList");
    voiceTagValueNode.append_child("language").text().set(strLanguage.c_str());
    if ("" != voiceTagPCMPath) {
        voiceTagValueNode.append_child("beepFile").text().set(voiceTagPCMPath.c_str());
    }
    voiceTagValueNode.append_child("text").text().set(strText.c_str());
    voiceTagValueNode.append_child("phoneme").text().set(strphoneme.c_str());

    std::ostringstream osss;
    voiceTagValueNode.print(osss);
    VR_LOGI("voiceTag Value: %s", osss.str().c_str());
    message = osss.str();
    VR_LOGI("message : : %s", message.c_str());
}

// Play guidance request
void
VR_VoiceBoxEngine::OnSpeak(IVECITransaction* pcTrans, const std::string& message)
{
    VR_LOGD_FUNC();

    if (NULL == m_pcExternalCallback) {
        VR_LOGE("The external callback is NULL");
        return;
    }

    if (m_bHandleTTSCrush) {
        VR_LOGI("TTS is crushed, we don't play prompt");
        if ((VR_REGION_US == m_country)
            && (VR_PVRState_WaitPlayTTS == m_PVRStateCurrent)) {
            VR_LOGI("TTS is crushed,coming in PVR screen, don't play PVR prompt");
            m_PVRStateCurrent = VR_PVRState_None;
        }
        if (NULL != pcTrans) {
            pcTrans->SpeakDone();
        }
        return;
    }

    // when tts crash do not need to play tts
    if ((m_bVoiceTagTtsCrash || m_bDoCancelVoiceTag) && (VBT_RECO_STATE_VOICE_TAG == m_strCurrentStateName)) {
        VR_LOGI("tts crash or cancelvoicetag do not need to play tts");
        if (NULL != pcTrans) {
            pcTrans->SpeakDone();
        }
        return;
    }

    if (VR_REGION_US == m_country) {
        VR_LOGI("PVR State %d", m_PVRStateCurrent);
        if (VR_PVRState_WaitPlayTTS == m_PVRStateCurrent) {
            VR_LOGI("PVR -> play tts");
            m_PVRStateCurrent = VR_PVRState_PlayingTTS;
        }
    }

    // During back canceling, we should not play the prompt
    if (m_bBackInterupted) {
        if (NULL != pcTrans) {
            pcTrans->SpeakDone();
        }
        return;
    }

    std::string strTTSText;
    pugi::xml_document doc;
    if (doc.load(message.c_str())) {
        strTTSText = doc.select_node("//text").node().child_value();
    }

    if ("" == strTTSText) {
        m_bTTSNull = true;
    }
    else {
        m_bTTSNull = false;
    }

    if (m_cTslParamManager.GetRenderDoneSyncFlag()
        && ("Follow-Up" == m_strCurActionClass)) {
        pcTrans->AddRef();
        m_cTslParamManager.SetPlayTransAfterRender(pcTrans, message);

        m_pcMsgController->PushFrontMessage(
            "<send event=\"confirm-rendering\" target=\"de\"/>");
        return;
    }

    if (m_bTTSNull) {
        // setting VRState
        OnRequest("<send event=\"sendVRState\"/>");

        if (NULL != pcTrans) {
            pcTrans->SpeakDone();
        }
        return;
    }

    PlayPrompt(pcTrans, message);
}

void
VR_VoiceBoxEngine::OnRequest(const std::string& message)
{
    if (NULL == m_pcMsgController) {
        VR_LOGI("VR_VoiceBoxEngine m_pcMsgController is null");
        return;
    }

    m_pcMsgController->PostMessage(message);
}

void
VR_VoiceBoxEngine::OnPlayTone(const std::string& message)
{
    VR_LOGD_FUNC();

    if (std::string::npos != message.find("Done")) {
        OnBeep(VR_BeepType_TSL_Done);
    }
}

// Cancel the current voice recognition session
bool
VR_VoiceBoxEngine::CancelRecoSession()
{
    VR_LOGD_FUNC();

    VR_LOGI("current session : %d", m_sessionState);
    if (VR_SessionStateStoped != m_sessionState
        && VR_SessionStateCanceled != m_sessionState
        && VR_SessionStatePaused != m_sessionState
        && VR_SessionStateResume != m_sessionState
        && VR_SessionStateAutoSendRecogState != m_sessionState
        && VR_SessionStateAutoTutoSendRecogState != m_sessionState
        && VR_SessionStateRestartSessionAfterTTS != m_sessionState
        && VR_SessionStateSendRecogAfterTTS != m_sessionState
        && VR_SessionStateStartOver != m_sessionState) {
        m_sessionState = VR_SessionStateTemporarilyCanceled;
    }

    if (m_bStartSessionWithBargeIn) {
        m_bStartSessionWithBargeIn = false;
        m_bBosDetected = false;
        VR_AudioStreamIF* pcAudioStream = VR_AudioStreamIF::Instance();
        if (NULL != pcAudioStream) {
            pcAudioStream->StopAudioIn();
        }

        VR_LOGI("Barge-In session is interrupted");
    }

    m_bDoCancelSession = true;

    // Cancel any ASR or TTS session that might be currently running
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->CancelSession(&transaction);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_LOGE("CancelSession Failed, result: %lx", result);
        return false;
    }

    // Wait until the engine is stopped
    VBT_BOOL bTimeout = VBT_FALSE;
    result = transaction->WaitForCompletion(INFINITE, &bTimeout);
    if (FAILED(result) || bTimeout) {
        VR_LOGE("Waiting Session Cancel Failed, result: %lx", result);
        // return false;
    }

    m_bDoCancelSession = false;

    StopAllTTS();

    return true;
}

void
VR_VoiceBoxEngine::SendVRState(
    const std::string& strState, const std::string& strPrompt,
    const std::string& strNBestScreen, const std::string& strResult)
{
    VR_LOGD_FUNC();
    std::string strReply;
    if (m_bNeedPromptPlaying && "listening" == strState) {
        m_bNeedPromptPlaying = false;
        std::string strState = "promptPlaying";
        VR_ConfigureIF * pcConfig = VR_ConfigureIF::Instance();
        if (NULL != pcConfig && pcConfig->getVROverPrompt()) {
            strState = "promptWithBargein";
        }

        strReply = BuildVRState(strState, strPrompt, "false", "");
        OnRecognized(strReply);
    }

    strReply = BuildVRState(strState, strPrompt, strNBestScreen, strResult);
    OnRecognized(strReply);
}

std::string
VR_VoiceBoxEngine::BuildVRState(
    const std::string& strState, const std::string& strPrompt,
    const std::string& strNBestScreen, const std::string& strResult)
{
    VR_VoiceBoxXmlBuilder xmlBulder;
    pugi::xml_node root_node = xmlBulder.buildDisplayElement("Common", "VRState");
    xmlBulder.buildGivenElement(root_node, "engineType", "local", "", "");
    xmlBulder.buildGivenElement(root_node, "state", strState, "", "");
    xmlBulder.buildGivenElement(root_node, "tag", "Voice Control", "", "");
    xmlBulder.buildGivenElement(root_node, "prompt", strPrompt, "", "");
    xmlBulder.buildGivenElement(root_node, "nbest", strNBestScreen, "", "");
    xmlBulder.buildGivenElement(root_node, "result", strResult, "", "");
    xmlBulder.buildGivenElement(root_node, "meter", "on", "", "");

    std::string strMeterCooperation;
    if ("quit" == strState) {
        strMeterCooperation = "on";
    }
    else {
        strMeterCooperation = (m_bTslDialog) ? "off" : "on";
    }
    xmlBulder.buildGivenElement(root_node, "meterCooperation", strMeterCooperation, "", "");

    return xmlBulder.getXmlString();
}

// Puts the VoiceBox Engine into a specific recognition state.
bool
VR_VoiceBoxEngine::SendRecognitionState(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }
    m_bCanceled = false;
    m_bTaskCompelete = false;
    std::string strActionKey = parser.getXmlKey();
    // Get the statename from the XML message
    std::string strStateName = parser.getValueByKey("agent");
    if ("startAgent" == strActionKey) {
        m_bPVRScreen = false;
        SetUpdateGammarFlg(false);

        if (NULL != m_pcCatalogManager && NULL != m_pcCatalogController && !m_bHavePVR) {
            m_bPauseReturnAfterVR = false;
            m_bSession = true;
            m_pcCatalogController->PostStatusMsg("<op name=\"dialogsession\"><value>true</value></op>");
            bool bNeedPauseGrammar = m_pcCatalogManager->GetNeedPauseGrammarFlg();
            if (bNeedPauseGrammar) {
                // preview pausegrammarupdate has return, consider whether need to call pausegrammarupdate
                VR_LOGI("start normale vr, need to pause grammar");
                auto pairTransaction = GetCurrentTransaction();
                if (!pairTransaction.first.empty()) {
                    m_pcCatalogManager->PauseGrammarUpdate(pairTransaction.first);
                }
                else {
                    VR_LOGI("no grammar loading when startup vr");
                    m_pcCatalogController->PostStatusMsg("<op name=\"checkpauseconditon\"><value>true</value></op>");
                }
            }
            else {
                VR_LOGI("preview pause grammar not return");
            }
        }

        m_bHavePVR = false;

        if ("adaptation" != strStateName) {
            std::string strPrompt;
            GetPromptByScreenId("VR-SB-01", strPrompt);

            SendVRState("idle", strPrompt, "false", "");
        }
    }

    m_bTslDialog = false;
    m_bWaitGreetingEnd = false;
    m_bDoCancelTslVR = false;
    m_bInEndTask = false;
    m_bSpeakAdaptPrompt = false;

    std::string startNativeVR = "<send event=\"startNativeVR\" target=\"de\"/>";
    m_agents[AgentType_Global]->ProcessMessage(startNativeVR);

    VR_LOGI("m_isIncomingMessage :%d", m_isIncomingMessage);
    if ("in_message" == strStateName) {
        GetPromptByScreenId("VR-HNT-03", VR_VoiceBoxEventSink::m_strPrompt);
        if (!m_isIncomingMessage) {
            VR_VoiceBoxXmlBuilder xmlBulder;
            pugi::xml_node node = xmlBulder.buildStartActionElement("phone", "getMsgDetailStatusNoChange");
            xmlBulder.buildGivenElement(node, "instanceId", m_messageinfo.instanceId, "", "");
            xmlBulder.buildGivenElement(node, "messageId", m_messageinfo.messageId, "", "");
            std::string strReply = xmlBulder.getXmlString();
            OnRequestAction(strReply);
            m_isIncomingMessage = true;
            return true;
        }
        else {
            m_isIncomingMessage = false;
            std::string strResult = "<event-result name=\"startAgent\" errcode=\"\"><agent>";
            strResult.append(strStateName);
            strResult.append("</agent> </event-result>");
            OnRecognized(strResult);
        }
    }

    VoiceMap<std::string, std::string>::iterator iter = m_mapAgenttoRecoState.find(strStateName);
    if (m_mapAgenttoRecoState.end() != iter) {
        m_strCurrentStateName = iter->second;
        if ("adaptation" == strStateName) {
            m_strInitStateName = m_strCurrentStateName;
            m_bSpeakAdaptPrompt = true;
        }
        if ("startAgent" == strActionKey) {
            m_strHintsStateName = VBT_RECO_STATE_GLOBAL;
        }
        else {
            m_strHintsStateName = m_strCurrentStateName;
        }
    }

    VR_LOGI("m_strCurrentStateName :%s", m_strCurrentStateName.c_str());

    if ("startAgent" == strActionKey) {
        if (VR_REGION_US == m_country) {
            VR_LOGI("PVR State %d", m_PVRStateCurrent);
            VR_LOGI("PVR -> VR");
            m_PVRStateCurrent = VR_PVRState_None;
        }

        m_mapAction.clear();

        VR_LOGP("receive startagent event");
        std::string strResult = "<event-result name=\"startAgent\" errcode=\"\"><agent>";
        strResult.append(strStateName);
        strResult.append("</agent> </event-result>");
        OnRecognized(strResult);

        if ("in_call" == strStateName) {
            if (VR_REGION_OC == m_country) {
                VR_LOGI("au-incall");
                VR_VoiceBoxXmlBuilder xmlBulder;
                pugi::xml_node xmlNode = xmlBulder.buildDisplayElement("Common", "ScreenDisplay");
                xmlBulder.buildGivenElement(xmlNode, "agent", "phone", "", "");
                xmlBulder.buildGivenElement(xmlNode, "content", "sendtone_speak_tones", "", "");
                std::string strReply = xmlBulder.getXmlString();
                VR_LOGI("strReply: %s", strReply.c_str());
                OnRecognized(strReply);
            }
            else {
                GetPromptByScreenId("VR-HNT-02", VR_VoiceBoxEventSink::m_strPrompt);
            }
        }
    }

    if ("in_call" != strStateName && "voice_tag" != strStateName && "in_message" != strStateName
        && "adaptation" != strStateName) {
        VR_LOGI("get hints");
        // Prepare the hints
        if (!GetHints()) {
            return false;
        }
    }

    return SendRecogState();
}

void
VR_VoiceBoxEngine::AddRecogItem(
    CVECIPtr<IVECIListItems>& optionalContextList,
    const std::string& strAgent,
    const std::string& strContext)
{
    if (NULL == optionalContextList.ptr()) {
        return;
    }

    CVECIPtr<IVECIParameterSet> spOptional;
    m_client->CreateParameterSet(&spOptional);
    if (NULL == spOptional.ptr()) {
        return;
    }

    spOptional->AddParameter(_T("Agent"), strAgent.c_str(), NULL, NULL);
    spOptional->AddParameter(_T("Context"), strContext.c_str(), NULL, NULL);
    optionalContextList->AddItem(spOptional);
}

bool
VR_VoiceBoxEngine::SendRecogState()
{
    CVECIPtr<IVECITransaction> transaction;

    HRESULT result = S_OK;

    if (VBT_RECO_STATE_VOICE_TAG != m_strCurrentStateName) {
        m_bSpeakOverBeep = true;
    }
    else {
        m_bSpeakOverBeep = false;
    }

    if ("Incoming Message" == m_strCurrentStateName) {
        VR_LOGI("strStateName: %s", m_strCurrentStateName.c_str());
        result = m_engineCommand->SendRecognitionState(
            &transaction,
            m_strCurrentStateName.c_str(),
            BoolTrue,
            VBT_FALSE,
            m_pVariables,
            NULL);
    }
    else {
        if (NULL == m_client.ptr()) {
            return false;
        }

        CVECIPtr<IVECIListItems> optionalContextList;
        m_client->CreateListItems(&optionalContextList);
        if (NULL == optionalContextList.ptr()) {
            return false;
        }

        SetOptionalContext(optionalContextList, m_strCurrentStateName, true);

        result = m_engineCommand->SendRecognitionState(
            &transaction,
            m_strCurrentStateName.c_str(),
            BoolTrue,
            VBT_FALSE,
            NULL,
            optionalContextList);
    }

    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_LOGE("SendRecognitionState Failed, result: %lx", result);
        return false;
    }
    VR_LOGI("result: %lx", result);

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
    }

    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnSendRecognitionState
            )
        );
    return true;
}

void
VR_VoiceBoxEngine::SetOptionalContext(
    CVECIPtr<IVECIListItems>& optionalContextList,
    const std::string& strStateName,
    bool bForSendReco)
{
    if (VR_REGION_US == m_country) {
        if (m_bSongInfoAvailable) {
            AddRecogItem(optionalContextList, "Music", "Song Information Available");
        }

        if (m_messageAvailable) {
            AddRecogItem(optionalContextList, "HFD", "Message Global");
        }

        if (("Global" == strStateName) || (std::string::npos != strStateName.find("Home"))) {
            if (m_isActiveFM) {
                AddRecogItem(optionalContextList, "Radio", "Radio Global Active FM");
            }

            if (m_isActiveAM) {
                AddRecogItem(optionalContextList, "Radio", "Radio Global Active AM");
            }

            if (m_isSatellite) {
                AddRecogItem(optionalContextList, "Radio", "Radio Global Satellite");
            }

            if (m_isActiveFMHD) {
                AddRecogItem(optionalContextList, "Radio", "Radio Global Active FM HD");
            }

            if (m_isActiveSatellite) {
                AddRecogItem(optionalContextList, "Radio", "Radio Global Active Satellite");
            }

            if (m_bHVACBasicActive) {
                AddRecogItem(optionalContextList, "HVAC", "HVAC Global Basic Active");
                AddRecogItem(optionalContextList, "HVAC", "HVAC Global Fan Advanced Active");
                AddRecogItem(optionalContextList, "HVAC", "HVAC Global Fan Basic Active");
                AddRecogItem(optionalContextList, "HVAC", "HVAC Global Temp Advanced Active");
                AddRecogItem(optionalContextList, "HVAC", "HVAC Global Temp Basic Active");
            }

            if (m_bHVACFrontScrnActive) {
                AddRecogItem(optionalContextList, "HVAC", "HVAC Global Front Screen Active");
            }
            if (m_bHVACRearScrnActive) {
                AddRecogItem(optionalContextList, "HVAC", "HVAC Global Rear Screen Active");
            }
            if (m_bHVACSteeringScrnActive) {
                AddRecogItem(optionalContextList, "HVAC", "HVAC Global Steering Screen Active");
            }
            if (m_bHVACConcModeActive) {
                AddRecogItem(optionalContextList, "HVAC", "HVAC Global Concierge Mode Active");
            }
            if (m_bHVACSeatActive) {
                AddRecogItem(optionalContextList, "HVAC", "HVAC Global Seat Active");
            }

            if ("Radio Home" == strStateName) {
                if (m_isActiveFM) {
                    AddRecogItem(optionalContextList, "Radio", "Radio Home Active FM");
                }

                if (m_isActiveAM) {
                    AddRecogItem(optionalContextList, "Radio", "Radio Home Active AM");
                }

                if (m_isSatellite) {
                    AddRecogItem(optionalContextList, "Radio", "Radio Home Satellite");
                }

                if (m_isActiveFMHD) {
                    AddRecogItem(optionalContextList, "Radio", "Radio Home Active FM HD");
                }

                if (m_isActiveSatellite) {
                    AddRecogItem(optionalContextList, "Radio", "Radio Home Active Satellite");
                }
            }

            if ("HVAC Home" == strStateName) {
                if (m_bHVACBasicActive) {
                    AddRecogItem(optionalContextList, "HVAC", "HVAC Home Fan Advanced Active");
                    AddRecogItem(optionalContextList, "HVAC", "HVAC Home Fan Basic Active");
                    AddRecogItem(optionalContextList, "HVAC", "HVAC Home Temp Advanced Active");
                    AddRecogItem(optionalContextList, "HVAC", "HVAC Home Temp Basic Active");
                }
            }

            if ((m_bTslNetworkAvailable && m_bTslAppsAvailable) || bForSendReco) {
                if (("Global" == strStateName) || (std::string::npos != strStateName.find("Home"))) {
                    AddRecogItem(optionalContextList, "Apps", "Apps Global");
                }
            }
        }

    }
    else if (VR_REGION_OC == m_country) {
        VR_ConfigureIF* pcConfig = VR_ConfigureIF::Instance();
        if (NULL != pcConfig) {
            if ((VR_PRODUCT_TYPE_L1 != pcConfig->getVRProduct())
                && (!m_bUpdatingMapData)) {
                AddRecogItem(optionalContextList, "Address Entry", "Address Entry Global");
                AddRecogItem(optionalContextList, "Destination", "In Route");
                AddRecogItem(optionalContextList, "Destination", "Destination Global");
                AddRecogItem(optionalContextList, "Destination", "Destination Menu");
                AddRecogItem(optionalContextList, "POI", "POI Global");

            }
        }
        AddRecogItem(optionalContextList, "Music", "Music Global Named Play Playlist");
        AddRecogItem(optionalContextList, "Music", "Music Global Named Play Song");
        AddRecogItem(optionalContextList, "Music", "Music Global Named");
        if (!bForSendReco || m_bPhoneConnected) {
            AddRecogItem(optionalContextList, "HFD", "HFD Global Named");
        }
    }
    else {

    }

}

std::string
VR_VoiceBoxEngine::GetAdaptationProfilePath(const std::string& strLanguage, const std::string& strProfileName)
{
    VR_LOGD_FUNC();

    std::string strAdaptationPath = VR_ConfigureIF::Instance()->getUsrPath();
    strAdaptationPath.append(strLanguage);

    VR_LOGI("adaptation path : %s", strAdaptationPath.c_str());
    int iMakeDir = mkdir(strAdaptationPath.c_str(), S_IRWXU);
    VR_LOGI("make dir %i", iMakeDir);
    // if (VR_ConfigureIF::Instance()->makeDir(strAdaptationPath)) {
    //     VR_LOGI("make adaptation path OK");
    // }
    // else {
    //     VR_LOGI("make adaptation path ERROR");
    // }
    strAdaptationPath.append("/");
    strAdaptationPath.append(strProfileName);
    strAdaptationPath.append(".spk");
    return strAdaptationPath;
}

bool
VR_VoiceBoxEngine::AdaptationProfileCanbeUsed(const std::string& strFilePath, const std::string& strFullFilePath)
{
    VR_LOGD_FUNC();

    if (!VR_ConfigureIF::Instance()->isFileExist(strFilePath)) {
        VR_LOGI("the file not exit");
        return false;
    }
    else {
        FILE * fp = fopen(strFullFilePath.c_str(), "r");
        if (NULL == fp) {
            VR_LOGI("the fp of the file is null");
            return false;
        }
        fseek (fp, 0, SEEK_END);
        long lSize = ftell(fp);
        if (0 == lSize) {
            VR_LOGI("the file text is null");
            fclose(fp);
            return false;
        }
        else {
            VR_LOGI("the file can be load");
        }
        fclose(fp);
    }

    return true;
}

bool
VR_VoiceBoxEngine::SaveNextRecognitionState(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strStateName = parser.getValueByKey("agent");
    VoiceMap<std::string, std::string>::iterator iter = m_mapAgenttoRecoState.find(strStateName);
    if (m_mapAgenttoRecoState.end() != iter) {
        m_strCurrentStateName = iter->second;
        m_strHintsStateName = m_strCurrentStateName;
    }

    VR_LOGI("get hints");
    // Prepare the hints
    if (!GetHints()) {
        return false;
    }

    m_sessionState = VR_SessionStateAutoSendRecogState;
    return true;
}

bool
VR_VoiceBoxEngine::SaveTuToRecognitionState(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strStateName = parser.getValueByKey("agent");
    VoiceMap<std::string, std::string>::iterator iter = m_mapAgenttoRecoState.find(strStateName);
    if (m_mapAgenttoRecoState.end() != iter) {
        m_strCurrentStateName = iter->second;
        m_strHintsStateName = m_strCurrentStateName;
    }

    // Prepare the hints
    if (!GetHints()) {
        return false;
    }

    m_sessionState = VR_SessionStateAutoTutoSendRecogState;
    return true;
}

bool
VR_VoiceBoxEngine::StartDictation(VR_VoiceBoxXmlParser& parser)
{
    return false;
}

bool
VR_VoiceBoxEngine::ButtonPressed(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    m_bWaitConfirmation = false;
    std::string strKeyValue = parser.getValueByKey("value");

    VR_LOGI("the button event : %s", strKeyValue.c_str());

    std::string strEventResult = "<event-result name=\"buttonPressed\">";
    strEventResult.append(" <keycode value=\"");
    strEventResult.append(strKeyValue);
    strEventResult.append("\"/>");
    strEventResult.append("</event-result>");

    if (!m_strBtnEventName.empty()) {
        VR_LOGI("the last button event is not completed!");
        OnRecognized(strEventResult);
        return true;
    }

    if (m_iDoneBeepID != VR_INVALID_ACTION_ID) {
        VR_LOGI("play done beep, we don't handle any button event");
        OnRecognized(strEventResult);
        return true;
    }

    // when handling start over process, ingore Meter Entry button or start over button
    if (("hard_key_enter_normal_press" == strKeyValue || "start_over" == strKeyValue)
        && (VR_Button_Null != m_enBtnProcessState || IsTaskCompeleteNeedQuit())) {
        VR_LOGI("Now handling start over or cancel process , will ignore this entry button...");
        OnRecognized(strEventResult);
        return true;
    }

    // when handling Meter_Back button or PttLong button, ignore Meter_Back button and Pttlong button
    if (("meter_hard_key_back_normal_press" == strKeyValue || "ptt_hard_key_long_press" == strKeyValue)
        && (m_bCanceled || VR_Button_Cancel == m_enBtnProcessState)) {
        VR_LOGI("Now handling cancel process, will ignore this button...");
        OnRecognized(strEventResult);
        return true;
    }

    if ("ptt_hard_key_press_special_in_endtaskstatus" == strKeyValue) {
        VR_LOGI("PTT during end task!");
        HandlePttDuringEndTask();
        OnRecognized(strEventResult);
        return true;
    }

    if (VR_SessionStateStoped == m_sessionState && "tutorials" != strKeyValue && "voice_training" != strKeyValue && "setting_voice_training_start" != strKeyValue) {
        VR_LOGI("session is stopped!");
        m_strBtnEventName = "";
        OnRecognized(strEventResult);
        return true;
    }

    if (VR_SessionStateBackRestart == m_sessionState) {
        m_bBackInterupted = true;
    }

    if (m_bPlayTTS) {
        if (NULL == m_pcExternalCallback) {
            OnRecognized(strEventResult);
            return false;
        }
        VR_LOGI("playing tts. stop playing");
        m_bPlayTTS = false;
        std::string strStopTTS = "<action agent=\"prompt\" op=\"stopTts\"><reqId>";
        strStopTTS.append(std::to_string(m_iCurTTSReqId));
        strStopTTS.append("</reqId></action>");

        int iStopID = OnRequestAction(strStopTTS);
        VR_LOGI("stop current speak id, %d", m_iCurTTSReqId);
        m_mapStopTTSTransation.insert(std::make_pair(iStopID, m_iCurTTSReqId));

        m_strBtnEventName = strKeyValue;
    }
    else {
        VR_LOGI("no tts , handle");
        // Dispatch the messages
        VoiceMap<std::string, MessageHandler>::const_iterator iterMap = m_mapMsgBtnHandler.find(strKeyValue);
        if (m_mapMsgBtnHandler.cend() != iterMap) {
            if (NULL != iterMap->second) {
                VR_LOGI("btn pressed need to cancel session");
                if (("help" != strKeyValue) && ("setting_voice_training_start" != strKeyValue)) {
                    VoiceList<std::string>::iterator iter = std::find(m_lstNeedReactiveAction.begin(), m_lstNeedReactiveAction.end(), m_strActionType);
                    if (m_bTaskCompelete && iter == m_lstNeedReactiveAction.end()) {
                        VR_LOGI("not need to CancelSession");
                    }
                    else {
                        CancelRecoSession();
                    }
                }
                VR_LOGI("handle the event");
                m_bTTSNull = true;
                (this->*(iterMap->second))(parser);
            }
        }
        m_strBtnEventName = "";
    }

    OnRecognized(strEventResult);
    return true;
}

bool
VR_VoiceBoxEngine::UpdateState(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (m_bInstallingAgent || m_bReInstallTSL) {
        m_listUpdateState.push_back(parser.getXmlString());
        return true;
    }

    VoiceVector<StructNode>::type updateVector = GetSettingsInfo(parser.getXmlString(), "item");
    UpdateStateInfo updateStateInfo;
    std::string strEventResult = "<event-result name=\"updateState\">";
    for (size_t i = 0; i < updateVector.size() - 1; i = i + 2) {
        updateStateInfo.strStateName = updateVector[i].Value;
        updateStateInfo.strStateValue = updateVector[i + 1].Value;
        VR_LOGI("strStateName = %s, strStateValue = %s", updateVector[i].Value.c_str(), updateVector[i + 1].Value.c_str());
        auto iterMap = m_mapUpdateStateHandler.find(updateStateInfo.strStateName);
        if (m_mapUpdateStateHandler.cend() == iterMap || NULL == iterMap->second) {
            continue;
        }

        (this->*(iterMap->second))(updateStateInfo);

        strEventResult.append(" <item key=\"");
        strEventResult.append(updateStateInfo.strStateName);
        strEventResult.append("\" value= \"");
        strEventResult.append(updateStateInfo.strStateValue);
        strEventResult.append("\"/>");
    }

    strEventResult.append("</event-result>");
    OnRecognized(strEventResult);

    return true;
}

bool
VR_VoiceBoxEngine::UpdatePhoneConnState(const UpdateStateInfo& updateStateInfo)
{
    VR_LOGD_FUNC();
    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    HRESULT result = S_OK;
    CVECIPtr<IVECITransaction> transaction;
    if ("disconnected" == updateStateInfo.strStateValue) {
        m_bPhoneConnected = false;
        m_messageAvailable = false;
        m_bSendMsgAvailable = false;

        if (NULL != m_pcCatalogController) {
            m_pcCatalogController->PostStatusMsg("<op name=\"deletephonebuf\"></op>");
        }
        if (NULL != m_pcMsgController) {
            m_pcMsgController->PostMessage("<grammar_disactive agent=\"phone\" sender=\"DE\"></grammar_disactive>");
        }

        result = m_engineCommand->SetPreference(
            &transaction, _T("System"),
            VBT_USR_PREF_SYSTEM_PHONECONNECTED, "False");
        if (FAILED(result) || (NULL == transaction.ptr())) {
            VR_LOGE("SetPreference[PHONECONNECTED]: 0x%lx", result);
        }

        result = m_engineCommand->SetPreference(
            &transaction, _T("HFD"),
            VBT_USR_PREF_HFD_SMSAVAILABLE, "False");
        if (FAILED(result) || (NULL == transaction.ptr())) {
            VR_LOGE("SetPreference[SMSAVAILABLE]: 0x%lx", result);
        }

        if (VR_REGION_US == m_country) {
            result = m_engineCommand->SaveSpeakerProfile(
                     &transaction, m_strAdaptationPath.c_str());
            if (FAILED(result) || (NULL == transaction.ptr())) {
                VR_LOGE("SaveSpeakerProfile: 0x%lx", result);
            }

            result = m_engineCommand->ResetSpeakerProfile(
                &transaction);
            if (FAILED(result) || (NULL == transaction.ptr())) {
                VR_LOGE("ResetSpeakerProfile: 0x%lx", result);
                return false;
            }
        }
    }
    else if ("connected" == updateStateInfo.strStateValue) {
        m_bPhoneConnected = true;
        result = m_engineCommand->SetPreference(
            &transaction, _T("System"),
            VBT_USR_PREF_SYSTEM_PHONECONNECTED, "True");
        if (FAILED(result) || (NULL == transaction.ptr())) {
            VR_LOGE("SetPreference[PHONECONNECTED]: 0x%lx", result);
            return false;
        }
    }
    else {
        if (NULL != m_pcCatalogController) {
            m_pcCatalogController->PostMessage("<grammar_disactive agent=\"phone\" sender=\"DE\"></grammar_disactive>");
        }
    }

    return true;
}

bool
VR_VoiceBoxEngine::UpdatePhoneDeviceId(const UpdateStateInfo& updateStateInfo)
{
    VR_LOGD_FUNC();
    if (NULL == m_pcCatalogPhone) {
        return false;
    }

    std::string strKeyValue = updateStateInfo.strStateValue;
    long int id = std::strtol(strKeyValue.c_str(), NULL, 10);
    std::ostringstream oss;
    oss << id;
    strKeyValue = oss.str();
    VR_LOGI("strKeyValue = %s", strKeyValue.c_str());
    if ("0" == strKeyValue) {
        return false;
    }

    m_deviceAddress = strKeyValue;
    VR_VoiceBoxVoiceTag voiceTag;
    voiceTag.CheckTempVoiceTag(strKeyValue);
    m_pcCatalogPhone->SetDeviceAddress(strKeyValue);

    return true;
}

bool
VR_VoiceBoxEngine::UpdateMusicConnState(const UpdateStateInfo& updateStateInfo)
{
    VR_LOGD_FUNC();
    if ("USB_1_CONNECTED" == updateStateInfo.strStateName) {
        m_bUsb1 = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else if ("USB_2_CONNECTED" == updateStateInfo.strStateName) {
        m_bUsb2 = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else if ("IPOD_1_CONNECTED" == updateStateInfo.strStateName) {
        m_bIpod1 = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else if ("IPOD_2_CONNECTED" == updateStateInfo.strStateName) {
        m_bIpod2 = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    if ("BTAUDIO_CONNECTED" == updateStateInfo.strStateName) {
        m_bBtAudio = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else {

    }

    if (m_bUsb1 || m_bUsb2 || m_bIpod1 || m_bIpod2) {
        VR_LOGI("has usb or ipod connected");
        m_strUsbOrIpodConnected = "True";
    }
    else {
        VR_LOGI("has not usb or ipod connected");
        m_strUsbOrIpodConnected = "False";
        if (NULL == m_engineCommand.ptr()) {
            VR_LOGE("m_engineCommand is NULL");
            return false;
        }

        CVECIPtr<IVECITransaction> transaction;
        HRESULT result = m_engineCommand->SetPreference(&transaction, _T("Music"),
            VBT_USR_PREF_MUSIC_AUDIOSOURCECONNECTED, "False");
        if (FAILED(result) || (NULL == transaction.ptr())) {
            VR_LOGE("SetPreference[AUDIOSOURCECONNECTED]: 0x%lx", result);
            return false;
        }
    }

    return true;
}

bool
VR_VoiceBoxEngine::UpdateGraceNoteMagicNum(const UpdateStateInfo& updateStateInfo)
{
    VR_LOGD_FUNC();
    if (NULL == m_engineCommand.ptr()) {
        VR_LOGE("m_engineCommand is NULL");
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SetSystemParameter(
        &transaction, _T("SpeechAndSound"),
        VBT_SYS_PREF_SPEECHANDSOUND_GRACENOTEMAGICNUMBER,
        updateStateInfo.strStateValue.c_str());
    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_LOGE("SetSystemParameter[GRACENOTEMAGICNUMBER]: 0x%lx", result);
        return false;
    }

    return true;
}

bool
VR_VoiceBoxEngine::UpdateTSLAvailable(const UpdateStateInfo& updateStateInfo)
{
    VR_LOGD_FUNC();
    std::string strAppAvailable = "False";
    if ("TSL_STATE_AVAILABLE" == updateStateInfo.strStateName) {
        if ("true" == updateStateInfo.strStateValue) {
            m_bTslNetworkAvailable = true;
            if (m_bTslAppsAvailable) {
                strAppAvailable = "True";
            }
        }
        else {
            m_bTslNetworkAvailable = false;
            strAppAvailable = "False";
        }
    }
    else if ("TSL_APPS_AVAILABLE" == updateStateInfo.strStateName) {
        if ("true" == updateStateInfo.strStateValue) {
            m_bTslAppsAvailable = true;
            if (m_bTslNetworkAvailable) {
                strAppAvailable = "True";
            }
        }
        else {
            m_bTslAppsAvailable = false;
            strAppAvailable = "False";
        }
    }
    else {

    }

    if (NULL == m_engineCommand.ptr()) {
        VR_LOGE("m_engineCommand is NULL");
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SetPreference(
        &transaction, VBT_AGENT_APPS,
        VBT_USR_PREF_APPS_APPSAVAILABLE, strAppAvailable.c_str());
    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_LOGE("SetPreference[APPSAVAILABLE]: 0x%lx", result);
        return false;
    }

    return true;
}

bool
VR_VoiceBoxEngine::UpdateTSLList(const UpdateStateInfo& updateStateInfo)
{
    VR_LOGD_FUNC();

    VR_LOGI("AppList = %s", updateStateInfo.strStateValue.c_str());
    if (!updateStateInfo.strStateValue.empty()) {
        OnRequest(updateStateInfo.strStateValue);
    }

    return true;
}

bool
VR_VoiceBoxEngine::UpdateOperestrict(const UpdateStateInfo& updateStateInfo)
{
    VR_LOGD_FUNC();
    if ("on" == updateStateInfo.strStateValue) {
        if (VBT_RECO_STATE_SPEAKER_ADAPTATION == m_strCurrentStateName) {
            StopTraining();
        }
    }
    else {
        VR_LOGI("strStateValue = %s, m_strCurrentStateName = %s", updateStateInfo.strStateValue.c_str(), m_strCurrentStateName.c_str());
    }

    return true;
}

bool
VR_VoiceBoxEngine::UpdateMessageState(const UpdateStateInfo& updateStateInfo)
{
    VR_LOGD_FUNC();

    VR_LOGI("strStateValue = %s", updateStateInfo.strStateValue.c_str());
    if (!updateStateInfo.strStateValue.empty()) {
        OnRequest(updateStateInfo.strStateValue);
    }

    return true;
}

bool
VR_VoiceBoxEngine::UpdateMusicState(const UpdateStateInfo& updateStateInfo)
{
    VR_LOGD_FUNC();
    m_bSongInfoAvailable = ("true" == updateStateInfo.strStateValue) ? true : false;

    return true;
}

bool
VR_VoiceBoxEngine::UpdateRadioState(const UpdateStateInfo& updateStateInfo)
{
    VR_LOGD_FUNC();
    if ("RADIO_STATE_FM_ACTIVE" == updateStateInfo.strStateName) {
        m_isActiveFM = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else if ("RADIO_STATE_AM_ACTIVE" == updateStateInfo.strStateName) {
        m_isActiveAM = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else if ("RADIO_XM_CONNECTED" == updateStateInfo.strStateName) {
        m_isSatellite = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else if ("RADIO_STATE_FM_HD_ACTIVE" == updateStateInfo.strStateName) {
        m_isActiveFMHD = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else if ("RADIO_STATE_XM_ACTIVE" == updateStateInfo.strStateName) {
        m_isActiveSatellite = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else {

    }

    return true;
}

bool
VR_VoiceBoxEngine::UpdateHVACFanTempRange(const UpdateStateInfo& updateStateInfo)
{
    VR_LOGD_FUNC();
    if (NULL == m_engineCommand.ptr()) {
        VR_LOGE("m_engineCommand is NULL");
        return false;
    }

    std::string strPreferenceName;
    if ("CLIMATE_FANSPEED_MAX" == updateStateInfo.strStateName) {
        strPreferenceName = VBT_USR_PREF_HVAC_MAXFANSPEED;
    }
    else if ("CLIMATE_TEMPERATURE_MIN" == updateStateInfo.strStateName) {
        strPreferenceName = VBT_USR_PREF_HVAC_MINTEMPERATURE;
        const int iHVACFahrenheitMin = 50;
        int iHVACFahrenheit = std::atoi(updateStateInfo.strStateValue.c_str());
        m_bHVACFahrenheit = (iHVACFahrenheit >= iHVACFahrenheitMin) ? true : false;
    }
    else if ("CLIMATE_TEMPERATURE_MAX" == updateStateInfo.strStateName) {
        strPreferenceName = VBT_USR_PREF_HVAC_MAXTEMPERATURE;
    }
    else {
        return true;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SetPreference(
        &transaction, _T("HVAC"),
        strPreferenceName.c_str(), updateStateInfo.strStateValue.c_str());
    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_LOGE("SetPreference[FANSPEED|TEMPERATURE range]: 0x%lx", result);
        return false;
    }

    return true;
}

bool
VR_VoiceBoxEngine::IsHVACFahrenheit()
{
    return m_bHVACFahrenheit;
}

bool
VR_VoiceBoxEngine::UpdateHVACState(const UpdateStateInfo& updateStateInfo)
{
    VR_LOGD_FUNC();
    if ("CLIMATE_BASIC_ACTIVE" == updateStateInfo.strStateName) {
        m_bHVACBasicActive = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else if ("CLIMATE_FRONT_SCREEN_ACTIVE" == updateStateInfo.strStateName) {
        m_bHVACFrontScrnActive = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else if ("CLIMATE_REAR_SCREEN_ACTIVE" == updateStateInfo.strStateName) {
        m_bHVACRearScrnActive = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else if ("CLIMATE_STEERING_SCREEN_ACTIVE" == updateStateInfo.strStateName) {
        m_bHVACSteeringScrnActive = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else if ("CLIMATE_LEXUS_CONCIERGE_SCREEN_ACTIVE" == updateStateInfo.strStateName) {
        m_bHVACLexusConciergeScreenActive = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else if ("CLIMATE_CONCIERGE_ACTIVE" == updateStateInfo.strStateName) {
        m_bHVACConciergeActive = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else if ("CLIMATE_FRONT_SEAT_SCREEN_ACTIVE" == updateStateInfo.strStateName) {
        m_bHVACFrontSeatScreenActive = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else if ("CLIMATE_REAR_SEAT_SCREEN_ACTIVE" == updateStateInfo.strStateName) {
        m_bHVACRearSeatScreenActive = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else if ("CLIMATE_SEAT_OPERATION_SCREEN_ACTIVE" == updateStateInfo.strStateName) {
        m_bHVACSeatOperationScreenActive = ("true" == updateStateInfo.strStateValue) ? true : false;
    }
    else {

    }

    m_bHVACConcModeActive = m_bHVACLexusConciergeScreenActive && m_bHVACConciergeActive;
    m_bHVACSeatActive = m_bHVACFrontSeatScreenActive && m_bHVACRearSeatScreenActive && m_bHVACSeatOperationScreenActive;

    return true;
}

bool
VR_VoiceBoxEngine::UpdatePhoneRegistered(const UpdateStateInfo& updateStateInfo)
{
    VR_LOGD_FUNC();
    if (NULL == m_engineCommand.ptr()) {
        VR_LOGE("m_engineCommand is NULL");
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    long int id = std::strtol(updateStateInfo.strStateValue.c_str(), NULL, 10);
    std::ostringstream oss;
    oss << id;
    std::string strPhoneRegistered = oss.str();
    VR_LOGI("strPhoneRegistered: %s", strPhoneRegistered.c_str());
    std::string strRegisteredValue = ("0" == strPhoneRegistered) ? "False" : "True";
    HRESULT result = m_engineCommand->SetPreference(&transaction, _T("System"),
        _T("PhoneRegistered"), strRegisteredValue.c_str());
    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_LOGE("SetPreference[PhoneRegistered]: 0x%lx", result);
        return false;
    }

    return true;
}

bool
VR_VoiceBoxEngine::UpdatePhoneSteeringType(const UpdateStateInfo& updateStateInfo)
{
    VR_LOGD_FUNC();
    if (NULL == m_engineCommand.ptr()) {
        VR_LOGE("m_engineCommand is NULL");
        return false;
    }

    HRESULT result = S_OK;
    CVECIPtr<IVECITransaction> transaction;
    if ("TEL" == updateStateInfo.strStateValue) {
        result = m_engineCommand->SetPreference(
            &transaction, _T("HFD"),
            _T("PhoneOffHook"), "False");
        if (FAILED(result) || (NULL == transaction.ptr())) {
            VR_LOGE("SetPreference[PhoneOffHook]: 0x%lx", result);
            return false;
        }
    }
     else if ("OFFHOOK" == updateStateInfo.strStateValue) {
        result = m_engineCommand->SetPreference(
            &transaction, _T("HFD"),
            _T("PhoneOffHook"), "True");
        if (FAILED(result) || (NULL == transaction.ptr())) {
            VR_LOGE("SetPreference[PhoneOffHook]: 0x%lx", result);
            return false;
        }
    }
    else {
        VR_LOGI("invaid parameter");
    }

    return true;
}


void
VR_VoiceBoxEngine::MessageAvaiableState(const std::string& message)
{
    VR_LOGD_FUNC();

    VR_VoiceBoxXmlParser parser(message);
    std::string strValue = parser.getValueByKey("value");
    VR_LOGI("strValue : %s", strValue.c_str());
    if ("1" == strValue) {
        m_messageAvailable = true;
    }
    else {
        m_messageAvailable = false;
        m_bSendMsgAvailable = false;
    }

}

void
VR_VoiceBoxEngine::SendMessageAvaiableState(const std::string& message)
{
    VR_LOGD_FUNC();

    pugi::xml_document doc;
    if (!doc.load_string(message.c_str())) {
        VR_LOGI("load failed");
        return;
    }

    std::string strInstanceId;
    std::string strMessageType;
    std::string strOutboxState = "False";
    pugi::xpath_node_set nodeSet = doc.select_nodes("//instanceId");
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); nodeSet.end() != it; ++it) {
        pugi::xml_node paramNode = it->node();
        for (pugi::xml_attribute attr = paramNode.first_attribute(); attr; attr = attr.next_attribute()) {
            if (0 == strcmp("id", attr.name())) {
                strInstanceId = attr.value();
            }
            if (0 == strcmp("messageType", attr.name())) {
                strMessageType = attr.value();
            }
            if (0 == strcmp("outbox", attr.name())) {
                strOutboxState = attr.value();
            }
        }
        long int MessageTypeId = std::strtol(strMessageType.c_str(), NULL, 10);
        VR_LOGI("MessageTypeId : %d", MessageTypeId);

        // BtMapMessageSupportType_SMS_GSM = 0x02 | BtMapMessageSupportType_SMS_CDMA = 0x04,
        const int BtMapMessageSupportType_SMS_GSM = 2;
        const int BtMapMessageSupportType_SMS_CDMA = 4;
        if (BtMapMessageSupportType_SMS_GSM & MessageTypeId) {
            strMessageType = "2";
            break;
        }
        else if (BtMapMessageSupportType_SMS_CDMA & MessageTypeId) {
            strMessageType = "4";
            break;
        }
        else {
            VR_LOGI("this messagetype is not support sms");
        }
    }

    strOutboxState = ("1" == strOutboxState) ? "True" : "False";
    std::string strMessage = "<action op=\"SendMessageInfo\"><Message id=\"";
    strMessage.append(strInstanceId);
    strMessage.append("\" messageType= \"");
    strMessage.append(strMessageType);
    strMessage.append("\"/>");
    strMessage.append("</action>");
    if (NULL != m_agents[AgentType_Phone]) {
        m_agents[AgentType_Phone]->ProcessMessage(strMessage);
    }

    m_bSendMsgAvailable = ("True" == strOutboxState) ? true : false;
    VR_LOGI("m_bSendMsgAvailable : %d", m_bSendMsgAvailable);
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SetPreference(
                     &transaction,
                     _T("HFD"),
                     VBT_USR_PREF_HFD_SMSAVAILABLE,
                     strOutboxState.c_str());
    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_LOGI("set phone smsavailable error");
    }
}

void
VR_VoiceBoxEngine::SetSpeakerProfile(const std::string& strKeyValue)
{
    VR_LOGD_FUNC();

    if (VR_REGION_US == m_country) {
        if (NULL == m_engineCommand.ptr()) {
            return;
        }

        m_strDevice.clear();
        m_strAdaptationPath.clear();
        m_strDevice = strKeyValue;
        m_strAdaptationPath = GetAdaptationProfilePath(m_strCultureName, m_strDevice);
        std::string relativePath = m_strCultureName + "/" + m_strDevice + ".spk";
        VR_LOGI("relativePath : %s", relativePath.c_str());
        if (AdaptationProfileCanbeUsed(relativePath, m_strAdaptationPath)) {
            CVECIPtr<IVECITransaction> transaction;
            HRESULT result = m_engineCommand->LoadSpeakerProfile(
                             &transaction,
             m_strAdaptationPath.c_str());
            if (FAILED(result) || (NULL == transaction.ptr())) {
                result = m_engineCommand->ResetSpeakerProfile(
                         &transaction);
                if (FAILED(result) || (NULL == transaction.ptr())) {
                    return;
                }
            }
        }
        else {
            VR_LOGI("the training file is not be loaded");
        }
    }

}

bool
VR_VoiceBoxEngine::ChangeSettings(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    VoiceVector<StructNode>::type settingVector = GetSettingsInfo(parser.getXmlString(), "param");
    std::string strKeyValue;
    // the size of updatevector is odd number
    for (size_t i = 0; i < settingVector.size() - 1; i = i + 2) {
        VR_LOGI("updateVector.Value: %s", settingVector[i].Value.c_str());
        VR_LOGI("updateVector.Value: %s", settingVector[i + 1].Value.c_str());
        if ("promptLevel" == settingVector[i].Value) {
            VR_LOGI("updateVector[i].Value   come on");
            VR_LOGI("updateVector[i].Value: %s", settingVector[i].Value.c_str());
            strKeyValue = settingVector[i + 1].Value;
            SavePromptLevel(strKeyValue);
            VR_LOGI("strKeyValue: %s", strKeyValue.c_str());
            if ("OFF" == strKeyValue) {
                VR_LOGI("strKeyValue: %s", strKeyValue.c_str());
                setPromptLevel(VR_PROMPTLEVEL_OFF);
            }
            else if ("SIMPLE" == strKeyValue) {
                VR_LOGI("strKeyValue: %s", strKeyValue.c_str());
                setPromptLevel(VR_PROMPTLEVEL_LOW);
            }
            else if ("FULL" == strKeyValue) {
                VR_LOGI("strKeyValue: %s", strKeyValue.c_str());
                setPromptLevel(VR_PROMPTLEVEL_HIGH);
            }
        }
    }

    std::string strEventResult = "<event-result name=\"changeSettings\">";
    // the size of updatevector is odd number
    for (size_t i = 0; i < settingVector.size() - 1; i = i + 2) {
        strEventResult.append(" <param name=\"");
        strEventResult.append(settingVector[i].Value);
        strEventResult.append("\" value= \"");
        strEventResult.append(settingVector[i+1].Value);
        strEventResult.append("\"/>");
    }
    strEventResult.append("</event-result>");
    VR_LOGI("strEventResult: %s", strEventResult.c_str());
    OnRecognized(strEventResult);
    VR_LOGI("changeSettings");
    return false;
}

VoiceVector<StructNode>::type
VR_VoiceBoxEngine::GetSettingsInfo(const std::string& message, const std::string& item)
{
    VR_LOGD_FUNC();

    VoiceVector<StructNode>::type promptVector;
    StructNode structNode;
    pugi::xml_document doc;
    if (!doc.load_string(message.c_str())) {
            VR_LOGI("load failed");
            return promptVector;
    }

    pugi::xpath_node_set nodeSet = doc.select_nodes((std::string("//") + item).c_str());
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); nodeSet.end() != it; ++it) {
        pugi::xml_node paramNode = it->node();
        for (pugi::xml_attribute attr = paramNode.first_attribute(); attr; attr = attr.next_attribute()) {
            structNode.Name = attr.name();
            structNode.Value = attr.value();
            VR_LOGI("name : %s", structNode.Name.c_str());
            VR_LOGI("Value : %s", structNode.Value.c_str());
            promptVector.push_back(structNode);
        }
    }

    return promptVector;
}

bool
VR_VoiceBoxEngine::ChangeLanguage(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    if (NULL == m_engineCommand) {
        return false;
    }
    SendDEStatus("voicetag", "1");
    std::string strLanguage = parser.getValueByKey("language");
    if ("en-us" == strLanguage || "fr-ca" == strLanguage || "es-mx" == strLanguage) {
        m_lstLanguage.push_back(strLanguage);
    }
    else {
        VR_LOGI("the language is error");
        return false;
    }

    if (m_bInstallingAgent || m_bReInstallTSL) {
        VR_LOGI("TSL is intalling, after intalled, changed language");
        return true;
    }

    if (!m_bEngineStarting) {
        VR_LOGI("engine isn't started");
        return true;
    }

    SendDEStatus("engine", "1");
    auto pairTransaction = GetCurrentTransaction();
    if (NULL != m_pcCatalogManager && !pairTransaction.first.empty() && !pairTransaction.second.empty()) {
        m_pcCatalogManager->CancelGrammarUpdate(pairTransaction.first, true);
    }

    if (VR_REGION_US == m_country) {
        CVECIPtr<IVECITransaction> transaction;
        HRESULT result = m_engineCommand->SaveSpeakerProfile(
                         &transaction,
                         m_strAdaptationPath.c_str());
        if (FAILED(result) || (NULL == transaction.ptr())) {
            VR_LOGI("SaveSpeakerProfile failed");
        }
    }

    CancelRecoSession();

    m_strLanguage = strLanguage;
    m_bChangeLanguage = true;

    if ("en-us" == strLanguage) {
        Restart("en-US");
    }
    else if ("fr-ca" == strLanguage) {
        Restart("fr-CA");
    }
    else if ("es-mx" == strLanguage) {
        Restart("es-MX");
    }
    else {

    }

    std::string notifyChangeLanguage = "<action-result agent=\"phone\" op=\"changeLanguage\"><language>";
    notifyChangeLanguage.append(strLanguage);
    notifyChangeLanguage.append("</language></action-result>");
    VR_VoiceBoxXmlParser tmpParser(notifyChangeLanguage);
    m_agents[AgentType_Phone]->ReplyQueryInfo(tmpParser);

    return false;
}

bool
VR_VoiceBoxEngine::ChangeLanguageInner(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand) {
        return false;
    }

    std::string strLanguage = parser.getValueByKey("language");
    m_strLanguage = strLanguage;
    m_bChangeLanguage = true;

    if ("en-us" == strLanguage) {
        Restart("en-US");
    }
    else if ("fr-ca" == strLanguage) {
        Restart("fr-CA");
    }
    else if ("es-mx" == strLanguage) {
        Restart("es-MX");
    }
    else {

    }

    std::string notifyChangeLanguage = "<action-result agent=\"phone\" op=\"changeLanguage\"><language>";
    notifyChangeLanguage.append(strLanguage);
    notifyChangeLanguage.append("</language></action-result>");
    VR_VoiceBoxXmlParser tmpParser(notifyChangeLanguage);
    m_agents[AgentType_Phone]->ReplyQueryInfo(tmpParser);
    return true;
}

bool
VR_VoiceBoxEngine::FullUpdateNotify(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    std::string strStatus = parser.getValueByKey("status");
    VR_LOGI("status : %s", strStatus.c_str());

    std::string strEventResult = "<event-result name=\"fullupdateNotify\">";
    strEventResult.append("<status>");
    strEventResult.append(strStatus);
    strEventResult.append("</status>");
    strEventResult.append("</event-result>");
    OnRecognized(strEventResult);

    if (VR_REGION_OC != m_country) {
        VR_LOGI("country is error");
        return false;
    }

    if ("off" == strStatus) {
        m_bUpdatingMapData = false;
    }
    else if ("navifulldata" == strStatus) {
        m_bUpdatingMapData = true;

        if ((VR_SessionStateStoped != m_sessionState)
            && (VR_SessionStateNone != m_sessionState)
            && (VR_INVALID_ACTION_ID == m_iDoneBeepID)) {
            StopAllTTS();
            OnBeep(VR_BeepType_Done);

            CancelRecoSession();
        }

        OnRequest("<grammar_delete agent=\"poi\" />");
    }
    else if ("finished" == strStatus) {
        m_bUpdatingMapData = false;

        if ((VR_SessionStateStoped != m_sessionState)
            && (VR_SessionStateNone != m_sessionState)
            && (VR_INVALID_ACTION_ID == m_iDoneBeepID)) {
            StopAllTTS();
            OnBeep(VR_BeepType_Done);

            CancelRecoSession();
        }

        VR_ConfigureIF* pcConfig = VR_ConfigureIF::Instance();
        std::string strLanguage = pcConfig->getVRLanguage();

        if (VR_LANGUAGE_EN_AU == strLanguage) {
            Restart("en-AU");
        }
    }
    else {
        VR_LOGI("status is out of our choose");
        return false;
    }

    return true;
}

bool
VR_VoiceBoxEngine::GetResourceState(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strState = "0";

    if (!m_lstCancelOption.empty()) {
        VR_LOGI("Canceling");
        strState = "2";
    }
    // else if (bAddingGrammar && !m_bEngineStarting) {
    //     // VR_LOGI("adding grammar and engine is stoped");
    //     VR_LOGI("engine is stoped");
    //     strState = "3";
    // }
    else if (!m_bEngineStarting) {
        VR_LOGI("engine is stoped");
        strState = "2";
    }
    else if (m_bInstallingAgent || m_bReInstallTSL) {
        VR_LOGI("engine is installing agents");
        strState = "2";
    }
    else {
        VR_LOGI("state is OK");
    }

    std::string strEventResult = "<event-result name=\"getResourceState\">";
    strEventResult.append("<state>");
    strEventResult.append(strState);
    strEventResult.append("</state>");
    strEventResult.append("</event-result>");

    OnRecognized(strEventResult);
    return true;
}

bool
VR_VoiceBoxEngine::PttShort(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    VR_LOGI("current session : %d", m_sessionState);
    VoiceList<std::string>::iterator iter = std::find(m_lstNeedReactiveAction.begin(), m_lstNeedReactiveAction.end(), m_strActionType);
    if (VR_SessionStatePaused == m_sessionState) {
        Resume(parser);
    }
    else if (VR_SessionStateResume == m_sessionState) {
        StartSession();
    }
    else if (VR_SessionStateRestartSessionAfterTTS == m_sessionState) {
        StartSession();
    }
    else if (VR_SessionStateAutoSendRecogState == m_sessionState) {
        VR_LOGI("menu clicked, stop tts. when session over, send recog state");
    }
    else if (VR_SessionStateAutoTutoSendRecogState == m_sessionState) {
        SendRecogState();
        m_sessionState = VR_SessionStateTemporarilyCanceled;
    }
    else if (VR_SessionStateBackRestart == m_sessionState) {
        VR_LOGI("back wait for startrecosession");
    }
    else if (m_bTaskCompelete && iter == m_lstNeedReactiveAction.end()) {
        VR_LOGI("need to quit VR");
    }
    else if (VR_SessionStateSendRecogAfterTTS == m_sessionState || VR_SessionStateStartOver == m_sessionState) {
        VR_LOGI("need to display hint");
    }
    else {
        m_sessionState = VR_SessionStateAutoRestartSession;
    }

    return true;
}

bool
VR_VoiceBoxEngine::PttLong(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    m_enBtnProcessState = VR_Button_Cancel;

    if (IsTaskCompeleteNeedQuit()) {
        VR_LOGI("this ptt long button do not need handle");
        return true;
    }

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }
    SendVRState("failed", "", "false", "");

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     _T("System"),
                     VBT_COMMAND_SYSTEM_CANCEL,
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    m_mapCmdResultHandler.insert(
            std::make_pair(
                strTrans.Get(),
                &VR_VoiceBoxEngine::OnPttLong
                )
            );
    m_sessionState = VR_SessionStatePttLong;

    return true;
}

bool
VR_VoiceBoxEngine::EntryNormalPress(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }
    m_enBtnProcessState = VR_Button_StartOver;

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     _T("System"),
                     VBT_COMMAND_SYSTEM_START_OVER,
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    m_mapCmdResultHandler.insert(
            std::make_pair(
                strTrans.Get(),
                &VR_VoiceBoxEngine::OnStartOver
                )
            );

    return true;
}

bool
VR_VoiceBoxEngine::BackNormalPress(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    m_enBtnProcessState = VR_Button_Cancel;
    if (IsTaskCompeleteNeedQuit()) {
        VR_LOGI("this Meter back button do not need handle");
        return true;
    }

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    SendVRState("quit", "", "false", "");

    // CVECIPtr<IVECITransaction> transaction;
    // HRESULT result = m_engineCommand->SendCommand(
    //                  &transaction,
    //                  _T("System"),
    //                  VBT_COMMAND_SYSTEM_CANCEL,
    //                  VBT_FALSE,
    //                  NULL,
    //                  NULL);
    // if (FAILED(result) || (NULL == transaction.ptr())) {
    //     return false;
    // }

    // CVECIOutStr strTrans;
    // result = transaction->GetTransactionId(&strTrans);
    // if (FAILED(result)) {
    //     return false;
    // }

    // m_mapCmdResultHandler.insert(
    //         std::make_pair(
    //             strTrans.Get(),
    //             &VR_VoiceBoxEngine::OnPttLong
    //             )
    //         );
    m_sessionState = VR_SessionStatePttLong;
    OnPttLong(parser);
    return true;
}

bool
VR_VoiceBoxEngine::SelectOne(VR_VoiceBoxXmlParser& parser)
{
    return Select(0);
}


bool
VR_VoiceBoxEngine::SelectTwo(VR_VoiceBoxXmlParser& parser)
{
    return Select(1);
}


bool
VR_VoiceBoxEngine::SelectThree(VR_VoiceBoxXmlParser& parser)
{
    return Select(2);
}


bool
VR_VoiceBoxEngine::SelectFour(VR_VoiceBoxXmlParser& parser)
{
    return Select(3);
}


bool
VR_VoiceBoxEngine::SelectFive(VR_VoiceBoxXmlParser& parser)
{
    return Select(4);
}

bool
VR_VoiceBoxEngine::Select(int iIndex)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SelectListItem(
                     &transaction,
                     (VBT_ULONG)iIndex);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    VR_LOGI("current string : %s", strTrans.Get());
    m_mapCmdResultHandler.insert(
            std::make_pair(
                strTrans.Get(),
                &VR_VoiceBoxEngine::OnSelect
                )
            );
    return true;
}


bool
VR_VoiceBoxEngine::Phone(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strStartAgent = "<event name=\"startCurrentAgent\">";
    strStartAgent.append("<agent>phone</agent>");
    strStartAgent.append("</event>");

    OnRequest(strStartAgent);
    return true;
}

bool
VR_VoiceBoxEngine::Navigation(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strStartAgent = "<event name=\"startCurrentAgent\">";
    strStartAgent.append("<agent>navi</agent>");
    strStartAgent.append("</event>");

    OnRequest(strStartAgent);
    return true;
}

bool
VR_VoiceBoxEngine::Apps(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strStartAgent = "<event name=\"startCurrentAgent\">";
    strStartAgent.append("<agent>apps</agent>");
    strStartAgent.append("</event>");

    OnRequest(strStartAgent);
    return true;
}

bool
VR_VoiceBoxEngine::Audio(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    VR_LOGP("DE: start go to audio button command...case : 215-6");


    std::string strStartAgent = "<event name=\"startCurrentAgent\">";
    strStartAgent.append("<agent>music</agent>");
    strStartAgent.append("</event>");

    OnRequest(strStartAgent);
    return true;
}

bool
VR_VoiceBoxEngine::Info(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strStartAgent = "<event name=\"startCurrentAgent\">";
    strStartAgent.append("<agent>information</agent>");
    strStartAgent.append("</event>");

    OnRequest(strStartAgent);
    return true;
}

bool
VR_VoiceBoxEngine::Climate(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    OnRequest("<event name=\"startCurrentAgent\"> <agent>climate</agent> </event>");

    return true;
}

bool
VR_VoiceBoxEngine::SpeakAdaptation(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strStartAgent = "<event name=\"startCurrentAgent\">";
    strStartAgent.append("<agent>adaptation</agent>");
    strStartAgent.append("</event>");

    OnRequest(strStartAgent);
    VR_LOGI("speak adaptation: %s", strStartAgent.c_str());

    return true;
}

bool
VR_VoiceBoxEngine::HandlePause(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     _T("System"),
                     VBT_COMMAND_SYSTEM_PAUSE,
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }
    m_sessionState = VR_SessionStatePaused;

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
    }
    return true;
}

bool
VR_VoiceBoxEngine::Pause(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    m_enBtnProcessState = VR_Button_Null;

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }
    m_sessionState = VR_SessionStatePaused;

    return true;
}

bool
VR_VoiceBoxEngine::Resume(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->Resume(
                     &transaction,
                     VBT_TRUE);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
        m_lstTransaction.push_front(m_strCurrentTransactionID);
    }

    m_sessionState = VR_SessionStateResume;
    VR_LOGI("resume strTrans: %s", strTrans.Get());
    m_mapCmdResultHandler.insert(
            std::make_pair(
                strTrans.Get(),
                &VR_VoiceBoxEngine::OnStartRecoSession
                )
            );
    return true;
}

bool
VR_VoiceBoxEngine::NextPage(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    std::string strAgent;
    std::string strCommand;
    if (m_bMoreHint) {
        strAgent = VBT_AGENT_HELP;
        strCommand = "Hints Navigation - Next Page";
    }
    else {
        strAgent = VBT_AGENT_SYSTEM;
        strCommand = VBT_COMMAND_SYSTEM_GLOBAL_NEXT_PAGE;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     strAgent.c_str(),
                     strCommand.c_str(),
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
    }

    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnNextPage
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::PrevPage(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    std::string strAgent;
    std::string strCommand;
    if (m_bMoreHint) {
        strAgent = VBT_AGENT_HELP;
        strCommand = "Hints Navigation - Previous Page";
    }
    else {
        strAgent = VBT_AGENT_SYSTEM;
        strCommand = VBT_COMMAND_SYSTEM_GLOBAL_PREVIOUS_PAGE;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     strAgent.c_str(),
                     strCommand.c_str(),
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
    }

    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnPrevPage
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::LastPage(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     _T("System"),
                     VBT_COMMAND_SYSTEM_GLOBAL_LAST_PAGE,
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
    }
    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnLastPage
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::FirstPage(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     _T("System"),
                     VBT_COMMAND_SYSTEM_GLOBAL_FIRST_PAGE,
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
    }
    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnFristPage
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::ConfirmYes(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = S_OK;
    if (InScreen("help_tutorial_confirmation")) {
        result = m_engineCommand->SendCommand(
                 &transaction,
                 _T("Help"),
                 VBT_COMMAND_CONFIRM_TUTORIAL_CONFIRM,
                 VBT_FALSE,
                 NULL,
                 NULL);
    }
    else {
        result = m_engineCommand->SendCommand(
                 &transaction,
                 _T("System"),
                 VBT_COMMAND_SYSTEM_GLOBAL_YES,
                 VBT_FALSE,
                 NULL,
                 NULL);
    }
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
    }
    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnConfirmYes
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::GoDirectly(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strCommand;

    if ("Active Route Confirmation" == m_strActionType) {
        strCommand = "Select User Intent - Replace Current Route";
    }
    else if ("Select Preset Assignment" == m_strActionType) {
        strCommand = "Select Preset Assignment - Destination";
    }
    else if ("Select Previous Destination Assignment" == m_strActionType) {
        strCommand = "Select Previous Destination Assignment - Destination";
    }
    else if ("Get Assignment" == m_strActionType) {
        strCommand = "Get Assignment - Replace Route";
    }
    else {
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     m_strAgentName.c_str(),
                     strCommand.c_str(),
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
    }

    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnGoDirectly
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::AddToRoute(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strCommand;
    if ("Active Route Confirmation" == m_strActionType) {
        strCommand = "Select User Intent - Add to Route";
    }
    else if ("Select Preset Assignment" == m_strActionType) {
        strCommand = "Select Preset Assignment - Waypoint";
    }
    else if ("Select Previous Destination Assignment" == m_strActionType) {
        strCommand = "Select Previous Destination Assignment - Waypoint";
    }
    else if ("Get Assignment" == m_strActionType) {
        strCommand = "Get Assignment - Add to Route";
    }
    else {
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     m_strAgentName.c_str(),
                     strCommand.c_str(),
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
    }

    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnAddToRoute
            )
        );
    return true;
}

bool
VR_VoiceBoxEngine::Call(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }
    VR_LOGI("VR_VoiceBoxEngine::Call");
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = S_OK;
    VR_LOGI("m_strScreenState: %s", m_strScreenState.c_str());
    if (InScreen("phone_speak_digits")) {
        result = m_engineCommand->SendCommand(
                 &transaction,
                 _T("HFD"),
                 VBT_COMMAND_DIGITS_FOLLOW_UP_CALL,
                 VBT_FALSE,
                 NULL,
                 NULL);
    }
    else if (InScreen("phone_one_call_contact_message_selected")) {
        VR_LOGI("m_isNameNull: %d", m_isNameNull);
        if (m_isNameNull) {
            result = m_engineCommand->SendCommand(
                     &transaction,
                     _T("HFD"),
                     VBT_COMMAND_ONE_CALL_SELECTED_CALL,
                     VBT_FALSE,
                     NULL,
                     NULL);
        }
        else {
            result = m_engineCommand->SendCommand(
                     &transaction,
                     _T("HFD"),
                     VBT_COMMAND_ONE_CONTACT_SELECTED_CALL,
                     VBT_FALSE,
                     NULL,
                     NULL);
        }
    }
    else if (InScreen("phone_one_message_selected")) {
        result = m_engineCommand->SendCommand(
                 &transaction,
                 _T("HFD"),
                 VBT_COMMAND_ONE_MESSAGE_SELECTED_CALL,
                 VBT_FALSE,
                 NULL,
                 NULL);
    }
    else {
    }

    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
        VR_LOGI("VR_VoiceBoxEngine::m_strCurrentTransactionID:%s", m_strCurrentTransactionID.c_str());
    }
    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnCall
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::SendMessage(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }
    VR_LOGI("VR_VoiceBoxEngine::SendMessage new");
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = S_OK;
    if (m_isNameNull) {
        result = m_engineCommand->SendCommand(
                 &transaction,
                 _T("HFD"),
                 VBT_COMMAND_ONE_CALL_SELECTED_SMS,
                 VBT_FALSE,
                 NULL,
                 NULL);
    }
    else {
        result = m_engineCommand->SendCommand(
                 &transaction,
                 _T("HFD"),
                 VBT_COMMAND_ONE_CONTACT_SELECTED_SMS_WITH_PHONETYPE_WITH_QUICK_REPLY,
                 VBT_FALSE,
                 NULL,
                 NULL);
    }

    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
        VR_LOGI("VR_VoiceBoxEngine::m_strCurrentTransactionID:%s", m_strCurrentTransactionID.c_str());
    }
    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnSendMessage
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::Reply(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }
    VR_LOGI("VR_VoiceBoxEngine::Reply");
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     _T("HFD"),
                     VBT_COMMAND_ONE_MESSAGE_SELECTED_REPLY,
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
        VR_LOGI("VR_VoiceBoxEngine::m_strCurrentTransactionID:%s", m_strCurrentTransactionID.c_str());
    }
    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnReply
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::ReadNext(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }
    VR_LOGI("VR_VoiceBoxEngine::ReadNext");
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     _T("HFD"),
                     VBT_COMMAND_ONE_MESSAGE_SELECTED_READ_NEXT,
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
        VR_LOGI("VR_VoiceBoxEngine::m_strCurrentTransactionID:%s", m_strCurrentTransactionID.c_str());
    }
    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnReadNext
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::ReadPrevious(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }
    VR_LOGI("VR_VoiceBoxEngine::ReadPrevious");
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     _T("HFD"),
                     VBT_COMMAND_ONE_MESSAGE_SELECTED_READ_PREVIOUS,
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
        VR_LOGI("VR_VoiceBoxEngine::m_strCurrentTransactionID:%s", m_strCurrentTransactionID.c_str());
    }
    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnReadPrevious
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::Tutorials(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strText;
    std::string strLang;
    VR_ConfigureIF * pcConfig = VR_ConfigureIF::Instance();
    if (NULL == pcConfig) {
        strLang = "en-us";
        strText = "Use the manual controls to select the tutorial you want to view.";
    }
    else {
        strLang = pcConfig->getVRLanguage();
        if ("en-us" == strLang) {
            strText = "Use the manual controls to select the tutorial you want to view.";
        }
        else if ("fr-ca" == strLang) {
            strText = "Utilisez les contrôles manuels pour sélectionner le tutoriel que vous voulez afficher.";
        }
        else if ("es-mx" == strLang) {
            strText = "Use los controles manuales para seleccionar el tutorial que desea ver.";
        }
        else {
        }
    }

    // Build the play guidance request XML message
    VR_VoiceBoxXmlBuilder xmlBulder;
    pugi::xml_node node = xmlBulder.buildStartActionElement("prompt", "playTts");
    xmlBulder.buildGivenElement(node, "language", strLang, "", "");
    xmlBulder.buildGivenElement(node, "text", strText, "", "");
    xmlBulder.buildGivenElement(node, "phoneme", "", "", "");
    std::string strPlayTts = xmlBulder.getXmlString();
    VR_LOGI("strPlayTts: %s", strPlayTts.c_str());
    m_iToturialBeepId = OnRequestAction(strPlayTts);
    VR_LOGI("m_iToturialBeepId, %d", m_iToturialBeepId);
    VR_VoiceBoxXmlBuilder xmlBulderAction;
    pugi::xml_node nodeAciton = xmlBulderAction.buildStartActionElement("help", "showTutorialsUI");
    m_strToturialResult = xmlBulderAction.getXmlString();
    VR_LOGI("m_strToturialResult: %s", m_strToturialResult.c_str());
    return true;
}

bool
VR_VoiceBoxEngine::VoiceTraining(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strText;
    std::string strLang;
    VR_ConfigureIF * pcConfig = VR_ConfigureIF::Instance();
    if (NULL == pcConfig) {
        strLang = "en-us";
        strText = "Going to voice training mode.";
    }
    else {
        strLang = pcConfig->getVRLanguage();
        if ("en-us" == strLang) {
            strText = "Going to voice training mode.";
        }
        else if ("fr-ca" == strLang) {
            strText = "Je vais à l'entraînement de la reconnaissance vocale.";
        }
        else if ("es-mx" == strLang) {
            strText = "Entrando al modo de reconocimiento de voz.";
        }
        else {
        }
    }

    // Build the play guidance request XML message
    VR_VoiceBoxXmlBuilder xmlBulder;
    pugi::xml_node node = xmlBulder.buildStartActionElement("prompt", "playTts");
    xmlBulder.buildGivenElement(node, "language", strLang, "", "");
    xmlBulder.buildGivenElement(node, "text", strText, "", "");
    xmlBulder.buildGivenElement(node, "phoneme", "", "", "");
    std::string strPlayTts = xmlBulder.getXmlString();
    VR_LOGI("strPlayTts: %s", strPlayTts.c_str());
    m_iVoiceTrainingBeepId = OnRequestAction(strPlayTts);
    VR_LOGI("m_iVoiceTrainingBeepId, %d", m_iVoiceTrainingBeepId);

    VR_VoiceBoxXmlBuilder xmlBulderDisplay;
    pugi::xml_node xmlNode = xmlBulderDisplay.buildDisplayElement("Common", "ScreenDisplay");
    xmlBulderDisplay.buildGivenElement(xmlNode, "agent", "help", "", "");
    xmlBulderDisplay.buildGivenElement(xmlNode, "content", "na_help_train_voice_recognition", "", "");
    m_strVoiceTrainingResult = xmlBulderDisplay.getXmlString();
    VR_LOGI("m_strVoiceTrainingResult: %s", m_strVoiceTrainingResult.c_str());
    return true;
}

bool
VR_VoiceBoxEngine::SettingToTraning(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    m_bSettingToStartScreen = true;
    return true;
}

bool
VR_VoiceBoxEngine::InternalStartVR(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (VR_REGION_US != m_country) {
        m_strInitAgentName = "topmenu";
    }

    std::string startAgent = "<event name=\"startAgent\"><agent>";
    startAgent.append(m_strInitAgentName);
    startAgent.append("</agent></event>");
    m_pcMsgController->PostMessage(startAgent);
    return true;
}

bool
VR_VoiceBoxEngine::ConfirmNo(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = S_OK;
    if (InScreen("help_tutorial_confirmation")) {
        result = m_engineCommand->SendCommand(
                 &transaction,
                 _T("Help"),
                 VBT_COMMAND_CONFIRM_TUTORIAL_REJECT,
                 VBT_FALSE,
                 NULL,
                 NULL);
    }
    else {
        result = m_engineCommand->SendCommand(
                 &transaction,
                 _T("System"),
                 VBT_COMMAND_SYSTEM_GLOBAL_NO,
                 VBT_FALSE,
                 NULL,
                 NULL);
    }
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
    }
    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnConfirmNo
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::AlongRoute(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     "POI",
                     "POI List - Along Route",
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
    }

    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnAlongRoute
            )
        );
    return true;
}

bool
VR_VoiceBoxEngine::NearDestination(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     "POI",
                     "POI List - Near Destination",
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
    }

    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnNearDestination
            )
        );
    return true;
}

bool
VR_VoiceBoxEngine::StartSession()
{
    VR_LOGD_FUNC();

    m_bSpeakAdaptPrompt = false;

    if (m_bCanceled) {
        VR_LOGI("m_bCanceled : %d", m_bCanceled);
        m_bCanceled = false;
        return false;
    }
    CVECIPtr<IVECITransaction> transaction;
    // For VoiceTag, the continue flag should be VBT_FALSE
    VBT_BOOL bContinuous = VBT_TRUE;
    HRESULT result = m_engineCommand->StartRecoSession(&transaction, bContinuous);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
        m_lstTransaction.push_front(m_strCurrentTransactionID);
    }
    m_sessionState = VR_SessionStateStarting;

    VR_LOGI("***start session : %s", strTrans.Get());
    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnStartRecoSession
            )
        );

    return true;
}

// Start a voice recognition session
bool
VR_VoiceBoxEngine::StartRecoSession(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    m_bSpeakAdaptPrompt = false;

    if ((VBT_RECO_STATE_VOICE_TAG == m_strCurrentStateName) && (m_bDoCancelVoiceTag || m_bVoiceTagTtsCrash)) {
        VR_LOGI("voicetag has been canced or tts crash, m_bDoCancelVoiceTag : %d", m_bDoCancelVoiceTag);
        HandleQuitVoiceTag();
        return false;
    }

    if ((NULL == m_engineCommand.ptr()) || (m_bCanceled)) {
        VR_LOGI("m_bCanceled : %d", m_bCanceled);
        m_bCanceled = false;
        return false;
    }

    if ("" == m_strCurrentStateName) {
        m_strCurrentStateName = m_strStartOverStateName;
        m_strHintsStateName = VBT_RECO_STATE_GLOBAL;
    }

    m_strPreSessionTransactionID = m_strCurrentTransactionID;

    VARIANT var;
    var.vt = VT_BOOL;
    var.boolVal = VARIANT_FALSE;
    m_client->SetProperty(PropClientManagedRender, &var);
    var.boolVal = VARIANT_TRUE;
    m_client->SetProperty(PropBackChangeContext, &var);

    CVECIPtr<IVECITransaction> transaction;
    // For VoiceTag, the continue flag should be VBT_FALSE
    VBT_BOOL bContinuous;
    if (VBT_RECO_STATE_VOICE_TAG == m_strCurrentStateName) {
        VR_LOGI("m_strCurrentStateName : %s", m_strCurrentStateName.c_str());
        bContinuous = VBT_FALSE;
    }
    else {
        bContinuous = VBT_TRUE;
    }
    HRESULT result = m_engineCommand->StartRecoSession(&transaction, bContinuous);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    m_sessionState = VR_SessionStateStarting;

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
        m_lstTransaction.push_front(m_strCurrentTransactionID);
    }

    VR_LOGI("***start session : %s", strTrans.Get());
    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnStartRecoSession
            )
        );

    m_enBtnProcessState = VR_Button_Null;
    return true;
}

void
VR_VoiceBoxEngine::HandleGrammarQuitVR()
{
    if (NULL == m_pcCatalogController) {
        return;
    }

    m_bSession = false;
    m_pcCatalogController->PostStatusMsg("<op name=\"dialogsession\"><value>false</value></op>");

    VR_LOGI("do not need to pause grammar");
    m_pcCatalogController->PostStatusMsg("<op name=\"checkpauseconditon\"><value>false</value></op>");
    if (m_bWaitResumeGrammar) {
        VR_LOGI("quit of VR, then resume grammar update");
        m_pcCatalogController->PostStatusMsg("<op name=\"needresume\"><value>true</value></op>");
    }
    else {
        auto pairTransaction = GetCurrentTransaction();
        if (!pairTransaction.first.empty() && !pairTransaction.second.empty()) {
            VR_LOGI("pause grammar has not return, first = %s, second = %s", pairTransaction.first.c_str(), pairTransaction.second.c_str());
            m_bPauseReturnAfterVR = true;
        }
        else {
            if (!m_bPhoneReloadData) {
                SetUpdateGammarFlg(true);
            }
            else {
                VR_LOGI("being reloaddata of phone");
            }
        }
    }
}

void
VR_VoiceBoxEngine::SendCancelResult()
{
    std::string cancelResult;
    while (!m_lstCancelOption.empty()) {
        std::string option = m_lstCancelOption.front();
        VR_LOGI("sendCancelMsg : get option = %s", option.c_str());
        if (option.empty()) {
            cancelResult = "<event-result name=\"cancel\"/>";
        }
        else {
            cancelResult = "<event-result name=\"cancel\" option=\"" + option +"\"/>";
        }

        if ("ttscrash" == option) {
            VR_LOGI("the cancel event of ttscrash is handled over");
            m_bHandleTTSCrush = false;
        }

        OnRecognized(cancelResult);
        VR_LOGI("sendCancelMsg : msg = %s", cancelResult.c_str());
        m_lstCancelOption.pop_front();
    }
}

// Cancel the current voice recognition session
bool
VR_VoiceBoxEngine::Cancel(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    m_bCanceled = true;
    VR_LOGI("m_sessionState = %d", m_sessionState);

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    if (m_bInstallingAgent || m_bReInstallTSL) {
        return false;
    }

    std::string strOption = parser.getValueByKey("option");
    if ("usbSwitch" == strOption) {
        VR_LOGI("after started engine, update state");
        m_bUpdateState = true;

        if (m_bTslDialog) {
            std::string option = parser.getValueByKey("option");
            m_lstCancelOption.push_back(option);

            VR_LOGI("in tsl screen, send quit information");
            std::string strForceEnd = "<action name=\"notifyAppRecognitionForceEnd\"/>";
            OnRequestAction(strForceEnd);
            return true;
        }

        if (m_bInEndTask) {
            if (!m_mapPlayTTSTransation.empty()) {
                VR_LOGI("Cancel in end task- Stop TTS");
                StopAllTTS();
            }
            return true;
        }
    }
    else if ("ttscrash" == strOption) {
        VR_LOGI("the app of tts is crushed, we need quit VR app");
        m_bHandleTTSCrush = true;
        if ((m_iStartBeepID != VR_INVALID_ACTION_ID)
            && (VR_SessionStateStartWithiBargeIn != m_sessionState)) {
            VR_LOGI("the start beep is playing, we need to finish the event that run beep done");
            m_iStartBeepID = VR_INVALID_ACTION_ID;
            HRESULT result = m_engineCommand->BeepDone();
            if (FAILED(result)) {
                VR_LOGI("beepdone is failed.");
            }
        }
        else if (m_iDoneBeepID != VR_INVALID_ACTION_ID) {
            VR_LOGI("the done beep is playing, we can quit now!");
            m_lstCancelOption.push_back(strOption);
            m_strMsgPrompt = "";
            m_bPlayTTS = false;

            SendCancelResult();
            NotifyResourceState();
            HandleQuitVR();
            return true;
        }
        else {
        }

        if (m_iToturialBeepId != VR_INVALID_ACTION_ID) {
            VR_LOGI("TTS is crashed, we don't start tutorial!");
            m_iToturialBeepId = VR_INVALID_ACTION_ID;
            m_strToturialResult = "";
        }

        if (m_iVoiceTrainingBeepId != VR_INVALID_ACTION_ID) {
            VR_LOGI("TTS is crashed, we don't start voice training!");
            m_iVoiceTrainingBeepId = VR_INVALID_ACTION_ID;
            m_strVoiceTrainingResult = "";
        }
    }

    if (m_bTslDialog || (VBT_RECO_STATE_VOICE_TAG == m_strCurrentStateName)) {
        std::string option = parser.getValueByKey("option");
        m_lstCancelOption.push_back(option);
        SendCancelResult();
        m_bCanceled = false;
        return true;
    }

    bool bDMCancel = false;
    std::string strTarget = parser.getValueByKey("target");
    if ("" == strTarget) {
        VR_LOGI("DM cancel");
        bDMCancel = true;
        std::string option = parser.getValueByKey("option");
        if (!m_lstCancelOption.empty() && !m_bHandleTTSCrush) {
            VR_LOGI("it is doing canceled event");
            m_lstCancelOption.push_back(option);
            return true;
        }
        else {
            m_lstCancelOption.push_back(option);
        }
        m_strBtnEventName = "";
        NotifyResourceState();
    }

    if (VR_REGION_US == m_country) {
        VR_LOGI("PVR State %d", m_PVRStateCurrent);
        if (VR_PVRState_PlayingTTS == m_PVRStateCurrent) {
            OnRequest("<event name=\"PvrTTS\"><StatusName>PVRExit</StatusName></event>");
            m_bPVRScreen = true;
            VR_LOGI("PVR Screen, wait TTS played");
            return true;
        }
        else if (VR_PVRState_WaitPlayTTS == m_PVRStateCurrent || VR_PVRState_PlayedTTS == m_PVRStateCurrent) {
            OnRequest("<event name=\"PvrTTS\"><StatusName>PVRExit</StatusName></event>");
            m_bPVRScreen = true;
            m_PVRStateCurrent = VR_PVRState_None;
        }
        else {
            m_PVRStateCurrent = VR_PVRState_None;
        }
    }

    if (VR_SessionStateStoped == m_sessionState
        || VR_SessionStateNone == m_sessionState) {
        if (bDMCancel) {
            SendCancelResult();
            NotifyResourceState();

            // case: at the start voice training screen need to send quitvrapp to VRUI
            // case: at startVR screen befor playing start beep need to send quitvrapp to VRUI and handle grammar
            if (m_bSettingToStartScreen || m_bPrepare) {
                m_bSettingToStartScreen = false;
                OnRecognized("<display xmlns=\"\" agent=\"Common\" content=\"QuitVRApp\"> </display>");
            }

            if (m_bPrepare) {
                m_bPrepare = false;
                HandleGrammarQuitVR();
            }
        }
        return true;
    }

    m_sessionState = VR_SessionStateCanceled;
    if ((m_iDoneBeepID == VR_INVALID_ACTION_ID) && bDMCancel) {
        StopAllTTS();
        CancelRecoSession();
        OnBeep(VR_BeepType_Done);
    }
    else if ((m_iDoneBeepID != VR_INVALID_ACTION_ID) && m_bHandleTTSCrush) {
        VR_LOGI("TTS crash, don't wait for done beep, we quit VR app!");
        m_iDoneBeepID = VR_INVALID_ACTION_ID;
        m_strMsgPrompt = "";
        m_bPlayTTS = false;
        SendCancelResult();
        NotifyResourceState();
        HandleQuitVR();
    }
    else {
        VR_LOGI("cancel event for other case!");
    }

    return true;
}

// Repeat the last voice recognition session
bool
VR_VoiceBoxEngine::Repeat(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    // Cancel the curent session
    if (!CancelRecoSession()) {
        return false;
    }

    CVECIPtr<IVECITransaction> spCurrentTrans;
    CVECIPtr<IVECITransaction> spLastTrans;
    HRESULT result = m_engineCommand->Repeat(&spCurrentTrans, VBT_TRUE, &spLastTrans);
    if (FAILED(result) || (NULL == spCurrentTrans.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = spCurrentTrans->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
    }
    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnRepeat
            )
        );

    if (NULL != spLastTrans.ptr()) {
        result = spCurrentTrans->GetTransactionId(&strTrans);
        if (FAILED(result)) {
            return false;
        }

        m_strLastTransactionID = strTrans.Get();
    }

    return true;
}

// Back the voice recognition session to the previous context.
bool
VR_VoiceBoxEngine::Back(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    VR_LOGI("Handle back command");
    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    m_bBackInterupted = false;

    VR_LOGI("cancel session");
    // Cancel the curent session
    VoiceList<std::string>::iterator iter = std::find(m_lstNeedReactiveAction.begin(), m_lstNeedReactiveAction.end(), m_strActionType);
    if (m_bTaskCompelete && iter == m_lstNeedReactiveAction.end()) {
        VR_LOGI("not need to CancelSession");
    }
    else {
        CancelRecoSession();
    }
    VR_LOGI("check more hints state");
    if (m_bMoreHint) {
        VR_LOGI("this is hints state");

        // Prepare the hints
        if (!GetHints()) {
            return false;
        }

        if (VR_SessionStateRestartSessionAfterTTS == m_sessionState
            || VR_SessionStatePaused == m_sessionState) {
            SendRecogState();
        }
        m_sessionState = VR_SessionStateAutoSendRecogState;
        return true;
    }

    CVECIPtr<IVECITransaction> spCurrentTrans;
    CVECIPtr<IVECITransaction> spPrevTrans;
    VR_LOGI("do back command");
    HRESULT result = m_engineCommand->Back(&spCurrentTrans, VBT_TRUE, &spPrevTrans);
    if (FAILED(result) || (NULL == spCurrentTrans.ptr())) {
        return false;
    }

    CVECIOutStr strCurTrans;
    result = spCurrentTrans->GetTransactionId(&strCurTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strCurTrans.Get()) {
        m_strCurrentTransactionID = strCurTrans.Get();
        VR_LOGI("current tran : %s", strCurTrans.Get());
    }

    CVECIOutStr strTrans;

    if (NULL != spPrevTrans.ptr()) {
        VR_LOGI("this is not the first session");
        result = spPrevTrans->GetTransactionId(&strTrans);
        if (FAILED(result)) {
            return false;
        }
        CVECIPtr<IVECIParsedMessage> spMsg;
        spPrevTrans->GetResultMessage(&spMsg);

        m_mapCmdResultHandler.insert(
            std::make_pair(
            strCurTrans.Get(),
            &VR_VoiceBoxEngine::OnBack));

        VR_LOGI("prev tran : %s", strTrans.Get());
        if (m_strPreSessionTransactionID == strTrans.Get()) {
            if (VBT_RECO_STATE_IN_CALL == m_strCurrentStateName) {
                std::string strScreen = "<display agent=\"Common\" content=\"ScreenDisplay\">"
                                            "<agent>phone</agent>"
                                            "<content>";
                std::string strContent = "phone_in_call";
                BuildScreenContent(strContent);
                if (VR_REGION_OC == m_country) {
                    strContent = "sendtone_speak_tones";
                }
                strScreen.append(strContent);
                strScreen.append("</content></display>");

                OnRecognized(strScreen);
            }
            else if (VBT_RECO_STATE_INCOMING_MESSAGE == m_strCurrentStateName) {
                std::string strScreen = "<display agent=\"Common\" content=\"ScreenDisplay\">"
                                            "<agent>phone</agent>"
                                            "<content>";

                std::string strContent = "phone_in_message";
                BuildScreenContent(strContent);

                strScreen.append(strContent);
                strScreen.append("</content></display>");

                OnRecognized(strScreen);
            }
            else if (VBT_RECO_STATE_SPEAKER_ADAPTATION == m_strCurrentStateName) {
                if ("1" == m_strStep) {
                    VR_LOGI("ADAPTATION");
                    m_sessionState = VR_SessionStateBackQuit;
                    OnBeep(VR_BeepType_Done);
                    return true;
                }
                else if ("2" == m_strStep) {
                    CVECIPtr<IVECITransaction> transaction;
                    HRESULT result = m_engineCommand->SendCommand(
                                     &transaction,
                                     VBT_RECO_STATE_SPEAKER_ADAPTATION,
                                     VBT_COMMAND_NOOP_START_SPEAKER_ADAPTATION,
                                     VBT_FALSE,
                                     NULL,
                                     NULL);
                    if (FAILED(result)) {
                        VR_LOGE("SendCommand adaptation: 0x%lx", result);
                        return false;
                    }

                    CVECIOutStr strTrans;
                    result = transaction->GetTransactionId(&strTrans);
                    if (FAILED(result)) {
                        VR_LOGE("GetTransactionId: 0x%lx", result);
                        return false;
                    }
                    m_mapCmdResultHandler.insert(
                            std::make_pair(
                                strTrans.Get(),
                                &VR_VoiceBoxEngine::OnSpeakAdaptation
                                )
                            );
                    return true;
                }
                else {
                    VR_LOGI("do nothing");
                }
            }
            else {
                VR_LOGI("get hints");
                GetHints();
            }
        }
        else {
            VR_LOGI("Process Agent Message");

            if (NULL != spMsg.ptr()) {
                CVECIOutStr strAgentName;
                (void)spMsg->GetAgentName(&strAgentName);

                if (strAgentName.IsEqual("HFD")) {
                    m_agents[AgentType_Phone]->ProcessAgentMessage(spMsg);
                }
                else if (strAgentName.IsEqual("Music") || strAgentName.IsEqual("Radio")) {
                    m_agents[AgentType_Audio]->ProcessAgentMessage(spMsg);
                }
                else if (strAgentName.IsEqual("Apps")) {
                    m_agents[AgentType_Apps]->ProcessAgentMessage(spMsg);
                }
                else if (strAgentName.IsEqual("HVAC")) {
                    m_agents[AgentType_Climate]->ProcessAgentMessage(spMsg);
                }
                else if (strAgentName.IsEqual("Root")
                    || strAgentName.IsEqual(VBT_AGENT_SYSTEM)
                    || strAgentName.IsEqual(VBT_AGENT_HELP)
                    || strAgentName.IsEqual(VBT_AGENT_SPEAKER_ADAPTATION)) {
                    m_agents[AgentType_Global]->ProcessAgentMessage(spMsg);
                }
                else if (strAgentName.IsEqual("Information")) {
                    m_agents[AgentType_Info]->ProcessAgentMessage(spMsg);
                }
                else if (strAgentName.IsEqual("Navigation")
                    || strAgentName.IsEqual(VBT_AGENT_ADDRESS_ENTRY)
                    || strAgentName.IsEqual(VBT_AGENT_DESTINATION)
                    || strAgentName.IsEqual(VBT_AGENT_POI)) {
                    VR_LOGI("VBT will call the function!");
                }
                else {

                }
            }
        }
        m_sessionState = VR_SessionStateBackRestart;
    }
    else {
        VR_LOGI("this is the first session");
        // VR quit back to haptic
        m_sessionState = VR_SessionStateBackQuit;
        OnBeep(VR_BeepType_Done);
    }

    return true;
}

// Restart the voice recognition session
bool
VR_VoiceBoxEngine::StartOver(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    m_enBtnProcessState = VR_Button_StartOver;

    VR_LOGI("current session %d", m_sessionState);
    if (VBT_RECO_STATE_IN_CALL == m_strCurrentStateName) {
        std::string strScreen = "<display agent=\"Common\" content=\"ScreenDisplay\">"
        "<agent>phone</agent>"
        "<content>";
        std::string strContent = "phone_in_call";
        BuildScreenContent(strContent);
        if (VR_REGION_OC == m_country) {
            strContent = "sendtone_speak_tones";
        }
        strScreen.append(strContent);
        strScreen.append("</content></display>");

        OnRecognized(strScreen);
    }
    else if (VBT_RECO_STATE_INCOMING_MESSAGE == m_strCurrentStateName) {
        std::string strScreen = "<display agent=\"Common\" content=\"ScreenDisplay\">"
        "<agent>phone</agent>"
        "<content>";

        std::string strContent = "phone_in_message";
        BuildScreenContent(strContent);

        strScreen.append(strContent);
        strScreen.append("</content></display>");

        OnRecognized(strScreen);
    }
    else {
    }

    if (VR_REGION_US == m_country) {
        m_strCurrentStateName = m_strInitStateName;
        m_strHintsStateName = VBT_RECO_STATE_GLOBAL;
    }
    m_strStartOverStateName = m_strCurrentStateName;
    if (VR_SessionStatePaused == m_sessionState
        || VR_SessionStateRestartSessionAfterTTS == m_sessionState) {
        VR_LOGI("get hints");
        // Prepare the hints
        if (!GetHints()) {
            return false;
        }

        SendRecogState();
        m_sessionState = VR_SessionStateTemporarilyCanceled;
    }
    else {
        // Prepare the hints
        if (!GetHints()) {
            return false;
        }

        VR_LOGI("after the session completed to do start over");
        m_sessionState = VR_SessionStateStartOver;
    }

    return true;
}

// Update the hints and play the related help guidance
bool
VR_VoiceBoxEngine::Help(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    // Cancel the curent session
    if (!CancelRecoSession()) {
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     _T("System"),
                     _T("System Help"),
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
    }
    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnHelp
            )
        );
    m_sessionState = VR_SessionStateRestartSessionAfterTTS;

    if (VR_REGION_US == m_country) {
        ShowHelpMoreHintsScreen();
    }

    return true;
}

bool
VR_VoiceBoxEngine::OnGetHints(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    VR_LOGP("receive getHints request");
    if (NULL == m_client.ptr()) {
        return false;
    }

    m_strAction = "";
    m_lstTransaction.clear();

    std::string strStateName = parser.getValueByKey("agentName");
    // std::string strPageSize = parser.getValueByKey("pageSize");

    std::string strResult = "<event-result name=\"getHints\" errcode=\"\" ><agentName>";
    strResult.append(strStateName);
    strResult.append("</agentName> <pageSize>5</pageSize> </event-result>");

    HRESULT retCode = m_client->CreateStringSet(&m_spHints);
    if (S_OK != retCode) {
        return false;
    }
    if (NULL == m_spHints.ptr()) {
        return false;
    }

    m_bMoreHint = false;

    VBT_INT nScreenId = m_mapHintsScreenID["none"];
    std::string strVBTStateName;

    GetVBTInfoByUIStateName(strStateName, nScreenId, strVBTStateName);

    m_strUIStateName = strStateName;

    CVECIPtr<IVECIListItems> optionalContextList;
    m_client->CreateListItems(&optionalContextList);
    if (NULL == optionalContextList.ptr()) {
        return false;
    }

    SetOptionalContext(optionalContextList, strVBTStateName, false);

    retCode = m_engineCommand->GetRecognitionStateHints("Global", nScreenId, optionalContextList, &m_spHints);
    if (S_OK != retCode) {
        VR_LOGI("GetRecognitionStateHints: %x", retCode);
        return false;
    }

    m_iIndex = 0;
    m_iHintSize = 0;

    m_spHints->GetSize(&m_iHintSize);
    (void)DisplayHints("", true);

    VR_LOGP("reply hints result");
    OnRecognized(strResult);

    return true;
}

bool
VR_VoiceBoxEngine::HandleMoreHints(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    m_bMoreHint = true;

    VR_LOGI("------------ VR_VoiceBoxEngine MoreHints");

    // Cancel the curent session
    if (!CancelRecoSession()) {
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     VBT_AGENT_HELP,
                     VBT_COMMAND_SYSTEM_MORE_HINTS,
                     VBT_FALSE,
                     NULL,
                     NULL);

    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnMoreHints
            )
        );

    m_iIndex = 0;

    if (VR_REGION_OC == m_country) {
        if (!GetMoreHintsforOC()) {
            VR_LOGI("get more hints for OC failed !!!");
        }
    }
    (void)DisplayHints("VR-HNT-04");

    return true;
}

bool
VR_VoiceBoxEngine::MoreHints(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    VR_LOGP("DE: start morehints button command...case : 215-8");

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    m_bMoreHint = true;
    m_iIndex = 0;

    if (VR_REGION_OC == m_country) {
        if (!GetMoreHintsforOC()) {
            VR_LOGI("get more hints for OC failed !!!");
        }
    }
    DisplayHints("VR-HNT-04");
    return true;
}

bool VR_VoiceBoxEngine::GetMoreHintsforOC()
{
    VR_LOGD_FUNC();
    HRESULT retCode = m_client->CreateStringSet(&m_spHints);
    if (S_OK != retCode) {
        return false;
    }
    if (NULL == m_spHints.ptr()) {
        return false;
    }

    VBT_INT nScreenId;
    if ("Global" == m_strCurrentStateName) {
        nScreenId = VR_VBT_AU_MOREHINT_SCREEN_ID_MAIN;
    }
    else if ("HFD Home" == m_strCurrentStateName) {
        nScreenId = VR_VBT_AU_MOREHINT_SCREEN_ID_PHONE;
    }
    else if ("Navigation Home" == m_strCurrentStateName || "POI Home" == m_strCurrentStateName) {
        nScreenId = VR_VBT_AU_MOREHINT_SCREEN_ID_NAVIGATION;
    }
    else if ("Music Home" == m_strCurrentStateName || "Radio Home" == m_strCurrentStateName
        || "Audio Home" == m_strCurrentStateName) {
        nScreenId = VR_VBT_AU_MOREHINT_SCREEN_ID_AUDIO;
    }
    else if ("Apps Home" == m_strCurrentStateName) {
        nScreenId = VR_VBT_AU_MOREHINT_SCREEN_ID_APPS;
    }
    else {
        nScreenId = VR_VBT_AU_MOREHINT_SCREEN_ID_MAIN;
    }

    CVECIPtr<IVECIListItems> optionalContextList;
    m_client->CreateListItems(&optionalContextList);
    if (NULL == optionalContextList.ptr()) {
        return false;
    }
    SetOptionalContext(optionalContextList, m_strCurrentStateName, false);

    retCode = m_engineCommand->GetRecognitionStateHints("Global", nScreenId, optionalContextList, &m_spHints);
    if (S_OK != retCode) {
        VR_LOGI("GetRecognitionStateHints: %x", retCode);
        return false;
    }

    m_spHints->GetSize(&m_iHintSize);

    return true;
}

bool
VR_VoiceBoxEngine::HandleActionMsg(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    m_strActionClass = parser.getValueByKey("actionClass");

    if (VR_REGION_OC == m_country) {
        std::string strActionType = parser.getValueByKey("actionType");
        if (("Escalating Error" != strActionType)
            && ("No Reco" != strActionType)
            && ("System Pause" != strActionType)
            && ("Help" != strActionType)) {
            // m_strActionType and m_strAgentName are only used for navigation
            m_strActionType = strActionType;
            m_strAgentName = parser.getValueByKey("agentName");
        }
    }
    else if (VR_REGION_US == m_country) {
        m_strActionType = parser.getValueByKey("actionType");
        m_strAgentName = parser.getValueByKey("agentName");
    }
    else {
        VR_LOGI("ERROR! Invalid Country !!");
    }

    VR_LOGI("m_strActionType = [%s], m_strActionClass = [%s]", m_strActionType.c_str(), m_strActionClass.c_str());

    if ("Task Complete" == m_strActionClass) {
        m_bTaskCompelete = true;
    }
    else {
        m_bTaskCompelete = false;
    }

    return true;
}

bool
VR_VoiceBoxEngine::HandleVRState(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (m_bTTSNull) {
        if ("" != m_strAction) {
            VR_LOGI("run the action");
            OnRequestAction(m_strAction);
        }

        if (m_bSpeakAdaptPrompt
            && (VR_SessionStatePaused == m_sessionState)) {
            VR_LOGI("it is training paused");
            std::string strPrompt;
            GetPromptByScreenId("VR-SB-05", strPrompt);
            SendVRState("paused", strPrompt, "false", "");
        }

        return true;
    }

    bool bBargeInPrompt = false;

    if (NULL != VR_ConfigureIF::Instance()) {
        if (VR_ConfigureIF::Instance()->getVROverPrompt()) {
            m_bBosDetected = false;
            VR_AudioStreamIF* pcAudioStream = VR_AudioStreamIF::Instance();
            if ((VBT_RECO_STATE_SPEAKER_ADAPTATION != m_strCurrentStateName)
                && (VBT_RECO_STATE_VOICE_TAG != m_strCurrentStateName)
                && (VR_PVRState_PlayingTTS != m_PVRStateCurrent)) {

                bool bTaskCompleteBargeIn = false;
                VoiceList<std::string>::iterator iter = std::find(
                    m_lstNeedReactiveAction.begin(),
                    m_lstNeedReactiveAction.end(),
                    m_strActionType
                    );
                if ((!m_cTslParamManager.GetRenderDoneSyncFlag())
                    && m_bTaskCompelete
                    && (iter != m_lstNeedReactiveAction.end())) {
                    bTaskCompleteBargeIn = true;
                }

                if ((VR_SessionStatePaused != m_sessionState)
                    && (VR_SessionStatePttLong != m_sessionState)
                    && (!m_bTaskCompelete || bTaskCompleteBargeIn)
                    && (NULL != pcAudioStream)) {
                    bBargeInPrompt = true;
                    pcAudioStream->StartAudioInWithBargeIn();
                }
            }
        }
    }

    VR_LOGI("current session : %d", m_sessionState);
    std::string strState;
    std::string strPrompt;
    if (VR_SessionStatePaused == m_sessionState) {
        GetPromptByScreenId("VR-SB-05", strPrompt);
        strState = "paused";
    }
    else {
        if (VR_SessionStateAutoSendRecogState != m_sessionState
            && VR_SessionStateStartOver != m_sessionState
            && VR_SessionStateAutoTutoSendRecogState != m_sessionState
            && m_bTaskCompelete) {
            VR_LOGI("m_bTaskCompelete true");
            strState = "reading";
        }
        else {
            VR_LOGI("m_bTaskCompelete false");

            strPrompt = VR_VoiceBoxEventSink::m_strPrompt;
            strState = bBargeInPrompt ? "promptWithBargein" : "promptPlaying";
        }
    }

    SendVRState(strState, strPrompt, "false", "");

    return true;
}

bool
VR_VoiceBoxEngine::SetRecoSessionFlag(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    m_sessionState = VR_SessionStateAutoRestartSession;
    return true;
}

bool
VR_VoiceBoxEngine::DisplayHints(const std::string& strHintScreenId, bool bGetMoreHint)
{
    VR_LOGD_FUNC();

    if (NULL == m_spHints) {
        return false;
    }

    std::string strDisplayContent;

    if (bGetMoreHint) {
        strDisplayContent = "HintsDisplay";
    }
    else {
        strDisplayContent = "ScreenDisplay";
    }

    if ((VR_REGION_OC == m_country) && ("Apps Home" == m_strCurrentStateName) && !m_vecAppName.empty()) {
        // when morehints, the first hints is always "Launch Apps", so insert "Apps" to the first
        if (m_bMoreHint && ("Apps" != m_vecAppName.at(0))) {
             m_vecAppName.insert(m_vecAppName.begin(), "Apps");
        }
        // erase the first hint of morehints screen, when back hints screen from morehints screen
        else if (!m_bMoreHint && ("Apps" == m_vecAppName.at(0))) {
            m_vecAppName.erase(m_vecAppName.begin());
        }
        else {
        }
        m_iHintSize = m_vecAppName.size();
    }

    const VBT_ULONG NATOPMENUIDLE = 4;
    const VBT_ULONG DEFAULTPAGESIZE = 5;
    VBT_ULONG iHintPageSize = (!m_bMoreHint && "na_topmenu_idle" == m_strContent) ? NATOPMENUIDLE : DEFAULTPAGESIZE;
    VR_VoiceBoxXmlBuilder xmlBulder;
    pugi::xml_node node = xmlBulder.buildDisplayElement("Common", strDisplayContent);
    xmlBulder.buildGivenElement(node, "agent", m_strUIStateName.c_str(), "", "");
    std::string strContent = (m_bMoreHint) ? m_strMoreHints : m_strContent;
    xmlBulder.buildGivenElement(node, "content", strContent.c_str(), "", ""); // content
    pugi::xml_node nodeHints = xmlBulder.buildGivenElement(node, "hints", "", "", "");
    pugi::xml_node nodeList = xmlBulder.buildGivenElement(nodeHints, "list", "", "", "");
    pugi::xml_node nodeHeader = xmlBulder.buildGivenElement(nodeList, "header", "", "", "");
    xmlBulder.buildGivenElement(nodeHeader, "startIndex", std::to_string(m_iIndex), "", "");
    xmlBulder.buildGivenElement(nodeHeader, "pageSize", std::to_string(iHintPageSize), "", "");
    xmlBulder.buildGivenElement(nodeHeader, "count", std::to_string(m_iHintSize), "", "");
    pugi::xml_node nodeItems = xmlBulder.buildGivenElement(nodeList, "items", "", "", "");

    VoiceVector<std::string>::type vecItem;
    std::string strHintStart;
    std::string strHintEnd;

    if ("fr-CA" == m_strCultureName) {
        strHintStart = "\u00AB ";
        strHintEnd = " \u00BB";
    }
    else {
        strHintStart = "\u201C";
        strHintEnd = "\u201D";
    }

    // Retrieve Each Hints
    if ((VR_REGION_OC == m_country) && ("Apps Home" == m_strCurrentStateName) && !m_vecAppName.empty()) {
        // when display app hints/morehints of OC, we use appname got from updateapplist
        for (VBT_ULONG i = 0; i < iHintPageSize; ++i) {
            if (m_iIndex < m_iHintSize && m_iIndex < m_vecAppName.size()) {
                std::string strHintTmp = "Launch " + m_vecAppName[m_iIndex];
                ++m_iIndex;
                VoiceVector<StructNode>::type attributeVector;
                StructNode strNode;
                strNode.Name = "hint";
                strNode.Value = strHintStart;
                strNode.Value.append(strHintTmp);
                strNode.Value.append(strHintEnd);
                attributeVector.push_back(strNode);
                xmlBulder.buildListItemChildElement(nodeItems, "", hint, attributeVector);
            }
            else {
                break;
            }
        }
    }
    else {
        for (VBT_ULONG i = 0; i < iHintPageSize; ++i) {
            CVECIOutStr strHint;
            (void)m_spHints->GetItem(m_iIndex, &strHint);
            if (m_iIndex < m_iHintSize) {
                ++m_iIndex;
            }
            else {
                break;
            }
            VoiceVector<StructNode>::type attributeVector;
            StructNode strNode;
            if (NULL != strHint.Get()) {
                std::string strHintTmp = strHint.Get();
                if ((VR_REGION_OC == m_country) && !m_bDiscAvailable && ("Disc" == strHintTmp)) {
                    // when disc not disconnected , ignore Disc hint
                    continue;
                }
                strNode.Name = "hint";
                strNode.Value = strHintStart;
                strNode.Value.append(strHintTmp);
                strNode.Value.append(strHintEnd);
            }
            attributeVector.push_back(strNode);
            xmlBulder.buildListItemChildElement(nodeItems, "", hint, attributeVector);
        }
    }

    std::string strReply = xmlBulder.getXmlString();
    OnRecognized(strReply);

    if (!strHintScreenId.empty()) {
        GetPromptByScreenId(strHintScreenId, VR_VoiceBoxEventSink::m_strPrompt);
    }

    return true;
}

void
VR_VoiceBoxEngine::GetUIInfoByVBTStateName(int& nScreenId, const std::string strStateName)
{
    VR_LOGD_FUNC();

    if (strStateName.empty()) {
        return;
    }

    nScreenId = m_mapHintsScreenID["none"];
    m_strContent.clear();
    m_strUIStateName.clear();
    m_strMoreHints.clear();

    if ("Global" == strStateName) {
        nScreenId = m_mapHintsScreenID["main"];
        m_strContent = "topmenu_idle";
        m_strUIStateName = "topmenu";
        m_strMoreHints = "topmenu_more_hints";

    }
    else if ("HFD Home" == strStateName) {
        nScreenId = m_mapHintsScreenID["phone"];
        m_strContent = "phone_idle";
        m_strUIStateName = "phone";
        m_strMoreHints = "phone_more_hints";
    }
    else if ("Navigation Home" == strStateName || "POI Home" == strStateName) {
        nScreenId = m_mapHintsScreenID["navigation"];
        m_strContent = "navi_idle";
        m_strUIStateName = "navi";
        m_strMoreHints = "navi_more_hints";
    }
    else if ("HVAC Home" == strStateName) {
        nScreenId = m_mapHintsScreenID["climate"];
        m_strContent = "climate_idle";
        m_strUIStateName = "climate";
        m_strMoreHints = "climate_more_hints";
    }
    else if ("Information Home" == strStateName) {
        nScreenId = m_mapHintsScreenID["info"];
        m_strContent = "info_idle";
        m_strUIStateName = "info";
    }
    else if ("Music Home" == strStateName || "Radio Home" == strStateName
        || "Audio Home" == strStateName) {
        nScreenId = m_mapHintsScreenID["audio"];
        m_strContent = "media_idle";
        m_strUIStateName = "media";
        m_strMoreHints = "media_more_hints";
    }
    else if ("Apps Home" == strStateName) {
        nScreenId = m_mapHintsScreenID["apps"];
        m_strContent = "apps_idle";
        m_strUIStateName = "apps";
        m_strMoreHints = "apps_more_hints";
    }
    else if ("Help" == strStateName) {
        if (VR_REGION_US != m_country) {
            // This case only work for NA
            return;
        }

        VR_ConfigureIF* pcConfig = VR_ConfigureIF::Instance();
        if (NULL == pcConfig) {
            // This condition could not be met.
            return;
        }

        nScreenId = m_mapHintsScreenID["help"];
        VR_PRODUCT_TYPE productType = pcConfig->getVRProduct();
        if (VR_PRODUCT_TYPE_L1 == productType) {
            m_strContent = "topmenu_idle";
            m_strUIStateName = "topmenu";
            m_strMoreHints = "topmenu_more_hints";
        }
        else {
            m_strContent = "help_idle";
            m_strUIStateName = "help";
            m_strMoreHints = "help_more_hints";
        }
    }
    else {
        nScreenId = m_mapHintsScreenID["help"];
        m_strContent = "help_idle";
        m_strUIStateName = "help";
    }

    BuildScreenContent(m_strContent);
    BuildScreenContent(m_strMoreHints);
}

void
VR_VoiceBoxEngine::GetVBTInfoByUIStateName(const std::string& strUISateName,
    int& nScreenId, std::string& strVBTStateName)
{
    VR_LOGD_FUNC();

    if (strUISateName.empty()) {
        return;
    }

    nScreenId = m_mapHintsScreenID["none"];
    m_strContent.clear();
    strVBTStateName.clear();

    if ("apps" == strUISateName) {
        nScreenId = m_mapHintsScreenID["apps"];
        m_strContent = "apps_idle";
        strVBTStateName = "Apps Home";
    }
    else if ("topmenu" == strUISateName) {
        nScreenId = m_mapHintsScreenID["main"];
        m_strContent = "topmenu_idle";
        strVBTStateName = "Global";
    }
    else if ("phone" == strUISateName) {
        nScreenId = m_mapHintsScreenID["phone"];
        m_strContent = "phone_idle";
        strVBTStateName = "HFD Home";
    }
    else if ("climate" == strUISateName) {
        nScreenId = m_mapHintsScreenID["climate"];
        m_strContent = "climate_idle";
        strVBTStateName = "HVAC Home";
    }
    else if ("information" == strUISateName) {
        nScreenId = m_mapHintsScreenID["info"];
        m_strContent = "info_idle";
        strVBTStateName = "Information Home";
    }
    else if ("music" == strUISateName) {
        nScreenId = m_mapHintsScreenID["audio"];
        m_strContent = "media_idle";
        if (VR_REGION_US == m_country) {
            strVBTStateName = "Music Home";
        }
        else if (VR_REGION_OC == m_country) {
            strVBTStateName = "Audio Home";
        }
        else {
        }
    }
    else if ("navi" == strUISateName) {
        nScreenId = m_mapHintsScreenID["navigation"];
        m_strContent = "navi_idle";
        strVBTStateName = "Navigation Home";
    }
    else if ("poi" == strUISateName) {
        nScreenId = m_mapHintsScreenID["navigation"];
        m_strContent = "navi_idle";
        strVBTStateName = "POI Home";
    }
    else if ("radio" == strUISateName) {
        nScreenId = m_mapHintsScreenID["audio"];
        m_strContent = "media_idle";
        strVBTStateName = "Radio Home";
    }

    BuildScreenContent(m_strContent);
}

// Get hints content from VBT
bool
VR_VoiceBoxEngine::PrepareHintsContent(const std::string& strStateName)
{
    if ((NULL ==  m_client.ptr()) || (NULL == m_engineCommand.ptr())) {
        return false;
    }

    HRESULT retCode = m_client->CreateStringSet(&m_spHints);
    if (S_OK != retCode) {
        return false;
    }
    if (NULL == m_spHints.ptr()) {
        return false;
    }

    VBT_INT nScreenId = m_mapHintsScreenID["none"];

    GetUIInfoByVBTStateName(nScreenId, strStateName);

    CVECIPtr<IVECIListItems> optionalContextList;
    m_client->CreateListItems(&optionalContextList);
    if (NULL == optionalContextList.ptr()) {
        return false;
    }

    SetOptionalContext(optionalContextList, strStateName, false);

    retCode = m_engineCommand->GetRecognitionStateHints("Global", nScreenId, optionalContextList, &m_spHints);
    if (S_OK != retCode) {
        VR_LOGI("GetRecognitionStateHints: %x", retCode);
        return false;
    }

    m_iIndex = 0;
    m_spHints->GetSize(&m_iHintSize);

    return true;
}

// Get the hints for the specified state name.
bool
VR_VoiceBoxEngine::GetHints()
{
    VR_LOGD_FUNC();

    VR_LOGI("this is not more hints state");
    m_bMoreHint = false;
    m_bNeedPromptPlaying = true;

    if (VBT_RECO_STATE_IN_CALL == m_strCurrentStateName
        || VBT_RECO_STATE_VOICE_TAG == m_strCurrentStateName
        || VBT_RECO_STATE_INCOMING_MESSAGE == m_strCurrentStateName
        || VBT_RECO_STATE_SPEAKER_ADAPTATION == m_strCurrentStateName) {
        VR_LOGI("there is not need to get hints");
        return true;
    }

    if (VR_REGION_US == m_country) {
        if (!PrepareHintsContent(m_strHintsStateName)) {
            return false;
        }
    }
    else {
        if (!PrepareHintsContent(m_strCurrentStateName)) {
            return false;
        }
    }

    (void)DisplayHints("VR-HNT-01");

    return true;
}

// When the guidance play is completed, this function will be called.
bool
VR_VoiceBoxEngine::SpeakDone(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    // play voicetag tts result
    if ((m_iPlayVoiceTagTts == m_iCurrActionId) && (VR_INVALID_ACTION_ID != m_iCurrActionId)) {
        TransationSpeakDone(m_iCurrActionId);
        if (VR_INVALID_ACTION_ID != m_stopVoiceTagTTs) {
            VR_LOGI("m_stopVoiceTagTTs %d", m_stopVoiceTagTTs);
            m_stopVoiceTagTTs = VR_INVALID_ACTION_ID;
            if (!m_mapStopTTSTransation.empty()) {
                VR_LOGI("clear m_mapStopTTSTransation");
                m_mapStopTTSTransation.clear();
            }
        }

        if ("" != m_iOss.str()) {
            OnRequestAction(m_iOss.str());
        }
        HandleQuitVoiceTag();
        m_enVoiceTagRecState = VR_VoiceTag_FinishRec;
        m_iPlayVoiceTagTts = VR_INVALID_ACTION_ID;

        return true;
    }
    if (!m_mapStopTTSTransation.empty()) {
        VR_LOGI("it is stoping tts");
        return true;
    }

    if ("" != m_strMsgPrompt) {
        VR_LOGI("close popup message");
        ClosePopupMsg();

        if ("" != m_strDisplayScreen) {
            VR_LOGI("jump screen");
            OnRequestAction(m_strDisplayScreen);
        }
    }

    if (NULL != VR_ConfigureIF::Instance()) {
        if (VR_ConfigureIF::Instance()->getVROverPrompt()) {
            m_bBosDetected = false;
            VR_AudioStreamIF* pcAudioStream = VR_AudioStreamIF::Instance();
            if (NULL != pcAudioStream) {
                pcAudioStream->StopAudioIn();
            }
        }
    }

    if (VR_REGION_US == m_country) {
        VR_LOGI("PVR State %d", m_PVRStateCurrent);
        if (VR_PVRState_PlayingTTS == m_PVRStateCurrent) {
            if (!m_lstCancelOption.empty()) {
                VR_LOGI("PVR -> cancel event to DM");
                HandleQuitPVRScreen();
                SendCancelResult();
                m_PVRStateCurrent = VR_PVRState_None;
                NotifyResourceState();
                return true;
            }
            else {
                VR_LOGI("PVR -> TTS over");
                m_PVRStateCurrent = VR_PVRState_PlayedTTS;
            }
        }
    }

    if (m_mapPlayTTSTransation.empty()) {
        VR_LOGI("play TTS is finished");
        return true;
    }

    TransationSpeakDone(m_iCurrActionId);

    if (VR_SessionStatePttLong == m_sessionState) {
        VR_LOGI("stop session");
        OnBeep(VR_BeepType_Done);
        return true;
    }

    VR_LOGI("play tts over");
    if (m_bPlayTTS) {
        VR_LOGI("it is by play tts!");
    }
    else { // this is not need  when the function of stop tts is achieved
        VR_LOGI("it is by stop tts!");
        VoiceMap<std::string, MessageHandler>::const_iterator iterMap = m_mapMsgBtnHandler.find(m_strBtnEventName);
        if (m_mapMsgBtnHandler.cend() != iterMap) {
            if (NULL != iterMap->second) {
                VR_LOGI("handle the event");
                if ("help" != m_strBtnEventName) {
                    CancelRecoSession();
                }
                m_bTTSNull = true;
                (this->*(iterMap->second))(parser);
            }
        }
        m_strBtnEventName = "";
    }
    m_bPlayTTS = false;

    if (m_bTaskCompelete && ("" != m_strAction)) {
        OnRequestAction(m_strAction);
    }

    if (VR_SessionStateRestartSessionAfterTTS == m_sessionState) {
        StartSession();
    }
    else if (VR_SessionStateQuitAfterTTS == m_sessionState) {
        VR_LOGI("stop session");

        // Notify the XML message to the external service
        OnBeep(VR_BeepType_Done);
    }
    else if (VR_SessionStateSendRecogAfterTTS == m_sessionState) {
        SendRecogState();
    }
    else {

    }

    if (m_bWaitGreetingEnd && (m_mapPlayTTSTransation.empty())) {
        // Check whether there have cached play request
        auto cTransAfterRender = m_cTslParamManager.GetPlayTransAfterRender();
        if (NULL == cTransAfterRender.first) {
            m_bWaitGreetingEnd = false;
            StartAppRecoSession();
        }
    }


    if (m_bQuitWaitForPrompt && (m_mapPlayTTSTransation.empty())) {
        m_bQuitWaitForPrompt = false;

        m_bWaitForDoneBeep = true;
        OnBeep(VR_BeepType_TSL_Done);
    }

    return true;
}

bool
VR_VoiceBoxEngine::StopSpeakDone(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if ((m_stopVoiceTagTTs == m_iCurrActionId)
        && (m_iCurrActionId != VR_INVALID_ACTION_ID)) {
        TransationStopSpeakDone(m_iCurrActionId);
        HandleQuitVoiceTag();
        m_enVoiceTagRecState = VR_VoiceTag_FinishRec;
        m_stopVoiceTagTTs = VR_INVALID_ACTION_ID;
        m_iPlayVoiceTagTts = VR_INVALID_ACTION_ID;

        return true;
    }

    if ("" != m_strMsgPrompt) {
        VR_LOGI("close popup message");
        ClosePopupMsg();

        if ("" != m_strDisplayScreen) {
            VR_LOGI("jump screen");
            OnRequestAction(m_strDisplayScreen);
        }
    }

    if (NULL != VR_ConfigureIF::Instance()) {
        if (VR_ConfigureIF::Instance()->getVROverPrompt()) {
            VR_AudioStreamIF* pcAudioStream = VR_AudioStreamIF::Instance();
            if ((!m_bBosDetected) && (NULL != pcAudioStream)) {
                pcAudioStream->StopAudioIn();
            }

            m_bBosDetected = false;
        }
    }

    if (VR_REGION_US == m_country) {
        VR_LOGI("PVR State %d", m_PVRStateCurrent);
        if (VR_PVRState_PlayingTTS == m_PVRStateCurrent) {
            if (!m_lstCancelOption.empty()) {
                VR_LOGI("PVR -> cancel event to DM");
                HandleQuitPVRScreen();
                SendCancelResult();
                m_PVRStateCurrent = VR_PVRState_None;
                NotifyResourceState();
                return true;
            }
            else {
                VR_LOGI("PVR -> TTS over");
                m_PVRStateCurrent = VR_PVRState_PlayedTTS;
            }
        }
    }

    if (m_bQuitWaitForPrompt) {
        m_bQuitWaitForPrompt = false;

        m_bWaitForDoneBeep = true;
        OnBeep(VR_BeepType_TSL_Done);
    }

    if (m_mapStopTTSTransation.empty()) {
        VR_LOGI("stopTTS is finished");
        if (!m_strAction.empty()) {
            OnRequestAction(m_strAction);
        }

        if ((VR_SessionStateSendRecogAfterTTS == m_sessionState)
            && m_bStartSessionWithBargeIn) {
            SendRecogState();
        }

        return true;
    }

    if (!TransationStopSpeakDone(m_iCurrActionId)) {
        VR_LOGI("speak done id is not same");
        return false;
    }

    if ((m_stopTraining == m_iCurrActionId)
        && (m_iCurrActionId != VR_INVALID_ACTION_ID)) {
        m_stopTraining = VR_INVALID_ACTION_ID;
        CancelRecoSession();
        VR_VoiceBoxXmlBuilder xmlBulder;
        pugi::xml_node xmlNode = xmlBulder.buildDisplayElement("Common", "ScreenDisplay");
        xmlBulder.buildGivenElement(xmlNode, "agent", "help", "", "");
        xmlBulder.buildGivenElement(xmlNode, "content", "na_help_train_voice_recognition", "", "");
        std::string strReply = xmlBulder.getXmlString();
        VR_LOGI("SpeakerAdaptionCatch: %s", strReply.c_str());
        OnRecognized(strReply);
    }

    if ("" == m_strBtnEventName) {
        VR_LOGI("The button event name is NULL. It is not a button event.");
        return true;
    }

    // Dispatch the messages
    VoiceMap<std::string, MessageHandler>::const_iterator iterMap = m_mapMsgBtnHandler.find(m_strBtnEventName);
    if (m_mapMsgBtnHandler.cend() != iterMap) {
        if (NULL != iterMap->second) {
            if ("help" != m_strBtnEventName) {
                VR_LOGI("stop tts to cancel session");
                VoiceList<std::string>::iterator iter = std::find(m_lstNeedReactiveAction.begin(), m_lstNeedReactiveAction.end(), m_strActionType);
                if (m_bTaskCompelete && iter == m_lstNeedReactiveAction.end()) {
                    VR_LOGI("not need to CancelSession");
                    StopAllTTS();
                }
                else {
                    CancelRecoSession();
                }
            }

            VR_LOGI("handle the event");
            m_bTTSNull = true;
            (this->*(iterMap->second))(parser);
        }
    }

    if (m_bTaskCompelete && ("" != m_strAction)) {
        OnRequestAction(m_strAction);
    }

    if (VR_SessionStateQuitAfterTTS == m_sessionState) {
        VR_LOGI("stop session");
        OnBeep(VR_BeepType_Done);
    }
    else if (VR_SessionStateSendRecogAfterTTS == m_sessionState) {
        SendRecogState();
    }
    else {

    }

    m_strBtnEventName = "";
    return true;
}

// When the beep play is completed, this function will be called.
bool
VR_VoiceBoxEngine::BeepDone(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    // handle voicetag beep, reply DM the result and closession
    if ((m_iPlayVoiceTagBeep == m_iCurrActionId) && (VR_INVALID_ACTION_ID != m_iCurrActionId)) {
        m_iPlayVoiceTagBeep = VR_INVALID_ACTION_ID;
        if (VR_INVALID_ACTION_ID != m_stopVoiceTagBeep) {
            m_stopVoiceTagBeep = VR_INVALID_ACTION_ID;
            VR_LOGI("stop beep action not come back");
        }
        if ("" != m_iOss.str()) {
           OnRequestAction(m_iOss.str());
        }
        HandleQuitVoiceTag();
        return true;
    }

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    if ((m_iStartBeepID == m_iCurrActionId) && (m_iCurrActionId != VR_INVALID_ACTION_ID)) {
        VR_LOGI("start beep");
        m_iStartBeepID = VR_INVALID_ACTION_ID;
        HRESULT result = m_engineCommand->BeepDone();
        if (FAILED(result)) {
            return false;
        }
    }

    if ((m_iDoneBeepID == m_iCurrActionId) && (m_iCurrActionId != VR_INVALID_ACTION_ID)) {
        m_strMsgPrompt = "";
        m_bPlayTTS = false;
        if (!m_lstCancelOption.empty()) {
            VR_LOGI("This is the cancel event by DM");
            SendCancelResult();
            NotifyResourceState();
        }
        HandleQuitVR();
    }

    if (m_bWaitForDoneBeep) {
        // Notify end task state to UI
        NotifyEndTaskState(false);
        m_bInEndTask = false;

        m_bWaitForDoneBeep = false;
        if (VR_AppRecoState_SendCaching != m_bAppRecoState) {

            std::string eventResult = "<event-result name=\"CancelAppRecognition\" ";
            if (m_bHandleTTSCrush) {
                VR_LOGI("the cancel event of ttscrash is handled over");
                m_bHandleTTSCrush = false;
                eventResult.append("option=\"ttscrash\" ");
            }
            eventResult.append("errcode=\"0\" />");
            OnRecognized(eventResult);

            if (m_bUpdateState) {
                VR_LOGI("log on/off -> stop UnInitialize initialize start");
                SendCancelResult();
            }
        }
        else {
            m_pcMsgController->PostMessage(m_strTslAppRecoStateMsg);
            m_strTslAppRecoStateMsg = "";
        }
    }

    return true;
}

// When the stop beep is completed, this function will be called.
bool
VR_VoiceBoxEngine::StopBeepDone(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if ((m_stopVoiceTagBeep == m_iCurrActionId) && (m_iCurrActionId != VR_INVALID_ACTION_ID)) {
        m_stopVoiceTagBeep = VR_INVALID_ACTION_ID;
        HandleQuitVoiceTag();
        OnRecognized(m_iOss.str());
        m_iPlayVoiceTagBeep = VR_INVALID_ACTION_ID;
    }

    return true;
}

// When the command is completed, call the following functions
bool
VR_VoiceBoxEngine::OnCommandComplete(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string transactionID = parser.getValueByKey("transactionid");
    std::string errorCode = parser.getValueByKey("errorstatus");
    VR_LOGI("OnCommandComplete: %s [%s]\n", transactionID.c_str(), errorCode.c_str());

    VoiceMap<std::string, MessageHandler>::const_iterator iterMap =
        m_mapCmdResultHandler.find(transactionID);
    if (m_mapCmdResultHandler.cend() != iterMap) {
        if (NULL != iterMap->second) {
            (this->*(iterMap->second))(parser);
            VR_LOGI("delete finish command info");
            m_mapCmdResultHandler.erase(iterMap);
        }
    }

    if (std::string::npos != transactionID.find("SetDataActiveSource")) {
        if (NULL != m_pcCatalogController) {
            std::string strCurrentMsg = m_pcCatalogController->GetCurrentMessage();
            VR_LOGI("strCurrentMsg = %s", strCurrentMsg.c_str());
            if (std::string::npos != strCurrentMsg.find("<grammar_active agent=\"media\"")
                || std::string::npos != strCurrentMsg.find("<grammar_disactive agent=\"media\"")) {
                SetUpdateGammarFlg(true);
            }
        }
    }

    return true;
}

// When SendRecognitionState is completed, this function will be called.
bool
VR_VoiceBoxEngine::OnSendRecognitionState(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if ("" == m_strCurrentStateName) {
        return true;
    }

    // if ((S_OK != ulCode) && (S_FALSE != ulCode)) {
    //     return false;
    // }
    VR_LOGI("current vbt name : %s", m_strCurrentStateName.c_str());

    if (VBT_RECO_STATE_VOICE_TAG == m_strCurrentStateName) {
        if (m_bPlayTTS) {
            VR_LOGI("playing tts, wait");
            m_sessionState = VR_SessionStateRestartSessionAfterTTS;
        }
        else {
            VR_LOGI("no tts");
            m_pcMsgController->PostMessage("<send event=\"start-internal\" target=\"de\"/>");
        }
    }
    else if (VBT_RECO_STATE_IN_CALL == m_strCurrentStateName) {
        if (m_bPlayTTS) {
            VR_LOGI("in call playing tts, wait");
            m_sessionState = VR_SessionStateRestartSessionAfterTTS;
            m_strPreSessionTransactionID = m_strCurrentTransactionID;
        }
        else {
            VR_LOGI("in call no tts");
            m_pcMsgController->PostMessage("<send event=\"start-internal\" target=\"de\"/>");
        }
    }
    else if (VBT_RECO_STATE_SPEAKER_ADAPTATION != m_strCurrentStateName) {
        if (VR_REGION_OC == m_country) {
            if (m_bPlayTTS) {
                VR_LOGI("playing tts, wait");
                m_sessionState = VR_SessionStateRestartSessionAfterTTS;
            }
            else {
                VR_LOGI("no tts");
                m_pcMsgController->PostMessage("<send event=\"start-internal\" target=\"de\"/>");
            }
            return true;
        }
        if (m_bStartSessionWithBargeIn) {
            m_strPreSessionTransactionID = m_strCurrentTransactionID;
            m_pcMsgController->PostMessage("<send event=\"start-bargein\" target=\"de\"/>");
        }
        else {
            m_pcMsgController->PostMessage("<send event=\"start-internal\" target=\"de\"/>");
        }
    }
    else {
        if (NULL == m_engineCommand.ptr()) {
            return false;
        }

        m_strPreSessionTransactionID = m_strCurrentTransactionID;

        CVECIPtr<IVECITransaction> transaction;
        HRESULT result = m_engineCommand->SendCommand(
                         &transaction,
                         VBT_RECO_STATE_SPEAKER_ADAPTATION,
                         VBT_COMMAND_NOOP_START_SPEAKER_ADAPTATION,
                         VBT_FALSE,
                         NULL,
                         NULL);
        if (FAILED(result)) {
            return false;
        }

        if (FAILED(result)) {
            return false;
        }

        if (FAILED(result)) {
            return false;
        }

        CVECIOutStr strTrans;
        result = transaction->GetTransactionId(&strTrans);
        if (FAILED(result)) {
            return false;
        }

        VR_LOGI("speak adaptation strTrans: %s", strTrans.Get());
        if (NULL != strTrans.Get()) {
            m_strCurrentTransactionID = strTrans.Get();
        }

        m_mapCmdResultHandler.insert(
                std::make_pair(
                    strTrans.Get(),
                    &VR_VoiceBoxEngine::OnSpeakAdaptation
                    )
                );
    }
    return true;
}

// When StartRecoSession is completed, this function will be called.
bool
VR_VoiceBoxEngine::OnStartRecoSession(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    VR_LOGI("current vbt name : %s", m_strCurrentStateName.c_str());

    if (NULL == m_pcMsgController) {
        return false;
    }

    if (m_lstTransaction.empty()) {
        VR_LOGI("start sessin Transaction is empty");
        return false;
    }
    else {
        VoiceList<std::string>::iterator iterF = m_lstTransaction.begin();
        std::string strTransaction = parser.getValueByKey("transactionid");
        if (strTransaction != *iterF) {
            VoiceList<std::string>::iterator iterFind;
            iterFind = std::find(m_lstTransaction.begin(), m_lstTransaction.end(), strTransaction);
            if (iterFind != m_lstTransaction.end()) {
                m_lstTransaction.erase(iterFind);
            }
            VR_LOGI("start sessin Transaction is not the new");
            return false;
        }
        else {
            m_lstTransaction.erase(iterF);
            VR_LOGI("start sessin Transaction is the new");
        }
    }

    if (VBT_RECO_STATE_SPEAKER_ADAPTATION == m_strCurrentStateName) {
        if (VR_SessionStateStarting == m_sessionState) {
            std::string strAction = "<action agent=\"help\" op=\"showTrainingEndMsg\" />";
            std::string strActionName = "showTrainingEndMsg";
            VR_LOGI("action op %s", strActionName.c_str());
            m_mapAction.insert(std::make_pair(
                    strActionName,
                    strAction));
            OnRequestAction(strAction);
            m_strAction = strAction;

            CVECIPtr<IVECITransaction> transaction;
            VR_LOGI("SaveSpeakerProfile: %s", m_strAdaptationPath.c_str());
            HRESULT result = m_engineCommand->SaveSpeakerProfile(
                             &transaction,
                             m_strAdaptationPath.c_str());
            OnBeep(VR_BeepType_Done);
            if (FAILED(result) || (NULL == transaction.ptr())) {
                VR_LOGI("SaveSpeakerProfile error");
                return false;
            }

            return true;
        }
    }

    if (VBT_RECO_STATE_VOICE_TAG == m_strCurrentStateName) {
        if (m_bPlayTTS) {
            VR_LOGI("This is voice tag.After TTS, quit.");
            m_iPlayVoiceTagTts = m_iCurTTSReqId;
            VR_LOGI("voicetag tts : %d", m_iPlayVoiceTagTts);
        }
        else {
            if (VR_VoiceTag_FinishRec != m_enVoiceTagRecState) {
                if ("" != m_iOss.str()) {
                    OnRequestAction(m_iOss.str());
                }
                HandleQuitVoiceTag();
            }
        }

        m_sessionState = VR_SessionStateStoped;
        m_strCurrentStateName = "";
        m_strHintsStateName = "";
        return true;
    }

    VR_LOGI("current session: %d", m_sessionState);
    switch (m_sessionState) {
    case VR_SessionStateNone:
    {
        VR_LOGE("session is not started");
        break;
    }
    case VR_SessionStateAutoSendRecogState:
    {
        VR_LOGI("send recog state");
        SendRecogState();
        break;
    }
    case VR_SessionStateStartOver:
    {
        VR_LOGI("start over");
        m_strCurrentStateName = m_strStartOverStateName;
        SendRecogState();
        break;
    }
    case VR_SessionStateAutoTutoSendRecogState:
    {
        VR_LOGI("tutorials to topmenu");
        SendRecogState();
        break;
    }
    case VR_SessionStateAutoRestartSession:
    {
        VR_LOGI("start session");
        StartSession();
        break;
    }
    case VR_SessionStatePaused:
    {
        if (VR_PROMPTLEVEL_LOW == m_iPromptLevel || VR_PROMPTLEVEL_OFF == m_iPromptLevel) {
            std::string strPrompt;
            GetPromptByScreenId("VR-SB-05", strPrompt);
            SendVRState("paused", strPrompt, "false", "");
        }
        break;
    }
    case VR_SessionStateStarting:
    case VR_SessionStateStartWithiBargeIn:
    {
        VR_LOGI("stop session");

        if (!m_cTslParamManager.GetRenderDoneSyncFlag()) {
            // Notify the XML message to the external service
            OnBeep(VR_BeepType_Done);
        }
        break;
    }
    case VR_SessionStateCanceled:
    {
        if (m_lstCancelOption.empty()) {
            OnBeep(VR_BeepType_Done);
        }
        break;
    }
    case VR_SessionStateStoped:
    case VR_SessionStateBackQuit:
    {
        VR_LOGI("session is stopped");
        break;
    }
    default:
    {
        VR_LOGI("session waiting start");
        break;
    }
    }

    return true;
}

// When Repeat is completed, this function will be called.
bool
VR_VoiceBoxEngine::OnRepeat(VR_VoiceBoxXmlParser& parser)
{
    // VBT_ULONG ulCode = S_OK;
    // if ((S_OK != ulCode) && (S_FALSE != ulCode)) {
    //     return false;
    // }

    // if (strTrans.Get() != m_strCurrentTransactionID) {
    //     return false;
    // }

    // if ("" == m_strLastTransactionID) {
    //     return false;
    // }

    // m_strLastTransactionID = "";

    StartSession();
    return true;
}

// When Back is completed, this function will be called.
bool
VR_VoiceBoxEngine::OnBack(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGI("m_sessionState = %d", m_sessionState);
    // VBT_ULONG ulCode = S_OK;
    // if ((S_OK != ulCode) && (S_FALSE != ulCode)) {
    //     return false;
    // }

    // if (strTrans.Get() != m_strCurrentTransactionID) {
    //     return false;
    // }

    // if ("" == m_strLastTransactionID) {
    //     return false;
    // }

    // m_strLastTransactionID = "";

    m_bBackInterupted = false;

    if ((VR_SessionStateBackRestart == m_sessionState)
        || (VR_SessionStateAutoRestartSession == m_sessionState)) {
        StartSession();
    }

    return true;
}

// When Help is completed, this function will be called.
bool
VR_VoiceBoxEngine::OnHelp(VR_VoiceBoxXmlParser& parser)
{
    // VBT_ULONG ulCode = S_OK;
    // if ((S_OK != ulCode) && (S_FALSE != ulCode)) {
    //     return false;
    // }

    // if (strTrans.Get() != m_strCurrentTransactionID) {
    //     return false;
    // }

    // OnRequest("<send event=\"start-internal\" target=\"de\"/>");

    if (m_bTTSNull) {
        StartSession();
    }
    else {
        m_sessionState = VR_SessionStateRestartSessionAfterTTS;
    }
    return true;
}

bool
VR_VoiceBoxEngine::OnHelpRecognized(VR_VoiceBoxXmlParser& parser)
{
    if (VR_REGION_US == m_country) {
        ShowHelpMoreHintsScreen();
    }

    return true;
}

bool
VR_VoiceBoxEngine::OnSelect(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (!m_bTaskCompelete) {
        if (m_bTTSNull) {
            StartSession();
        }
        else {
            m_sessionState = VR_SessionStateRestartSessionAfterTTS;
        }
    }
    else {
        if (m_bTTSNull) {
            VR_LOGI("stop session");

            // Notify the XML message to the external service
            OnBeep(VR_BeepType_Done);
        }
        else {
            m_sessionState = VR_SessionStateQuitAfterTTS;
        }
    }
    return true;
}


bool
VR_VoiceBoxEngine::OnSpeakAdaptation(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGI("voice traning completed");
    if (!m_bTTSNull) {
        m_sessionState = VR_SessionStateRestartSessionAfterTTS;
    }
    else {
        StartSession();
    }

    return true;
}

bool
VR_VoiceBoxEngine::OnMoreHints(VR_VoiceBoxXmlParser& parser)
{
    if (!m_bTTSNull) {
        m_sessionState = VR_SessionStateRestartSessionAfterTTS;
    }
    else {
        StartSession();
    }
    return true;
}

bool
VR_VoiceBoxEngine::OnNextPage(VR_VoiceBoxXmlParser& parser)
{
    if (!m_bTTSNull) {
        m_sessionState = VR_SessionStateRestartSessionAfterTTS;
    }
    else {
        StartSession();
    }
    return true;
}

bool
VR_VoiceBoxEngine::OnPrevPage(VR_VoiceBoxXmlParser& parser)
{
    if (!m_bTTSNull) {
        m_sessionState = VR_SessionStateRestartSessionAfterTTS;
    }
    else {
        StartSession();
    }
    return true;
}

bool
VR_VoiceBoxEngine::OnLastPage(VR_VoiceBoxXmlParser& parser)
{
    if (!m_bTTSNull) {
        m_sessionState = VR_SessionStateRestartSessionAfterTTS;
    }
    else {
        StartSession();
    }
    return true;
}

bool
VR_VoiceBoxEngine::OnFristPage(VR_VoiceBoxXmlParser& parser)
{
    if (!m_bTTSNull) {
        m_sessionState = VR_SessionStateRestartSessionAfterTTS;
    }
    else {
        StartSession();
    }
    return true;
}

bool
VR_VoiceBoxEngine::OnAlongRoute(VR_VoiceBoxXmlParser& parser)
{
    if (!m_bTaskCompelete) {
        if (m_bTTSNull) {
            StartSession();
        }
        else {
            m_sessionState = VR_SessionStateRestartSessionAfterTTS;
        }
    }
    else {
        if (m_bTTSNull) {
            VR_LOGI("stop session");

            // Notify the XML message to the external service
            OnBeep(VR_BeepType_Done);
        }
        else {
            m_sessionState = VR_SessionStateQuitAfterTTS;
        }
    }
    return true;
}

bool
VR_VoiceBoxEngine::OnNearDestination(VR_VoiceBoxXmlParser& parser)
{
    if (!m_bTaskCompelete) {
        if (m_bTTSNull) {
            StartSession();
        }
        else {
            m_sessionState = VR_SessionStateRestartSessionAfterTTS;
        }
    }
    else {
        if (m_bTTSNull) {
            VR_LOGI("stop session");

            // Notify the XML message to the external service
            OnBeep(VR_BeepType_Done);
        }
        else {
            m_sessionState = VR_SessionStateQuitAfterTTS;
        }
    }
    return true;
}

bool
VR_VoiceBoxEngine::OnConfirmYes(VR_VoiceBoxXmlParser& parser)
{
    if (!m_bTaskCompelete) {
        if (m_bTTSNull) {
            StartSession();
        }
        else {
            m_sessionState = VR_SessionStateRestartSessionAfterTTS;
        }
    }
    else {
        if (m_bTTSNull) {
            VR_LOGI("stop session");

            // Notify the XML message to the external service
            OnBeep(VR_BeepType_Done);
        }
        else {
            m_sessionState = VR_SessionStateQuitAfterTTS;
        }
    }
    return true;
}

bool
VR_VoiceBoxEngine::OnGoDirectly(VR_VoiceBoxXmlParser& parser)
{
    if (!m_bTaskCompelete) {
        if (m_bTTSNull) {
            StartSession();
        }
        else {
            m_sessionState = VR_SessionStateRestartSessionAfterTTS;
        }
    }
    else {
        if (m_bTTSNull) {
            VR_LOGI("stop session");

            // Notify the XML message to the external service
            OnBeep(VR_BeepType_Done);
        }
        else {
            m_sessionState = VR_SessionStateQuitAfterTTS;
        }
    }
    return true;
}

bool
VR_VoiceBoxEngine::OnAddToRoute(VR_VoiceBoxXmlParser& parser)
{
    if (!m_bTaskCompelete) {
        if (m_bTTSNull) {
            StartSession();
        }
        else {
            m_sessionState = VR_SessionStateRestartSessionAfterTTS;
        }
    }
    else {
        if (m_bTTSNull) {
            VR_LOGI("stop session");

            // Notify the XML message to the external service
            OnBeep(VR_BeepType_Done);
        }
        else {
            m_sessionState = VR_SessionStateQuitAfterTTS;
        }
    }
    return true;
}

bool
VR_VoiceBoxEngine::OnCall(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGI("VR_VoiceBoxEngine::OnCall:");
    if (!m_bTTSNull) {
        m_sessionState = VR_SessionStateQuitAfterTTS;
    }
    else {
        VR_LOGI("stop session");

        // Notify the XML message to the external service
        OnBeep(VR_BeepType_Done);
    }
    return true;
}

bool
VR_VoiceBoxEngine::OnSendMessage(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGI("VR_VoiceBoxEngine::OnSendMessage:");
    if (!m_bTTSNull) {
        m_sessionState = VR_SessionStateRestartSessionAfterTTS;
    }
    else {
        StartSession();
    }

    return true;
}

bool
VR_VoiceBoxEngine::OnReply(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    if (!m_bTTSNull) {
        m_sessionState = VR_SessionStateRestartSessionAfterTTS;
    }
    else {
        StartSession();
    }

    return true;
}

bool
VR_VoiceBoxEngine::OnReadNext(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (!m_bTTSNull) {
        m_sessionState = VR_SessionStateRestartSessionAfterTTS;
    }
    else {
        StartSession();
    }
    return true;
}

bool
VR_VoiceBoxEngine::OnReadPrevious(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (!m_bTTSNull) {
        m_sessionState = VR_SessionStateRestartSessionAfterTTS;
    }
    else {
        StartSession();
    }
    return true;
}

bool
VR_VoiceBoxEngine::OnConfirmNo(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (VR_SessionStateAutoTutoSendRecogState == m_sessionState) {
        if (m_bTTSNull) {
            SendRecogState();
        }
        else {
            VR_LOGI("after TTS, send recog and get hints");
            m_sessionState = VR_SessionStateSendRecogAfterTTS;
        }
    }
    else {
        if (m_bTTSNull) {
            VR_LOGI("start session");
            StartSession();
        }
        else {
            VR_LOGI("after TTS, start session");
            m_sessionState = VR_SessionStateRestartSessionAfterTTS;
        }
    }
    return true;
}

bool
VR_VoiceBoxEngine::OnInstallAgent(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    // restart engine
    std::string strRestartEngine = "<send event=\"restartEngine\"/>";
    OnRecognized(strRestartEngine);


    // test
    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    // Start the dialog engine with the specified culture
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->RestartEngine(
                     &transaction,
                     m_strCultureName.c_str()
                     );
    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_LOGE("Start Engine Failed, result: %lx, transaction: %p", result, transaction.ptr());
        return false;
    }

    // Make it a sync operation, wait until the engine is started
    VBT_BOOL bTimeout = VBT_FALSE;
    result = transaction->WaitForCompletion(INFINITE, &bTimeout);
    if (FAILED(result) || bTimeout) {
        VR_LOGE("Waiting Engine Start Failed, result: %lx, timeout: %n", result, bTimeout);
        return false;
    }

    return true;
}

bool
VR_VoiceBoxEngine::OnStartEngine(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    std::string errorCode = parser.getValueByKey("errorstatus");
    if (m_bChangeLanguage) {
        m_bChangeLanguage = false;
        if ("0x0" == errorCode) {
            ChangeLanguageResult(true);
        }
        else {
            ChangeLanguageResult(false);
            m_bEngineStarting = false;
        }
    }
    else {
        m_bEngineStarting = ("0x0" == errorCode) ? true : false;
    }

    VR_LOGI("m_bUpdateState: %d", m_bUpdateState);
    if (m_bEngineStarting && m_bUpdateState) {
        m_bUpdateState = false;
        VR_LOGI("update state that saved before uninitialize");
        if (!AutoUpdateState()) {
            VR_LOGE("updata state error");
        }

        AutoUpdateGrammar();
    }

    NotifyResourceState();

    return true;
}

bool VR_VoiceBoxEngine::AutoUpdateState()
{
    VR_LOGD_FUNC();

    VR_ConfigureIF * pcConfig = VR_ConfigureIF::Instance();
    if (NULL == pcConfig) {
        VR_LOGE("pcConfig is NULL");
        return false;
    }

    auto iterMap = m_mapUpdateStateHandler.begin();
    UpdateStateInfo updateStateInfo;
    for (; m_mapUpdateStateHandler.cend() != iterMap; ++iterMap) {
        std::string strUpdateValue;
        pcConfig->getStateValueByStateKey(iterMap->first, strUpdateValue);
        updateStateInfo.strStateName = iterMap->first;
        updateStateInfo.strStateValue = strUpdateValue;
        VR_LOGI("strStateName= %s, strStateValue =%s", updateStateInfo.strStateName.c_str(), updateStateInfo.strStateValue.c_str());
        (this->*(iterMap->second))(updateStateInfo);
    }

    return true;
}

bool
VR_VoiceBoxEngine::AutoUpdateGrammar()
{
    VR_LOGD_FUNC();
    if (NULL == m_pcMsgController) {
        VR_LOGE("m_engineCommand or m_pcMsgController is NULL");
        return false;
    }

    VR_ConfigureIF * pcConfig = VR_ConfigureIF::Instance();
    if (NULL == pcConfig) {
        VR_LOGE("pcConfig is NULL");
        return false;
    }

    auto iterLst = m_lstUpdateGrammar.begin();
    for (; m_lstUpdateGrammar.end() != iterLst; ++iterLst) {
        std::string strGrammarValue;
        pcConfig->getGrammarValueByGrammarKey(*iterLst, strGrammarValue);
        if (!strGrammarValue.empty()) {
            VR_LOGI("strGrammarKey = %s, strGrammarValue = %s", (*iterLst).c_str(), strGrammarValue.c_str());
            m_pcMsgController->PostMessage(strGrammarValue);
        }
    }

    return true;
}

void
VR_VoiceBoxEngine::ChangeLanguageResult(const bool bResult)
{

    std::string strChangeLanguage = "<event name=\"changeLanguage-internal\">";
    strChangeLanguage.append("<language>");
    strChangeLanguage.append(m_lstLanguage.back());
    strChangeLanguage.append("</language>");
    strChangeLanguage.append("</event>");
    bool bFinish = false;
    if (bResult) {
        if (m_strLanguage == m_lstLanguage.back()) {
            m_bEngineStarting = true;
            bFinish = true;
        }
        else {
            m_bEngineStarting = false;
            OnRequest(strChangeLanguage);
            VR_LOGI("start engine not OK");
        }
    }
    else {
        if (m_strLanguage != m_lstLanguage.back()) {
            OnRequest(strChangeLanguage);
        }
        else {
            bFinish = true;
        }
    }
    if (bFinish) {
        if ((NULL != m_engineCommand.ptr())) {
            CVECIPtr<IVECITransaction> transaction;
            std::string strPhoneConnect = m_bPhoneConnected ? "True" : "False";
            VR_LOGI("strPhoneConnect : %s", strPhoneConnect.c_str());
            HRESULT result = m_engineCommand->SetPreference(
                             &transaction,
                             _T("System"),
                             VBT_USR_PREF_SYSTEM_PHONECONNECTED,
                             strPhoneConnect.c_str());
            if (FAILED(result) || (NULL == transaction.ptr())) {
                VR_LOGI("set phoneconnect error");
            }
            std::string strMessageAvaiable = m_bSendMsgAvailable ? "True" : "False";
            VR_LOGI("strMessageAvaiable : %s", strMessageAvaiable.c_str());
            result = m_engineCommand->SetPreference(
                     &transaction,
                     _T("HFD"),
                     VBT_USR_PREF_HFD_SMSAVAILABLE,
                     strMessageAvaiable.c_str());
            if (FAILED(result) || (NULL == transaction.ptr())) {
                VR_LOGI("set phone smsavailable error");
            }

        }
        setPromptLevel(m_iPromptLevel);
        InitMusicState();
        SendChangeLanguageResult(m_strLanguage);
        m_lstLanguage.clear();
    }
}

bool
VR_VoiceBoxEngine::OnPttLong(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (m_bTTSNull) {
        // Notify the XML message to the external service
        OnBeep(VR_BeepType_Done);
    }
    else {
        m_sessionState = VR_SessionStateQuitAfterTTS;
    }

    return true;
}

// Start the VoiceBox engine with the specified culture
bool
VR_VoiceBoxEngine::Restart(const std::string &strCultureName)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        VR_LOGE("The engine have not been initialized");
        return false;
    }

    m_mapActiveSouceTrans.clear();
    m_pairSourceIdTransactionId.first.clear();
    m_pairSourceIdTransactionId.second.clear();
    m_bEngineStarting = false;
    NotifyResourceState();

    // Start the dialog engine with the specified culture
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->RestartEngine(
                     &transaction,
                     strCultureName.c_str()
                     );
    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "Retart Engine Failed, result: %lx, transaction: %p", result, transaction.ptr());
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnStartEngine
            )
        );

    // Make it a sync operation, wait until the engine is started
    VBT_BOOL bTimeout = VBT_FALSE;
    result = transaction->WaitForCompletion(INFINITE, &bTimeout);
    if (FAILED(result) || bTimeout) {
        VR_LOGE("Waiting Engine ReStart Failed, result: %lx, timeout: %n", result, bTimeout);
        return false;
    }

    m_strCultureName = strCultureName;
    m_strInstallCulture = strCultureName;

    // Disable VoiceBox's stream audio out function
    // According to VBT's suggestion
    std::string strMuteResponses = "true";
    if ("en-AU" == strCultureName) {
        strMuteResponses = "false";
    }
    result = m_engineCommand->SetSystemParameter(
             NULL,
             _T("SystemParam"),
             VBT_SYS_PREF_SYSTEMPARAM_MUTERESPONSES,
             strMuteResponses.c_str()
             );

    if (FAILED(result)) {
        VR_LOGE("SetSystemParameter Failed, result: %lx", result);
    }

    if ("en-AU" == strCultureName) {
        UpdateMapGrammar();
    }

    if (VR_REGION_US == m_country) {
        std::string relativePath;
        if ("" == m_strDevice) {
            m_strAdaptationPath = GetAdaptationProfilePath(strCultureName, "default");
            relativePath = strCultureName + "/default" + ".spk";
        }
        else {
            m_strAdaptationPath = GetAdaptationProfilePath(strCultureName, m_strDevice);
            relativePath = strCultureName + "/" + m_strDevice + ".spk";
        }

        VR_LOGI("relativePath : %s", relativePath.c_str());
        if (AdaptationProfileCanbeUsed(relativePath, m_strAdaptationPath)) {
            result = m_engineCommand->LoadSpeakerProfile(
                     &transaction,
                     m_strAdaptationPath.c_str());
            if (FAILED(result) || (NULL == transaction.ptr())) {
                VR_LOGI("error :SpeakAdaptation error");
            }
        }
        else {
            VR_LOGI("the training file is not be loaded");
        }
    }

    return true;
}

bool
VR_VoiceBoxEngine::StartAppRecognition(const std::string& message)
{
    VR_LOGD_FUNC();

    if (message.empty() || m_bDoCancelTslVR) {
        VR_LOGI("VBT Button Down - Ignore by cancel");
        return false;
    }

    m_bWaitGreetingEnd = false;
    m_bDoCancelTslVR = false;
    VoiceVector<std::string>::type messages;

    VR_VoiceBoxAppsXml appsXml;
    if (!appsXml.GetVBTXml(message, messages)) {
        return false;
    }

    if (1 == messages.size()) {
        if ("VBT Button Down" != appsXml.GetVBTActionType(messages[0])) {
            return false;
        }

        if (m_bIgnoreTslPtt) {
            VR_LOGI("VBT Button Down - Ignore PTT");
            return false;
        }

        auto cTransAfterRender = m_cTslParamManager.GetPlayTransAfterRender();
        if (NULL != cTransAfterRender.first) {
            return true;
        }

        if (!m_mapPlayTTSTransation.empty()) {
            VR_LOGI("VBT Button Down - Stop TTS");
            StopAllTTS();
        }

        CVECIPtr<IVECITransaction> transaction;
        HRESULT result = m_engineCommand->CancelSession(&transaction);
        if (FAILED(result) || (NULL == transaction.ptr())) {
            VR_LOGI("VBT Button Down - Cancel Session Failed");
        }

        return StartAppRecoSession();
    }
    else if (messages.size() > 1) {
        if ("VBT Send Command" != appsXml.GetVBTActionType(messages[0])) {
            return false;
        }
        if ("VBT Button Down" != appsXml.GetVBTActionType(messages[1])) {
            return false;
        }

        VR_LOGI("Send App Command");
        CVECIPtr<IVECITransaction> transaction;
        HRESULT result = m_engineCommand->SendXmlMessage(&transaction, messages[0].c_str());
        if (FAILED(result) || (NULL == transaction.ptr())) {
            return false;
        }

        CVECIOutStr strTrans;
        result = transaction->GetTransactionId(&strTrans);
        if (FAILED(result)) {
            return false;
        }

        m_bTslDialog = true;
        m_bInEndTask = false;
        m_agents[AgentType_Global]->ProcessMessage(messages[1]);
        m_cTslParamManager.SetRenderDoneSyncFlag(true);
        m_mapCmdResultHandler.insert(
            std::make_pair(
                strTrans.Get(),
                &VR_VoiceBoxEngine::OnSendAppCommand
                )
            );

        m_cTslParamManager.SetStartMessage(message);
        return true;
    }
    else {
        return false;
    }

    return true;
}

bool
VR_VoiceBoxEngine::CancelAppRecognition(const std::string& message)
{
    VR_LOGD_FUNC();

    if (m_bHandleTTSCrush) {
        VR_LOGI("TSL VR, TTS crash!");
        StopAllTTS();

        if (m_iStartBeepID != VR_INVALID_ACTION_ID) {
            VR_LOGI("the start beep is playing, we need to finish the event that run beep done");
            m_iStartBeepID = VR_INVALID_ACTION_ID;
            HRESULT result = m_engineCommand->BeepDone();
            if (FAILED(result)) {
                VR_LOGI("beepdone is failed.");
            }
        }

        if (m_iDoneBeepID != VR_INVALID_ACTION_ID) {
            m_iDoneBeepID = VR_INVALID_ACTION_ID;
            VR_LOGI("the done beep is playing, we must wait cancel app session!");
        }

        if (m_bWaitForDoneBeep) {
            VR_LOGI("TSL VR, playing tsl done beep!");
            m_bWaitForDoneBeep = false;
            m_bAppRecoState = VR_AppRecoState_Invalid;

            // Notify end task state to UI
            NotifyEndTaskState(false);
            m_bInEndTask = false;

            m_strTslAppRecoStateMsg = "";

            VR_LOGI("the cancel event of ttscrash is handled over");
            m_bHandleTTSCrush = false;
            std::string eventResult = "<event-result name=\"CancelAppRecognition\""" option=\"ttscrash\" errcode=\"0\" />";
            OnRecognized(eventResult);
            if (m_bUpdateState) {
                VR_LOGI("log on/off -> stop UnInitialize initialize start");
                SendCancelResult();
            }

            if (!m_lstCancelOption.empty()) {
                VR_LOGI("clear cancel result");
                SendCancelResult();
                NotifyResourceState();
                HandleQuitVR();
            }

            return true;
        }

        if (m_bQuitWaitForPrompt) {
            VR_LOGI("TSL VR, playing tsl last prompt!");
            m_bQuitWaitForPrompt = false;
            m_bWaitForDoneBeep = true;
            m_bAppRecoState = VR_AppRecoState_Invalid;

            OnBeep(VR_BeepType_TSL_Done);
            return true;
        }
    }

    if (message.empty()) {
        return false;
    }

    if (m_cTslParamManager.GetCancelFlag()) {
        return false;
    }
    m_cTslParamManager.SetCancelFlag(true);
    m_cTslParamManager.SetRenderDoneSyncFlag(false);

    // Notify end task state to UI
    NotifyEndTaskState(true);

    m_listAppMessages.clear();
    m_cTslParamManager.SetBackToRecoStateFlag(false);
    m_cTslParamManager.SetRecoStateMessage("");
    m_cTslParamManager.SetStartMessage("");
    m_cTslParamManager.SetRecoStateTransaction("");
    m_cTslParamManager.SetRenderingFlag(false);
    m_bDoCancelTslVR = true;
    m_bWaitGreetingEnd = false;
    m_sessionState = VR_SessionStateStoped;
    VoiceVector<std::string>::type messages;
    m_bIgnoreTslPtt = true;

    VR_VoiceBoxAppsXml appsXml;
    appsXml.GetVBTXml(message, messages);

    if (messages.size() > 0) {
        if ("VBT Cancel" != appsXml.GetVBTActionType(messages[0])) {
            return false;
        }
    }

    {
        if ((!m_bTaskCompelete) && (!m_mapPlayTTSTransation.empty())) {
            VR_LOGI("VBT Cancel - Stop TTS");
            StopAllTTS();
        }

        m_bTslDialog = false;
        auto cTransAfterRender = m_cTslParamManager.GetPlayTransAfterRender();
        if (NULL != cTransAfterRender.first) {
            cTransAfterRender.first->SpeakDone();
            cTransAfterRender.first->Release();
            m_cTslParamManager.SetPlayTransAfterRender(NULL, "");
        }

        VR_LOGI("Cancel Recognition Session");
        CVECIPtr<IVECITransaction> transaction;
        HRESULT result = m_engineCommand->CancelSession(&transaction);
        if (FAILED(result) || (NULL == transaction.ptr())) {
            return false;
        }

        result = m_engineCommand->CancelSpecific(&transaction, "SendXmlMessage");
        if (FAILED(result) || (NULL == transaction.ptr())) {
            return false;
        }
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->GetVersion(&transaction);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    m_bQuitWaitForPrompt = false;
    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnGetVersion
            )
        );

    m_bInEndTask = true;
    return true;
}

bool
VR_VoiceBoxEngine::SendAppXmlMessage(const std::string& message)
{
    VR_LOGD_FUNC();

    if (message.empty()) {
        return false;
    }

    VoiceVector<std::string>::type messages;

    VR_VoiceBoxAppsXml appsXml;
    if (!appsXml.GetVBTXml(message, messages)) {
        return false;
    }

    if (messages.size() > 0) {
        std::string msgClass = appsXml.GetMessageClass(messages[0]);
        if ("Driver" == msgClass) {
            VR_LOGI("Send XML : Driver Reply");
            HRESULT result = m_engineCommand->DriverXmlReply(messages[0].c_str());
            if (FAILED(result)) {
                return false;
            }

            return true;
        }

        std::string actionType = appsXml.GetVBTActionType(messages[0]);
        if ("VBT Cancel" == actionType) {
            VR_LOGI("Send XML : VBT Cancel");

            auto cTransAfterRender = m_cTslParamManager.GetPlayTransAfterRender();
            if (NULL != cTransAfterRender.first) {
                return true;
            }

            if (!m_mapPlayTTSTransation.empty()) {
                VR_LOGI("Send XML : VBT Cancel - Stop TTS");
                m_bWaitGreetingEnd = false;
                StopAllTTS();
            }

            if (m_bIgnoreTslPtt) {
                VR_LOGI("Send XML : Ignore VBT Cancel of PTT");
                return true;
            }

            m_bWaitConfirmation = false;
            CVECIPtr<IVECITransaction> transaction;
            HRESULT result = m_engineCommand->CancelSession(&transaction);
            if (FAILED(result) || (NULL == transaction.ptr())) {
                return false;
            }
        }
        else if ("Rendering Done" == actionType) {
            VR_LOGI("Send XML : Rendering Done");
            std::string clientRenderDone = "<send event=\"clientRenderDone\" target=\"de\"/>";
            m_agents[AgentType_Global]->ProcessMessage(clientRenderDone);

            m_cTslParamManager.SetRenderingFlag(false);
            PlayAfterRender();
        }
        else if ("VBT System Resume" == actionType) {
            VR_LOGI("Send XML : Global System Resume");
            m_bWaitConfirmation = false;
            CVECIPtr<IVECITransaction> transaction;
            HRESULT result = m_engineCommand->CancelSession(&transaction);
            if (FAILED(result) || (NULL == transaction.ptr())) {
                return false;
            }

            CVECIPtr<IVECITransaction> resumeTransaction;
            result = m_engineCommand->Resume(&resumeTransaction, VBT_TRUE);
            if (FAILED(result) || (NULL == transaction.ptr())) {
                return false;
            }
        }
        else if ("Play Tone" == actionType) {
            std::string toneName = appsXml.GetVBTActionParamValue(messages[0], "Tone Name");
            VR_LOGI("Send XML : Play Tone [%s]", toneName.c_str());
            if ("Done" == toneName) {

            }
            else if ("Transition" == toneName) {

            }
            else if ("Off-board processing" == toneName) {

            }
            else {
                // do nothing
            }
        }
        else if ("VBT Send Command"  == actionType) {
            VR_LOGI("Send XML : VBT Send Command");

            auto cTransAfterRender = m_cTslParamManager.GetPlayTransAfterRender();
            if (NULL != cTransAfterRender.first) {
                cTransAfterRender.first->SpeakDone();
                cTransAfterRender.first->Release();
                m_cTslParamManager.SetPlayTransAfterRender(NULL, "");
            }

            if (!m_mapPlayTTSTransation.empty()) {
                VR_LOGI("Send XML : Command - Stop TTS");
                m_bWaitGreetingEnd = false;
                StopAllTTS();
            }

            m_bWaitConfirmation = false;
            CVECIPtr<IVECITransaction> transaction;
            HRESULT result = m_engineCommand->CancelSession(&transaction);
            if (FAILED(result) || (NULL == transaction.ptr())) {
                return false;
            }

            std::string command = appsXml.GetVBTCommand(messages[0]);
            if ("System Global Back" == command) {
                VR_LOGI("Send XML : Global Back");
                CVECIPtr<IVECITransaction> backTransaction;
                CVECIPtr<IVECITransaction> preTransaction;
                result = m_engineCommand->Back(&backTransaction, VBT_TRUE, &preTransaction);
                if (FAILED(result) || (NULL == backTransaction.ptr())) {
                    return false;
                }

                if (NULL == preTransaction.ptr()) {
                    VR_LOGI("Send XML : Global Back - Exit");
                    return true;
                }

                CVECIOutStr strPreTrans;
                result = preTransaction->GetTransactionId(&strPreTrans);
                if (FAILED(result) || NULL == strPreTrans.Get()) {
                    return false;
                }

                m_cTslParamManager.SetBackToRecoStateFlag(false);
                if (m_cTslParamManager.GetRecoStateTransaction() == strPreTrans.Get()) {
                    m_cTslParamManager.SetBackToRecoStateFlag(true);
                }

                CVECIOutStr strTrans;
                result = backTransaction->GetTransactionId(&strTrans);
                if (FAILED(result) || (NULL == strTrans.Get())) {
                    return false;
                }

                m_mapCmdResultHandler.insert(
                    std::make_pair(
                        strTrans.Get(),
                        &VR_VoiceBoxEngine::OnAppBack
                        )
                    );

                return true;
            }
            else if ("System Repeat" == command) {
                VR_LOGI("Send XML : System Repeat");
                CVECIPtr<IVECITransaction> resumeTransaction;
                result = m_engineCommand->Resume(&resumeTransaction, VBT_TRUE);
                if (FAILED(result) || (NULL == resumeTransaction.ptr())) {
                    return false;
                }

                return true;
            }
            else if ("System Start Over" == command) {
                VR_LOGI("Send XML : System Start Over");
                m_bIgnoreTslPtt = true;
            }
            else {

            }

            CVECIPtr<IVECITransaction> sendXml;
            result = m_engineCommand->SendXmlMessage(&sendXml, messages[0].c_str());
            if (FAILED(result) || (NULL == sendXml.ptr())) {
                return false;
            }

            CVECIOutStr strTransId;
            result = sendXml->GetTransactionId(&strTransId);
            if (FAILED(result)) {
                return false;
            }

            m_mapCmdResultHandler.insert(
                std::make_pair(
                    strTransId.Get(),
                    &VR_VoiceBoxEngine::OnSendAppXmlMsg
                    )
                );
        }
        else if ("VBT Send Recognition State" == actionType) {
            VR_LOGI("Send Recognition State");
            if (!m_mapPlayTTSTransation.empty()
                && (std::string::npos == m_strActionType.find("Start Over"))) {
                StopAllTTS();
                m_strTslAppRecoStateMsg = message;
                m_bAppRecoState = VR_AppRecoState_SendCaching;
                VR_LOGI("Send Recognition State : Caching");
                return true;
            }

            m_bDoCancelTslVR = false;
            m_bTaskCompelete = false;
            m_bIgnoreTslPtt = true;
            m_bInEndTask = false;
            m_strCurrentStateName = "";
            m_bSpeakAdaptPrompt = false;

            m_cTslParamManager.SetCancelFlag(false);
            m_bPauseReturnAfterVR = false;
            m_bSession = true;
            m_pcCatalogController->PostStatusMsg("<op name=\"dialogsession\"><value>true</value></op>");

            if (NULL != m_pcCatalogManager && NULL != m_pcCatalogController) {
                SetUpdateGammarFlg(false);
                bool bNeedPauseGrammar = m_pcCatalogManager->GetNeedPauseGrammarFlg();
                if (bNeedPauseGrammar) {
                    // preview pausegrammarupdate has return, consider whether need to call pausegrammarupdate
                    VR_LOGI("start TSLVR, need to pause grammar");
                    auto pairTransaction = GetCurrentTransaction();
                    if (!pairTransaction.first.empty()) {
                        m_pcCatalogManager->PauseGrammarUpdate(pairTransaction.first);
                    }
                    else {
                        VR_LOGI("no grammar loading when startup TSLVR");
                        VR_LOGI("start TSLVR, need to pause grammar");
                        m_pcCatalogController->PostStatusMsg("<op name=\"checkpauseconditon\"><value>true</value></op>");
                    }
                }
                else {
                    VR_LOGI("preview pause grammar not return");
                }
            }

            CVECIPtr<IVECITransaction> transaction;
            HRESULT result = m_engineCommand->SendXmlMessage(&transaction, messages[0].c_str());
            if (FAILED(result) || (NULL == transaction.ptr())) {
                return false;
            }

            CVECIOutStr strTrans;
            result = transaction->GetTransactionId(&strTrans);
            if (FAILED(result) || (NULL == strTrans.Get())) {
                return false;
            }

            m_bAppRecoState = VR_AppRecoState_Sending;

            m_mapCmdResultHandler.insert(
                std::make_pair(
                    strTrans.Get(),
                    &VR_VoiceBoxEngine::OnSendAppRecognitionState
                    )
                );

            m_cTslParamManager.SetRecoStateMessage(message);
            m_cTslParamManager.SetRecoStateTransaction(strTrans.Get());
        }
        else {
            VR_LOGI("Send XML : others");
            CVECIPtr<IVECITransaction> transaction;
            HRESULT result = m_engineCommand->SendXmlMessage(&transaction, messages[0].c_str());
            if (FAILED(result) || (NULL == transaction.ptr())) {
                return false;
            }
        }
    }

    return true;
}

bool
VR_VoiceBoxEngine::SendAvcLanAppXmlMsg(const std::string& message, int actionSeqId)
{
    VR_LOGD_FUNC();

    if (message.empty()) {
        return false;
    }

    VoiceVector<std::string>::type messages;

    VR_VoiceBoxAppsXml appsXml;
    if (!appsXml.GetVBTXml(message, messages)) {
        return false;
    }

    if (IsStartApp(appsXml, messages)) {
        VR_LOGI("MEU StartAppRecognition");

        switch (m_bAppRecoState) {
        case VR_AppRecoState_Valid:
            {
                StartAppRecognition(message);
                std::string eventResult = "<event-result name=\"SendAvcLanAppXmlMsg\" errcode=\"0\"/>";
                OnRecognized(eventResult);
            }
            break;
        case VR_AppRecoState_Sending:
        case VR_AppRecoState_SendCaching:
            {
                VR_MsgInfo msgInfo;
                msgInfo.iMsgId = actionSeqId;
                msgInfo.strMsg = message;
                m_listAppMessages.push_back(msgInfo);
            }
            break;
        case VR_AppRecoState_Invalid:
            {
                std::string eventResult = "<event-result name=\"SendAvcLanAppXmlMsg\" errcode=\"1\"/>";
                OnRecognized(eventResult);
            }
            break;
        default:
            break;
        }

        return true;
    }
    else {
        if (!SendAppXmlMessage(message)) {
            return false;
        }
    }

    return true;
}

bool
VR_VoiceBoxEngine::IsStartApp(VR_VoiceBoxAppsXml& appsXml,
    const VoiceVector<std::string>::type& vecMessage)
{
    int iSize = vecMessage.size();
    if (1 == iSize) {
        if ("VBT Button Down" != appsXml.GetVBTActionType(vecMessage[0])) {
            return false;
        }
    }
    else if (2 == iSize) {
        if ("VBT Send Command" != appsXml.GetVBTActionType(vecMessage[0])) {
            return false;
        }
        if ("VBT Button Down" != appsXml.GetVBTActionType(vecMessage[1])) {
            return false;
        }
    }
    else {
        return false;
    }

    VR_LOGI("IsStartApp -> start app recognition");
    return true;
}

bool
VR_VoiceBoxEngine::OnSendAppRecognitionState(VR_VoiceBoxXmlParser& parser)
{
    std::string transactionID = parser.getValueByKey("transactionid");
    std::string errorCode = parser.getValueByKey("errorstatus");
    VR_LOGI("OnSendAppRecognitionState: %s [%s]\n", transactionID.c_str(), errorCode.c_str());

    if ("0x0" != errorCode) {
        m_bAppRecoState = VR_AppRecoState_Invalid;

        while (!m_listAppMessages.empty()) {
            VR_MsgInfo msgInfo = m_listAppMessages.front();
            m_pcMsgController->PostMessage(msgInfo.strMsg, msgInfo.iMsgId);
            m_listAppMessages.pop_front();
        }

        std::string strForceEnd = "<action name=\"notifyAppRecognitionForceEnd\"/>";
        OnRequestAction(strForceEnd);
        return false;
    }
    else {
        m_bAppRecoState = VR_AppRecoState_Valid;

        while (!m_listAppMessages.empty()) {
            VR_MsgInfo msgInfo = m_listAppMessages.front();
            m_pcMsgController->PostMessage(msgInfo.strMsg, msgInfo.iMsgId);
            m_listAppMessages.pop_front();
        }
    }

    return true;
}

bool
VR_VoiceBoxEngine::OnSendAppCommand(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string errorCode = parser.getValueByKey("errorstatus");
    VR_LOGI("OnSendAppCommand: [%s]\n", errorCode.c_str());

    if (("0x0" != errorCode) || m_bTaskCompelete) {
        return false;
    }

    m_bIgnoreTslPtt = false;

    if (!m_mapPlayTTSTransation.empty()) {
        m_bWaitGreetingEnd = true;
        return true;
    }

    auto cTransAfterRender = m_cTslParamManager.GetPlayTransAfterRender();
    if (NULL != cTransAfterRender.first) {
        m_bWaitGreetingEnd = true;
        return true;
    }

    return StartAppRecoSession();
}

bool
VR_VoiceBoxEngine::OnSendAppXmlMsg(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string errorCode = parser.getValueByKey("errorstatus");
    VR_LOGI("OnSendAppXmlMsg: [%s]\n", errorCode.c_str());

    if (("0x0" != errorCode) || m_bTaskCompelete) {
        return false;
    }

    if (!m_mapPlayTTSTransation.empty()) {
        m_bWaitGreetingEnd = true;
        return true;
    }

    auto cTransAfterRender = m_cTslParamManager.GetPlayTransAfterRender();
    if (NULL != cTransAfterRender.first) {
        m_bWaitGreetingEnd = true;
        return true;
    }

    return StartAppRecoSession();
}

bool
VR_VoiceBoxEngine::StartAppRecoSession()
{
    VR_LOGD_FUNC();

    if (m_bDoCancelTslVR) {
        return false;
    }

    VARIANT var;
    var.vt = VT_BOOL;
    var.boolVal = VARIANT_TRUE;
    m_client->SetProperty(PropClientManagedRender, &var);
    var.boolVal = VARIANT_TRUE;
    m_client->SetProperty(PropBackChangeContext, &var);

    CVECIPtr<IVECITransaction> transaction;
    VBT_BOOL bContinuous = VBT_TRUE;
    HRESULT result = m_engineCommand->StartRecoSession(&transaction, bContinuous);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
        m_lstTransaction.push_front(m_strCurrentTransactionID);
    }

    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnStartAppRecoSession
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::OnStartAppRecoSession(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string transactionID = parser.getValueByKey("transactionid");
    std::string errorCode = parser.getValueByKey("errorstatus");
    VR_LOGI("OnStartAppRecoSession: %s [%s]\n", transactionID.c_str(), errorCode.c_str());

    return true;
}

bool
VR_VoiceBoxEngine::OnGetVersion(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if ((!m_bTaskCompelete) && (!m_mapPlayTTSTransation.empty())) {
        VR_LOGI("OnGetVersion - Stop TTS");
        StopAllTTS();
    }

    HandleGrammarQuitVR();

    if (!m_mapPlayTTSTransation.empty()) {
        m_bQuitWaitForPrompt = true;
        return true;
    }

    if (m_bQuitWaitForPrompt || m_bWaitForDoneBeep) {
        VR_LOGI("OnGetVersion: Ignore redundant Done beep");
        return true;
    }

    m_bWaitForDoneBeep = true;
    OnBeep(VR_BeepType_TSL_Done);

    return true;
}

bool
VR_VoiceBoxEngine::OnAppBack(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string errorCode = parser.getValueByKey("errorstatus");
    VR_LOGI("OnAppBack: [%s]\n", errorCode.c_str());

    if (("0x0" != errorCode) || m_bTaskCompelete) {
        return false;
    }

    if (!m_mapPlayTTSTransation.empty()) {
        m_bWaitGreetingEnd = true;
        return true;
    }

    auto cTransAfterRender = m_cTslParamManager.GetPlayTransAfterRender();
    if (NULL != cTransAfterRender.first) {
        m_bWaitGreetingEnd = true;
        return true;
    }

    if (m_cTslParamManager.GetBackToRecoStateFlag()) {
        std::string strRecoStateMsg = m_cTslParamManager.GetRecoStateMessage();
        std::string strStartMsg = m_cTslParamManager.GetStartMessage();
        if (strRecoStateMsg.empty() || strStartMsg.empty()) {
            return false;
        }

        m_pcMsgController->PushFrontMessage(strStartMsg);
        m_pcMsgController->PushFrontMessage(strRecoStateMsg);
        return true;
    }

    return StartAppRecoSession();
}

HRESULT
VR_VoiceBoxEngine::CreateVocon4FrontEndShare(
    void* userData,
    IVBTVocon4FrontEndShare** ppShare
)
{
    VR_LOGD_FUNC();

    *ppShare = &s_frontEndShare;
    return S_OK;
}

void
VR_VoiceBoxEngine::OnBosDetected()
{
    VR_LOGD_FUNC();

    m_pcMsgController->PostMessage("<event name=\"onBosDetected\" />");
}

bool VR_VoiceBoxEngine::HandleBosDetected(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    // If there is no playing, do nothing
    if (!m_bPlayTTS) {
        return false;
    }

    m_strBtnEventName = "";
    m_bBosDetected = true;

    std::string strStopTTS = "<action agent=\"prompt\" op=\"stopTts\"><reqId>";
    strStopTTS.append(std::to_string(m_iCurTTSReqId));
    strStopTTS.append("</reqId></action>");

    int iStopID = OnRequestAction(strStopTTS);
    VR_LOGI("stop current speak id, %d", m_iCurTTSReqId);
    m_mapStopTTSTransation.insert(std::make_pair(iStopID, m_iCurTTSReqId));

    CancelRecoSession();

    m_bStartSessionWithBargeIn = true;
    if (VR_SessionStateAutoSendRecogState != m_sessionState
        && VR_SessionStateStartOver != m_sessionState
        && VR_SessionStateAutoTutoSendRecogState != m_sessionState
        && VR_SessionStateSendRecogAfterTTS != m_sessionState) {
        m_pcMsgController->PostMessage("<send event=\"start-bargein\" target=\"de\"/>");
    }
    return true;
}

void
VR_VoiceBoxEngine::setPromptLevel(int promptLevel)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        VR_LOGE("The engine have not been initialized");
        return;
    }
    m_iPromptLevel = promptLevel;
    std::string level;
    if (VR_PROMPTLEVEL_OFF == promptLevel) {
        level = SILENT_VERBOSITY;
        VR_LOGI("level : %s", level.c_str());
    }
    else if (VR_PROMPTLEVEL_LOW == promptLevel) {
        level = LOW_VERBOSITY;
        VR_LOGI("level : %s", level.c_str());
    }
    else if (VR_PROMPTLEVEL_HIGH == promptLevel) {
        level = HIGH_VERBOSITY;
        VR_LOGI("level : %s", level.c_str());
    }
    else {
        VR_LOGI("return");
        return;
    }
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SetSystemParameter(
                     &transaction,
                     _T("SystemParam"),
                     VBT_SYS_PREF_SYSTEMPARAM_VERBOSITY,
                     level.c_str());
    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_LOGE("SetSystemParameter Failed, result: %lx, transaction: %p", result, transaction.ptr());
        return;
    }
}

// Start a voice recognition session
bool
VR_VoiceBoxEngine::StartRecoSessionWithBargeIn(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    if (!m_bStartSessionWithBargeIn) {
        return false;
    }
    m_bStartSessionWithBargeIn = false;
    m_bWaitGreetingEnd = false;
    m_bSpeakAdaptPrompt = false;

    char* pcAecData = NULL;
    unsigned int nAecDataSize = 0;
    VR_AudioStreamIF* pcAudioStream = VR_AudioStreamIF::Instance();
    if (NULL != pcAudioStream) {
        unsigned int size = pcAudioStream->GetAecAudioTypeDataSize();
        if (0 == size) {
            return false;
        }

        pcAecData = VR_new char[size];
        if (NULL == pcAecData) {
            return false;
        }

        if (!pcAudioStream->GetAecAudioTypeData(nAecDataSize, pcAecData)) {
            delete[] pcAecData;
            pcAecData = NULL;
            return false;
        }
    }

    VR_LOGI("Get Aec data successfully!");

    VARIANT var;
    var.vt = VT_BOOL;
    var.boolVal = VARIANT_FALSE;
    if (m_cTslParamManager.GetRenderDoneSyncFlag()) {
        var.boolVal = VARIANT_TRUE;
    }
    m_client->SetProperty(PropClientManagedRender, &var);
    var.boolVal = VARIANT_TRUE;
    m_client->SetProperty(PropBackChangeContext, &var);

    CVECIPtr<IVECITransaction> transaction;
    // For VoiceTag, the continue flag should be VBT_FALSE
    VBT_BOOL bContinuous = VBT_TRUE;
    HRESULT result = m_engineCommand->StartRecoSessionWithBargeIn(
        &transaction,
        bContinuous,
        VBT_ULONG(pcAecData),
        nAecDataSize
    );

    delete[] pcAecData;
    pcAecData = NULL;

    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    VR_LOGI("Start Reco Session with barge in successfully!");
    m_sessionState = VR_SessionStateStartWithiBargeIn;

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    if (NULL != strTrans.Get()) {
        m_strCurrentTransactionID = strTrans.Get();
        m_lstTransaction.push_front(m_strCurrentTransactionID);
    }

    VR_LOGI("Start session : %s", strTrans.Get());
    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnStartRecoSession
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::HandleAction(const std::string& strAction)
{
    VR_LOGD_FUNC();

    VR_VoiceBoxXmlParser parser(strAction);
    std::string strActionName = parser.getXmlKey();
    VR_LOGI("action op %s", strActionName.c_str());
    m_mapAction.insert(std::make_pair(
            strActionName,
            strAction));

    if (VR_SessionStateAutoSendRecogState != m_sessionState
        && VR_SessionStateAutoTutoSendRecogState != m_sessionState
        && m_bTaskCompelete) {
        m_strAction = strAction;
    }
    else {
        OnRequestAction(strAction);
    }
    return true;
}

void
VR_VoiceBoxEngine::HandleQuitVR()
{
    VR_LOGD_FUNC();

    m_strMsgPrompt = "";
    m_bPlayTTS = false;
    m_iStartBeepID = VR_INVALID_ACTION_ID;
    m_iDoneBeepID = VR_INVALID_ACTION_ID;
    m_strCurrentStateName = "";
    m_strInitStateName = "";
    m_strHintsStateName = "";
    m_strInitAgentName = "";
    m_bBackInterupted = false;
    m_bHavePVR = false;
    m_bPVRScreen = false;
    m_bSettingToStartScreen = false;
    m_bPrepare = false;
    m_strScreenState.clear();
    m_enBtnProcessState = VR_Button_Null;
    NotifyResourceState();
    m_bSpeakAdaptPrompt = false;

    m_sessionState = VR_SessionStateStoped;

    if (VR_REGION_US == m_country) {
        m_PVRStateCurrent = VR_PVRState_None;
    }

    SendVRState("quit", "", "false", "");

    if ("" == m_strAction) {
        std::string strQuitVRApp = "<display xmlns=\"\" agent=\"Common\" content=\"QuitVRApp\">"
                                "</display>";

        OnRecognized(strQuitVRApp);
    }
    else {
        m_strAction = "";
    }

    // check whether TSL app  is running
    if (!m_bTslDialog) {
        if (NULL != VR_ConfigureIF::Instance()) {
            if (VR_ConfigureIF::Instance()->getVROverPrompt()) {
                VR_AudioStreamIF* pcAudioStream = VR_AudioStreamIF::Instance();
                if (NULL != pcAudioStream) {
                    pcAudioStream->StopAudioIn();
                }
            }
        }
        StopAllTTS();

        HandleGrammarQuitVR();
    }
}

bool
VR_VoiceBoxEngine::HandleQuitVRApp(const std::string& strMessage)
{
    VR_LOGD_FUNC();

    if (m_mapAction.empty()) {
        VR_LOGI("there is not action");
        return false;
    }

    VR_VoiceBoxXmlParser parser(strMessage);
    std::string strActionKey = parser.getXmlKey();
    VR_LOGI("action op %s", strActionKey.c_str());

    VoiceMap<std::string, std::string>::const_iterator cit = m_mapAction.find(strActionKey);

    if (cit != m_mapAction.cend()) {
        std::string strErrCode = parser.getValueByKey("errcode");
        std::string strAgent = parser.getValueByKey("agent");
        VR_LOGI("action errcode %s", strErrCode.c_str());
        bool bNeedVRQuit = false;
        if ("false" == strErrCode || "climate" == strAgent) {
            bNeedVRQuit = true;
        }
        else if ("true" == strErrCode) {
            if (VR_REGION_OC == m_country) {
                bNeedVRQuit = (("media" == strAgent) || "phone" == strAgent) ? true :false;
            }
            else {
                if (m_lstUCAppQuitVROp.end() != std::find(m_lstUCAppQuitVROp.begin(), m_lstUCAppQuitVROp.end(), strActionKey)) {
                    bNeedVRQuit = false;
                }
                else {
                    bNeedVRQuit = true;
                }
            }
        }
        else {

        }

        if (bNeedVRQuit) {
            std::string strQuitVRApp = "<display xmlns=\"\" agent=\"Common\" content=\"QuitVRApp\">"
                                       "</display>";

            OnRecognized(strQuitVRApp);

            m_strAction = "";
        }
        m_mapAction.erase(cit);
    }
    else {
        VR_LOGI("it is not action");
        return false;
    }

    return true;
}

bool
VR_VoiceBoxEngine::BrowseAlbums(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    BrowseMusicByType("Browse Albums");

    return true;
}

bool
VR_VoiceBoxEngine::BrowseArtists(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    BrowseMusicByType("Browse Artists");

    return true;
}

bool
VR_VoiceBoxEngine::BrowseSongs(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    BrowseMusicByType("Browse Songs");

    return true;
}

bool
VR_VoiceBoxEngine::BrowseComposers(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    BrowseMusicByType("Browse Composers");

    return true;
}

bool
VR_VoiceBoxEngine::BrowseGenres(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    BrowseMusicByType("Browse Genres");

    return true;
}

bool
VR_VoiceBoxEngine::BrowsePodcasts(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    BrowseMusicByType("Browse Podcasts");

    return true;
}

bool
VR_VoiceBoxEngine::BrowsePlaylists(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    BrowseMusicByType("Browse Playlists");

    return true;
}

bool
VR_VoiceBoxEngine::BrowseAudiobooks(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    BrowseMusicByType("Browse Audiobooks");

    return true;
}

bool
VR_VoiceBoxEngine::BrowseMusicByType(const std::string& strMusicType)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->SendCommand(
                     &transaction,
                     _T("Music"),
                     strMusicType.c_str(),
                     VBT_FALSE,
                     NULL,
                     NULL);
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    m_mapCmdResultHandler.insert(
            std::make_pair(
                strTrans.Get(),
                &VR_VoiceBoxEngine::OnMusicType));

    return true;
}

bool
VR_VoiceBoxEngine::OnMusicType(VR_VoiceBoxXmlParser& parser)
{
    if (!m_bTaskCompelete) {
        if (m_bTTSNull) {
            StartSession();
        }
        else {
            m_sessionState = VR_SessionStateRestartSessionAfterTTS;
        }
    }
    else {
        if (m_bTTSNull) {
            VR_LOGI("stop session");

            // Notify the XML message to the external service
            OnBeep(VR_BeepType_Done);
        }
        else {
            m_sessionState = VR_SessionStateQuitAfterTTS;
        }
    }

    return true;
}

int
VR_VoiceBoxEngine::OnRequestAction(const std::string& strMsg)
{
    VR_LOGD_FUNC();

    m_ActionIDSync.SynchronizeStart();
    int iID = m_iCurReqId;
    ++m_iCurReqId;

    const int VR_VBT_MAX_REQUEST_ID = 100000;
    if (m_iCurReqId >= VR_VBT_MAX_REQUEST_ID) {
        m_iCurReqId = 1;
    }
    m_ActionIDSync.SynchronizeEnd();

    if (NULL != m_pcExternalCallback) {
        m_pcExternalCallback->OnRequestAction(strMsg, iID);
    }

    return iID;
}

void
VR_VoiceBoxEngine::InsertMusicUpdateGrammar(
    const std::string& strTransId, const std::string& strSourceId, const std::string& bLast)
{
    VR_LOGI("InsertMusicUpdateGrammar: strTransId = %s, strSourceId = %s", strTransId.c_str(), strSourceId.c_str());
    if (strSourceId.empty() || strTransId.empty()) {
        return;
    }
    if (m_pairSourceIdTransactionId.first.empty()) {
        m_pairSourceIdTransactionId.first = strSourceId;
    }

    m_pairSourceIdTransactionId.second.insert(std::make_pair(strTransId, bLast));
}

void
VR_VoiceBoxEngine::ConfirmMusicUpdateGrammar(
    const std::string& strTransId, const bool bGrammarErrorCode)
{
    VR_LOGD_FUNC();
    const int MINSOURCEID = 1;
    const int MAXSOURCEID = 4;
    int iSourceId = std::atoi(m_pairSourceIdTransactionId.first.c_str());
    if (iSourceId < MINSOURCEID || iSourceId > MAXSOURCEID || strTransId.empty()) {
        VR_LOGI("iSourceId = %s, strTransId = %s", m_pairSourceIdTransactionId.first.c_str(), strTransId.c_str());
        return;
    }

    m_bGrammarErrorCode = m_bGrammarErrorCode && bGrammarErrorCode;
    auto iter = m_pairSourceIdTransactionId.second.find(strTransId);
    if (m_pairSourceIdTransactionId.second.end() != iter) {
        if ("true" == iter->second) {
            // it is the last transactionid, need to send grammar_result
            if (1 == m_pairSourceIdTransactionId.second.size()) {
                std::string strErrCode = m_bGrammarErrorCode ? "0" : "1";
                SendGrammarResult("grammar", "media", m_pairSourceIdTransactionId.first, strErrCode);
                SetGrammarInitSourceId("5");
            }
            else {
                VR_LOGI("error: this's not last transaction");
            }

            m_bGrammarErrorCode = true;
            m_pairSourceIdTransactionId.first.clear();
            m_pairSourceIdTransactionId.second.clear();
        }
        else {
            m_pairSourceIdTransactionId.second.erase(iter);
        }
    }

    return;
}

void
VR_VoiceBoxEngine::SetUpdateGammarFlg(bool bFinishUpdate)
{
    if (NULL != m_pcCatalogController) {
        if (bFinishUpdate) {
            m_pcCatalogController->PostStatusMsg("<op name=\"finishupdate\"><value>true</value></op>");
        }
        else {
            m_pcCatalogController->PostStatusMsg("<op name=\"finishupdate\"><value>false</value></op>");
        }

    }
}

// strAudioNameSrc :in/out parameter
void
VR_VoiceBoxEngine::ChangeAudioSourceName(std::string& strAudioName)
{
    if ("USB" == strAudioName && m_bIpod1 && m_bUsb2) {
        strAudioName = "USB2";
    }
    else if ("iPod" == strAudioName && m_bUsb1 && m_bIpod2) {
        strAudioName = "iPod2";
    }
    else {

    }

    return;
}

std::string
VR_VoiceBoxEngine::GetAudioConnected(const std::string& strAudioName)
{
    std::string strAudioConnect;
    if ("USB" == strAudioName) {
        if (m_bUsb1) {
            strAudioConnect = "True";
        }
        else {
            if (m_bIpod1 && m_bUsb2) {
                strAudioConnect = "True";
            }
            else {
                strAudioConnect = "False";
            }
        }
    }
    else if ("USB2" == strAudioName) {
        if (m_bIpod1 && m_bUsb2) {
            strAudioConnect = "False";
        }
        else {
            strAudioConnect = (m_bUsb2) ? "True" : "False";
        }
    }
    else if ("iPod" == strAudioName) {
        if (m_bIpod1) {
            strAudioConnect = "True";
        }
        else {
            if (m_bUsb1 && m_bIpod2) {
                strAudioConnect = "True";
            }
            else {
                strAudioConnect = "False";
            }
        }
    }
    else if ("iPod2" == strAudioName) {
        if (m_bUsb1 && m_bIpod2) {
            strAudioConnect = "False";
        }
        else {
            strAudioConnect = (m_bIpod2) ? "True" : "False";
        }
    }
    else if ("Bluetooth Audio" == strAudioName) {
        strAudioConnect = (m_bBtAudio) ? "True" : "False";
    }
    else {
        strAudioConnect = "False";
    }

    return strAudioConnect;
}

std::string
VR_VoiceBoxEngine::GetAudioConnected()
{
    return m_strUsbOrIpodConnected;
}

bool
VR_VoiceBoxEngine::InScreen(const std::string& strContent)
{
    std::string strScreenContent = strContent;
    if (VR_REGION_US == m_country) {
        strScreenContent = "na_" + strScreenContent;
    }

    if (strScreenContent != m_strScreenState) {
        return false;
    }

    return true;
}

void
VR_VoiceBoxEngine::BuildScreenContent(std::string& strContent)
{
    if ("" == strContent) {
        VR_LOGI("content is empty");
        return;
    }

    std::string strScreenContent = strContent;

    if (VR_REGION_US == m_country) {
        strScreenContent = "na_" + strScreenContent;
    }

    strContent = strScreenContent;
}

bool
VR_VoiceBoxEngine::PVRAction(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    std::string strStatusName = parser.getValueByKey("StatusName");

    m_bTslDialog = false;
    m_bInEndTask = false;
    m_bWaitGreetingEnd = false;
    m_bDoCancelTslVR = false;

    if ("PVREnter" == strStatusName) {
        m_bPVRScreen = true;
        m_bHavePVR = true;
        m_bTaskCompelete = false;
        m_bPauseReturnAfterVR = false;
        m_bSession = true;
        m_pcCatalogController->PostStatusMsg("<op name=\"dialogsession\"><value>true</value></op>");
        SetUpdateGammarFlg(false);
        if (NULL != m_pcCatalogManager && NULL != m_pcCatalogController) {
            bool bNeedPauseGrammar = m_pcCatalogManager->GetNeedPauseGrammarFlg();
            if (bNeedPauseGrammar) {
                // preview pausegrammarupdate has return, consider whether need to call pausegrammarupdate
                VR_LOGI("start normale vr, need to pause grammar");
                auto pairTransaction = GetCurrentTransaction();
                if (!pairTransaction.first.empty()) {
                    m_pcCatalogManager->PauseGrammarUpdate(pairTransaction.first);
                }
                else {
                    VR_LOGI("no grammar loading when startup vr");
                    m_pcCatalogController->PostStatusMsg("<op name=\"checkpauseconditon\"><value>true</value></op>");
                }
            }
            else {
                VR_LOGI("preview pause grammar has not return");
            }
        }

        m_PVRStateCurrent = VR_PVRState_WaitPlayTTS;
        if (NULL == m_engineCommand.ptr()) {
            return false;
        }
        CVECIPtr<IVECITransaction> transaction;
        HRESULT result = m_engineCommand->SendCommand(
                         &transaction,
                         VBT_AGENT_HELP,
                         "No-op Getting Started With Voice",
                         VBT_FALSE,
                         NULL,
                         NULL);
        if (FAILED(result) || (NULL == transaction.ptr())) {
            return false;
        }
    }
    else if ("PVRExit" == strStatusName) {
        CancelRecoSession();
    }
    else {
    }

    std::string strEventResult = "<event-result name=\"PvrTTS\">"
                                     "<StatusName>";
    strEventResult.append(strStatusName);
    strEventResult.append("</StatusName>");
    strEventResult.append("</event-result>");

    OnRequestAction(strEventResult);

    return true;
}

void
VR_VoiceBoxEngine::GetPromptByScreenId(const std::string& strScreenId, std::string& strPrompt)
{
    if (strScreenId.empty()) {
        return;
    }

    strPrompt.clear();
    m_bNBestFlg = false;

    if (VR_REGION_OC == m_country) {
        VoiceMap<std::string, std::string>::iterator iterOC = m_mapScreenPromptOC.find(strScreenId);
        if (iterOC != m_mapScreenPromptOC.end()) {
            strPrompt = iterOC->second;
        }
    }
    else if (VR_REGION_US == m_country) {
        int index = 0;
        if ("en-US" == m_strCultureName) {
            index = 2;
        }
        else if ("fr-CA" == m_strCultureName) {
            index = 1;
        }
        else if ("en-MX" == m_strCultureName) {
            index = 0;
        }
        else {

        }

        VoiceMap<std::string, VoiceVector<std::string>::type >::type::iterator iter = m_mapScreenPrompt.find(strScreenId);
        if (iter != m_mapScreenPrompt.end()) {
            strPrompt = iter->second[index];
        }
    }

    m_bNBestFlg = ("VR-SYS-01" == strScreenId) ? true : false;

    VR_LOGI("GetPromptByScreenId: strScreenId = %s, strPrompt = %s", strScreenId.c_str(), strPrompt.c_str());

    return;
}

bool
VR_VoiceBoxEngine::TransationSpeakDone()
{
    VR_LOGD_FUNC();
    VoiceMap<int, IVECITransaction*>::const_iterator iterMap =
        m_mapPlayTTSTransation.begin();

    while (iterMap != m_mapPlayTTSTransation.cend()) {
        if (NULL != iterMap->second) {
            m_bPlayTTS = false;
            iterMap->second->SpeakDone();
            iterMap->second->Release();
        }
        m_mapPlayTTSTransation.erase(iterMap++);
    }

    m_mapStopTTSTransation.clear();
    return true;
}

bool
VR_VoiceBoxEngine::TransationSpeakDone(const int& iActionID)
{
    VR_LOGD_FUNC();

    VR_LOGI("speak size : %d", m_mapPlayTTSTransation.size());
    IVECITransaction* pTran = NULL;
    VoiceMap<int, IVECITransaction*>::const_iterator iterMap =
        m_mapPlayTTSTransation.find(iActionID);
    if (m_mapPlayTTSTransation.cend() != iterMap) {
        VR_LOGI("find IVECITransaction");
        pTran = iterMap->second;
        m_mapPlayTTSTransation.erase(iterMap);
    }
    else {
        return false;
    }

    if (NULL != pTran) {
        HRESULT result = pTran->SpeakDone();
        m_bPlayTTS = false;
        pTran->Release();
        if (FAILED(result)) {
            return false;
        }
    }
    else {
        return false;
    }
    VR_LOGI("speak size : %d", m_mapPlayTTSTransation.size());

    return true;
}

bool
VR_VoiceBoxEngine::TransationStopSpeakDone(const int& iActionID)
{
    VR_LOGD_FUNC();

    VR_LOGI("speak size : %d", m_mapStopTTSTransation.size());
    m_bPlayTTS = false;
    int iPlayTTSID = 0;
    VoiceMap<int, int>::const_iterator iterMap =
        m_mapStopTTSTransation.find(iActionID);
    if (m_mapStopTTSTransation.cend() != iterMap) {
        VR_LOGI("find IVECITransaction");
        iPlayTTSID = iterMap->second;
        m_mapStopTTSTransation.erase(iterMap);
    }
    else {
        return false;
    }

    VR_LOGI("speak size : %d", m_mapStopTTSTransation.size());
    return TransationSpeakDone(iPlayTTSID);
}

void
VR_VoiceBoxEngine::StopAllTTS()
{
    VR_LOGD_FUNC();

    VoiceMap<int, IVECITransaction*>::const_iterator iterMap =
        m_mapPlayTTSTransation.cbegin();
    if (m_mapPlayTTSTransation.cend() != iterMap) {
        if (!m_bHandleTTSCrush && !m_bVoiceTagTtsCrash) {
            std::string strStopTTS = "<action agent=\"prompt\" op=\"stopTts\"><reqId>";
            strStopTTS.append(std::to_string(iterMap->first));
            strStopTTS.append("</reqId></action>");
            OnRequestAction(strStopTTS);
        }
        else if (m_bHandleTTSCrush) {
            VR_LOGI("tts is crushed, we don't need to send the request of stop tts!");
            if (VR_REGION_US == m_country) {
                VR_LOGI("tts is crushed, PVR State %d", m_PVRStateCurrent);
                if (VR_PVRState_PlayingTTS == m_PVRStateCurrent) {
                    VR_LOGI("tts is crushed, we need wait for next cancel event!");
                    m_PVRStateCurrent = VR_PVRState_None;
                    m_bHavePVR = false;
                    m_bPVRScreen = false;

                    OnRecognized("<display xmlns=\"\" agent=\"Common\" content=\"QuitVRApp\"> </display>");
                    HandleGrammarQuitVR();
                }
            }
        }
        else {
        }

        if (NULL != iterMap->second) {
            iterMap->second->SpeakDone();
            iterMap->second->Release();
        }

        ++iterMap;
    }
    m_bPlayTTS = false;

    m_mapPlayTTSTransation.clear();
    m_mapStopTTSTransation.clear();

    if (m_bHandleTTSCrush) {
        if ("" != m_strMsgPrompt) {
            VR_LOGI("tts crash, close popup message");
            ClosePopupMsg();
            m_strMsgPrompt = "";
        }

        if (m_bTaskCompelete && ("" != m_strAction)) {
            if (std::string::npos != m_strAction.find("launchApp")) {
                VR_LOGI("tts crash, don't run launching app");
                m_strAction = "";
            }
            else {
                VR_LOGI("tts crash, run action");
                OnRequestAction(m_strAction);
                m_strAction = "";
            }
        }
    }
}

bool
VR_VoiceBoxEngine::AgentHelp(VR_VoiceBoxXmlParser& parser)
{
    std::string strAgent = parser.getValueByKey("agent");

    VoiceMap<std::string, std::string>::iterator iter = m_mapAgenttoRecoState.find(strAgent);
    if (m_mapAgenttoRecoState.end() != iter) {
        m_strCurrentStateName = iter->second;
        m_strHintsStateName = m_strCurrentStateName;
    }

    VR_LOGI("get hints");
    // Prepare the hints
    if (!GetHints()) {
        return false;
    }

    return true;
}

bool
VR_VoiceBoxEngine::HintPage(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strPageType = parser.getValueByKey("pageType");
    const int  MOREHINTPAGESIZE = 5;
    if ("firstPage" == strPageType) {
        VR_LOGI("first hints page");
        m_iIndex = 0;
    }
    else if ("lastPage" == strPageType) {
        VR_LOGI("last hints page");
        int iMod = m_iHintSize % MOREHINTPAGESIZE;
        if (0 == iMod) {
            m_iIndex = m_iHintSize - MOREHINTPAGESIZE;
        }
        else {
            m_iIndex = MOREHINTPAGESIZE * (m_iHintSize / MOREHINTPAGESIZE);
        }
    }
    else if ("nextPage" == strPageType) {
        VR_LOGI("next hints page");
    }
    else if ("prevPage" == strPageType) {
        VR_LOGI("previous hints page");
        int iMod = m_iIndex % MOREHINTPAGESIZE;
        iMod = (iMod == 0) ? MOREHINTPAGESIZE : iMod;
        m_iIndex = m_iIndex - iMod - MOREHINTPAGESIZE;
    }
    else {
        return false;
    }

    DisplayHints("VR-HNT-04");

    return true;
}

bool
VR_VoiceBoxEngine::UpdateMapData(VR_VoiceBoxXmlParser& parser)
{
    std::string strCountryID = parser.getValueByKey("countryID");
    VR_LOGI("current %s, next %s", m_strCountryID.c_str(), strCountryID.c_str());
    if (strCountryID != m_strCountryID) {
        VR_ConfigureIF* pcConfig = VR_ConfigureIF::Instance();
        std::string strLanguage = pcConfig->getVRLanguage();
        if (VR_LANGUAGE_EN_AU == strLanguage) {
            Restart("en-AU");
        }
    }

    return true;
}

bool
VR_VoiceBoxEngine::InitialPersonData(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (!m_bEngineStarting) {
        VR_LOGI("engine isn't started");
        OnRecognized("<event-result name=\"initialpersondata\" />");
        return true;
    }
    SendDEStatus("voicetag", "1");
    SendDEStatus("engine", "1");
    auto pairTransaction = GetCurrentTransaction();
    if (NULL != m_pcCatalogManager && !pairTransaction.first.empty() && !pairTransaction.second.empty()) {
        m_pcCatalogManager->CancelGrammarUpdate(pairTransaction.first, true);
    }

    DeleteALLSmartAgent();

    if (NULL != m_pcMsgController) {
        ResetTslAgents();

        if (NULL != m_pcCatalogManager) {
            m_pcCatalogManager->InitialPersonData();
        }

        Restart(m_strCultureName);
    }

    InitMusicState();

    OnRecognized("<event-result name=\"initialpersondata\" />");

    return true;
}

bool
VR_VoiceBoxEngine::Prepare(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    m_bPrepare = true;
    m_bHandleTTSCrush = false;
    m_bVoiceTagTtsCrash = false;
    m_bDoCancelVoiceTag = false;
    m_strCurrentStateName = "";
    m_bSpeakAdaptPrompt = false;
    m_cTslParamManager.SetRenderDoneSyncFlag(false);

    // Recording the initial domain type/
    // when start VR, Prepare() will be invoked
    m_strInitAgentName = parser.getValueByKey("agent");
    VoiceMap<std::string, std::string>::iterator iter = m_mapAgenttoRecoState.find(m_strInitAgentName);
    if (m_mapAgenttoRecoState.end() != iter) {
        m_strInitStateName = iter->second;
    }

    OnRecognized("<event-result name=\"prepare\" errcode=\"0\"></event-result>");
    m_enBtnProcessState = VR_Button_Null;

    return true;
}

bool
VR_VoiceBoxEngine::IncomingMessageInfo(VR_VoiceBoxXmlParser& parser)
{
    m_messageinfo.instanceId = parser.getValueByKey("instanceId");
    m_messageinfo.messageId = parser.getValueByKey("messageId");
    m_messageinfo.phoneNumber = parser.getValueByKey("phoneNumber");
    m_messageinfo.messageType = parser.getValueByKey("messageType");
    m_messageinfo.sender = parser.getValueByKey("sender");
    m_messageinfo.phoneType = parser.getValueByKey("phoneType");
    m_messageinfo.subject = parser.getValueByKey("subject");
    m_messageinfo.contactID = parser.getValueByKey("contactId");
    return true;
}

bool
VR_VoiceBoxEngine::GetUpdateMapState() const
{
    return m_bUpdatingMapData;
}

bool
VR_VoiceBoxEngine::OnDMStartedNotify(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    std::string strStartFinish = "<action agent=\"destatus\" op=\"notifyStartFinish\"></action>";
    OnRequestAction(strStartFinish);

    if (!m_bEngineReady || !m_bEngineStarting) {
        SendDEStatus("engine", "1");
        return false;
    }

    if (VR_REGION_US == m_country) {
        std::string strSupportedLanguage = "<action name=\"notifyTSLVRInfo\">"
                                              "<TSLINFO>"
                                                "<Action type=\"Notify Supported Language\"/>"
                                                "<LanguageList>"
                                                  "<Item>"
                                                    "<Parameter name=\"Language\" value=\"en\"/>"
                                                  "</Item>"
                                                  "<Item>"
                                                    "<Parameter name=\"Language\" value=\"fr\"/>"
                                                  "</Item>"
                                                  "<Item>"
                                                    "<Parameter name=\"Language\" value=\"es\"/>"
                                                  "</Item>"
                                                "</LanguageList>"
                                              "</TSLINFO>"
                                            "</action>";
        OnRequestAction(strSupportedLanguage);
    }
    else if (VR_REGION_OC == m_country) {
        std::string strSupportedLanguage = "<action name=\"notifyTSLVRInfo\">"
                                              "<TSLINFO>"
                                                "<Action type=\"Notify Supported Language\"/>"
                                                "<LanguageList>"
                                                  "<Item>"
                                                    "<Parameter name=\"Language\" value=\"en\"/>"
                                                  "</Item>"
                                                "</LanguageList>"
                                              "</TSLINFO>"
                                            "</action>";
        OnRequestAction(strSupportedLanguage);

        // query route exist state when first start
        OnInfoQueryMessage("<action agent=\"navi\" op=\"queryRouteExist\" />");
    }
    else {
        // do nothing
    }

    if ((NULL != m_engineCommand.ptr())) {
        std::string strPhoneConnect = m_bPhoneConnected ? "True" : "False";
        VR_LOGI("strPhoneConnect : %s", strPhoneConnect.c_str());
        CVECIPtr<IVECITransaction> transaction;
        HRESULT result = m_engineCommand->SetPreference(
                         &transaction,
                         _T("System"),
                         VBT_USR_PREF_SYSTEM_PHONECONNECTED,
                         strPhoneConnect.c_str());
        if (FAILED(result) || (NULL == transaction.ptr())) {
            VR_LOGI("set phoneconnect error");
        }
        std::string strMessageAvaiable = m_bSendMsgAvailable ? "True" : "False";
        VR_LOGI("strMessageAvaiable : %s", strMessageAvaiable.c_str());
        result = m_engineCommand->SetPreference(
                 &transaction,
                 _T("HFD"),
                 VBT_USR_PREF_HFD_SMSAVAILABLE,
                 strMessageAvaiable.c_str());
        if (FAILED(result) || (NULL == transaction.ptr())) {
            VR_LOGI("set phone smsavailable error");
        }

        InitMusicState();
    }

    bool isVDataUpdate = (enVDataStatus_VersionChange == m_enVDataStatus ? true : false);
    bool isNeedInstallApp = false;
    if (NULL != m_pcCatalogManager) {
        m_pcCatalogManager->HandleGrammarAfterVDATAUpdate(isVDataUpdate, isNeedInstallApp);
    }

    SendDEStatus("init", "0");
    if (isNeedInstallApp) {
        GetInstallTslAgent();
        if (m_vecTSLInstallInfo.empty()) {
            VR_LOGI("there are no installed app file needed to reinstall");
            SendDEStatus("engine", "0");
            if (NULL != m_pcCatalogManager) {
                m_pcCatalogManager->UpdateGrammarFinishedStatus("app", "");
                bool isVDataUpdate = (enVDataStatus_VersionChange == m_enVDataStatus ? true : false);
                m_pcCatalogManager->RequestGrammarExceptApp(isVDataUpdate);
            }
            return true;
        }
        else {
            SendDEStatus("engine", "1");
            m_bReInstallTSL = true;
            m_bInstallingAgent = true;

            // Before install, cancel the grammar related operation
            auto pairTransaction = GetCurrentTransaction();
            if (NULL != m_pcCatalogManager && !pairTransaction.first.empty() && !pairTransaction.second.empty()) {
                m_pcCatalogManager->CancelGrammarUpdate(pairTransaction.first, true);
                SetUpdateGammarFlg(false);
            }
            if (UninstallAllAgents()) {
                VR_LOGI("unstall OK, wait for intall agent");
            }
            else {
                NotifyInstallAgentResult(false);
            }
        }

    }
    else {
        SendDEStatus("engine", "0");
    }

    return true;
}

bool
VR_VoiceBoxEngine::OnStartOver(VR_VoiceBoxXmlParser& parser)
{
    if (m_bTTSNull) {
        if (VR_SessionStateStartOver == m_sessionState) {
            if (VR_REGION_US == m_country) {
                m_strCurrentStateName = m_strInitStateName;
            }

            SendRecogState();
        }
    }
    else {
        m_sessionState = VR_SessionStateSendRecogAfterTTS;
    }

    return true;
}

void
VR_VoiceBoxEngine::NotifyInstallAgentResult(bool bInstallResult)
{
    // If install agent failed, we need to delete all the installed agents.
    if (!bInstallResult) {
        ResetTslAgents();
    }

    if (VR_REGION_US == m_country) {
        // After install, reset the engine to current language
        RestartForInstallTslAgent(m_strCultureName);
        m_strInstallCulture = m_strCultureName;
    }

    if (m_bReInstallTSL) {
        m_vecTSLInstallInfo.clear();
        m_iInstallVecIndex = 0;
        m_bInstallingAgent = false;
        m_bReInstallTSL = false;

        // Process the caching update state messages
        while (!m_listUpdateState.empty()) {
            std::string strUpdateState = m_listUpdateState.front();
            m_listUpdateState.pop_front();
            VR_VoiceBoxXmlParser parser(strUpdateState);
            UpdateState(parser);
        }

        SetUpdateGammarFlg(true);
        if (NULL != m_pcCatalogManager) {
            m_pcCatalogManager->UpdateGrammarFinishedStatus("app", "");

            // re update grammar of FM, audiosource and phone
            m_lstUpdateGrammar.clear();
            m_lstUpdateGrammar.push_back(VR_RADIO_GRAMMAR_NEW_FMGENRE);
            m_lstUpdateGrammar.push_back(VR_MUSIC_GRAMMAR_NEW_AUDIOSOURCE);
            m_lstUpdateGrammar.push_back(VR_PHONE_GRAMMAR_DISACTIVE);
            m_lstUpdateGrammar.push_back(VR_PHONE_GRAMMAR_INIT);
            m_lstUpdateGrammar.push_back(VR_PHONE_GRAMMAR_ACTIVE);
            m_lstUpdateGrammar.push_back(VR_PHONE_GRAMMAR_NEW_PHONETYPE);
            m_lstUpdateGrammar.push_back(VR_PHONE_GRAMMAR_NEW_MESSAGETYPE);
            m_lstUpdateGrammar.push_back(VR_PHONE_GRAMMAR_NEW_QUICKREPLAYMESSAGE);
            AutoUpdateGrammar();
            InitAutoUpdateStateGrammar();
            // update music grammar
            bool isVDataUpdate = (enVDataStatus_VersionChange == m_enVDataStatus ? true : false);
            m_pcCatalogManager->RequestGrammarExceptApp(isVDataUpdate);
        }

        if (!m_lstLanguage.empty()) {
            VR_LOGI("After installed, change language");
            ChangeLanguageResult(true);
        }
        else {
            SendDEStatus("engine", "0");
        }

        return;
    }

    if (!m_bAvcLan) { // DCU
        VR_LOGI("DCU notifies the process of installing agent");
        // Notify the install process for the last one,
        // so that restart engine operation is included in TSL install process.
        if (bInstallResult && !m_vecTSLInstallInfo.empty()) {
            int lastIndex = m_vecTSLInstallInfo.size() - 1;
            NotifyInstallProgress(m_vecTSLInstallInfo[lastIndex].path, lastIndex);
        }
    }
    else { // MEU
        VR_LOGI("MEU doesn't notify the process of installing agent");
    }

    std::string strInstallFormat = "<event-result name=\"install\" errcode=\"0\">"
                                     "<TSLINFO>"
                                       "<Action type=\"Installed Result\">"
                                         "<Parameter name=\"Result\" value=\"%s\"/>"
                                       "</Action>"
                                     "</TSLINFO>"
                                   "</event-result>";
    const int nInstallFormatLen = 256;
    char result[nInstallFormatLen] = { 0 };
    snprintf(result, sizeof(result), strInstallFormat.c_str(), (bInstallResult? "True" : "False"));
    OnRecognized(result);

    m_vecTSLInstallInfo.clear();
    m_iInstallVecIndex = 0;
    m_bInstallingAgent = false;

    // Process the caching update state messages
    while (!m_listUpdateState.empty()) {
        std::string strUpdateState = m_listUpdateState.front();
        m_listUpdateState.pop_front();
        VR_VoiceBoxXmlParser parser(strUpdateState);
        UpdateState(parser);
    }

    SetUpdateGammarFlg(true);
    if (!m_lstLanguage.empty()) {
        VR_LOGI("After installed, change language");
        ChangeLanguageResult(true);
    }
}

bool
VR_VoiceBoxEngine::InstallDownloadableAgents(const std::string& message)
{
    // Install TSL agent
    VR_LOGI("Install TSL agents");

    VR_VoiceBoxXmlParser parser(message);
    std::string strActionType = parser.getValueByKey("type");
    if ("Install Agent" != strActionType) {
        return false;
    }

    m_bInstallingAgent = true;

    SaveInstallTslAgent(message);

    // Before install, cancel the grammar related operation
    auto pairTransaction = GetCurrentTransaction();
    if (NULL != m_pcCatalogManager && !pairTransaction.first.empty() && !pairTransaction.second.empty()) {
        m_pcCatalogManager->CancelGrammarUpdate(pairTransaction.first, true);
        SetUpdateGammarFlg(false);
    }

    if (!DoInstallDownloadableAgents(parser)) {
        return false;
    }

    return true;
}

bool
VR_VoiceBoxEngine::DoInstallDownloadableAgents(VR_VoiceBoxXmlParser& parser)
{
    m_vecTSLInstallInfo.clear();
    m_iInstallVecIndex = 0;

    VoiceVector<Parameter>::type parameterVector = parser.getMsgParameterValue();
    if (m_bAvcLan) { // MEU
        VR_LOGI("MEU install tsl agent");
        for (size_t i = 1; i + 1 < parameterVector.size(); i += 2) {
            TSLInstallAgentInfo info;
            if ("Path" == parameterVector[i].nameValue) {
                info.path = parameterVector[i].value;
                VR_LOGI("Path: %s", info.path.c_str());
            }
            info.culture = GetCultureByFile(info.path);
            m_vecTSLInstallInfo.push_back(info);
        }

        if (m_vecTSLInstallInfo.empty()) {
            NotifyInstallAgentResult(true);
        }
        else {
            if (!DoInstallAgent(m_vecTSLInstallInfo[m_iInstallVecIndex])) {
                NotifyInstallAgentResult(false);
                return false;
            }
        }
    }
    else { // DCU
        VR_LOGI("DCU install tsl agent");
        bool bUninstallAll = false;
        if ("Uninstall Previous Agent" == parameterVector[0].nameValue) {
            if ("True" == parameterVector[0].value) {
                bUninstallAll = true;
            }

            VR_LOGI("Install - Uninstall Previous agents : %d", bUninstallAll);
        }

        for (size_t i = 1; i + 1 < parameterVector.size(); i += 2) {
            TSLInstallAgentInfo info;
            if ("Path" == parameterVector[i].nameValue) {
                info.path = parameterVector[i].value;
                VR_LOGI("Path: %s", info.path.c_str());
            }

            if ("Language" == parameterVector[i + 1].nameValue) {
                info.culture = GetCulture(parameterVector[i + 1].value);
                VR_LOGI("Language: %s", info.culture.c_str());
            }

            m_vecTSLInstallInfo.push_back(info);
        }

        NotifyInstallProgress("", -1);

        if (bUninstallAll) {
            if (UninstallAllAgents()) {
                if (m_vecTSLInstallInfo.empty()) {
                    NotifyInstallAgentResult(true);
                }
            }
            else {
                NotifyInstallAgentResult(false);
                return false;
            }
        }
        else {
            if (m_vecTSLInstallInfo.empty()) {
                NotifyInstallAgentResult(true);
            }
            else {
                if (!DoInstallAgent(m_vecTSLInstallInfo[m_iInstallVecIndex])) {
                    NotifyInstallAgentResult(false);
                    return false;
                }
            }
        }
    }
    return true;
}

std::string
VR_VoiceBoxEngine::GetCultureByFile(const std::string& strPath)
{
    std::string strCulture;
    size_t iStartPos = strPath.find_last_of("_");
    size_t iEndPos = strPath.find_last_of(".");
    size_t iLength = iEndPos - iStartPos - 1;
    strCulture = strPath.substr(iStartPos + 1, iLength);

    VR_LOGI("tsl culture : %s", strCulture.c_str());
    return strCulture;
}

void
VR_VoiceBoxEngine::SaveInstallTslAgent(const std::string& strMessage)
{
    VR_LOGD_FUNC();

    GetInstallTslAgent();

    VoiceVector<StructNode>::type vecInstall = GetSettingsInfo(strMessage, "Parameter");
    std::string strName;
    std::string strValue;

    std::string strPath;
    std::string strPathFile;
    std::string strFile;
    TSLSaveInstallInfo tslInstallInfo;

    size_t iSize = vecInstall.size();
    for (size_t iIndex = 0; iIndex < iSize - 1; iIndex += 2) {
        strName = vecInstall.at(iIndex).Value;
        strValue = vecInstall.at(iIndex + 1).Value;

        if ("Uninstall Previous Agent" == strName && "True" == strValue) {
            m_mapInstallPath.clear();
        }

        if ("Path" == strName) {
            strPathFile = strValue;
            if (GetInstallPath(strValue, strPath, strFile)) {
                VoiceMap<std::string, TSLSaveInstallInfo>::iterator it = m_mapInstallPath.find(strPath);

                tslInstallInfo.lstFile.clear();
                if (it != m_mapInstallPath.end()) {
                    tslInstallInfo.lstFile = it->second.lstFile;
                    m_mapInstallPath.erase(it);
                    VoiceList<std::string>::iterator iter = std::find(tslInstallInfo.lstFile.begin(),
                                                                    tslInstallInfo.lstFile.end(),
                                                                    strFile);
                    if (iter !=  tslInstallInfo.lstFile.end()) {
                        VR_LOGI("this file is existed, don't save!");
                    }
                    else {
                        tslInstallInfo.lstFile.push_back(strFile);
                    }
                }
                else {
                    tslInstallInfo.lstFile.push_back(strFile);
                }
            }
        }

        if ("Language" == strName) {
            if (m_bAvcLan) {
                tslInstallInfo.strLanguage = GetCultureByFile(strPathFile);
            }
            else {
                tslInstallInfo.strLanguage = GetCulture(strValue);
            }
            m_mapInstallPath.insert(
                std::make_pair(
                    strPath,
                   tslInstallInfo
                    )
                );
        }
    }

    SaveInstallTslAgent();
}

bool
VR_VoiceBoxEngine::GetInstallPath(const std::string& strPath, std::string& strInstallPath, std::string& strFile)
{
    strInstallPath = "";
    strFile = "";
    size_t iPos = strPath.find_last_of("/");

    if (std::string::npos != iPos) {
        strInstallPath = strPath.substr(0, iPos + 1);

        // remove ".dap"
        size_t iLength =  strPath.length() - iPos - 5;
        strFile = strPath.substr(iPos + 1, iLength);
        return true;
    }
    else {
        return false;
    }
}

void
VR_VoiceBoxEngine::SaveInstallTslAgent()
{
    VR_LOGD_FUNC();

    std::string strSaveInstall = "";
    if (m_mapInstallPath.empty()) {
        strSaveInstall = "";
    }
    else {
        strSaveInstall = "<Intstall><AgentList>";
        VoiceMap<std::string, TSLSaveInstallInfo>::iterator itor = m_mapInstallPath.begin();
        VoiceList<std::string>::type lstFile;

        VR_LOGI("m_mapInstallPath size = %d", m_mapInstallPath.size());
        for (; itor != m_mapInstallPath.end(); ++itor) {
            lstFile = itor->second.lstFile;
            VR_LOGI("file size = %d", lstFile.size());
            if (!lstFile.empty()) {
                VR_LOGI("file path = %s, language = %s", itor->first.c_str(), itor->second.strLanguage.c_str());
                strSaveInstall = strSaveInstall + "<Item>";
                strSaveInstall = strSaveInstall + "<file ";
                strSaveInstall = strSaveInstall + "path=\"";
                strSaveInstall = strSaveInstall + itor->first + "\" ";
                strSaveInstall = strSaveInstall + "language=\"";
                strSaveInstall = strSaveInstall + itor->second.strLanguage + "\">";

                strSaveInstall = strSaveInstall + "<fileList>";
                strSaveInstall = strSaveInstall + "<fileItem>";
                VoiceList<std::string>::const_iterator itFile = lstFile.cbegin();
                while (itFile != lstFile.cend()) {
                    strSaveInstall = strSaveInstall + "<file name=\"";
                    strSaveInstall = strSaveInstall + *itFile;
                    VR_LOGI("    name = %s", itFile->c_str());
                    strSaveInstall = strSaveInstall + "\" />";
                    ++itFile;
                }

                strSaveInstall = strSaveInstall + "</fileItem>";
                strSaveInstall = strSaveInstall + "</fileList>";
                strSaveInstall = strSaveInstall + "</file>";
                strSaveInstall = strSaveInstall + "</Item>";
            }
        }

        strSaveInstall = strSaveInstall + "</AgentList></Intstall>";
    }

    // set BL data
    VR_VoiceBoxDataStorage storage;
    storage.PutInstallTslAgent(strSaveInstall);
}

void
VR_VoiceBoxEngine::GetInstallTslAgent()
{
    VR_LOGD_FUNC();

    m_vecTSLInstallInfo.clear();
    m_mapInstallPath.clear();

    std::string strSaveInstall = "";
    VR_VoiceBoxDataStorage storage;
    storage.GetInstallTslAgent(strSaveInstall);
    VR_LOGI("get save xml: %s", strSaveInstall.c_str());
    if ("" == strSaveInstall) {
        return;
    }

    VoiceVector<StructNode>::type vecInstall = GetSettingsInfo(strSaveInstall, "file");
    std::string strPath;
    std::string strPrePath = "";
    std::string strFile;
    TSLInstallAgentInfo info;
    TSLSaveInstallInfo tslInstallInfo;
    size_t iSize = vecInstall.size();

    for (size_t iIndex = 0; iIndex < iSize; ++iIndex) {
        if ("path" == vecInstall.at(iIndex).Name) {
            strPath = vecInstall.at(iIndex).Value;

            if ((strPrePath != strPath) && ("" == strPrePath)) {
                tslInstallInfo.strLanguage = "";
                tslInstallInfo.lstFile.clear();
                strPrePath = strPath;
            }
            else if (strPrePath != strPath) {
                m_mapInstallPath.insert(
                    std::make_pair(
                        strPrePath,
                       tslInstallInfo
                        )
                    );
                strPrePath = strPath;

                VoiceMap<std::string, TSLSaveInstallInfo>::iterator it = m_mapInstallPath.find(strPath);

                if (it != m_mapInstallPath.end()) {
                    VR_LOGI("this is not first time when get this path(%s)", strPath.c_str());
                    tslInstallInfo.lstFile = it->second.lstFile;
                }
                else {
                    VR_LOGI("this is first time when get this path(%s)", strPath.c_str());
                    tslInstallInfo.lstFile.clear();
                }
            }
            else {
            }
        }
        else if ("language" == vecInstall.at(iIndex).Name) {
            info.culture  = vecInstall.at(iIndex).Value;
            tslInstallInfo.strLanguage = info.culture;
        }
        else if ("name" == vecInstall.at(iIndex).Name) {
            strFile = vecInstall.at(iIndex).Value;
            info.path = strPath + strFile + ".dap";
            m_vecTSLInstallInfo.push_back(info);
            tslInstallInfo.lstFile.push_back(strFile);
        }
        else {
            continue;
        }
    }

    m_mapInstallPath.insert(
        std::make_pair(
            strPath,
           tslInstallInfo
            )
        );

    VoiceMap<std::string, TSLSaveInstallInfo>::const_iterator itorInstall;
    VoiceList<std::string>::const_iterator itorFile;
    for (itorInstall = m_mapInstallPath.cbegin();
        itorInstall != m_mapInstallPath.cend();
        ++itorInstall) {
        VR_LOGI("Path:%s,Language:%s", itorInstall->first.c_str(), itorInstall->second.strLanguage.c_str());
        itorFile = itorInstall->second.lstFile.cbegin();
        while (itorFile != itorInstall->second.lstFile.cend()) {
            VR_LOGI("File:%s", itorFile->c_str());
            ++itorFile;
        }
    }
}

bool
VR_VoiceBoxEngine::UninstallAllAgents()
{
    VR_LOGD_FUNC();

    std::string culture;
    if (VR_REGION_US == m_country) {
        if (!UninstallAllAgentsInSync("es-MX")) {
            return false;
        }

        if (!UninstallAllAgentsInSync("fr-CA")) {
            return false;
        }

        culture = "en-US";
    }
    else if (VR_REGION_OC == m_country) {
        culture = "en-AU";
    }
    else {
        return false;
    }

    // Uninstall all TSL agent
    VR_LOGI("Uninstall all TSL agents");
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->UninstallAllDownloadableAgent(
        &transaction,
        NULL,
        culture.c_str()
    );
    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_LOGI("Uninstall all Failed");
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnAllAgentsUninstalled
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::UninstallAllAgentsInSync(std::string culture)
{
    VR_LOGD_FUNC();

    // Uninstall all the agents
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->UninstallAllDownloadableAgent(
        &transaction,
        NULL,
        culture.c_str()
    );

    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_LOGI("Uninstall all %s agents failed: %xl", culture.c_str(), result);
        return false;
    }

    VBT_BOOL bTimeout = VBT_FALSE;
    result = transaction->WaitForCompletion(INFINITE, &bTimeout);
    if (FAILED(result) || bTimeout) {
        VR_LOGI("Wait uninstall %s agents failed: %xl", culture.c_str(), result);
        return false;
    }

    return true;
}

bool
VR_VoiceBoxEngine::DoInstallAgent(TSLInstallAgentInfo& installInfo)
{
    VR_LOGD_FUNC();

    if (m_strInstallCulture != installInfo.culture) {
        if (!RestartForInstallTslAgent(installInfo.culture)) {
            return false;
        }

        m_strInstallCulture = installInfo.culture;
    }

    // Prepare the agent file path
    CVECIPtr<IVECIStringSet> spDAPFile;
    m_client->CreateStringSet(&spDAPFile);
    spDAPFile->AddItem(installInfo.path.c_str());

    // Install the agent
    m_ulInstallErrorCode = S_OK;
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->InstallDownloadableAgent(
        &transaction,
        NULL,
        _T("/tmp/"),
        installInfo.culture.c_str(),
        spDAPFile
    );
    if (FAILED(result) || (NULL == transaction.ptr())) {
        VR_LOGI("Install Failed");
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    m_mapCmdResultHandler.insert(
        std::make_pair(
            strTrans.Get(),
            &VR_VoiceBoxEngine::OnAgentInstalled
            )
        );

    return true;
}

bool
VR_VoiceBoxEngine::OnAllAgentsUninstalled(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string errorCode = parser.getValueByKey("errorstatus");
    unsigned long error = strtoul(errorCode.c_str(), NULL, 0);

    if (S_OK != error) {
        VR_LOGI("Install Downloadable Agent Failed: %d", error);
        NotifyInstallAgentResult(false);
        return false;
    }

    if (!m_vecTSLInstallInfo.empty()) {
        m_iInstallVecIndex = 0;
        if (!DoInstallAgent(m_vecTSLInstallInfo[m_iInstallVecIndex])) {
            NotifyInstallAgentResult(false);
            return false;
        }
    }

    return true;
}

bool
VR_VoiceBoxEngine::OnAgentUninstalled(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    std::string errorCode = parser.getValueByKey("errorstatus");
    unsigned long error = strtoul(errorCode.c_str(), NULL, 0);

    if (S_OK != error) {
        VR_LOGI("Uninstall Downloadable Agent Failed");
        NotifyInstallAgentResult(false);
        return false;
    }

    if (!DoInstallAgent(m_vecTSLInstallInfo[m_iInstallVecIndex])) {
        NotifyInstallAgentResult(false);
        return false;
    }

    return true;
}

bool
VR_VoiceBoxEngine::OnAgentInstalled(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string errorCode = parser.getValueByKey("errorstatus");
    unsigned long error = strtoul(errorCode.c_str(), NULL, 0);

    if ((S_OK != error)
        && (static_cast<unsigned long>(ERR_VBT_ENGINE_XML_AGENT_ALREADY_INSTALLED) != m_ulInstallErrorCode)) {
        VR_LOGI("Install Downloadable Agent Failed: 0x%08lx", m_ulInstallErrorCode);
        NotifyInstallAgentResult(false);
        return false;
    }
    else if (static_cast<unsigned long>(ERR_VBT_ENGINE_XML_AGENT_ALREADY_INSTALLED) == m_ulInstallErrorCode) {
        VR_LOGI("Install Download Agent Failed, because the download is installed!");
        return true;
    }
    else {
    }

    if (!m_bAvcLan) { // DCU
        VR_LOGI("DCU install one agent finished, update process, install next agent");
        // Notify TSL service install process, except the last one
        if (static_cast<int>(m_vecTSLInstallInfo.size()) > (m_iInstallVecIndex + 1)) {
            NotifyInstallProgress(m_vecTSLInstallInfo[m_iInstallVecIndex].path, m_iInstallVecIndex);
        }

        ++m_iInstallVecIndex;
        if (static_cast<int>(m_vecTSLInstallInfo.size()) > m_iInstallVecIndex) {
            if (!DoInstallAgent(m_vecTSLInstallInfo[m_iInstallVecIndex])) {
                NotifyInstallAgentResult(false);
                return false;
            }
        }
        else {
            NotifyInstallAgentResult(true);
        }
    }
    else { // MEU

        if (m_bReInstallTSL) {
            VR_LOGI("reinstalling, MEU install one agent");
            ++m_iInstallVecIndex;
            if (static_cast<int>(m_vecTSLInstallInfo.size()) > m_iInstallVecIndex) {
                if (!DoInstallAgent(m_vecTSLInstallInfo[m_iInstallVecIndex])) {
                    NotifyInstallAgentResult(false);
                    return false;
                }
            }
            else {
                NotifyInstallAgentResult(true);
            }
        }
        else {
            VR_LOGI("MEU install one agent finished");
            NotifyInstallAgentResult(true);
        }
    }

    return true;
}

std::string
VR_VoiceBoxEngine::GetCulture(std::string& language)
{
    std::string culture;
    if (VR_REGION_US == m_country) {
        if ("en" == language) {
            culture = "en-US";
        }
        else if ("fr" == language) {
            culture = "fr-CA";
        }
        else if ("es" == language) {
            culture = "es-MX";
        }
        else {
            // do nothing
        }
    }
    else if (VR_REGION_OC == m_country) {
        if ("en" == language) {
            culture = "en-AU";
        }
    }
    else {
    }

    return culture;
}

void
VR_VoiceBoxEngine::ClosePopupMsg()
{
    VR_LOGD_FUNC();

    std::string strDismissPopupMessage = "<display agent=\"Common\" content=\"DismissPopupMessage\">";
    strDismissPopupMessage.append("<prompt>");
    strDismissPopupMessage.append(m_strMsgPrompt);
    strDismissPopupMessage.append("</prompt>");
    strDismissPopupMessage.append("<type>normal</type>");
    strDismissPopupMessage.append("</display>");

    OnRequestAction(strDismissPopupMessage);
    m_strMsgPrompt = "";
}

void
VR_VoiceBoxEngine::UpdateMapGrammar()
{
    VR_ConfigureIF* pcConfig = VR_ConfigureIF::Instance();
    std::string strMessage = "<action-result agent=\"navi\" op=\"requestDefaultInfo\" ><countryId>";
    if (NULL != pcConfig) {
        if (VR_PRODUCT_TYPE_L1 != pcConfig->getVRProduct()) {
            std::string strMapdata = pcConfig->getMapDataPath();
            if (1 == pcConfig->getCountryIDForVBT()) {
                VR_LOGI("AU");
                m_strCountryID = "1";
                strMapdata = strMapdata + "/ENA_AU.osd";
                strMessage.append("1");
            }
            else {
                VR_LOGI("NZ");
                m_strCountryID = "2";
                strMapdata = strMapdata + "/ENA_NZ.osd";
                strMessage.append("2");
            }

            std::fstream fileOSD;
            fileOSD.open(strMapdata, std::ios::in);
            if (fileOSD) {
                fileOSD.close();

                VR_LOGI("set CountryGrammarArchive");
                m_engineCommand->SetPreference(
                     NULL,
                     VBT_AGENT_SYSTEM,
                     VBT_USR_PREF_SYSTEM_COUNTRYGRAMMARARCHIVE,
                     strMapdata.c_str()
                     );
            }
            else {
                VR_LOGI("file is not exist!");
                m_engineCommand->SetPreference(
                     NULL,
                     VBT_AGENT_SYSTEM,
                     VBT_USR_PREF_SYSTEM_COUNTRYGRAMMARARCHIVE,
                     ""
                     );
            }

            VR_LOGI("set POI Grammar");
            OnRequest("<grammar_init agent=\"poi\" />");
        }
    }
    else {
        strMessage.append("1");
    }
    strMessage.append("</countryId></action-result>");
    VR_VoiceBoxXmlParser parser(strMessage);
    m_agents[AgentType_Navi]->ReplyQueryInfo(parser);
}

void
VR_VoiceBoxEngine::SendGrammarResult(
    const std::string& strOp, const std::string& strAgent,
    const std::string& strGrammarId, const std::string& strErrCode)
{
    if (!m_bNeedGrammarResult || m_bUpdateState) {
        VR_LOGI("do not send the grammar_result to DM");
        return;
    }
    std::string strGrammarResult = "<grammar_result op=\"";
    strGrammarResult.append(strOp);
    strGrammarResult.append("\" agent=\"");
    strGrammarResult.append(strAgent);
    strGrammarResult.append("\" grammarid=\"");
    strGrammarResult.append(strGrammarId);
    strGrammarResult.append("\" errcode=\"");
    strGrammarResult.append(strErrCode);
    strGrammarResult.append("\" />");
    VR_LOGI("strGrammarResult = %s", strGrammarResult.c_str());
    OnRecognized(strGrammarResult);
}

bool
VR_VoiceBoxEngine::OnEscalatingError(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (VR_REGION_US == m_country) {
        ShowHelpMoreHintsScreen();
    }

    return true;
}

void
VR_VoiceBoxEngine::ShowHelpMoreHintsScreen()
{
    VR_LOGD_FUNC();

    if (("na_topmenu_idle" == m_strScreenState)
        || ("na_media_idle" == m_strScreenState)
        || ("na_phone_idle" == m_strScreenState)
        || ("na_apps_idle" == m_strScreenState)
        || ("na_climate_idle" == m_strScreenState)) {
        // Prepare the help related hints
        PrepareHintsContent("Help");

        // show more hints screen on top screen
        m_bMoreHint = true;
        m_iIndex = 0;

        DisplayHints("VR-HNT-04");
    }
}

void
VR_VoiceBoxEngine::SetActiveSouceTrans(
    const std::string& strTransId, const std::string& strOp, const std::string& strSouceId)
{
    VR_LOGI("strTransId = %s, strOp = %s, strSouceId = %s", strTransId.c_str(), strOp.c_str(), strSouceId.c_str());
    m_mapActiveSouceTrans.insert(std::make_pair(strTransId, std::make_pair(strOp, strSouceId)));
}

void
VR_VoiceBoxEngine::GetOpActiveSouce(const std::string& strTransId, std::string& strOp, std::string& strActiveSource)
{
    strOp.clear();
    strActiveSource.clear();

    VR_LOGI("strTransId = %s", strTransId.c_str());

    VoiceMap<std::string, std::pair<std::string, std::string>>::iterator iter = m_mapActiveSouceTrans.find(strTransId);
    if (m_mapActiveSouceTrans.end() != iter) {
        strOp = iter->second.first;
        strActiveSource = iter->second.second;
    }
}

std::string
VR_VoiceBoxEngine::getHints(const std::string& hintsParams)
{
    VR_LOGD_FUNC();

    if (hintsParams.empty() || NULL == m_client.ptr()) {
        return "";
    }

    VR_VoiceBoxXmlParser parser(hintsParams);

    m_strAction = "";
    m_lstTransaction.clear();

    std::string strStateName = parser.getValueByKey("agentName");
    // std::string strPageSize = parser.getValueByKey("pageSize");

    HRESULT retCode = m_client->CreateStringSet(&m_spHints);
    if (S_OK != retCode) {
        return "";
    }
    if (NULL == m_spHints.ptr()) {
        return "";
    }

    m_bMoreHint = false;

    VBT_INT nScreenId = m_mapHintsScreenID["none"];
    std::string strVBTStateName;

    GetVBTInfoByUIStateName(strStateName, nScreenId, strVBTStateName);

    m_strUIStateName = strStateName;

    CVECIPtr<IVECIListItems> optionalContextList;
    m_client->CreateListItems(&optionalContextList);
    if (NULL == optionalContextList.ptr()) {
        return "";
    }

    SetOptionalContext(optionalContextList, strVBTStateName, false);

    if (NULL == m_engineCommand.ptr()) {
        return "";
    }

    retCode = m_engineCommand->GetRecognitionStateHints("Global", nScreenId, optionalContextList, &m_spHints);
    if (S_OK != retCode) {
        VR_LOGI("GetRecognitionStateHints: %x", retCode);
        return "";
    }

    m_iIndex = 0;
    m_iHintSize = 0;

    m_spHints->GetSize(&m_iHintSize);
    std::string strHints = SynchronizeHints();

    return strHints;
}

std::string
VR_VoiceBoxEngine::SynchronizeHints()
{
    VR_LOGD_FUNC();

    if (NULL == m_spHints) {
        return "";
    }

    const int  HINTPAGESIZEUS = 4;
    const int  HINTPAGESIZEOC = 5;
    int iHintPageSize = (VR_REGION_OC == m_country) ? HINTPAGESIZEOC : HINTPAGESIZEUS;

    VR_VoiceBoxXmlBuilder xmlBulder;
    pugi::xml_node node = xmlBulder.buildDisplayElement("Common", "HintsDisplay");
    xmlBulder.buildGivenElement(node, "agent", m_strUIStateName.c_str(), "", "");
    xmlBulder.buildGivenElement(node, "content", m_strContent.c_str(), "", ""); // content
    pugi::xml_node nodeHints = xmlBulder.buildGivenElement(node, "hints", "", "", "");
    pugi::xml_node nodeList = xmlBulder.buildGivenElement(nodeHints, "list", "", "", "");
    pugi::xml_node nodeHeader = xmlBulder.buildGivenElement(nodeList, "header", "", "", "");
    xmlBulder.buildGivenElement(nodeHeader, "startIndex", std::to_string(m_iIndex), "", "");
    xmlBulder.buildGivenElement(nodeHeader, "pageSize", std::to_string(iHintPageSize), "", "");
    xmlBulder.buildGivenElement(nodeHeader, "count", std::to_string(m_iHintSize), "", "");
    pugi::xml_node nodeItems = xmlBulder.buildGivenElement(nodeList, "items", "", "", "");

    VoiceVector<std::string>::type vecItem;
    std::string strHintStart;
    std::string strHintEnd;
    if ("fr-CA" == m_strCultureName) {
        strHintStart = "\u00AB ";
        strHintEnd = " \u00BB";
    }
    else {
        strHintStart = "\u201C";
        strHintEnd = "\u201D";
    }

    // Retrieve Each Hints
    for (int i = 0; i < iHintPageSize; ++i) {
        CVECIOutStr strHint;
        (void)m_spHints->GetItem(m_iIndex, &strHint);
        if (m_iIndex < m_iHintSize) {
            ++m_iIndex;
        }
        else {
            break;
        }

        VoiceVector<StructNode>::type attributeVector;
        StructNode strNode;

        strNode.Name = "hint";
        if (NULL != strHint.Get()) {
            std::string strHintTmp = strHint.Get();
            strNode.Value = strHintStart;
            strNode.Value.append(strHintTmp);
            strNode.Value.append(strHintEnd);
        }
        attributeVector.push_back(strNode);

        xmlBulder.buildListItemChildElement(nodeItems, "", hint, attributeVector);
    }

    return xmlBulder.getXmlString();
}

void
VR_VoiceBoxEngine::SetNBestScreenFlg(const bool& bNBestFlg)
{
    m_bNBestFlg = bNBestFlg;
}

bool
VR_VoiceBoxEngine::GetNBestScreenFlg()
{
    return m_bNBestFlg;
}

bool
VR_VoiceBoxEngine::ShowHintScreen(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    VR_LOGI("get hints");
    // Prepare the hints
    if (!GetHints()) {
        return false;
    }

    return true;
}

bool
VR_VoiceBoxEngine::IsAppsInstalled()
{
    return m_bTslAppsAvailable;
}

bool
VR_VoiceBoxEngine::IsAppsEnabled()
{
    return m_bTslNetworkAvailable;
}

void VR_VoiceBoxEngine::NotifyResourceState()
{
    VR_LOGD_FUNC();

    if (m_bWaitResumeGrammar) {
        VR_LOGI("m_bWaitResumeGrammar is true");
    }

    std::string strState = "0";
    if (!m_lstCancelOption.empty() || !m_bEngineStarting || m_bWaitResumeGrammar) {
        strState = "1";
    }

    if (strState == m_strCurResourceState) {
        VR_LOGI("m_strCurResourceState [%s] is not changed", strState.c_str());
        return;
    }
    else {
        m_strCurResourceState = strState;
        VR_LOGI("m_strCurResourceState is changed to [%s]", m_strCurResourceState.c_str());
        SendDEStatus("engine", m_strCurResourceState);
    }
}

void VR_VoiceBoxEngine::SendDEStatus(const std::string& strType, const std::string& strStatus)
{
    std::string strEventResult = "<action agent=\"destatus\" op=\"notifyResourceState\">";
    strEventResult.append("<resourceStateType>");
    strEventResult.append(strType);
    strEventResult.append("</resourceStateType>");
    strEventResult.append("<resourceState>");
    strEventResult.append(strStatus);
    strEventResult.append("</resourceState>");
    strEventResult.append("</action>");

    OnRequestAction(strEventResult);
}

void
VR_VoiceBoxEngine::GrammarDiffDetail(const std::string& strMessage)
{
    VR_LOGD_FUNC();
    if (strMessage.empty()) {
        return;
    }

    VoiceList<std::string>::type lstName;
    GetValidMusicTableName(lstName);
    if (lstName.empty()) {
        return;
    }

    pugi::xml_document document;
    if (!document.load(strMessage.c_str())) {
        VR_LOGI("load fail ");
        return;
    }

    VoiceVector<pugi::xml_node>::type vecDiffInfo;
    pugi::xml_document newDoc;
    pugi::xpath_node_set nodset = document.select_nodes("grammar_diff/category[@name]");
    for (pugi::xpath_node_set::const_iterator it = nodset.begin(); nodset.end() != it; ++it) {
        pugi::xml_node nodeCategory = it->node();
        std::string strName = nodeCategory.attribute("name").value();
        if (lstName.end() != std::find(lstName.begin(), lstName.end(), strName)) {
            for (pugi::xml_node nodeInner = nodeCategory.first_child(); nodeInner; nodeInner = nodeInner.next_sibling()) {
                if (0 == strcmp("add", nodeInner.name()) || 0 == strcmp("delete", nodeInner.name())) {
                    pugi::xml_node newNode = newDoc.append_child("category");
                    newNode.append_attribute("name").set_value(strName.c_str());
                    newNode.append_copy(nodeInner);
                    vecDiffInfo.push_back(newNode);
                }
            }
        }
    }

    if (vecDiffInfo.empty()) {
        VR_LOGI("no data need to update");
        return;
    }

    // grammar_diff node info
    pugi::xml_node rootNode = document.first_child();
    while (rootNode.remove_child("category")) {
        // delete all child nodes;
    }
    std::ostringstream ossroot;
    rootNode.print(ossroot);
    ProcessGrammarMSg(ossroot.str().c_str());

    pugi::xml_node& lastNode = vecDiffInfo[vecDiffInfo.size() -1];
    lastNode.insert_attribute_after("isLast", lastNode.attribute("name")) = "true";
    for (size_t index = 0; index <  vecDiffInfo.size(); ++index) {
        std::ostringstream oss;
        vecDiffInfo[index].print(oss);
        ProcessGrammarMSg(oss.str().c_str());
    }

    return;
}

void
VR_VoiceBoxEngine::GetValidMusicTableName(VoiceList<std::string>::type& lstName)
{
    lstName.clear();

    if (VR_REGION_OC != m_country && VR_REGION_US != m_country) {
        return;
    }

    lstName.push_back("song");
    lstName.push_back("album");
    lstName.push_back("artist");
    lstName.push_back("playlist");

    if (VR_REGION_US == m_country) {
        lstName.push_back("genre");
        lstName.push_back("audiobook");
        lstName.push_back("composer");
        lstName.push_back("podcast");
    }
}

void
VR_VoiceBoxEngine::GrammarNewDetail(const std::string& strMessage)
{
   if (strMessage.empty()) {
        return;
    }

    pugi::xml_document document;
    if (!document.load(strMessage.c_str())) {
        VR_LOGI("load fail ");
        return;
    }

    pugi::xpath_node_set nodset = document.select_nodes("grammar_new/category[@name]");
    for (pugi::xpath_node_set::const_iterator it = nodset.begin(); nodset.end() != it; ++it) {
        pugi::xml_node node = (*it).node();
        std::string strName = node.attribute("name").value();
        if (m_lstGrammarNewName.end() != std::find(m_lstGrammarNewName.begin(), m_lstGrammarNewName.end(), strName)) {
            std::ostringstream oss;
            node.print(oss);
            std::string strCategory = oss.str();
            ProcessGrammarMSg(strCategory);
        }
    }
}

void
VR_VoiceBoxEngine::StopTraining()
{
    if (m_bPlayTTS) {
        VR_LOGI("playing tts. stop playing");
        m_bPlayTTS = false;
        std::string strStopTTS = "<action agent=\"prompt\" op=\"stopTts\"><reqId>";
        strStopTTS.append(std::to_string(m_iCurTTSReqId));
        strStopTTS.append("</reqId></action>");

        m_stopTraining = OnRequestAction(strStopTTS);
        VR_LOGI("m_stopTraining, %d", m_stopTraining);
        m_mapStopTTSTransation.insert(std::make_pair(m_stopTraining, m_iCurTTSReqId));
    }
    else {
        VR_LOGI("no tts , handle");
        CancelRecoSession();
        VR_VoiceBoxXmlBuilder xmlBulder;
        pugi::xml_node xmlNode = xmlBulder.buildDisplayElement("Common", "ScreenDisplay");
        xmlBulder.buildGivenElement(xmlNode, "agent", "help", "", "");
        xmlBulder.buildGivenElement(xmlNode, "content", "na_help_train_voice_recognition", "", "");
        std::string strReply = xmlBulder.getXmlString();
        VR_LOGI("SpeakerAdaptionCatch: %s", strReply.c_str());
        OnRecognized(strReply);
    }
}

void
VR_VoiceBoxEngine::ResetTslAgents()
{
    VR_LOGD_FUNC();

    if (VR_REGION_US == m_country) {
        UninstallAllAgentsInSync("es-MX");
        UninstallAllAgentsInSync("fr-CA");
        UninstallAllAgentsInSync("en-US");
    }
    else if (VR_REGION_OC == m_country) {
        UninstallAllAgentsInSync("en-AU");
    }
    else {
        // do nothing
    }
}

std::string
VR_VoiceBoxEngine::GetMDActiveSourceId()
{
    return m_strDMActiveSourceId;
}

void
VR_VoiceBoxEngine::SetGrammarInitSourceId(const std::string& strGrammarInitSourceId)
{
    m_strGrammarInitSourceId = strGrammarInitSourceId;
}

bool
VR_VoiceBoxEngine::ResetGrammarUpdate()
{
    if (NULL == m_pcCatalogController) {
        return false;
    }
    std::string strCurrentCatalogMsg = m_pcCatalogController->GetCurrentMessage();
    if ((std::string::npos != strCurrentCatalogMsg.find("<grammar_init"))
        || (std::string::npos != strCurrentCatalogMsg.find("<grammar_active agent=\"phone\""))
        || (std::string::npos != strCurrentCatalogMsg.find("<grammar_diff"))
        || (std::string::npos != strCurrentCatalogMsg.find("<add>"))
        || (std::string::npos != strCurrentCatalogMsg.find("<delete>"))
        || (std::string::npos != strCurrentCatalogMsg.find("<grammar_disactive agent=\"phone\">"))) {
        return false;
    }
    else {
        m_pcCatalogController->PostStatusMsg("<op name=\"updateagain\"></op>");
        return true;
    }
}

bool
VR_VoiceBoxEngine::GetEngineStatus()
{
    // During install agents, the VR engine should not be used
    if (m_bInstallingAgent || m_bReInstallTSL) {
        return false;
    }

    return m_bEngineStarting;
}

void
VR_VoiceBoxEngine::SetWaitResumeGrammar(bool bWaitResumeGrammar)
{
    VR_LOGD_FUNC();
    if (bWaitResumeGrammar && m_bPauseReturnAfterVR) {
        VR_LOGI("aftrer quitvrapp, pausegrammar return");
        m_bPauseReturnAfterVR = false;
        m_pcCatalogController->PostStatusMsg("<op name=\"needresume\"><value>true</value></op>");
    }

    m_bWaitResumeGrammar = bWaitResumeGrammar;

    NotifyResourceState();
}

void
VR_VoiceBoxEngine::SetCurrentTransaction(const std::string& strAgent, const std::string& strTransId)
{
    m_pairTransactionSync.SynchronizeStart();
    m_pairTransaction.first = strAgent;
    m_pairTransaction.second = strTransId;
    m_pairTransactionSync.SynchronizeEnd();
}

std::pair<std::string, std::string>
VR_VoiceBoxEngine::GetCurrentTransaction()
{
    m_pairTransactionSync.SynchronizeStart();
    auto pairTransaction = m_pairTransaction;
    m_pairTransactionSync.SynchronizeEnd();
    return pairTransaction;
}

bool
VR_VoiceBoxEngine::RestartForInstallTslAgent(const std::string &strCultureName)
{
    VR_LOGD_FUNC();

    if (NULL == m_engineCommand.ptr()) {
        return false;
    }

    // Start the dialog engine with the specified culture
    CVECIPtr<IVECITransaction> transaction;
    HRESULT result = m_engineCommand->RestartEngine(
                     &transaction,
                     strCultureName.c_str()
                     );
    if (FAILED(result) || (NULL == transaction.ptr())) {
        return false;
    }

    CVECIOutStr strTrans;
    result = transaction->GetTransactionId(&strTrans);
    if (FAILED(result)) {
        return false;
    }

    // Make it a sync operation, wait until the engine is started
    VBT_BOOL bTimeout = VBT_FALSE;
    result = transaction->WaitForCompletion(INFINITE, &bTimeout);
    if (FAILED(result) || bTimeout) {
        return false;
    }

    return true;
}

void
VR_VoiceBoxEngine::NotifyInstallProgress(const std::string& strPath, int index)
{
    if (m_bReInstallTSL || !m_bInstallingAgent) {
        return;
    }

    const int nProgressFormatLen = 1024;
    std::string strProgressFormat = "<action name=\"notifyInstallProgress\">"
                                      "<TSLINFO>"
                                        "<Action type=\"Install Progress\">"
                                          "<Parameter name=\"Path\" value=\"%s\"/>"
                                          "<Parameter name=\"Index\" value=\"%d\"/>"
                                        "</Action>"
                                      "</TSLINFO>"
                                    "</action>";

    char progress[nProgressFormatLen] = { 0 };
    snprintf(progress, sizeof(progress), strProgressFormat.c_str(), strPath.c_str(), index);
    OnRequestAction(progress);
}

std::string
VR_VoiceBoxEngine::GetCultureName()
{
    return m_strCultureName;
}

void
VR_VoiceBoxEngine::SendChangeLanguageResult(const std::string& strLanguage)
{
    std::string strEventResult = "<event-result name=\"changeLanguage\">";
    strEventResult.append("<language>");
    strEventResult.append(strLanguage);
    strEventResult.append("</language>");
    strEventResult.append("</event-result>");

    OnRecognized(strEventResult);

    return;
}

bool
VR_VoiceBoxEngine::OnInstallAgentNotification(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strCurrentAgentName = parser.getValueByKey("agentname");
    std::string strInstallErrorCode = parser.getValueByKey("errorcode");
    m_ulInstallErrorCode = strtoul(strInstallErrorCode.c_str(), NULL, 0);
    VR_LOGI("OnInstallAgentNotification: %s [%s]\n", strCurrentAgentName.c_str(), strInstallErrorCode.c_str());

    if (static_cast<unsigned long>(ERR_VBT_ENGINE_XML_AGENT_ALREADY_INSTALLED) == m_ulInstallErrorCode) {
        VR_LOGI("Install Download Agent Failed, because the download is installed!");
        if (NULL == m_engineCommand || NULL == m_client.ptr()) {
            VR_LOGI("m_engineCommand or m_client is NULL!");
            return false;
        }

        // Prepare the agent file path
        CVECIPtr<IVECIStringSet> spAgentName;
        m_client->CreateStringSet(&spAgentName);
        spAgentName->AddItem(strCurrentAgentName.c_str());

        CVECIPtr<IVECITransaction> transaction;
        HRESULT result = m_engineCommand->UninstallDownloadableAgent(
            &transaction,
            NULL,
            m_strInstallCulture.c_str(),
            spAgentName
        );

        if (FAILED(result) || (NULL == transaction.ptr())) {
            VR_LOGI("uninstall Failed");
            return false;
        }

        CVECIOutStr strTrans;
        result = transaction->GetTransactionId(&strTrans);
        if (FAILED(result)) {
            return false;
        }

        m_mapCmdResultHandler.insert(
            std::make_pair(
                strTrans.Get(),
                &VR_VoiceBoxEngine::OnAgentUninstalled
                )
            );
    }
    return true;
}

void
VR_VoiceBoxEngine::HandlePttDuringEndTask()
{
    VR_LOGD_FUNC();

    if (!m_mapPlayTTSTransation.empty()) {
        VR_LOGI("PTT during end task - Stop TTS");
        StopAllTTS();
    }
}

void
VR_VoiceBoxEngine::NotifyEndTaskState(bool bState)
{
    VR_LOGD_FUNC();

    std::string strEndTaskNotification;
    if (bState) {
        strEndTaskNotification = "<display agent=\"Common\" content=\"EndTaskStatus\">"
                                   "<state>1</state>"
                                 "</display>";
    }
    else {
        strEndTaskNotification = "<display agent=\"Common\" content=\"EndTaskStatus\">"
                                   "<state>0</state>"
                                 "</display>";
    }

    OnRequestAction(strEndTaskNotification);
}

void
VR_VoiceBoxEngine::NotifyActionClass(const std::string& strActionClass)
{
    m_strCurActionClass = strActionClass;

    if ("Task Complete" == m_strCurActionClass) {
        m_bIgnoreTslPtt = true;
    }

    if (m_cTslParamManager.GetRenderDoneSyncFlag()
        && ("Follow-Up" == m_strCurActionClass)) {
        m_cTslParamManager.SetRenderingFlag(true);
    }
}

void
VR_VoiceBoxEngine::PostCatalogStatus(const std::string& strCatalogStatus)
{
    if (std::string::npos != strCatalogStatus.find("needcancelgrammar")) {
        m_bNeedGrammarResult = false;
    }

    if (NULL != m_pcCatalogController && !strCatalogStatus.empty()) {
        m_pcCatalogController->PostStatusMsg(strCatalogStatus);
    }
}

void
VR_VoiceBoxEngine::PushFrontMessage(const std::string& strCatalogMsg)
{
    if (NULL == m_pcCatalogController || strCatalogMsg.empty()) {
        return;
    }

    m_pcCatalogController->PushFrontMessage(strCatalogMsg);
}

void
VR_VoiceBoxEngine::SetPhoneDataSyncTransId(const std::string& strDataSyncTransId)
{
    m_cPhoneSyncTransIdSync.SynchronizeStart();
    m_strPhoneSyncTransId = strDataSyncTransId;
    m_cPhoneSyncTransIdSync.SynchronizeEnd();
}

std::string
VR_VoiceBoxEngine::GetPhoneDataSyncTransId()
{
    std::string strDataSyncTransId;
    m_cPhoneSyncTransIdSync.SynchronizeStart();
    strDataSyncTransId = m_strPhoneSyncTransId;
    m_cPhoneSyncTransIdSync.SynchronizeEnd();
    return strDataSyncTransId;
}

bool
VR_VoiceBoxEngine::IsBeingVRSession()
{
    return m_bSession;
}

void
VR_VoiceBoxEngine::SetPhoneReloadDataFlg(bool bPhoneReloadData)
{
    m_bPhoneReloadData = bPhoneReloadData;
}

bool
VR_VoiceBoxEngine::ConfirmRendering(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (m_cTslParamManager.GetRenderingFlag()) {
        VR_LOGI("UIE still rendering the screen");
        return true;
    }

    PlayAfterRender();
    return true;
}

void
VR_VoiceBoxEngine::PlayAfterRender()
{
    auto cTransAfterRender = m_cTslParamManager.GetPlayTransAfterRender();
    if (NULL != cTransAfterRender.first) {
        PlayPrompt(cTransAfterRender.first, cTransAfterRender.second);
        cTransAfterRender.first->Release();
        m_cTslParamManager.SetPlayTransAfterRender(NULL, "");
    }
}

void
VR_VoiceBoxEngine::SetNeedGrammarResult(bool bNeedGrammarResult)
{
    m_bNeedGrammarResult = bNeedGrammarResult;
}

void
VR_VoiceBoxEngine::InitMusicState()
{
    if (NULL == m_pcMsgController || NULL == m_engineCommand) {
        VR_LOGE("m_pcMsgController or m_engineCommand is NULL");
        return;
    }

    m_pcMsgController->PostMessage("<grammar_disactive agent=\"media\" reply=\"false\" grammarid=\"5\"/>");

    CVECIPtr<IVECITransaction> transaction;
    HRESULT retCode = m_engineCommand->SetPreference(&transaction, _T("Music"), VBT_USR_PREF_MUSIC_AUDIOSOURCECONNECTED, "False");
    if (FAILED(retCode) || (NULL == transaction.ptr())) {
        VR_LOGE("SetPreference: 0x%lx or transaction is NULL", retCode);
    }
}

bool VR_VoiceBoxEngine::IsTaskCompeleteNeedQuit()
{
    if (m_bTaskCompelete) {
        VoiceList<std::string>::iterator iter = std::find(m_lstNeedReactiveAction.begin(), m_lstNeedReactiveAction.end(), m_strActionType);
        if (iter == m_lstNeedReactiveAction.end()) {
            VR_LOGI("this TaskCompelete action Need Quit");
            return true;
        }
    }
    return false;
}

void
VR_VoiceBoxEngine::setVDATAUpdateStatus(const enVDataStatus& vdataStatus)
{
    m_enVDataStatus = vdataStatus;
    VR_LOGI("m_enVDataStatus: %d", m_enVDataStatus);
}

void VR_VoiceBoxEngine::SendRequestGrammar(const std::string& strAgentName, const std::string& strGrammarID)
{
    VR_LOGD_FUNC();
    std::string strRequestGrammar = "<action agent=\"" + strAgentName + "\" op=\"requestGrammar\"";
    if ("" != strGrammarID) {
        strRequestGrammar.append(" grammarID=\"");
        strRequestGrammar.append(strGrammarID);
        strRequestGrammar.append("\"");
    }
    strRequestGrammar.append(" />");
    OnRequestAction(strRequestGrammar);
}

void
VR_VoiceBoxEngine::HandleQuitPVRScreen()
{
    VR_LOGD_FUNC();

    if (m_bPVRScreen) {
        m_bHavePVR = false;
        m_bPVRScreen = false;

        OnRecognized("<display xmlns=\"\" agent=\"Common\" content=\"QuitVRApp\"> </display>");
        HandleGrammarQuitVR();
    }
}

void
VR_VoiceBoxEngine::HandleQuitVoiceTag()
{
    VR_LOGD_FUNC();

    OnRequestAction(VR_ACTION_CLOSESESSION);
    m_sessionState = VR_SessionStateStoped;
}

/* EOF */
