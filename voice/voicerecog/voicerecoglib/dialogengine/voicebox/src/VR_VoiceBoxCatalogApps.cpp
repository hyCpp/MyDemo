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
#include "VR_Log.h"

/* VBT Header */
#ifndef VR_VOICEBOXSOURCE_H
#    include "VR_VoiceBoxSource.h"
#endif

/* Suntec Header */
#ifndef VR_VOICEBOXCATALOGAPPS_H
#    include "VR_VoiceBoxCatalogApps.h"
#endif

#ifndef VR_VOICEBOXLOG_H
#    include "VR_VoiceBoxLog.h"
#endif

VR_VoiceBoxCatalogApps::VR_VoiceBoxCatalogApps(
    IVECIEngineClient& client,
    VR_VoiceBoxEngineCallback& engineCallback
    )
: m_client(client)
, m_engineCallback(engineCallback)
{
}

VR_VoiceBoxCatalogApps::~VR_VoiceBoxCatalogApps()
{
}

bool
VR_VoiceBoxCatalogApps::Initialize()
{
    HRESULT result = m_client.CreateAgentDispatcher(&m_agentDataCommandApps);
    if (S_OK != result) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "CreateAgentDispatcher: 0x%lx", result);
        return false;
    }

    if (NULL == m_agentDataCommandApps.ptr()) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, 0, "m_agentDataCommandApps is NULL");
        return false;
    }

    result = m_agentDataCommandApps->Init(VBT_AGENT_APPS);
    if (S_OK != result) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "Init: 0x%lx", result);
        return false;
    }

    return true;
}

bool
VR_VoiceBoxCatalogApps::Cataloguing(
    VR_VoiceBoxXmlParser& parser
    )
{
    if (NULL == m_agentDataCommandApps.ptr()) {
        VR_LOGE("m_agentDataCommandApps is NULL");
        return false;
    }

    CVECIPtr<IVECITransaction> spTrans;

    HRESULT retCode = m_agentDataCommandApps->SetDataSynchronized(NULL, _T(""), _T(""),
        _T(""), VBT_FALSE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    VoiceVector<AppCategory>::type vecAppCategory;
    parser.getApps(vecAppCategory);
    std::string strTableName;

    VoiceVector<AppCategory>::const_iterator citor = vecAppCategory.cbegin();
    for (; citor != vecAppCategory.cend(); ++citor) {
        if (citor->strName == "apps") {
            strTableName = "AppsAgentApps";
        }
        else if (citor->strName == "appsoffboard") {
            strTableName = "AppsAgentAppsOffboard";
        }
        else {
            continue;
        }

        CVECIPtr<IVECIListItems> contactList;
        m_client.CreateListItems(&contactList);

        LoadAppsInfo(citor->vecFormal, *contactList);

        retCode = m_agentDataCommandApps->ReloadData(NULL, _T(""),
            strTableName.c_str(), contactList);
        if (S_OK != retCode) {
            VR_LOGE("ReloadData: 0x%lx", retCode);
            return false;
        }
    }

    retCode = m_agentDataCommandApps->SetDataSynchronized(NULL, _T(""), _T(""),
        _T(""), VBT_TRUE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    retCode = m_agentDataCommandApps->ReloadGrammar(&spTrans, _T(""),  _T(""));
    if (S_OK != retCode || NULL == spTrans.ptr()) {
        VR_LOGE("ReloadGrammar: 0x%lx or spTrans is NULL", retCode);
        return false;
    }

    CVECIOutStr strTrans;
    retCode = spTrans->GetTransactionId(&strTrans);
    if (FAILED(retCode) || NULL == strTrans.Get()) {
        VR_LOGE("GetTransactionId: 0x%lx or strTrans.Get() is NULL", retCode);
        return false;
    }

    std::string strTransaction = strTrans.Get();
    m_engineCallback.SetCurrentTransaction("apps", strTransaction);

    VR_LOGI("grammar : %s", strTransaction.c_str());

    return true;
}

void
VR_VoiceBoxCatalogApps::LoadAppsInfo(
    const VoiceVector<AppFormal>::type& vecAppsInfo,
    IVECIListItems& listItems
)
{
    VoiceVector<AppFormal>::const_iterator citorFormal = vecAppsInfo.cbegin();
    VoiceVector<Alias>::const_iterator citorAlias;
    CVECIPtr<IVECIParameterSet> spContact;
    while (citorFormal != vecAppsInfo.cend()) {
        for (citorAlias = citorFormal->vecAlias.cbegin();
            citorAlias != citorFormal->vecAlias.cend();
            ++citorAlias) {
            m_client.CreateParameterSet(&spContact);
            if (NULL == spContact) {
                return;
            }
            spContact->AddParameter(_T("nId"), citorFormal->strId.c_str(), NULL, NULL);
            spContact->AddParameter(_T("formal"), citorFormal->strName.c_str(), NULL, NULL);
            spContact->AddParameter(_T("shortcut"), citorFormal->strShortCut.c_str(), NULL, NULL);
            spContact->AddParameter(_T("alias"), citorAlias->strName.c_str(), NULL, NULL);
            spContact->AddParameter(_T("pronunciation"), citorAlias->strPron.c_str(), NULL, NULL);
            listItems.AddItem(spContact);
        }
        ++citorFormal;
    }
}

void
VR_VoiceBoxCatalogApps::PauseGrammarApps()
{
    if (NULL == m_agentDataCommandApps.ptr()) {
        VR_LOGE("m_agentDataCommandApps is NULL");
        return;
    }
    std::pair<std::string, std::string> pairAgent2TransId = m_engineCallback.GetCurrentTransaction();
    if (pairAgent2TransId.first.empty()) {
        VR_LOGI("grammar has updated finish");
        return;
    }

    VR_LOGI("PauseGrammarUpdate of apps, first = %s, second = %s", pairAgent2TransId.first.c_str(), pairAgent2TransId.second.c_str());

    CVECIPtr<IVECITransaction> spTrans;

    HRESULT retCode =  m_agentDataCommandApps->PauseGrammarUpdate(&spTrans);
     if (FAILED(retCode)  || NULL == spTrans.ptr()) {
        VR_LOGE("PauseGrammarUpdate: 0x%lx or spTrans is NULL", retCode);
        return;
    }

    VBT_BOOL bSaveTimeout = VBT_FALSE;
    const long OVER_TIME = 20000;
    retCode = spTrans->WaitForCompletion(OVER_TIME, &bSaveTimeout);
    if (FAILED(retCode) || bSaveTimeout) {
        VR_LOGE("WaitForCompletion failed: %lx or PauseGrammarUpdate over time", retCode);
        return;
    }

    CVECIOutStr strTrans;
    retCode = spTrans->GetTransactionId(&strTrans);
    if (FAILED(retCode) || NULL == strTrans.Get()) {
        VR_LOGE("GetTransactionId: 0x%lx or strTrans.Get() is NULL", retCode);
        return;
    }

    std::string strTransaction = strTrans.Get();
    VR_LOGI("strTransaction = %s", strTransaction.c_str());
}

void
VR_VoiceBoxCatalogApps::ResumeGrammarApps()
{
    if (NULL == m_agentDataCommandApps.ptr()) {
        VR_LOGE("m_agentDataCommandApps is NULL");
        return;
    }

    CVECIPtr<IVECITransaction> spTrans;
    CVECIOutStr strTrans;
    HRESULT retCode = m_agentDataCommandApps->ResumeGrammarUpdate(&spTrans);
    if (FAILED(retCode) || NULL == spTrans.ptr()) {
        VR_LOGE("ResumeGrammarUpdate: 0x%lx or spTrans is NULL", retCode);
        return;
    }

    retCode = spTrans->GetTransactionId(&strTrans);
    if (FAILED(retCode) || NULL == strTrans.Get()) {
        VR_LOGE("GetTransactionId: 0x%lx or strTrans.Get() is NULL", retCode);
        return;
    }

    std::string strTransaction = strTrans.Get();
    VR_LOGI("strTransaction = %s", strTransaction.c_str());
}

void
VR_VoiceBoxCatalogApps::CancelGrammarUpdate(bool bSync)
{
    if (NULL == m_agentDataCommandApps.ptr()) {
        VR_LOGE("m_agentDataCommandApps is NULL");
        return;
    }
    CVECIPtr<IVECITransaction> spTrans;
    HRESULT retCode = m_agentDataCommandApps->CancelGrammarUpdate(&spTrans);
    if (S_OK != retCode || NULL == spTrans.ptr()) {
        VR_LOGE("CancelGrammarUpdate: 0x%lx or spTrans is NULL", retCode);
        return;
    }

    if (bSync) {
        VBT_BOOL bSaveTimeout = VBT_FALSE;
        const long OVER_TIME = 20000;
        retCode = spTrans->WaitForCompletion(OVER_TIME, &bSaveTimeout);
        if (FAILED(retCode) || bSaveTimeout) {
            VR_LOGE("WaitForCompletion failed: %lx or CancelGrammarUpdate over time", retCode);
            return;
        }
    }

    CVECIOutStr strTrans;
    retCode = spTrans->GetTransactionId(&strTrans);
    if (FAILED(retCode) || NULL == strTrans.Get()) {
        VR_LOGE("GetTransactionId: 0x%lx or strTrans.Get() is NULL", retCode);
        return;
    }

    std::string strTransaction = strTrans.Get();
    VR_LOGI("strTransaction = %s", strTransaction.c_str());
}
/* EOF */
