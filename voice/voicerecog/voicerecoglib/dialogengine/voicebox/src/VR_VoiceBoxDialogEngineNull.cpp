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

#include <sstream>

/* Suntec Header */
#ifndef VR_VOICEBOXDIALOGENGINENULL_H
#    include "VR_VoiceBoxDialogEngineNull.h"
#endif

#ifndef VR_VOICEBOXXMLPARSER_H
#    include "VR_VoiceBoxXmlParser.h"
#endif

#ifndef VR_LOG_H
#    include "VR_Log.h"
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

// Constructor
VR_VoiceBoxDialogEngineNull::VR_VoiceBoxDialogEngineNull()
: m_pcExternalCallback(NULL)
{
}

// Destructor
VR_VoiceBoxDialogEngineNull::~VR_VoiceBoxDialogEngineNull()
{
    m_pcExternalCallback = NULL;
}

// Create the dialog engine related instances and initialize them.
bool VR_VoiceBoxDialogEngineNull::Initialize(
    VR_DialogEngineListener *pcExternalCallback,
    const VR_Settings &settings
    )
{
    VR_LOGI("VR_VoiceBoxDialogEngineNull Initializing\n");
    m_pcExternalCallback = pcExternalCallback;

    VR_ConfigureIF * pcConfig = VR_ConfigureIF::Instance();
    if (NULL != pcConfig) {
        m_country = pcConfig->getVRContry();
    }

    VR_LOGI("VR_VoiceBoxDialogEngineNull Initialize successfully\n");
    return true;
}

// Start the dialog engine
bool VR_VoiceBoxDialogEngineNull::Start()
{
    return true;
}

// Stop the dialog engine
void VR_VoiceBoxDialogEngineNull::Stop()
{
}

// Uninitialize the dialog engine
void VR_VoiceBoxDialogEngineNull::UnInitialize()
{
    m_pcExternalCallback = NULL;
}

// Process the messages that received from external service or self
bool VR_VoiceBoxDialogEngineNull::SendMessage(const std::string& message, int actionSeqId)
{
    VR_LOGD_FUNC();
      // Check the header contents
    const int HEADER_LEN = 150;
    std::string header = message.substr(0, HEADER_LEN);

    if (std::string::npos != header.find("startedNotify")) {
        std::string strStartFinish = "<action agent=\"destatus\" op=\"notifyStartFinish\"></action>";
        SendDMMessage(strStartFinish, actionSeqId);

        std::string strActionResult = "<action agent=\"destatus\" op=\"notifyResourceState\">"
                                         "<resourceStateType>engine</resourceStateType>"
                                         "<resourceState>1</resourceState>"
                                     "</action>";
        SendDMMessage(strActionResult, actionSeqId);

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
            SendDMMessage(strSupportedLanguage, actionSeqId);
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
            SendDMMessage(strSupportedLanguage, actionSeqId);
        }
    }
    else if (std::string::npos != header.find("getSupportedLanguage")) {
        std::string strSupportedLanguage;
        if (VR_REGION_US == m_country) {
            strSupportedLanguage = "<event-result name=\"getSupportedLanguage\" errcode=\"1\">"
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
            strSupportedLanguage = "<event-result name=\"getSupportedLanguage\" errcode=\"1\">"
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
        SendDMMessage(strSupportedLanguage, actionSeqId);
    }
    else if (std::string::npos != header.find("Install Agent")) {
        std::string strInstallFormat = "<event-result name=\"install\" errcode=\"1\">"
                                     "<TSLINFO>"
                                       "<Action type=\"Installed Result\">"
                                         "<Parameter name=\"Result\" value=\"False\"/>"
                                       "</Action>"
                                     "</TSLINFO>"
                                   "</event-result>";
        SendDMMessage(strInstallFormat, actionSeqId);
    }
    else if (std::string::npos != header.find("<event ")) {
        std::string strEventResult;
        AddResultToNode(strEventResult, message, true);
        SendDMMessage(strEventResult, actionSeqId);
    }
    else if (std::string::npos != header.find("<grammar_")) {
        std::string strGrammarResult;
        BuildGrammarResultXML(message, strGrammarResult);
        SendDMMessage(strGrammarResult, actionSeqId);
    }

    return true;
}

std::string
VR_VoiceBoxDialogEngineNull::getHints(const std::string& hintsParams)
{
    return "";
}

void VR_VoiceBoxDialogEngineNull::SendDMMessage(const std::string& strmsg, int actionSeqId)
{
    VR_LOGI("Message: %s\n", strmsg.c_str());
    if (NULL != m_pcExternalCallback) {
        m_pcExternalCallback->OnRequestAction(strmsg, actionSeqId);
    }
}

void VR_VoiceBoxDialogEngineNull::AddResultToNode(std::string& strDestxml, const std::string& strSourxml, bool isAppendErrCode)
{
    pugi::xml_document doc;
    if (!doc.load_string(strSourxml.c_str())) {
        VR_LOGE("Load xml failed");
        return;
    }

    pugi::xml_node node = doc.first_child();

    std::string nodename = node.name();
    nodename.append("-result");
    node.set_name(nodename.c_str());
    if (isAppendErrCode) {
        node.append_attribute("errcode") = "1";
    }

    std::ostringstream ss;
    node.print(ss);
    strDestxml = ss.str();
}

void VR_VoiceBoxDialogEngineNull::BuildGrammarResultXML(const std::string& strGrammarxml, std::string& strGrammarResultxml)
{
    VR_VoiceBoxXmlParser parser(strGrammarxml);
    std::string strRootName = parser.getXmlKey();
    std::string strAgent = parser.getValueByKey("agent");
    std::string strGrammarID = parser.getValueByKey("grammarid");

    std::string strOp;
    if ("grammar_disactive" == strRootName) {
        strOp = "disactive";
    }
    else if ("grammar_active" == strRootName) {
        strOp = "active";
    }
    else {
        strOp = "grammar";
    }

    pugi::xml_document docXML;
    pugi::xml_node root_node = docXML.append_child("grammar_result");
    root_node.append_attribute("agent").set_value(strAgent.c_str());
    root_node.append_attribute("op").set_value(strOp.c_str());
    if ("" != strGrammarID) {
        root_node.append_attribute("grammarid").set_value(strGrammarID.c_str());
    }
    root_node.append_attribute("errcode").set_value("1");

    std::ostringstream ss;
    root_node.print(ss);
    strGrammarResultxml = ss.str();
}

/* EOF */
