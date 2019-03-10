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

#include "VR_DataAccessorManagerCN.h"
#include "Vr_Asr_Engine.h"
#include "VR_Log.h"

VR_DataAccessorManagerCN::VR_DataAccessorManagerCN(VR_AsrRequestor* asrRequestor, VR_DECommonIF* common, VR_ConfigureIF* config)
    : VR_DataAccessorManager(asrRequestor, common, config)
{
}

void VR_DataAccessorManagerCN::setMusicGrammarActive(
    const std::string &grammarID,
    bool enable,
    int songCount,
    int otherCount)
{
    VR_LOGD_FUNC();
    VoiceList<std::string>::type ruleList;
    if ((songCount + otherCount) > VR_MAX_MUSIC_GRAMMAR_COUNT
        && songCount <= VR_MAX_MUSIC_GRAMMAR_COUNT) {
        ruleList.push_back("needToDisableTopmenuSong");
    }
    m_pDataSynchronizer->setGrammarActive("ctx_media_" + grammarID, enable, ruleList);
}

void VR_DataAccessorManagerCN::setPhoneContactGrammarActive(bool enable)
{
    VR_LOGD_FUNC();
    m_pDataSynchronizer->setGrammarActive("ctx_phone_1", enable);
}

// invoke in ASR thread
void VR_DataAccessorManagerCN::notifyGrammarCallback(const N_Vr::N_Asr::C_Event_Notify &notifyEvent, const std::string &category, const std::string &contextID, unsigned int totalCount)
{
}

// invoke in ASR thread
void VR_DataAccessorManagerCN::deletePhoneGrammarCallback(const N_Vr::N_Asr::C_Event_Phase &phaseEvent, const std::string &category, SaveGrammarTaskInfo &info)
{
    VR_LOGD_FUNC();
    N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type msgType = phaseEvent.Get_Event_Phase_Type();
    if (N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_End != msgType) {
        return;
    }

    unsigned int availableSize = getAvailableGrammarSize();
    if (info.size <= availableSize) {
        // send save request to ASR, finished when receive it's callback
        saveGrammarToFile(info);
    }
    else {
        if (!requestDeleteMediaGrammar(info)) {
            saveGrammarToFile(info);
        }
    }
}

void VR_DataAccessorManagerCN::requestUpdateGrammar(N_Vr::N_Asr::C_Request_Context_List_Update &updateMsg)
{
    m_updateMsgList.push_back(updateMsg);
    m_pDataSynchronizer->preUpdateBegin();
}

void VR_DataAccessorManagerCN::checkDirSizeForIFlytek()
{
    VR_LOGD_FUNC();
    // IFlytek fixed 20M after initialize, 23000 music item 30M grammar
    // currentSize = count * 30 * 1024 * 1024 / 23000

    if (m_updateMsgList.empty()) {
        return;
    }

    SaveGrammarTaskInfo info;
    info.isActive = false;
    info.size = 0;

    VoiceList<N_Vr::N_Asr::C_Request_Context_List_Update>::iterator it = m_updateMsgList.begin();
    while (m_updateMsgList.end() != it) {
        info.size += it->m_list_spo_Term_For_Add.size() * 1368; // 1368 = 30 * 1024 * 1024 / 23000;
        std::string contextID = it->m_string_Id_Context;
        std::string category = getCategoryByContextID(contextID);
        it->m_function_On_Event_Phase = boost::bind(&VR_DataAccessorManager::updateGrammarCallback,
                                                            this,
                                                            _1,
                                                            category);
        ++it;
    }

    // get current SourceID from contextID
    std::string currentSourceID = "0";
    std::string testContextID = m_updateMsgList.front().m_string_Id_Context;
    if (testContextID != "ctx_phone_contact_name_list_1") {
        currentSourceID = testContextID.substr(testContextID.find_last_of('_') + 1);
    }
    info.sourceID = currentSourceID;

    if (getGrammarSpaceForSave(info)) {
        for (auto &updateMsg : m_updateMsgList) {
            m_pDataSynchronizer->updateIFlytekGrammar(updateMsg);
        }
        m_updateMsgList.clear();
        m_pDataSynchronizer->preUpdateEnd();
    }
    else {
        handleGrammarSpaceNotEnough(info);
    }
}

void VR_DataAccessorManagerCN::onSaveGrammarToFile(const std::string &sourceID)
{
    VR_LOGD_FUNC();
    VR_LOGD("save grammarFile ID:%s", sourceID.c_str());
    for (auto &updateMsg : m_updateMsgList) {
        m_pDataSynchronizer->updateIFlytekGrammar(updateMsg);
    }
    m_updateMsgList.clear();
    m_pDataSynchronizer->preUpdateEnd();
}

void VR_DataAccessorManagerCN::onUpdateGrammarCategoryFailed(const std::string &sourceID, const std::string &size)
{
    std::string globalActivedSourceID(getState(VR_GLOBALSTATE_ACTIVED_SOURCE_ID));
    if (globalActivedSourceID == sourceID) {
        checkDirSizeForIFlytek(); // current sourceID is actived, check size again.
        return;
    }

    m_isMusicGrammarDroped = true;
    m_emptyGrammarIDSet.insert(sourceID);
    for (auto &categoryName : m_musicCategoryList) {
        std::string contextID = m_categoryContextIDMap[categoryName] + sourceID;
        VoiceList<N_Vr::N_Asr::C_Request_Context_List_Update>::iterator it = m_updateMsgList.begin();
        while (m_updateMsgList.end() != it) {
            if (contextID == it->m_string_Id_Context) {
                m_updateMsgList.erase(it);
                break;
            }
            ++it;
        }
        if (m_updateMsgList.empty()) {
            m_pDataSynchronizer->preUpdateEnd();
        }

        onUpdateGrammarCategoryFinish(categoryName);
    }
}

// invoke in DE thread
void VR_DataAccessorManagerCN::handleGrammarSpaceNotEnough(const std::string &infoMsg)
{
    SaveGrammarTaskInfo info = getTaskInfoFromXMLMsg(infoMsg);
    handleGrammarSpaceNotEnough(info);
}

// invoke in DE thread
void VR_DataAccessorManagerCN::handleGrammarSpaceNotEnough(SaveGrammarTaskInfo &info)
{
    std::string globalActivedSourceID(getState(VR_GLOBALSTATE_ACTIVED_SOURCE_ID));
    info.isActive = (globalActivedSourceID == info.sourceID);

    // get sourceIDs that can be deleted
    VoiceList<std::string>::type sourceIDList;
    sourceIDList.push_back("1");
    sourceIDList.push_back("2");
    sourceIDList.push_back("3");
    sourceIDList.push_back("4");
    sourceIDList.remove(globalActivedSourceID);
    sourceIDList.remove(info.sourceID);
    sourceIDList.push_front(info.sourceID);

    std::string firstSourceID = sourceIDList.front();
    sourceIDList.pop_front();

    info.sourceIDList.swap(sourceIDList);

    if ("0" == firstSourceID) {
        std::string contextID = "ctx_phone_contact_name_list_1";
        m_pDataSynchronizer->deleteGrammarFile(
            contextID,
            boost::bind(&VR_DataAccessorManagerCN::deletePhoneGrammarCallback,
                this,
                _1,
                "contact",
                info));
    }
    else {
        handleDeleteMediaGrammar(firstSourceID, info);
    }
}

std::string VR_DataAccessorManagerCN::getCategoryByContextID(const std::string &contextID)
{
    for (auto pair : m_categoryContextIDMap) {
        if (std::string::npos != contextID.find(pair.second)) {
            return pair.first;
        }
    }
    return "";
}

/* EOF */
