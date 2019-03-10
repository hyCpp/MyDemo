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
 * @file VR_DataAccessorManager.h
 * @brief Declaration file of VR_DataAccessorManager.
 *
 * This file includes the declaration of VR_DataAccessorManager.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DATA_ACCESSOR_MANAGER_H
#define VR_DATA_ACCESSOR_MANAGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_VoiceTagManager.h"
#include "VR_DataSynchronizer.h"
#include "VR_DECommonIF.h"
#include "VR_ConfigureIF.h"

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "MEM_list.h"
#include "MEM_map.h"
#include "MEM_queue.h"

#include <pcrecpp.h>
#include <boost/function.hpp>
#include <pugixml.hpp>
#include <string>

class sqlite3;
class VR_AsrRequestor;
class VR_DataAccessor;

typedef boost::shared_ptr<VR_DataSynchronizer> spVR_DataSynchronizer;
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DataAccessorContact);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DataAccessorNaviIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DataAccessorVoiceTag);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DataAccessorMedia);

extern const std::string VR_GLOBALSTATE_ACTIVED_SOURCE_ID;
extern const int VR_MAX_MUSIC_GRAMMAR_COUNT;

namespace pugi
{
    typedef boost::shared_ptr<xml_document> sp_xml_document;
}

struct SaveGrammarTaskInfo
{
    bool isActive;
    unsigned int size;
    std::string contextID;
    std::string category;
    std::string sourceID;
    VoiceList<std::string>::type sourceIDList;
};

/**
 * @brief The VR_DataAccessorManager class
 *
 * cache data for DialogEngine and send data to ASR for update grammar
 *
 * attention:
 * grammar_enable phone msg should receive before grammar_new contact msg
 */

class VR_DataAccessorManager : public VR_VoiceTagManager
{
public:
    VR_DataAccessorManager(VR_AsrRequestor* asrRequestor, VR_DECommonIF* common, VR_ConfigureIF* config);
    virtual ~VR_DataAccessorManager();

    // state
    virtual void updateState(const std::string &stateMsg);
    virtual void getUpdateState(std::string& stateMsg);
    virtual void setUpdateStateCallback(boost::function<void(const std::string &)> &callback);
    virtual void setNotifyCallback(boost::function<void(const std::string &)> &callback);

    // grammar
    virtual void updateGrammar(const std::string &grammarMsg);
    virtual void updateGrammar(pugi_vr::sp_xml_document spGrammarMsgDoc);
    virtual void clearAllGrammar();

    virtual bool saveVoiceTagGrammar(const std::string &voiceTagID, const std::string &name, const std::string &phoneme, const std::string &deviceAddress);
    virtual bool deleteVoiceTagGrammar(const VoiceList<std::pair<std::string, std::string>>::type &deleteList, const std::string &deviceAddress);
    virtual bool deleteAllVoiceTagGrammar(const std::string &deviceAddress);
    virtual bool clearVoiceTagGrammar();

    // update voice tag grammar callback
    void voiceTagUpdateGrammarCallback(const N_Vr::N_Asr::C_Event_Phase &phaseEvent, const std::string &deviceAddress);
    void voiceTagDeleteGrammarCallback(const N_Vr::N_Asr::C_Event_Phase &phaseEvent, const std::string &deviceAddress);

    // update grammar callback
    void updateGrammarCallback(const N_Vr::N_Asr::C_Event_Phase &phaseEvent, const std::string &category);
    void clearGrammarCallback(const N_Vr::N_Asr::C_Event_Phase &phaseEvent);

    // keep grammar file size limited, invoke in ASR thread
    virtual void notifyGrammarCallback(const N_Vr::N_Asr::C_Event_Notify &notifyEvent, const std::string &category, const std::string &contextID, unsigned int totalCount = 0);
    void deleteMediaGrammarCallback(const N_Vr::N_Asr::C_Event_Phase &phaseEvent, const std::string &category, SaveGrammarTaskInfo &info);
    void saveGrammarFileCallback(const N_Vr::N_Asr::C_Event_Phase &phaseEvent, const std::string &category);
    bool requestDeleteMediaGrammar(SaveGrammarTaskInfo &info);
    void saveGrammarToFile(SaveGrammarTaskInfo &info);

    // handle delete grammar file request, invoke in DE thread
    void onDeleteMediaGrammar(const std::string &deviceID, const std::string &infoMsg);
    // handle save grammar file request, invoke in DE thread
    virtual void onSaveGrammarToFile(const std::string &sourceID);
    // invoke in DE thread
    virtual void handleGrammarSpaceNotEnough(const std::string &infoMsg);
    virtual void handleGrammarSpaceNotEnough(SaveGrammarTaskInfo &info);

    // info
    virtual void getInfo(const std::string &operation, const std::string &reqMsg, std::string &response);

    virtual void setCountryID(const std::string &countryID);

    // receive recognize status as grammar manager
    void onRecognizeBegin();
    void onRecognizeEnd();

    // handle update grammar finish
    virtual void onUpdateGrammarCategoryFinish(const std::string &category);
    virtual void onUpdateGrammarCategoryFailed(const std::string &sourceID, const std::string &size);
    void onUpdateGrammarFinish();
    void triggerGenerateGrammar();

    virtual void resetGrammarGeneration();
    void restartGenerateGrammar();

    bool checkContextIDAvailable(const std::string &contextID);
    std::string getContextIDAgent(const std::string &contextID);
    std::string getContextIDIndex(const std::string &contextID);

    void setGenerateGrammarEnable(bool isEnable);

    bool generateVoiceTagGrammar(std::string &deviceAddress);
    void removeGrammarDiffMsg(const std::string &agent, const std::string &grammarID);

    bool isRecognizing();

    // eliminate repeated music item id and get the music item name from DB
    void handleMusicResult(pugi::xml_node &result, const std::string &category);

    void processAudioSourceList(pugi::xml_node &audioSourceNode);
    std::string getMediaSourceName(std::string uiSourceName);

    virtual void generateDisplayGrammar(const VoiceList<std::string>::type &itemList);
    virtual void generateDisplayGrammarCallback(const N_Vr::N_Asr::C_Event_Phase &phaseEvent);
protected:
    spVR_DataSynchronizer m_pDataSynchronizer;
    VoiceList<std::string>::type m_musicCategoryList;
    VoiceMap<std::string, std::string>::type m_categoryContextIDMap;

    boost::function<void(const std::string &)> m_updateStateCallback;
    boost::function<void(const std::string &)> m_notifyCallback;

    sqlite3 * m_dbHandler;
    int m_dbResult;
    std::string m_activedGrammarID;

    spVR_DataAccessorContact m_accessorContact;
    spVR_DataAccessorNaviIF m_accessorNavi;
    spVR_DataAccessorVoiceTag m_accessorVoiceTag;
    spVR_DataAccessorMedia m_accessorMedia;

    VR_ConfigureIF* m_configure;
    VR_DECommonIF* m_deCommonIF;

    bool m_isMusicGrammarDroped;
    bool m_isPhoneGrammarActive;

    bool m_requestUpdateMediaGrammarFinish;

    std::string m_voiceTagContextID;

    VoiceMap<std::string, std::string>::type m_stateMap;
    VoiceMap<std::string, std::string>::type m_updatingDictionaryStateMap;

    VoiceQueue<pugi::sp_xml_document>::type m_grammarMsgQuque;
    VoiceList<pugi::sp_xml_document>::type m_generateGrammarList;
    pugi::sp_xml_document m_generatingGrammarMsg;
    bool m_bIsRecognizing;
    bool m_generateGrammarEnable;

    int (*m_musicGrammarCallback)(void* addList, int columnNum, char **columnValue, char **columnName);

    VoiceMap<std::string, std::string>::type m_sourceNameMap;
    VoiceSet<std::string>::type m_emptyGrammarIDSet;
    std::string m_priorGrammarID;

    // regex expression for music alias
    static pcrecpp::RE m_bracketsRule;
    static pcrecpp::RE m_featuringRule;
    static pcrecpp::RE m_consecutiveSpaceRule;

    // keep grammar file size limited, invoke in ASR thread
    virtual void requestUpdateGrammar(N_Vr::N_Asr::C_Request_Context_List_Update &updateMsg);
    unsigned int getAvailableGrammarSize();
    off_t getGrammarDirSize(const std::string &path);
    bool getGrammarSpaceForSave(SaveGrammarTaskInfo &info);

    // invoke in DE thread
    void handleDeleteMediaGrammar(const std::string &sourceID, SaveGrammarTaskInfo &info);
    void requestMediaGrammar(const std::string &sourceID);

    // SaveGrammarTaskInfo transform
    std::string getXMLMsgFromTaskInfo(const SaveGrammarTaskInfo &info);
    SaveGrammarTaskInfo getTaskInfoFromXMLMsg(const std::string &infoMsg);

    // grammar dir size check
    virtual void checkDirSizeForIFlytek();
    bool checkGrammarDirSize(int size);

    // grammar usability
    virtual void setMusicGrammarActive(const std::string &grammarID, bool enable, int songCount, int otherCount);
    virtual void setPhoneContactGrammarActive(bool enable);
    void grammarActive(const std::string &contextID, const VoiceList<std::string>::type &ruleIDList);
    void grammarActive(const std::string &contextID, const std::string &ruleID);
    void grammarDisactive(const std::string &contextID, const VoiceList<std::string>::type &ruleIDList);
    void grammarDisactive(const std::string &contextID, const std::string &ruleID);
    bool isCategoryAvailable(const std::string &category);
    void getAvailableCategory(VoiceList<std::string>::type &categoryList, int songCount, int otherCount);

    // handle grammar msg
    void handleGrammarMsg(pugi::sp_xml_document &spGrammarMsgDoc);
    void handleGrammarActiveMsg(pugi::sp_xml_document &spGrammarMsgDoc);
    void handleGrammarGenerateMsg(pugi::sp_xml_document &spGrammarMsgDoc);
    void handleGrammarPreactiveMsg(pugi::sp_xml_document &spGrammarMsgDoc);
    void handleGrammarDispreactiveMsg(pugi::sp_xml_document &spGrammarMsgDoc);
    void generateGrammar(pugi::sp_xml_document &spGrammarMsgDoc);

    // handle grammar update
    std::string selectGrammarType(std::string categoryName);
    void handleMediaGrammarInit(const std::string &path, const std::string &grammarID, int songCount, int otherCount);
    void handlePhoneGrammarInit(const std::string &path);
    void handleMediaGrammarDiff(pugi::xml_node &category, const std::string &grammarID, int songCount, int otherCount);
    virtual void handleGrammarNew(pugi::xml_node &category);
    void handleGrammarDynamic(pugi::xml_node &category);

    // handle grammar active
    void handleMusicActive(bool isActive, const std::string &grammarID, int songCount, int otherCount, const std::string &path, bool needResponse = true);
    void handlePhoneActive(bool isActive, const std::string &path, bool needResponse = true);

    // process grammar new message
    void processContactGrammarNew(VoiceList<spC_Term>::type &addList, const std::string &path);

    // void processQuickReplyMsgGrammarNew(pugi::xml_node &messageNode, VoiceList<spC_Term>::type &addList);
    // void processAudioSourceGrammarNew(pugi::xml_node &idNode, VoiceList<spC_Term>::type &addList);
    // void processNoGrammarAndSaveToDB(const std::string &tableName, pugi::xml_node &idNode);
    // void processOtherGrammarNew(pugi::xml_node &idNode, std::string &categoryName, VoiceList<spC_Term>::type &addList);

    // info
    VR_DataAccessor * getAccessor(const std::string &operation);

    // init
    void createTable();

    // sqlite3 callback
    static int getSingleColumnList(void* stringList, int columnNum, char **columnValue, char **columnName);
    static int getSingleColumnValue(void* stringValue, int columnNum, char **columnValue, char **columnName);
    static int genContactCTermListCallback(void* addList, int columnNum, char **columnValue, char **columnName);
    static int genMusicCTermListCallback(void* addList, int columnNum, char **columnValue, char **columnName);
    static int getRecordCountCallback(void *number, int columnNum, char **columnValue, char **columnName);

    static bool getMusicItemList(const std::string &itemName, VoiceList<std::string>::type &list);
    static void generateMusicCTermList(VoiceList<spC_Term>::type &cTermList, const unsigned int &id, const std::string &name);
    // process special char in name
    static bool getContactNameList(const std::string &itemName, VoiceList<std::string>::type &list);

    virtual void getMusicCTermList(VoiceList<spC_Term>::type &cTermList, const unsigned int &id, const std::string &name, const std::string &pronIDStr = std::string(), const std::string &pronNameStr = std::string());

    // DataAccessor DB
    int getRecordCount(sqlite3 *dbHandler, const std::string &tableName);
    void insertRecord(const std::string &tableName, const std::string &value1, const std::string &value2, const std::string &extendValue = std::string());

    // state
    void notifyStateUpdated();
    void initializeState();
    void updateMusicGrammarState(const std::string &categoryName, const std::string &grammarID);
    virtual void updateOtherGrammarState(const std::string &categoryName);
    void checkMusicGrammarState();

    void getFullName(std::string &fullName, const std::string firstName, const std::string lastName, bool isNormal = true);
    static std::string trim(std::string str);
    const std::string & getState(const std::string &stateKey);
    void setState(const std::string &stateKey, const std::string &stateValue);

    // check grammar need update
    void checkGrammarNeedUpdate();

    void handleMediaNullDevice();

    void formatSourceName(std::string &sourceName);
};



#endif /* VR_DATA_ACCESSOR_MANAGER_H */
/* EOF */
