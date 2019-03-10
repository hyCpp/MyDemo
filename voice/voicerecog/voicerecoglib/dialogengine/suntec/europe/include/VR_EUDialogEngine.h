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

/**
 * @file VR_EUDialogEngine.h
 * @brief dialog engine class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_EU_DIALOGENGINE_H
#define VR_EU_DIALOGENGINE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "config.h"
#include "VR_DataProcessor.h"
#include "VR_DialogEngineListener.h"
#include "VR_DialogEngineIF.h"
#include "VR_MonitorForBack.h"
#include "uscxml/concurrency/BlockingQueue.h"
#include "uscxml/plugins/invoker/vr/VRServiceRequestor.h"
#include "pugixml.hpp"

#include "MEM_stack.h"
#include "MEM_map.h"
#include "MEM_vector.h"
#include "uscxml/Message.h"
#include "VR_InterpreterManager.h"

#include "BL_Base.h"
#include "CL_TimeDefine.h"
#include <atomic>
#include <boost/scoped_array.hpp>
#include <boost/shared_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "Vr_Asr_Engine.h"
#include "Vr_Asr_Event_Notify.h"
#include "VR_DataAccessorManagerEU.h"
#include "VR_AsrRequestor.h"
#include "VR_AudioInSource.h"
#include "VR_DECommonIF.h"
#include "VR_ConfigureIF.h"
#include "VR_DEMessageBuilder.h"
#include "VR_IntentionParser.h"
#include "VR_DE_Task.h"
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DEDataManager); // contain language and country
typedef boost::shared_ptr<N_Vr::N_Asr::C_Request_Activate> spC_Request_Activate;
typedef boost::shared_ptr<voice::VR_DE_Task> spVR_DE_Task;
typedef boost::shared_ptr<N_Vr::N_Asr::C_Term> spC_Term;

namespace uscxml
{
class Event;
}

/**
 * @brief The VR_DialogController class
 *
 * dialog controller class
 */
class VR_API VR_EUDialogEngine : public VR_DialogEngineIF, public VRServiceRequestor, public VR_MonitorForBack, public VR_AsrRequestor
{
public:
    struct VR_GrammarRequest
    {
        std::string agent;
        std::string grammarID;
    };

    struct BackStateInfo
    {
        BackStateInfo()
        {

        }

        std::string agentName;
        std::string stateName;
        std::string stateAttr;
    };

    enum ResourceState
    {
        READY = 0,
        GRAMMAR_NOT_READY = 1,
        ASR_LANGUAGE_NOT_RESOURCE = 2
     };

    enum PlayedType
    {
        NONE = 0,
        TTS = 1,
        BEEP = 2
    };

public:
    VR_EUDialogEngine(VR_ConfigureIF* configureInstance);
    virtual ~VR_EUDialogEngine();

    virtual bool Initialize(VR_DialogEngineListener* listerner, const VR_Settings &settings);
    virtual std::string getHints(const std::string& hintsParams) override;
    virtual bool Start();
    virtual void Stop();
    virtual bool SendMessage(const std::string& event, int actionSeqId = VR_ACTION_SEQ_ID_INVALID);
    virtual void UnInitialize();


    // virtual VR_ServiceRequestor
    virtual void requestService(const uscxml::Event &interalEvent);

    // ASR callbacks with Boost bind
    virtual bool updateGrammar(N_Vr::N_Asr::C_Request_Context_List_Update& updateMsg);
    virtual bool genVoiceTagPhoneme()
    {
        return true;
    }

    virtual void setGrammarActive(const std::string &contextID, bool isActive, const VoiceList<std::string>::type &ruleIDList);
    virtual void updateGrammarCategoryFinish(const std::string &category);
    virtual void updateGrammarCategoryFailed(const std::string &sourceID, const std::string &size);
    virtual void updateGrammarFinish();
    virtual void grammarSpaceNotEnough(const std::string &infoMsg);
    virtual void deleteMediaGrammar(const std::string &deviceID, const std::string &infoMsg);
    virtual void saveGrammarFile(const std::string &sourceID);
    virtual void loadGracenoteFinish(const std::string &loadResult);

    void onAsrInit(N_Vr::N_Asr::C_Event_Init_Notify const & notify);
    void onAsrPhase(N_Vr::N_Asr::C_Event_Phase const& phase);
    void onAsrNotify(N_Vr::N_Asr::C_Event_Notify const& notify);
    void onAsrResult(N_Vr::N_Asr::C_Event_Result const& result);

    // for monitor inherit to VR_DialogEngineIFM
    void pushTmpToStack();
    void saveTmpState(const std::string& stateName, const std::string& stateAttr);
    std::string getTmpAttr();
    void clearSameAgentState();
    void clearTmpState();
    void saveCurrentState(const std::string& stateName, const std::string& stateAttr);

    bool stopCurrentDialog();
    void initializeAsrPersonalData();

protected:
    VR_DISALLOW_EVIL_CONSTRUCTORS(VR_EUDialogEngine);

    typedef void(VR_EUDialogEngine::*ptr_handleFunc)(uscxml::Event&);
    VoiceMap<std::string, ptr_handleFunc>::type _eventHandles;
    VoiceMap<std::string, ptr_handleFunc>::type _asyncEventHandles;
    VoiceMap<std::string, spC_Request_Activate>::type _grammarDeactiveMap;
    VR_DataAccessorManagerEU* _dataAccessorManager;
    PlayedType m_lastPlayed;

    // init in construct
    VR_DialogEngineListener* _actionListener;
    VR_InterpreterManager* _interManager;
    boost::shared_ptr<VR_AudioInSource>  _audioInSource;
    tthread::thread* _thread;
    tthread::recursive_mutex _mutex;
    tthread::condition_variable _condVar;

    // data init in Initilize fun
    uscxml::concurrency::BlockingQueue<uscxml::Event*> _internalServiceQueue;
    VoiceStack<BackStateInfo>::type _stateSatck; // back
    std::string _currentAgent;
    std::string _lastAgent;
    std::pair<std::string, std::string> _currentIntention;
    spVR_DEDataManager _deDataManager;

    std::string _backStateName; // save the state name in backing, it is also the mark for transition when changeAgent
    bool _resendEvent;
    std::string _forward_back;
    BackStateInfo _tmpStateBack;
    BackStateInfo _currentStateBackChangeAgent;
    bool _isRunning;
    int _seqId;
    VR_DataProcessor _dataProcessor;
    VoiceMap<std::string, std::string>::type _setting;
    uscxml::Event _currentEvent;
    bool _continueAsr;
    N_Vr::N_Asr::C_Engine* _engine;
    std::string _grammarStr;
    int _asrResultId;

    enum AddressType
    {
        address_state = 0,
        address_city = 1,
        address_street = 2
    };

    struct  AddressForAsr
    {
        bool s_bAddress;
        AddressType s_enAddressType;
        unsigned int s_id;
    };

    AddressForAsr _addressForAsr;

    std::atomic<bool> _asrIsRunning;
    bool _changeLanguage;
    bool _fullUpdateNavi;
    std::string _languageId;
    bool _needBargein;
    bool _interrputRecv;
    ResourceState _resourceState;
    std::string _countrySearchId;

    VoiceList<int>::type _listPlayTTsSeq;
    VoiceList<int>::type _listPlayBeepSeq;
    // the seqIds of play TTS that need to stop
    VoiceList<int>::type _listStopTTsSeq;
    // the seqIds of play BEEP that need to stop
    VoiceList<int>::type _listStopBeepSeq;

    tthread::recursive_mutex _mutexStartBeep;
    tthread::condition_variable _condVarStartBeep;
    int m_startBeepSeqId;
    int m_startBeepLatency;

    bool _bHasQuitVRApp;
    bool _bHasCloseSession;
    bool _bNaviFullData;
    bool _bMusicFilter;

    tthread::recursive_mutex _mutexRequest;
    VR_ConfigureIF* m_pConfigureIF;
    VR_DECommonIF* m_pDECommonIF;
    VR_DEMessageBuilder* m_pDEMessageBuilder;
    VR_IntentionParser* m_pIntentionParser;
    bool m_isAsrReturnPlayBeep;

    DWORD m_audioInPrepareTime;
    bool m_sessionState;
    // std::string m_option;
    VoiceList<std::string>::type optionList;

    enum CancelType
    {
        CANCEL_NONE = 0,
        CANCEL_PENDING = 1
    };

    CancelType _canceltype;

    enum AsrInitState
    {
        INIT_NONE,
        INIT_FAIL,
        INIT_SUCCESS
    };

    enum AgentLoadedState
    {
        LOAD_NONE,
        LOAD_FAIL,
        LOAD_SUCCESS
    };

    AsrInitState m_asrInitStateFirstTime;
    AgentLoadedState m_agentLoadStateFirstTime;

    std::string m_asrResult;
    bool m_receiveAsrResult;

    bool m_DMStarted;
    bool m_bVDataError;

    VoiceList<std::string>::type m_regenerationContextIDList;
    VoiceList<VR_GrammarRequest>::type m_grammarRequestList;

    // for vdata error ,asr quit
    bool m_isCanceledByDE;
    // for asr return end_of_speech event twice
    bool m_isAsrSpeechEndRecv;
    bool m_isIpod;
    std::string m_routeExistStatus;

    // save VRState status
    std::string m_interpreterState;

    std::string m_screenContent;

private:

    void doStop();
    void loadHandleEventFuncMap();
    void initGrammarDeactiveMap();
    std::string namelistToStr(uscxml::Event& reqCopy, const std::string& dataName);
    std::string namelistToStr(uscxml::Event& reqCopy);

    static void run(void* instance); // for thread
    void doRun();
    void step(uscxml::Event* pEvent);
    void quitVRApp();
    void responseAsrError();
    // void initDic();
    // void initVRState();

    std::string parseAsrToIntention(pugi::xml_node doc);
    // void loadJsonData(const std::string& filePath, uscxml::Data& outData);
    VoiceList<std::string>::type parserPrompt(pugi::xml_node& doc);
    void parserPromptReplaceKeyWithValue(std::string& promptStr, VoiceMap<std::string, std::string>::type& dicMap);
    void parserVRState(pugi::xml_node& doc);
    void removeSpaceInAsrResult(pugi::xml_node asrNode);
    void resetAsr(bool enableNavi);
    void clearTempData();
    void setItemValue(pugi::xml_node doc, const std::string skey, const std::string svalue);

    // external event
    void processActionResultFromDM(const std::string& eventStr, int seqId);
    void processGrammarFromDM(const std::string& eventStr);
    void processEventFromDM(const std::string& eventStr);
    void processStartOverEvent();
    void processCancelEvent(std::string& option);
    bool processStartAgentEvent(const pugi::xml_node& eventNode);
    bool processPrepareAgentEvent(const pugi::xml_node& eventNode);
    void processGetHintsEvent(const pugi::xml_node& eventNode);
    void processStartDictationEvent();
    void processUpdateStateEvent(const std::string& eventStr);
    void processSettingEvent(const pugi::xml_node& eventNode);
    void processActionResult(const std::string& eventName, int seqId);
    void processInitializePersonData();
    void processChangeLanguage(const pugi::xml_node& language);

    void processStartBeepEnd(int seqId);

    void postEvent(uscxml::Event& reqCopy);

    void doHandleAsr(uscxml::Event& reqCopy);
    // internal event
    void requestVR(const std::string& grammer);
    int generateSeqID();
    int requestAction(const std::string& action);
    void handlePreInitInterpreter(uscxml::Event& reqCopy);
    void handlePreUpdateData(uscxml::Event& reqCopy);
    void handleDoBack(uscxml::Event& reqCopy);
    void handleAsr(uscxml::Event& reqCopy);
    void handleTTS(uscxml::Event& reqCopy);
    void handleTTSOnly(uscxml::Event& reqCopy);
    void handleDisplay(uscxml::Event& reqCopy);
    void handleFetchItem(uscxml::Event& reqCopy);
    void handleCloseSession(uscxml::Event& reqCopy);
    void handleFetchData(uscxml::Event& reqCopy);
    void handleResendAsrEvent(uscxml::Event& reqCopy);
    void handleAction(uscxml::Event& reqCopy);
    void handleInterrupt(uscxml::Event& reqCopy);
    void interruptActionResult();
    void handleReturnKeyOrButton(uscxml::Event&);
    void handleTTSStop(uscxml::Event& reqCopy);
    void handleTTSWithAsr(uscxml::Event& reqCopy);

    void handleAsrStartover(uscxml::Event& reqCopy);
    void handleAsrImmediately(uscxml::Event& reqCopy);
    void handlePopCurrentAgentStacks(uscxml::Event& reqCopy);
    void handleIsAddrSupported(uscxml::Event& reqCopy);
    void handleBack(uscxml::Event& evt);
    void handleChangeAgent(uscxml::Event& evt);
    void handleDoChangeAgent(uscxml::Event& evt);
    void handleBackAgent(uscxml::Event& evt);
    void handleInitAsr(uscxml::Event& evt);
    void handleInitInterpreter(uscxml::Event& evt);
    void handleStartOverEvent(uscxml::Event& evt);
    void handleSendMessage(uscxml::Event& evt);
    void handleQuitVRApp(uscxml::Event& evt);
    void handleUpdateGrammarCategoryFinish(uscxml::Event& evt);
    void handleUpdateGrammarCategoryFailed(uscxml::Event& evt);
    void handleUpdateGrammarFinish(uscxml::Event& evt);
    void handleGrammarSpaceNotEnough(uscxml::Event& evt);
    void handleDeleteMediaGrammar(uscxml::Event& evt);
    void handleSaveGrammarFile(uscxml::Event& evt);
    void handleAsrEvent(uscxml::Event& evt);
    void handleChangeSourceByName(uscxml::Event& reqCopy);
    // invoke in uscxml thread
    void handleDoUpdateDataModel(uscxml::Event& reqCopy);
    void handleReceiveOpResult(uscxml::Event& reqCopy);
    void handleAgentLoadedSuccessFirstTime(uscxml::Event& evt);
    void handleAgentLoadedFailFirstTime(uscxml::Event& evt);

    // for eu load gracenote finish
    void handleLoadGracenoteFinish(uscxml::Event& evt);

    int reqDmPlayBeep(const std::string& beepPath);
    int reqDmPlayTTS(const std::string& ttsTxt);
    int reqDmPlayTTS(const VoiceList<std::string>::type& txtList);
    int reqDmStopTTS();

    std::string parseAsrGrammar(uscxml::Event& reqCopy);
    std::string changeGrammarIDForAsr(const std::string &grammar);

    void onStateUpdate(const std::string &msgToDM);

    void initPhoneTypeName();

    // for sessionStateChanged
    void sessionStateSet(bool val);
    bool sessionStateGet();

    // for resourceState Msg Format Change
    void resourceStateChange(std::string type, ResourceState value);

    // for cancel option Msg
    void sendCancelMsg();
    void replyCancelIfDEStopped();

    // for preInit updatestate
    void doStateUpdate(const std::string &msgToDM, bool notifly);

    // remove same contact name, get full contact id by search DB and set the max confidence phonetypeid
    void preprocessPhoneTypeResult(pugi::xml_node &result);

    // remove same tunner station, if  fm is same with dab remove the fm asr item
    void preprocessTunnerSameResult(pugi::xml_node &result);

    void notifyStartFinishAfterCheck();

    // for prepare message done.preinit
    std::string buildInitParm();
    void processContentBeforeDisplay(pugi::xml_node &displayNode);
    void processListBeforeDisplay(pugi::xml_node &displayNode);

    // for "goto RESPONSE_EVENT_RESULT"
    void responseEventResult(pugi::xml_node &eventNode);

    void handleAsrResult(const std::string &asrResult);

    // for PersonData Delete
    void sendEventToDETask(uscxml::Event evt);
    VoiceList<spVR_DE_Task>::type _listDETaskSeq;

    // check interpreter event, if closession don't need send to de
    bool isEventNeedHandle(uscxml::Event* pEvent);

    bool checkPlaylistAvailable(std::string &playlistID);

    void requestGrammarAfterCheck();
    void processStartedNotify();
    void requestGrammar();
    void onRequestGrammarFinish();

    // for tts crash
    void clearTtsAndBeepList();
    bool isTtsCrash();

    void changeASREngineLanguage(const std::string languageID);

    void changeSourceByNameFailed();

    // for eu media gracenote tts phoneme
    std::string getMediaGracenotePhoneme(std::string& ttsPromptId, std::string& ttsContent);
    bool stripHashInPhoneme(std::string& str);

    // judge button key event type(send event to interpreter)
    bool checkButtonPressEventAvailiable(const std::string& buttonKeycode, const std::string& VRState);

    void getDisplayItemList(pugi::xml_node &result, const std::string &category, VoiceList<std::string>::type &itemlist);
};

#endif
/* EOF */
