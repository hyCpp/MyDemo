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
#include <fstream>
#include <sstream>

/* VBT Header */
#include "VR_Log.h"
/* Suntec Header */
#ifndef VR_CONFIGUREIF_H
#    include "VR_ConfigureIF.h"
#endif

#ifndef VR_VOICEBOXDATASTORAGE_H
#    include "VR_VoiceBoxDataStorage.h"
#endif

#ifndef VR_VOICEBOXCATALOGMANAGER_H
#    include "VR_VoiceBoxCatalogManager.h"
#endif

#ifndef VR_VOICEBOXCATALOGAPPS_H
#    include "VR_VoiceBoxCatalogApps.h"
#endif

#ifndef VR_VOICEBOXCATALOGAUDIO_H
#    include "VR_VoiceBoxCatalogAudio.h"
#endif

#ifndef VR_VOICEBOXCATALOGPHONE_H
#    include "VR_VoiceBoxCatalogPhone.h"
#endif

#ifndef VR_VOICEBOXCATALOGPOI_H
#    include "VR_VoiceBoxCatalogPoi.h"
#endif

#ifndef VR_VOICEBOXXMLPARSER_H
#    include "VR_VoiceBoxXmlParser.h"
#endif

#ifndef VR_VOICEBOXLOG_H
#    include "VR_VoiceBoxLog.h"
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

#define VR_GRAMMAR_STATUS_FINISHED_FILE "/data/vr/config/grammar_status_finished"

VR_VoiceBoxCatalogManager::VR_VoiceBoxCatalogManager(
    IVECIEngineClient& client,
    IVECIEngineCommand& engineCommand,
    VR_VoiceBoxEngineCallback& engineCallback
    )
: m_client(client)
, m_engineCommand(engineCommand)
, m_engineCallback(engineCallback)
, m_catalogApps(NULL)
, m_catalogAudio(NULL)
, m_catalogPhone(NULL)
, m_catalogPoi(NULL)
, m_isUpdateGrammar(false)
, m_bNeedPauseGrammar(true)
, m_bVBTError(false)
{
}

VR_VoiceBoxCatalogManager::~VR_VoiceBoxCatalogManager()
{
    delete m_catalogApps;
    m_catalogApps = NULL;
    delete m_catalogAudio;
    m_catalogAudio = NULL;
    delete m_catalogPhone;
    m_catalogPhone = NULL;
    delete m_catalogPoi;
    m_catalogPoi = NULL;
}

bool
VR_VoiceBoxCatalogManager::Initialize()
{
    m_catalogApps = VR_new VR_VoiceBoxCatalogApps(m_client, m_engineCallback);
    if (NULL == m_catalogApps) {
        VR_LOGE("Create Catalog for Audio Failed!!!");
        return false;
    }

    if (NULL != m_catalogApps) {
        m_catalogApps->Initialize();
    }

    m_catalogAudio = VR_new VR_VoiceBoxCatalogAudio(m_client, m_engineCommand, m_engineCallback);
    if (NULL == m_catalogAudio) {
        VR_LOGE("Create Catalog for Audio Failed!!!");
        return false;
    }

    if (NULL != m_catalogAudio) {
        m_catalogAudio->Initialize();
    }

    m_catalogPhone = VR_new VR_VoiceBoxCatalogPhone(m_client, m_engineCallback);
    if (NULL == m_catalogPhone) {
        VR_LOGE("Create Catalog for Phone Failed!!!");
        return false;
    }

    if (NULL != m_catalogPhone) {
        m_catalogPhone->Initialize();
    }

    m_catalogPoi = VR_new VR_VoiceBoxCatalogPoi(m_client, m_engineCallback);
    if (NULL == m_catalogPoi) {
        VR_LOGE("Create Catalog for POI Failed!!!");
        return false;
    }

    if (NULL != m_catalogPoi) {
        m_catalogPoi->Initialize();
    }

    return true;
}

void
VR_VoiceBoxCatalogManager::ProcessMessage(const std::string& message)
{
    m_bVBTError = false;
    m_engineCallback.SetNeedGrammarResult(true);
    m_bNeedPauseGrammar = true;

    VR_VoiceBoxXmlParser parser(message);
    std::string strXmlKey = parser.getXmlKey();
    std::string strAgent = parser.getValueByKey("agent");
    bool bEngineStarted = m_engineCallback.GetEngineStatus();
    VR_LOGI("strXmlKey = %s, strAgent = %s", strXmlKey.c_str(), strAgent.c_str());
    if (bEngineStarted) {
        bool result = true;
        if ("grammar_init" == strXmlKey || "grammar_diff" == strXmlKey) {
            if ("phone" == strAgent) {
                m_isUpdateGrammar = true;
                m_engineCallback.SendDEStatus("voicetag", "1");
                m_engineCallback.SetPhoneReloadDataFlg(true);
                result = m_catalogPhone->SetupPhoneBookData(parser);
                if (!result) {
                    m_isUpdateGrammar = false;
                    m_engineCallback.SetPhoneReloadDataFlg(false);
                }
            }
            else if ("media" == strAgent) {
                result = m_catalogAudio->SetupMusicData(parser);
            }
            else if ("poi" == strAgent) {
                result = m_catalogPoi->Cataloguing(parser);
            }
            else {
                m_engineCallback.SetUpdateGammarFlg(true);
            }
        }
        else if ("category" == strXmlKey) {
            if (std::string::npos != message.find("<category name=\"quickreplymessage\"")) {
                result = m_catalogPhone->UpdateHFDQuickReplyMessages(parser);
            }
            else if (std::string::npos != message.find("<category name=\"phonetype\"")) {
                result = m_catalogPhone->UpdateHFDPhoneTypes(parser);
            }
            else if (std::string::npos != message.find("<category name=\"messagetype\"")) {
                result = m_catalogPhone->UpdateHFDMessageTypes(parser);
            }
            else if (std::string::npos != message.find("<category name=\"audiosource\"")) {
                result = m_catalogAudio->UpdateMusicAudioSources(parser);
            }
            else if (std::string::npos != message.find("<category name=\"fmgenre\"")) {
                result = m_catalogAudio->UpdateRadioFMGenres(parser);
            }
            else if (std::string::npos != message.find("<category name=\"satchannelname\"")) {
                result = m_catalogAudio->UpdateRadioSatelliteChannelNames(parser);
            }
            else if (std::string::npos != message.find("<category name=\"satchannelnumber\"")) {
                result = m_catalogAudio->UpdateRadioSatelliteChannelNumbers(parser);
            }
            else if (std::string::npos != message.find("<category name=\"satgenre\"")) {
                result = m_catalogAudio->UpdateRadioSatelliteGenres(parser);
            }
            else if (std::string::npos != message.find("<add>")) {
                result = m_catalogAudio->UpdateMusicDiffInfo(parser, "add");
            }
            else if (std::string::npos != message.find("<delete>")) {
                result = m_catalogAudio->UpdateMusicDiffInfo(parser, "delete");
            }
            else {
                m_engineCallback.SetUpdateGammarFlg(true);
            }
        }
        // else if ("grammar_diff" == strXmlKey) {
        //     result = m_catalogAudio->MusicGrammarDiff(parser);
        // }
        else if ("grammar_active" == strXmlKey) {
            if ("phone" == strAgent) {
                if (m_isUpdateGrammar) {
                    m_isUpdateGrammar = false;
                    std::string grammarInitResult = "<grammar_result op=\"active\" agent=\"phone\" grammarid=\"1\" errcode=\"0\"/>";
                    m_engineCallback.OnRecognized(grammarInitResult);
                    m_engineCallback.SetUpdateGammarFlg(true);
                    m_engineCallback.NotifyPhoneGrammarFinished();
                }
                else {
                    m_engineCallback.SendDEStatus("voicetag", "1");
                    m_engineCallback.SetPhoneReloadDataFlg(true);
                    result = m_catalogPhone->SetupPhoneBookData(parser);
                    if (!result) {
                        m_engineCallback.SetPhoneReloadDataFlg(false);
                    }
                }
            }
            else if ("media" == strAgent) {
                result = m_catalogAudio->MusicGrammarActive(parser);
            }
            else {
                m_engineCallback.SetUpdateGammarFlg(true);
            }
        }
        else if ("grammar_disactive" == strXmlKey) {
            if ("phone" == strAgent) {
                m_engineCallback.SendDEStatus("voicetag", "1");
                result = m_catalogPhone->PhoneGrammarDisActive(parser);
            }
            else if ("media" == strAgent) {
                result = m_catalogAudio->MusicGrammarDisActive(parser);
            }
            else {
                m_engineCallback.SetUpdateGammarFlg(true);
            }
        }
        else if ("grammar_voicetag" == strXmlKey) {
            m_engineCallback.SendDEStatus("voicetag", "1");
            result = m_catalogPhone->UpdateVoiceTagGrammar(parser);
        }
        else if ("grammar_audiosource_oc" == strXmlKey) {
            result = m_catalogAudio->MusicGrammarAudioSourceOC(parser);
        }
        else if ("grammar_delete" == strXmlKey) {
            result = m_catalogPoi->DeleteGrammar();
        }
        else if ("grammar_reloadgrammar" == strXmlKey) {
            result = m_catalogPhone->UpdateHFDAgentContacts();
        }
        else {
            m_engineCallback.SetUpdateGammarFlg(true);
        }

        if (!result) {
            VR_LOGI("there are some error of VBT");
            m_bVBTError = true;
            m_engineCallback.SetUpdateGammarFlg(true);
        }
    }
    else {
        VR_LOGI("the engine is not started");
        if ("grammar_init" == strXmlKey || "grammar_diff" == strXmlKey) {
            if ("media" == strAgent) {
                std::string strGrammarId = parser.getValueByKey("grammarid");
                m_engineCallback.SendGrammarResult("grammar", "media", strGrammarId, "1");
            }
            else if ("phone" == strAgent) {
                m_engineCallback.SendGrammarResult("grammar", "phone", "1", "2");
            }
            else {

            }
        }
        // else if ("grammar_diff" == strXmlKey) {
        //    std::string strGrammarId = parser.getValueByKey("grammarid");
        //     m_engineCallback.SendGrammarResult("grammar", "media", strGrammarId, "1");
        // }
        else if ("grammar_active" == strXmlKey) {
            if ("phone" == strAgent) {
                m_engineCallback.SendGrammarResult("active", "phone", "1", "2");
            }
            else if ("media" == strAgent) {
                std::string strReply = parser.getValueByKey("reply");
                if ("true" == strReply) {
                    std::string strGrammarId = parser.getValueByKey("grammarid");
                    m_engineCallback.SendGrammarResult("active", "media", strGrammarId.c_str(), "1");
                }
            }
            else {

            }
        }
        else if ("grammar_disactive" == strXmlKey) {
            if ("phone" == strAgent) {
                std::string strSender = parser.getValueByKey("sender");
                if ("DE" != strSender) {
                    m_engineCallback.SendGrammarResult("disactive", "phone", "1", "2");
                }
            }
            else if ("media" == strAgent) {
                std::string strReply = parser.getValueByKey("reply");
                if ("true" == strReply) {
                    std::string strGrammarId = parser.getValueByKey("grammarid");
                    m_engineCallback.SendGrammarResult("disactive", "media", strGrammarId.c_str(), "1");
                }
            }
        }
        else {
            VR_LOGI("engine is not started, strXmlKey = %s", strXmlKey.c_str());
        }

        m_engineCallback.SetUpdateGammarFlg(true);
    }

}

VR_VoiceBoxCatalogPhone*
VR_VoiceBoxCatalogManager::GetCatalogPhone()
{
    return m_catalogPhone;
}

VR_VoiceBoxCatalogAudio*
VR_VoiceBoxCatalogManager::GetCatalogAudio()
{
    return m_catalogAudio;
}

void
VR_VoiceBoxCatalogManager::PauseGrammarUpdate(const std::string& strAgent)
{
    VR_LOGI("strAgent = %s", strAgent.c_str());
    if (strAgent.empty()) {
        return;
    }

    if ("music" == strAgent) {
        m_catalogAudio->PauseGrammarMusic();
    }
    else if ("radio" == strAgent) {
        m_catalogAudio->PauseGrammarRadio();
    }
    else if ("hfd" == strAgent) {
        m_catalogPhone->PauseGrammarPhone();
    }
    else if ("apps" == strAgent) {
        m_catalogApps->PauseGrammarApps();
    }
    else if ("poi" == strAgent) {
        m_catalogPoi->PauseGrammarPoi();
    }
    else {

    }

    return;
}

void
VR_VoiceBoxCatalogManager::ResumeGrammarUpdate(const std::string& strAgent)
{
    VR_LOGI("strAgent = %s", strAgent.c_str());
    if (strAgent.empty()) {
        return;
    }

    if ("music" == strAgent) {
        m_catalogAudio->ResumeGrammarMusic();
    }
    else if ("radio" == strAgent) {
        m_catalogAudio->ResumeGrammarRadio();
    }
    else if ("hfd" == strAgent) {
        m_catalogPhone->ResumeGrammarPhone();
    }
    else if ("apps" == strAgent) {
        m_catalogApps->ResumeGrammarApps();
    }
    else if ("poi" == strAgent) {
        m_catalogPoi->ResumeGrammarPoi();
    }
    else {

    }

    return;
}

void
VR_VoiceBoxCatalogManager::SendPhoneGrammarResult(const std::string& strPhoneGrammar)
{
    std::string strOp;
    if (std::string::npos != strPhoneGrammar.find("<grammar_active agent=\"phone\"")) {
        strOp = "active";
    }
    else if (std::string::npos != strPhoneGrammar.find("<grammar_disactive agent=\"phone\"")) {
        strOp = "disactive";
    }
    else if (std::string::npos != strPhoneGrammar.find("<grammar_init agent=\"phone\"")) {
        strOp = "grammar";
    }
    else {

    }

    m_engineCallback.SendGrammarResult(strOp, "phone", "1", "2");

    return;
}

std::pair<std::string, std::string>
VR_VoiceBoxCatalogManager::GetCurrentTransaction()
{
    return m_engineCallback.GetCurrentTransaction();
}

void
VR_VoiceBoxCatalogManager::CancelGrammarUpdate(const std::string& strAgent, bool bSync)
{
    VR_LOGD_FUNC();
    if ("music" == strAgent && NULL != m_catalogAudio) {
        m_catalogAudio->CancelMusicGrammarUpdate(bSync);
    }
    else if ("radio" == strAgent && NULL != m_catalogAudio) {
        m_catalogAudio->CancelRadioGrammarUpdate(bSync);
    }
    else if ("hfd" == strAgent && NULL != m_catalogPhone) {
        m_catalogPhone->CancelGrammarUpdate(bSync);
    }
    else if ("apps" == strAgent && NULL != m_catalogApps) {
        m_catalogApps->CancelGrammarUpdate(bSync);
    }
    else if ("poi" == strAgent && NULL != m_catalogPoi) {
        m_catalogPoi->CancelGrammarUpdate(bSync);
    }
    else {

    }

    return;
}

void
VR_VoiceBoxCatalogManager::InitialPersonData()
{
    if (NULL != m_catalogPhone) {
        m_catalogPhone->InitialHFDPersonData();
    }

    if (NULL != m_catalogAudio) {
        m_catalogAudio->InitialMusicPersonData();
    }

    return;
}

void
VR_VoiceBoxCatalogManager::SetNeedPauseGrammarFlg(bool bNeedPauseGrammarFlg)
{
    m_bNeedPauseGrammar = bNeedPauseGrammarFlg;
}

bool
VR_VoiceBoxCatalogManager::GetNeedPauseGrammarFlg()
{
    return m_bNeedPauseGrammar;
}

void
VR_VoiceBoxCatalogManager::ClearCurrentTransaction()
{
    m_engineCallback.SetCurrentTransaction("", "");
}

bool
VR_VoiceBoxCatalogManager::GetVBTErrorFlg()
{
    return m_bVBTError;
}

void VR_VoiceBoxCatalogManager::HandleGrammarAfterVDATAUpdate(bool isVDataUpdate, bool &isNeedInstallApp)
{
    VR_LOGD_FUNC();
    isNeedInstallApp = false;
    if (isVDataUpdate) {
        // when first check VDATA update, delete grammar status file, and init value in DB
        if (0 != remove(VR_GRAMMAR_STATUS_FINISHED_FILE)) {
            VR_LOGI("remove grammar status file failed , maybe the file not exist!");
        }
        else {
            VR_LOGI("remove grammar status file successfully !");
        }
        // init grammar status
        unsigned int statusvalue = 0x1F;
        VR_VoiceBoxDataStorage storage;
        storage.PutGrammarStatusValue(statusvalue);

        // when version change ,we delete all grammar data, and request make new grammar
        // this to add quest install app first, because grammar will pause when installing app
        isNeedInstallApp = true;
    }
    else {
        // check whether grammarstatus file exit, if exist, no deed to request reproduce grammar
        std::ifstream fileIn(VR_GRAMMAR_STATUS_FINISHED_FILE, std::ios::in);
        if (fileIn) {
            VR_LOGI("grammar_status_finished file exist, no need reproduce grammar ");
            return;
        }

        // if restart when request make new grammar, we should check grammar status and do the unfinished grammar
        m_MapUndoGrammar.clear();
        if (!GetGrammarUndoStatus(m_MapUndoGrammar)) {
            VR_LOGI("no need reproduce grammar ");
            return;
        }
        VoiceMultiMap<std::string, std::string>::iterator it = m_MapUndoGrammar.find("app");
        if (it != m_MapUndoGrammar.end()) {
            // do install app first
            isNeedInstallApp = true;
            m_MapUndoGrammar.erase(it);
        }
        else {
            RequestGrammarExceptApp(false);
        }
    }

}

void VR_VoiceBoxCatalogManager::RequestGrammarExceptApp(bool isVDataUpdate)
{
    VR_LOGD_FUNC();
    VR_LOGI("isVDataUpdate = %d", isVDataUpdate);

    if (isVDataUpdate) {
        // after VDATA update , request music grammar
        // media grammarID is from 1 to 4
        for (int i = 1; i < 5; ++i) {
            std::stringstream ss;
            ss << i;
            m_engineCallback.SendRequestGrammar("media", ss.str());
        }
    }
    else {
        for (VoiceMultiMap<std::string, std::string>::iterator it = m_MapUndoGrammar.begin(); it != m_MapUndoGrammar.end(); ++it) {
            m_engineCallback.SendRequestGrammar(it->first, it->second);
        }
        m_MapUndoGrammar.clear();
    }

}

bool VR_VoiceBoxCatalogManager::UpdateGrammarFinishedStatus(const std::string& strGrammarName, const std::string& strGrammarID)
{

    int iStep;
    std::string strName = strGrammarName + strGrammarID;
    VR_LOGI("grammarName + grammarID : %s", strName.c_str());
    if ("app" == strName) {
        iStep = 0;
    }
    else if ("media1" == strName) {
        iStep = 1;
    }
    else if ("media2" == strName) {
        iStep = 2;
    }
    else if ("media3" == strName) {
        iStep = 3;
    }
    else if ("media4" == strName) {
        iStep = 4;
    }
    else {
        VR_LOGI("cannot find grammar, check input parameter !!");
        return false;
    }

    unsigned int value = 1 << iStep;
    unsigned int currentValue = 0x00;

    VR_VoiceBoxDataStorage storage;
    storage.GetGrammarStatusValue(currentValue);
    VR_LOGI("currentValue = %d", currentValue);
    value = ~value;
    currentValue &= value;
    currentValue &= 0x1F;
    VR_LOGI("currentValue = %d", currentValue);

    // write back
    storage.PutGrammarStatusValue(currentValue);

    // if all grammar are done (which currentValue = 0), create the grammar status file otherwise return
    if (0 == currentValue) {
        std::ofstream fileOut(VR_GRAMMAR_STATUS_FINISHED_FILE, std::ios::out | std::ios::trunc);
        if (fileOut) {
            VR_LOGI("create grammar status file successfully ");
        }
        else {
            VR_LOGE("create grammar status file failed ");
        }
    }

    return true;
}

bool VR_VoiceBoxCatalogManager::GetGrammarUndoStatus(VoiceMultiMap<std::string, std::string>::type& mapUndoGrammar)
{

    unsigned int currentValue = 0x00;
    VR_VoiceBoxDataStorage storage;
    storage.GetGrammarStatusValue(currentValue);
    currentValue &= 0x1F;
    VR_LOGI("currentValue = %d", currentValue);

    if (0 == currentValue) {
        VR_LOGI("there is no grammar need to do ");
        return false;
    }

    mapUndoGrammar.clear();
    unsigned int index = 1;
    if (currentValue & index) {
        mapUndoGrammar.insert(std::pair<std::string, std::string>("app", ""));
    }
    index = index << 1;
    if (currentValue & index) {
        mapUndoGrammar.insert(std::pair<std::string, std::string>("media", "1"));
    }
    index = index << 1;
    if (currentValue & index) {
        mapUndoGrammar.insert(std::pair<std::string, std::string>("media", "2"));
    }
    index = index << 1;
    if (currentValue & index) {
        mapUndoGrammar.insert(std::pair<std::string, std::string>("media", "3"));
    }
    index = index << 1;
    if (currentValue & index) {
        mapUndoGrammar.insert(std::pair<std::string, std::string>("media", "4"));
    }
    return true;
}


/* EOF */
