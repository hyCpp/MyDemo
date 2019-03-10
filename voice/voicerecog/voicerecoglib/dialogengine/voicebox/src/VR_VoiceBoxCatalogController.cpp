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

/* VBT Header */

/* Suntec Header */
#include "VR_Log.h"
#ifndef VR_VOICEBOXCATALOGCONTROLLER_H
#    include "VR_VoiceBoxCatalogController.h"
#endif

#ifndef VR_VOICEBOXCATALOGMANAGER_H
#    include "VR_VoiceBoxCatalogManager.h"
#endif

#ifndef VR_VOICEBOXENGINEIF_H
#    include "VR_VoiceBoxEngineIF.h"
#endif

#ifndef CXX_BL_AUTOSYNC_H
#   include "BL_AutoSync.h"
#endif

#ifndef VR_VOICEBOXLOG_H
#    include "VR_VoiceBoxLog.h"
#endif

#ifndef VR_VOICEBOXXMLPARSER_H
#    include "VR_VoiceBoxXmlParser.h"
#endif

BL_SyncObject   VR_VoiceBoxCatalogController::s_cSync;   ///< Sync object

// Constructor
VR_VoiceBoxCatalogController::VR_VoiceBoxCatalogController()
: m_msglist()
, m_statuslst()
, m_voiceBoxEngine(NULL)
, m_pcCatalogManager(NULL)
, m_bUpdateGrammar(true)
, m_bCheckPauseConditon(false)
, m_bNeedResumeGrammar(false)
, m_bDialogSession(false)
, m_bNeedCancelGrammar(false)
, m_totalindex(0)
, m_index(0)
{
}

// Destructor
VR_VoiceBoxCatalogController::~VR_VoiceBoxCatalogController()
{
    m_voiceBoxEngine = NULL;
    m_pcCatalogManager = NULL;
}

// Initialize the Message Controller
bool
VR_VoiceBoxCatalogController::Initialize(VR_VoiceBoxEngineIF* pcEngine)
{
    if (NULL == pcEngine) {
        return false;
    }

    m_voiceBoxEngine = pcEngine;
    m_pcCatalogManager = m_voiceBoxEngine->GetCatalogManager();
    RegisterName("VR_VBT_AGENT_DATA_THREAD");
    return true;
}

// Start the Message Controller thread
void
VR_VoiceBoxCatalogController::Start()
{
    StartRegistThread();
}

// Stop the Message Controller thread
void
VR_VoiceBoxCatalogController::Stop()
{
    StopThread();
}

// Post message to the Message Controller
bool
VR_VoiceBoxCatalogController::PostMessage(const std::string& message, int actionSeqId)
{
    VR_LOGD_FUNC();
    // If the m_pcCatalogManager is not exist,
    // we could not process the message.
    if (NULL == m_pcCatalogManager) {
        return false;
    }

    m_totalindex++;

    VR_LOGI("m_totalindex = %d, message = %s", m_totalindex, message.c_str());

    // Add new messages to the list
    {
        BL_AutoSync cAutoSync(s_cSync);
        if (std::string::npos != message.find("<category name=\"audiosource\"")) {
            m_strAudioSource = message;
        }
        else {
            m_msglist.push_back(message);
        }
    }

    Notify();

    return true;
}

// Post message to the Message Controller
bool
VR_VoiceBoxCatalogController::PostStatusMsg(const std::string& message, int actionSeqId)
{
    // If the m_pcCatalogManager is not exist,
    // we could not process the message.
    if (NULL == m_pcCatalogManager) {
        return false;
    }

    // Add new messages to the list
    {
        BL_AutoSync cAutoSync(m_cStatusSync);
        VR_LOGI("message = %s", message.c_str());
        m_statuslst.push_back(message);
    }

    Notify();

    return true;
}

std::string
VR_VoiceBoxCatalogController::PopStatusMsg()
{

    std::string strStatusMsg;
    {
        BL_AutoSync cAutoSync(m_cStatusSync);
        if (!m_statuslst.empty()) {
            strStatusMsg = m_statuslst.front();
            m_statuslst.pop_front();
        }

    }

    return strStatusMsg;
}

// Push message to the front of message list
bool
VR_VoiceBoxCatalogController::PushFrontMessage(const std::string& message, int actionSeqId)
{
    VR_LOGD_FUNC();
    // If the m_pcCatalogManager is not exist,
    // we could not process the message.
    if (NULL == m_pcCatalogManager) {
        return false;
    }

    // Add new messages to the list
    {
        BL_AutoSync cAutoSync(s_cSync);
        if (std::string::npos != message.find("<category name=\"audiosource\"")) {
            m_strAudioSource = message;
        }
        else {
            m_msglist.push_front(message);
        }
    }

    Notify();

    return true;
}

// Process the messages
void
VR_VoiceBoxCatalogController::Run()
{
    while (true) {
        while (!m_statuslst.empty()) {
            if (TRUE == CheckQuit()) {
                break;
            }

            std::string strStatusMsg = PopStatusMsg();
            if (!strStatusMsg.empty()) {
                UpdateGrammarStatus(strStatusMsg);
            }
        }

        if (TRUE == CheckQuit()) {
            break;
        }

        if (m_bCheckPauseConditon) {
            if (NULL != m_pcCatalogManager) {
                std::pair<std::string, std::string> pairAgent2TransId = m_pcCatalogManager->GetCurrentTransaction();
                if (!pairAgent2TransId.first.empty()) {
                    m_bCheckPauseConditon = false;
                    m_pcCatalogManager->SetNeedPauseGrammarFlg(false);
                    m_pcCatalogManager->PauseGrammarUpdate(pairAgent2TransId.first);
                }
            }
        }

        if (m_bNeedResumeGrammar) {
            if (NULL != m_pcCatalogManager) {
                std::pair<std::string, std::string> pairAgent2TransId = m_pcCatalogManager->GetCurrentTransaction();
                if (!pairAgent2TransId.first.empty()) {
                    m_bNeedResumeGrammar = false;
                    m_pcCatalogManager->ResumeGrammarUpdate(pairAgent2TransId.first);
                }
            }
        }

        if (m_bNeedCancelGrammar) {
            if (NULL != m_pcCatalogManager) {
                std::pair<std::string, std::string> pairAgent2TransId = m_pcCatalogManager->GetCurrentTransaction();
                if (!pairAgent2TransId.first.empty()) {
                    m_bNeedCancelGrammar = false;
                    m_pcCatalogManager->CancelGrammarUpdate(pairAgent2TransId.first, false);
                }
            }
        }

        if (m_strAudioSource.empty() && m_strUpdateAgainMsg.empty() && m_msglist.empty()) {
            VR_LOGI("no message, wait");
            Wait();
            continue;
        }

        if (TRUE == CheckQuit()) {
            break;
        }

        if (m_bUpdateGrammar && !m_bDialogSession) {
            VR_LOGI("m_bUpdateGrammar is true");
            m_bNeedCancelGrammar = false;
            if (!m_strAudioSource.empty()) {
                m_bUpdateGrammar = false;
                BL_AutoSync cAutoSync(s_cSync);
                m_strCurrentMsg = m_strAudioSource;
                m_index++;
                VR_LOGI("m_index = %d begin, m_strCurrentMsg = %s", m_index, m_strAudioSource.c_str());
                m_pcCatalogManager->ProcessMessage(m_strAudioSource);
                VR_LOGI("m_index = %d end, m_strCurrentMsg = %s", m_index, m_strAudioSource.c_str());
                m_strAudioSource.clear();
            }
            else if (!m_strUpdateAgainMsg.empty()) {
                m_bUpdateGrammar = false;
                m_strCurrentMsg = m_strUpdateAgainMsg;
                m_index++;
                VR_LOGI("m_index = %d begin, m_strCurrentMsg = %s", m_index, m_strUpdateAgainMsg.c_str());
                m_pcCatalogManager->ProcessMessage(m_strUpdateAgainMsg);
                VR_LOGI("m_index = %d end, m_strCurrentMsg = %s", m_index, m_strUpdateAgainMsg.c_str());
                m_strUpdateAgainMsg.clear();
                m_index++;
            }
            else {
                while (!m_msglist.empty() && m_bUpdateGrammar) {
                    if (TRUE == CheckQuit()) {
                        break;
                    }

                    {
                        BL_AutoSync cAutoSync(s_cSync);
                        if (!m_msglist.empty()) {
                            m_strCurrentMsg = m_msglist.front();
                            m_msglist.pop_front();
                        }
                        else {
                            continue;
                        }
                    }
                    m_bUpdateGrammar = false;
                    m_index++;
                    VR_LOGI("m_index = %d begin, m_strCurrentMsg = %s", m_index, m_strCurrentMsg.c_str());

                    m_pcCatalogManager->ProcessMessage(m_strCurrentMsg);

                    VR_LOGI("m_index = %d end, m_strCurrentMsg = %s", m_index, m_strCurrentMsg.c_str());
                }
            }
        }
        else {
            VR_LOGI("m_bUpdateGrammar is false, wait");
            Wait();
        }
    }
}

void
VR_VoiceBoxCatalogController::ClearMessage()
{
    BL_AutoSync cAutoSync(s_cSync);
    m_msglist.clear();
}

void
VR_VoiceBoxCatalogController::DeletePhoneGrammarBuf()
{
    VR_LOGD_FUNC();

    BL_AutoSync cAutoSync(s_cSync);

    VoiceList<std::string>::iterator iter =  m_msglist.begin();
    for (; m_msglist.end() != iter;) {
        std::string strTmp = *iter;
        if ((std::string::npos != strTmp.find("quickreplymessage")
            || std::string::npos != strTmp.find("phonetype")
            || std::string::npos != strTmp.find("messagetype"))
            || std::string::npos != strTmp.find("<grammar_active agent=\"phone\"")
            || std::string::npos != strTmp.find("<grammar_disactive agent=\"phone\"")
            || std::string::npos != strTmp.find("<grammar_init agent=\"phone\"")) {
            VR_LOGI("delete phone grammar msg = %s", strTmp.c_str());
            m_pcCatalogManager->SendPhoneGrammarResult(strTmp);
            iter = m_msglist.erase(iter);
            --m_totalindex;
        }
        else {
            ++iter;
        }
    }

    VR_LOGI("m_totalindex = %d", m_totalindex);
}

void
VR_VoiceBoxCatalogController::NotifyPhoneGrammarFinished()
{
    VR_LOGD_FUNC();

    BL_AutoSync cAutoSync(s_cSync);

    bool isPhoneGrammar = false;
    VoiceList<std::string>::iterator iter =  m_msglist.begin();
    for (; m_msglist.end() != iter;) {
        std::string strTmp = *iter;
        if (std::string::npos != strTmp.find("<grammar_voicetag agent=\"phone\"")
            || std::string::npos != strTmp.find("<grammar_active agent=\"phone\"")
            || std::string::npos != strTmp.find("<grammar_disactive agent=\"phone\"")
            || std::string::npos != strTmp.find("<grammar_init agent=\"phone\"")) {
            VR_LOGI("phone grammar msg = %s", strTmp.c_str());
            isPhoneGrammar = true;
            break;
        }
        ++iter;
    }
    if (!isPhoneGrammar) {
        m_voiceBoxEngine->SendDEStatus("voicetag", "0");
    }
}

void
VR_VoiceBoxCatalogController::UpdateGrammarStatus(const std::string& strStatusMsg)
{
    VR_LOGI("strStatusMsg = %s", strStatusMsg.c_str());
    if (strStatusMsg.empty() || NULL == m_pcCatalogManager) {
         return;
    }

    VR_VoiceBoxXmlParser parser(strStatusMsg);
    std::string strXmlKey = parser.getXmlKey();
    std::string strValue = parser.getValueByKey("value");
    bool bFlg = ("true" == strValue) ? true : false;
    if ("checkpauseconditon" == strXmlKey) {
        m_bCheckPauseConditon = bFlg;
    }
    else if ("needresume" == strXmlKey) {
        m_bNeedResumeGrammar = bFlg;
    }
    else if ("finishupdate" == strXmlKey) {
        VR_LOGI("m_index = %d FinishUpdateGammar", m_index);
        m_bUpdateGrammar = bFlg;
        if (m_bUpdateGrammar) {
            m_strCurrentMsg.clear();
        }
    }
    else if ("updateagain" == strXmlKey) {
        m_strUpdateAgainMsg = m_strCurrentMsg;
    }
    else if ("deletephonebuf" == strXmlKey) {
        DeletePhoneGrammarBuf();
    }
    else if ("nofiyphonegrammarfinished" == strXmlKey) {
        NotifyPhoneGrammarFinished();
    }
    else if ("clearcurrenttransaction" == strXmlKey) {
        m_pcCatalogManager->ClearCurrentTransaction();
    }
    else if ("needpausegrammar" == strXmlKey) {
        m_pcCatalogManager->SetNeedPauseGrammarFlg(bFlg);
    }
    else if ("dialogsession" == strXmlKey) {
        m_bDialogSession = bFlg;
        if (!m_bDialogSession && m_pcCatalogManager->GetVBTErrorFlg()) {
            m_bUpdateGrammar = true;
        }
    }
    else if ("needcancelgrammar" == strXmlKey) {
        VR_LOGI("m_strCurrentMsg = %s", m_strCurrentMsg.c_str());
        m_bNeedCancelGrammar = true;
        m_strUpdateAgainMsg = m_strCurrentMsg;
    }
    else {

    }

    return;
}

std::string
VR_VoiceBoxCatalogController::GetCurrentMessage()
{
    return m_strCurrentMsg;
}

/* EOF */
