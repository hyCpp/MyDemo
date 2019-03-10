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

#include "VR_DataSynchronizer.h"
#include "VR_AsrRequestor.h"
#include "VR_Log.h"
#include "VR_Def.h"
#include "Vr_Asr_Engine.h"

#include <sstream>

using namespace N_Vr::N_Asr;

VR_DataSynchronizer::VR_DataSynchronizer(VR_AsrRequestor* asrRequestor)
    :m_asrRequestor(asrRequestor)
    , m_isPreUpdate(false)
{
}

VR_DataSynchronizer::~VR_DataSynchronizer()
{
}

spC_Term VR_DataSynchronizer::getCTerm(
    unsigned int id,
    const std::string &name,
    const std::string &pron)
{
    spC_Term item(VR_new N_Vr::N_Asr::C_Term);
    item->m_i_User_Data_Lo = id;
    if (!name.empty()) {
        item->m_vector_string_Orthography.push_back(name);
    }
    if (!pron.empty()) {
        unsigned char * buffer = NULL;
        buffer = VR_new unsigned char[pron.size()];
        memcpy(buffer, pron.c_str(), pron.size());

        C_Buffer pronBuffer;
        pronBuffer.m_i_Size = pron.size();
        pronBuffer.m_sai_Buffer.reset(buffer);

        item->m_vector_buffer_Transcriptions.push_back(pronBuffer);
    }
    return item;
}

spC_Term VR_DataSynchronizer::getCTermByNameList(
    unsigned int id,
    const VoiceList<std::string>::type &nameList,
    const std::string &pron)
{
    spC_Term item(VR_new N_Vr::N_Asr::C_Term);
    item->m_i_User_Data_Lo = id;

    for (auto it = nameList.begin(); it != nameList.end(); it++) {
        if (!it->empty()) {
            item->m_vector_string_Orthography.push_back(*it);
        }
    }
    if (!pron.empty()) {
        unsigned char * buffer = NULL;
        buffer = VR_new unsigned char[pron.size()];
        memcpy(buffer, pron.c_str(), pron.size());

        C_Buffer pronBuffer;
        pronBuffer.m_i_Size = pron.size();
        pronBuffer.m_sai_Buffer.reset(buffer);

        item->m_vector_buffer_Transcriptions.push_back(pronBuffer);
    }
    return item;
}

spC_Term VR_DataSynchronizer::getCTermWithPronID(
    unsigned int id,
    const std::string &name,
    unsigned int pronID)
{
    spC_Term item = getCTerm(id, name);
    if (static_cast<unsigned int>(-1) == pronID) {
        return item;
    }
    item->m_b_Use_Id_Trans = true;
    item->m_i_Id_Trans_Lo = pronID;
    return item;
}

spC_Term VR_DataSynchronizer::getCTermWithPronIdNameList(unsigned int id, const VoiceList<std::string>::type &nameList, unsigned int pronID)
{
    spC_Term item = VR_DataSynchronizer::getCTermByNameList(id, nameList);
    if (static_cast<unsigned int>(-1) == pronID) {
        return item;
    }
    item->m_b_Use_Id_Trans = true;
    item->m_i_Id_Trans_Lo = pronID;
    return item;
}

bool VR_DataSynchronizer::deleteGrammar(const std::string &contextID, std::string type)
{
    VoiceList<spC_Term>::type list;
    return updateGrammar(
        contextID,
        type,
        list,
        list,
        boost::bind(&VR_DataSynchronizer::phaseCallback, this, _1),
        boost::bind(&VR_DataSynchronizer::notifyCallback, this, _1),
        true);
}

bool VR_DataSynchronizer::deleteGrammar(
    const std::string &contextID,
    std::string type,
    boost::function<void(C_Event_Phase const &)> phaseCallbackFuncPoint)
{
    VoiceList<spC_Term>::type list;
    return updateGrammar(
        contextID,
        type,
        list,
        list,
        phaseCallbackFuncPoint,
        boost::bind(&VR_DataSynchronizer::notifyCallback, this, _1),
        true);
}

bool VR_DataSynchronizer::deleteGrammar(
    const std::string &contextID,
    std::string type,
    VoiceList<spC_Term>::type &delList,
    boost::function<void(C_Event_Phase const &)> phaseCallbackFuncPoint)
{
    VoiceList<spC_Term>::type list;
    return updateGrammar(
        contextID,
        type,
        list,
        delList,
        phaseCallbackFuncPoint,
        boost::bind(&VR_DataSynchronizer::notifyCallback, this, _1));
}

bool VR_DataSynchronizer::updateGrammar(
    const std::string &contextID,
    std::string type,
    VoiceList<spC_Term>::type &addList,
    bool clear)
{
    return updateGrammar(
        contextID,
        type,
        addList,
        boost::bind(&VR_DataSynchronizer::phaseCallback, this, _1),
        clear);
}

bool VR_DataSynchronizer::updateGrammar(
    const std::string &contextID,
    std::string type,
    VoiceList<spC_Term>::type &addList,
    boost::function<void(C_Event_Phase const &)> phaseCallbackFuncPoint,
    bool clear)
{
    VoiceList<spC_Term>::type list;
    return updateGrammar(
        contextID,
        type,
        addList,
        list,
        phaseCallbackFuncPoint,
        boost::bind(&VR_DataSynchronizer::notifyCallback, this, _1),
        clear);
}

bool VR_DataSynchronizer::updateGrammar(
    const std::string &contextID,
    std::string type,
    VoiceList<spC_Term>::type &addList,
    boost::function<void(C_Event_Phase const &)> phaseCallbackFuncPoint,
    boost::function<void(C_Event_Notify const &)> notifyCallbackFuncPoint,
    bool clear)
{
    VoiceList<spC_Term>::type list;
    return updateGrammar(
        contextID,
        type,
        addList,
        list,
        phaseCallbackFuncPoint,
        notifyCallbackFuncPoint,
        clear);
}

bool VR_DataSynchronizer::updateGrammar(
    const std::string &contextID,
    std::string type,
    VoiceList<spC_Term>::type &addList,
    VoiceList<spC_Term>::type &delList)
{
    return updateGrammar(
        contextID,
        type,
        addList,
        delList,
        boost::bind(&VR_DataSynchronizer::phaseCallback, this, _1));
}

bool VR_DataSynchronizer::updateGrammar(
    const std::string &contextID,
    std::string type,
    VoiceList<spC_Term>::type &addList,
    VoiceList<spC_Term>::type &delList,
    boost::function<void(C_Event_Phase const &)> phaseCallbackFuncPoint)
{
    return updateGrammar(
        contextID,
        type,
        addList,
        delList,
        phaseCallbackFuncPoint,
        boost::bind(&VR_DataSynchronizer::notifyCallback, this, _1));
}

bool VR_DataSynchronizer::updateGrammar(
    const std::string &contextID,
    std::string type,
    VoiceList<spC_Term>::type &addList,
    VoiceList<spC_Term>::type &delList,
    boost::function<void(C_Event_Phase const &)> phaseCallbackFuncPoint,
    boost::function<void(C_Event_Notify const &)> notifyCallbackFuncPoint,
    bool clear)
{
    VR_LOGD("Grammar contextID: %s", contextID.c_str());
    C_Request_Context_List_Update updateMsg;
    updateMsg.m_string_Id_Party.assign("Origin");
    updateMsg.m_string_Id_Context.assign(contextID);
    updateMsg.m_e_Requst_Type = E_Data_Request_Type::E_Data_Request_Type_Context_List_Update;

    if (!addList.empty()) {
        updateMsg.m_list_spo_Term_For_Add.swap(addList);
    }
    if (!delList.empty()) {
        updateMsg.m_list_spo_Term_For_Delete.swap(delList);
    }

    updateMsg.m_function_On_Event_Phase = phaseCallbackFuncPoint;
    updateMsg.m_function_On_Event_Notify = notifyCallbackFuncPoint;

    if (!updateMsg.m_function_On_Event_Phase) {
        VR_LOGD("PhaseCallBack function point is NULL");
    }
    if (!updateMsg.m_function_On_Event_Notify) {
        VR_LOGD("NotifyCallBack function point is NULL");
    }

    updateMsg.m_b_Clear = clear;
    if (!type.empty()) {
        updateMsg.m_string_Id_Category =  type;
    }
    bool result = m_asrRequestor->updateGrammar(updateMsg);
    if (result) {
        m_asrRequestIdList.push_back(updateMsg.m_i_Id_Request);
    }
    return result;
}

bool VR_DataSynchronizer::updateIFlytekGrammar(N_Vr::N_Asr::C_Request_Context_List_Update &updateMsg)
{
    VR_LOGD("Grammar contextID: %s", updateMsg.m_string_Id_Context.c_str());
    updateMsg.m_string_Id_Party.assign("Origin");
    updateMsg.m_e_Requst_Type = E_Data_Request_Type::E_Data_Request_Type_Context_List_Update;
    updateMsg.m_b_Save_List_Context = true;

    if (!updateMsg.m_function_On_Event_Phase) {
        updateMsg.m_function_On_Event_Phase = boost::bind(&VR_DataSynchronizer::phaseCallback, this, _1);
    }

    if (!updateMsg.m_function_On_Event_Notify) {
        updateMsg.m_function_On_Event_Notify = boost::bind(&VR_DataSynchronizer::notifyCallback, this, _1);
    }

    bool result = m_asrRequestor->updateGrammar(updateMsg);
    if (result) {
        m_asrRequestIdList.push_back(updateMsg.m_i_Id_Request);
    }
    return result;
}

bool VR_DataSynchronizer::updateGrammarWithoutSave(N_Vr::N_Asr::C_Request_Context_List_Update &updateMsg)
{
    VR_LOGD("Grammar contextID: %s", updateMsg.m_string_Id_Context.c_str());
    updateMsg.m_string_Id_Party.assign("Origin");
    updateMsg.m_e_Requst_Type = E_Data_Request_Type::E_Data_Request_Type_Context_List_Update;
    updateMsg.m_b_Save_List_Context = false;

    if (!updateMsg.m_function_On_Event_Phase) {
        updateMsg.m_function_On_Event_Phase = boost::bind(&VR_DataSynchronizer::phaseCallback, this, _1);
    }

    if (!updateMsg.m_function_On_Event_Notify) {
        updateMsg.m_function_On_Event_Notify = boost::bind(&VR_DataSynchronizer::notifyCallback, this, _1);
    }

    bool result = m_asrRequestor->updateGrammar(updateMsg);
    if (result) {
        m_asrRequestIdList.push_back(updateMsg.m_i_Id_Request);
    }
    return result;
}

bool VR_DataSynchronizer::deleteGrammarFile(
    const std::string &contextID,
    boost::function<void(C_Event_Phase const &)> phaseCallbackFuncPoint)
{
    VR_LOGD("Delete Grammar File, ContextID: %s", contextID.c_str());
    C_Request_Context_List_Update updateMsg;
    updateMsg.m_string_Id_Party.assign("Origin");
    updateMsg.m_string_Id_Context.assign(contextID);
    updateMsg.m_e_Requst_Type = E_Data_Request_Type::E_Data_Request_Type_Context_List_File_Delete;

    if (nullptr == phaseCallbackFuncPoint) {
        updateMsg.m_function_On_Event_Phase = boost::bind(&VR_DataSynchronizer::phaseCallback, this, _1);
    }
    else {
        updateMsg.m_function_On_Event_Phase = phaseCallbackFuncPoint;
    }

    if (!updateMsg.m_function_On_Event_Notify) {
        updateMsg.m_function_On_Event_Notify = boost::bind(&VR_DataSynchronizer::notifyCallback, this, _1);
    }

    updateMsg.m_b_Clear = true;
    bool result = m_asrRequestor->updateGrammar(updateMsg);
    return result;
}

bool VR_DataSynchronizer::saveGrammarFile(
    const std::string &contextID,
    boost::function<void(C_Event_Phase const &)> phaseCallbackFuncPoint)
{
    VR_LOGD("Save Grammar File, ContextID: %s", contextID.c_str());
    C_Request_Context_List_Update updateMsg;
    updateMsg.m_string_Id_Party.assign("Origin");
    updateMsg.m_string_Id_Context.assign(contextID);
    updateMsg.m_e_Requst_Type = E_Data_Request_Type::E_Data_Request_Type_Context_List_File_Save;

    if (nullptr == phaseCallbackFuncPoint) {
        updateMsg.m_function_On_Event_Phase = boost::bind(&VR_DataSynchronizer::phaseCallback, this, _1);
    }
    else {
        updateMsg.m_function_On_Event_Phase = phaseCallbackFuncPoint;
    }

    if (!updateMsg.m_function_On_Event_Notify) {
        updateMsg.m_function_On_Event_Notify = boost::bind(&VR_DataSynchronizer::notifyCallback, this, _1);
    }

    bool result = m_asrRequestor->updateGrammar(updateMsg);
    return result;
}

bool VR_DataSynchronizer::stopGrammar()
{
    bool result = true;
    C_Request_Context_List_Update updateMsg;
    updateMsg.m_b_Cancel = true;
    for (VoiceList<unsigned int>::iterator it = m_asrRequestIdList.begin();
        it != m_asrRequestIdList.end();
        ++it) {
        updateMsg.m_i_Id_Request = *it;
        result = result && m_asrRequestor->updateGrammar(updateMsg);
        updateMsg.m_function_On_Event_Phase = boost::bind(&VR_DataSynchronizer::phaseCallback, this, _1);
        updateMsg.m_function_On_Event_Notify = boost::bind(&VR_DataSynchronizer::notifyCallback, this, _1);
    }
    m_asrRequestIdList.clear();
    return result;
}

void VR_DataSynchronizer::genVoiceTagPhoneme()
{
    m_asrRequestor->genVoiceTagPhoneme();
}

void VR_DataSynchronizer::setGrammarActive(const std::string &contextID, bool isActive, const VoiceList<std::string>::type &ruleIDList)
{
    m_asrRequestor->setGrammarActive(contextID, isActive, ruleIDList);
}

// invoke in asr thread
void VR_DataSynchronizer::notifyUpdateGrammarCategoryFinish(const std::string &category)
{
    m_asrRequestor->updateGrammarCategoryFinish(category);
}

void VR_DataSynchronizer::notifyUpdateGrammarCategoryFailed(const std::string &sourceID, const std::string &size)
{
    m_asrRequestor->updateGrammarCategoryFailed(sourceID, size);
}

void VR_DataSynchronizer::notifyGrammarSpaceNotEnough(const std::string &infoMsg)
{
    m_asrRequestor->grammarSpaceNotEnough(infoMsg);
}

void VR_DataSynchronizer::notifyDeleteMediaGrammar(const std::string &deviceID, const std::string &infoMsg)
{
    m_asrRequestor->deleteMediaGrammar(deviceID, infoMsg);
}

void VR_DataSynchronizer::notifySaveGrammarFile(const std::string &sourceID)
{
    m_asrRequestor->saveGrammarFile(sourceID);
}
// invoke in asr thread
void VR_DataSynchronizer::notifyAsrLoadGracenoteFinish(const std::string &loadResult)
{
    m_asrRequestor->loadGracenoteFinish(loadResult);
}

// invoke in DE thread
void VR_DataSynchronizer::updateGrammarCategoryFinish(const std::string &category)
{
    if (!m_asrRequestIdList.empty()) {
        m_asrRequestIdList.pop_front();
    }
}

// invoke in DE thread
void VR_DataSynchronizer::preUpdateBegin()
{
    m_isPreUpdate = true;
}

void VR_DataSynchronizer::preUpdateEnd()
{
    m_isPreUpdate = false;
}

// invoke in DE thread
bool VR_DataSynchronizer::isAsrIdle()
{
    return (m_asrRequestIdList.empty() && !m_isPreUpdate);
}

void VR_DataSynchronizer::phaseCallback(N_Vr::N_Asr::C_Event_Phase const &phaseEvent)
{
}

void VR_DataSynchronizer::notifyCallback(N_Vr::N_Asr::C_Event_Notify const &notifyEvent)
{
}

void VR_DataSynchronizer::resetRequest()
{
    m_asrRequestIdList.clear();
}

// build ASR Cterm object By gracenote name
spC_Term VR_DataSynchronizer::getGracenoteCtermByName(
    const VoiceList<std::string>::type &nameList,
    const std::string& category)
{
    VR_LOGD_FUNC();
    spC_Term item(VR_new N_Vr::N_Asr::C_Term);

    if (!nameList.empty()) {

        // set clc domain
        item->m_string_Trans_Category = category;

        // set media name list
        for (auto it = nameList.begin(); it != nameList.end(); it++) {
            item->m_vector_string_Orthography.push_back(*it);
        }

        // set phoneme flag defalut false
        item->m_b_Use_Id_Trans = true;
    }
    return item;
}

// build ASR Cterm object By gracenote id
spC_Term VR_DataSynchronizer::getGracenoteCtermById(
    const VoiceList<std::string>::type &nameList,
    unsigned int pronID,
    const std::string& category)
{
    VR_LOGD_FUNC();
    spC_Term item(VR_new N_Vr::N_Asr::C_Term);

    // pronID not -1
    if (static_cast<unsigned int>(-1) != pronID) {

        // set media name list [if gracenote Id not found phoneme need To use text]
        for (auto it = nameList.begin(); it != nameList.end(); it++) {
            item->m_vector_string_Orthography.push_back(*it);
        }

        // set clc domain
        item->m_string_Trans_Category = category;

        // set gracenote id
        item->m_i_Id_Trans_Lo = pronID;

        // set phoneme flag defalut false
        item->m_b_Use_Id_Trans = true;
    }

    return item;
}

bool VR_DataSynchronizer::loadAsrGracenoteClc(
    const std::string& magicString,
    boost::function<void(N_Vr::N_Asr::C_Event_Notify const &)> notifyCallbackFuncPoint)
{
    VR_LOGD("load Asr gracenote clc");

    C_Request_Context_List_Update updateMsg;

    updateMsg.m_e_Requst_Type = E_Data_Request_Type::E_Data_Request_Type_Transcription_Id_Version_Update;

    updateMsg.m_string_Transcription_Id_Version = magicString;
    updateMsg.m_function_On_Event_Phase = boost::bind(&VR_DataSynchronizer::phaseCallback, this, _1);
    updateMsg.m_function_On_Event_Notify = notifyCallbackFuncPoint;

    bool result = m_asrRequestor->updateGrammar(updateMsg);
    if (result) {
        m_asrRequestIdList.push_back(updateMsg.m_i_Id_Request);
    }
    return result;
}

/* EOF */
