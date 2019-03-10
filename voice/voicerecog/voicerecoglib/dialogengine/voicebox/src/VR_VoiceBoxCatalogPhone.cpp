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
#include "VR_Log.h"

/* VBT Header */
#ifndef VR_VOICEBOXSOURCE_H
#    include "VR_VoiceBoxSource.h"
#endif

/* Suntec Header */
#ifndef VR_VOICEBOXCATALOGPHONE_H
#    include "VR_VoiceBoxCatalogPhone.h"
#endif

#ifndef VR_VOICEBOXXMLPARSER_H
#    include "VR_VoiceBoxXmlParser.h"
#endif

#ifndef VR_VOICEBOXLOG_H
#    include "VR_VoiceBoxLog.h"
#endif

#ifndef VR_CONFIGUREIF_H
#include "VR_ConfigureIF.h"
#endif

#ifndef VR_VOICETAGIDMANAGER_H
#include "VR_VoiceTagIDManager.h"
#endif

#ifndef VR_VOICEBOXDATASTORAGE_H
#    include "VR_VoiceBoxDataStorage.h"
#endif

#ifndef VR_VOICEBOXPHONEDATA_H
#    include "VR_VoiceBoxPhoneData.h"
#endif

using namespace nutshell;

#define VR_VOICETAG_DB_PATH     "voicetagDB/"
#define VR_VOICETAG_PCM_PATH    "pcm/"

// voicetag value node name

#define VR_VOICETAG_PHONEME_NODE    "phoneme"
#define VR_VOICETAG_TEMPDATA        "tempData"
#define VR_VOICETAG_ID_PREFIX       "VoiceTagID"

VoiceMap<std::string, std::string>::type VR_VoiceBoxCatalogPhone::m_mapSaveTransactionId;
VR_VoiceBoxCatalogPhone::VR_VoiceBoxCatalogPhone(
    IVECIEngineClient& client,
    VR_VoiceBoxEngineCallback& engineCallback
    )
: m_client(client)
, m_engineCallback(engineCallback)
, m_isDisactiveFromDE(false)
{

}

VR_VoiceBoxCatalogPhone::~VR_VoiceBoxCatalogPhone()
{
}

bool
VR_VoiceBoxCatalogPhone::Initialize()
{
    HRESULT result = m_client.CreateAgentDispatcher(&m_agentDataCommand);
    if (S_OK != result) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "CreateAgentDispatcher: 0x%lx", result);
        return false;
    }
    if (NULL == m_agentDataCommand.ptr()) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, 0, "m_agentDataCommand is NULL");
        return false;
    }

    result = m_agentDataCommand->Init(VBT_AGENT_HFD);
    if (S_OK != result) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "Init: 0x%lx", result);
        return false;
    }

    return true;
}

void
VR_VoiceBoxCatalogPhone::UnInitialize()
{
    m_mapSaveTransactionId.clear();
}

void
VR_VoiceBoxCatalogPhone::CreateVoiceTag()
{

}

bool
VR_VoiceBoxCatalogPhone::UpdateVoiceTagGrammar(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    std::string strOp = parser.getValueByKey("op");

    VR_LOGI("strOp : %s", strOp.c_str());

    if ("addGrammar" == strOp) {
        std::string strFullName = parser.getValueByKey("fullName");
        std::string strPhoneme = parser.getValueByKey("phoneme");
        VR_LOGI("strFullName : %s", strFullName.c_str());
        VR_LOGI("strPhoneme : %s", strPhoneme.c_str());
        return RecompileTheGrammar(strFullName, strPhoneme);
    }
    else if ("DeleteAllVoiceTagGrammar" == strOp) {
        std::string strAddress = parser.getValueByKey("deviceAddress");
        VR_LOGI("strAddress : %s", strAddress.c_str());
        return RecompileTheGrammar(strAddress);
    }
    else {
        VR_LOGI("no need to handle !");
        return false;
    }

    return true;
}

bool
VR_VoiceBoxCatalogPhone::RecompileTheGrammar(const std::string &strFullName, const std::string &phoneme)
{
    VR_LOGD_FUNC();

    CVECIPtr<IVECITransaction> spTrans;
    if (NULL == m_agentDataCommand.ptr()) {
        VR_LOGE("m_agentDataCommand is NULL");
        return false;
    }
    HRESULT retCode = m_agentDataCommand->SetDataSynchronized(&spTrans, _T(""), _T("HFDAgentContacts"), _T("contacts"), VBT_FALSE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%x", retCode);
        return false;
    }

    CVECIPtr<IVECIListItems> contactList;
    (void)m_client.CreateListItems(&contactList);
    CVECIPtr<IVECIListItems> deleteContactList;
    (void)m_client.CreateListItems(&deleteContactList);
    // GetContactNodeInfo(contactNode, phoneme, deleteContactList, contactList);
    GetContactNodeInfo(strFullName, deleteContactList);

    retCode = m_agentDataCommand->RemoveData(&spTrans, _T(""), _T("HFDAgentContacts"), deleteContactList);
    if (S_OK != retCode) {
        VR_LOGE("RemoveData: 0x%x", retCode);
        return false;
    }
    GetContactNodeInfo(strFullName, phoneme, contactList);
    retCode = m_agentDataCommand->AddData(&spTrans, _T(""), _T("HFDAgentContacts"), contactList);
    if (S_OK != retCode) {
        VR_LOGE("AddData: 0x%x", retCode);
        return false;
    }

    retCode = m_agentDataCommand->SetDataSynchronized(&spTrans, _T(""), _T("HFDAgentContacts"), _T("contacts"), VBT_TRUE);
    VR_LOGI("RecompileTheGrammar2");
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%x", retCode);
        return false;
    }

    // CVECIPtr<IVECIListItems> deleteContactGrammarList = GetOneVoiceTagInfo(strFullName);
    // if (NULL == deleteContactGrammarList.ptr()) {
    //     return false;
    // }
    // retCode = m_agentDataCommand->RemoveGrammar(&spTrans, _T(""),  _T("HFDAgentContacts"), _T("contacts"), deleteContactGrammarList);
    // if (S_OK != retCode) {
    //     VR_LOGE("RemoveGrammar: 0x%x", retCode);
    //     return false;
    // }
    retCode = m_agentDataCommand->ReloadGrammar(&spTrans, _T(""),  _T("HFDAgentContacts"));
    if (S_OK != retCode) {
        VR_LOGE("ReloadGrammar: 0x%x", retCode);
        return false;
    }

    CVECIOutStr strTrans;
    retCode = spTrans->GetTransactionId(&strTrans);
    if (FAILED(retCode) || NULL == strTrans.Get()) {
        VR_LOGE("GetTransactionId: 0x%lx or strTrans.Get() is NULL", retCode);
        return false;
    }

    std::string strTransaction = strTrans.Get();
    m_engineCallback.SetCurrentTransaction("hfd", strTransaction);
    m_mapSaveTransactionId.insert(std::make_pair(strTransaction, "VoiceTag"));

    return true;
}

bool
VR_VoiceBoxCatalogPhone::RecompileTheGrammar(const std::string &strDeviceAddress)
{
    VR_LOGD_FUNC();

    if (!m_vecContact.empty()) {
        CVECIPtr<IVECITransaction> spTrans;
        if (NULL == m_agentDataCommand.ptr()) {
            VR_LOGE("m_agentDataCommand is NULL");
            return false;
        }
        HRESULT retCode = m_agentDataCommand->SetDataSynchronized(&spTrans, _T(""), _T("HFDAgentContacts"), _T("contacts"), VBT_FALSE);
        if (S_OK != retCode) {
            VR_LOGE("SetDataSynchronized: 0x%x", retCode);
            return false;
        }

        CVECIPtr<IVECIListItems> contactList;
        (void)m_client.CreateListItems(&contactList);
        CVECIPtr<IVECIListItems> deleteContactList;
        (void)m_client.CreateListItems(&deleteContactList);
        // GetContactNodeInfo(contactNode, phoneme, deleteContactList, contactList);
        DeleteContactIDInfo(strDeviceAddress, deleteContactList);

        retCode = m_agentDataCommand->RemoveData(&spTrans, _T(""), _T("HFDAgentContacts"), deleteContactList);
        if (S_OK != retCode) {
            VR_LOGE("RemoveData: 0x%x", retCode);
            return false;
        }
        CVECIPtr<IVECIListItems> deleteContactNameList = GetAllVoiceTagInfo(strDeviceAddress);
        if (NULL == deleteContactNameList.ptr()) {
            return false;
        }
        ADDContactIDInfo(strDeviceAddress, contactList);
        retCode = m_agentDataCommand->AddData(&spTrans, _T(""), _T("HFDAgentContacts"), contactList);
        if (S_OK != retCode) {
            VR_LOGE("AddData: 0x%x", retCode);
            return false;
        }

        retCode = m_agentDataCommand->SetDataSynchronized(&spTrans, _T(""), _T("HFDAgentContacts"), _T("contacts"), VBT_TRUE);
        if (S_OK != retCode) {
            VR_LOGE("SetDataSynchronized: 0x%x", retCode);
            return false;
        }
        // retCode = m_agentDataCommand->RemoveGrammar(&spTrans, _T(""),  _T("HFDAgentContacts"), _T("contacts"), deleteContactNameList);
        // if (S_OK != retCode) {
        //     VR_LOGE("RemoveGrammar: 0x%x", retCode);
        //     return false;
        // }
        retCode = m_agentDataCommand->ReloadGrammar(&spTrans, _T(""),  _T("HFDAgentContacts"));
        if (S_OK != retCode) {
            VR_LOGE("ReloadGrammar: 0x%x", retCode);
            return false;
        }

        CVECIOutStr strTrans;
        retCode = spTrans->GetTransactionId(&strTrans);
        if (FAILED(retCode) || NULL == strTrans.Get()) {
            VR_LOGE("GetTransactionId: 0x%lx or strTrans.Get() is NULL", retCode);
            return false;
        }

        std::string strTransaction = strTrans.Get();
        m_engineCallback.SetCurrentTransaction("hfd", strTransaction);
        m_mapSaveTransactionId.insert(std::make_pair(strTransaction, "VoiceTag"));
    }
    else {
        VR_LOGI("the phone book not exit");
        int key = VR_VoiceTagIDManager::getInstance()->getVoiceTagStorageKey(strDeviceAddress);
        VR_VoiceBoxDataStorage storage;
        storage.DeleteValue(key);
        return false;
    }
    return true;
}

void
VR_VoiceBoxCatalogPhone::GetDeleteContactNameList(const std::string &deviceAddress, VoiceList<std::string>::type &nameList)
{
    VR_LOGD_FUNC();

    int key = VR_VoiceTagIDManager::getInstance()->getVoiceTagStorageKey(deviceAddress);

    std::string deviceVoiceTagValueStr;
    VR_VoiceBoxDataStorage storage;
    VR_LOGI("Delete VoiceTag Begin");
    storage.GetValue(key, deviceVoiceTagValueStr);
    VR_LOGI("VoiceTag Value: %s", deviceVoiceTagValueStr.c_str());
    pugi::xml_document deviceVoiceTagValueDoc;
    deviceVoiceTagValueDoc.load_string(deviceVoiceTagValueStr.c_str());

    pugi::xml_node voiceTagValueNode = deviceVoiceTagValueDoc.first_child();
    pugi::xml_node contactNode;
    VoiceList<std::string>::type voiceTagIDList;
    std::string tempContactName;
    while (!voiceTagValueNode.empty()) {

        std::string voiceTagID = voiceTagValueNode.name();
        VR_LOGI("voiceTagID: %s", voiceTagID.c_str());
        if (std::string::npos != voiceTagID.find(VR_VOICETAG_ID_PREFIX)) {
            voiceTagIDList.push_back(std::string(voiceTagValueNode.name()).substr(std::string(VR_VOICETAG_ID_PREFIX).size()));
            contactNode = voiceTagValueNode.child("contact");
            if (contactNode) {
                tempContactName = contactNode.attribute("name").as_string();
                if ("" == tempContactName) {
                    tempContactName = " ";
                }
                VR_LOGI("nameList name : %s", tempContactName.c_str());
                nameList.push_back(tempContactName);
            }
        }
        else {
            deviceVoiceTagValueDoc.remove_child(voiceTagID.c_str());
        }

        voiceTagValueNode = voiceTagValueNode.next_sibling();
    }
}

void
VR_VoiceBoxCatalogPhone::GetContactNameList(const std::string &deviceAddress, VoiceList<std::string>::type &nameList)
{
    VR_LOGD_FUNC();

    int key = VR_VoiceTagIDManager::getInstance()->getVoiceTagStorageKey(deviceAddress);

    std::string deviceVoiceTagValueStr;
    VR_VoiceBoxDataStorage storage;
    VR_LOGI("Delete VoiceTag Begin");
    storage.GetValue(key, deviceVoiceTagValueStr);
    VR_LOGI("VoiceTag Value: %s", deviceVoiceTagValueStr.c_str());
    pugi::xml_document deviceVoiceTagValueDoc;
    deviceVoiceTagValueDoc.load_string(deviceVoiceTagValueStr.c_str());

    pugi::xml_node voiceTagValueNode = deviceVoiceTagValueDoc.first_child();
    pugi::xml_node contactNode;
    VoiceList<std::string>::type voiceTagIDList;
    std::string tempContactName;
    while (!voiceTagValueNode.empty()) {

        std::string voiceTagID = voiceTagValueNode.name();
        VR_LOGI("voiceTagID: %s", voiceTagID.c_str());
        if (std::string::npos != voiceTagID.find(VR_VOICETAG_ID_PREFIX)) {
            voiceTagIDList.push_back(std::string(voiceTagValueNode.name()).substr(std::string(VR_VOICETAG_ID_PREFIX).size()));
            contactNode = voiceTagValueNode.child("contact");
            if (contactNode) {
                tempContactName = contactNode.attribute("name").as_string();
                if ("" == tempContactName) {
                    tempContactName = " ";
                }
                VR_LOGI("nameList name : %s", tempContactName.c_str());
                nameList.push_back(tempContactName);
            }
        }
        else {
            deviceVoiceTagValueDoc.remove_child(voiceTagID.c_str());
        }

        voiceTagValueNode = voiceTagValueNode.next_sibling();
    }
    deviceVoiceTagValueStr.clear();
    storage.DeleteValue(key);
}

void
VR_VoiceBoxCatalogPhone::DeleteContactIDInfo(const std::string &strDeviceAddress,
    CVECIPtr<IVECIListItems>& deleteContactList)
{
    VR_LOGD_FUNC();

    VoiceList<std::string>::type nameList;
    GetDeleteContactNameList(strDeviceAddress, nameList);

    for (VoiceList<std::string>::iterator iter = nameList.begin(); iter != nameList.end(); ++iter) {
        for (size_t i = 0; i < m_vecContact.size(); ++i) {
            CVECIPtr<IVECIParameterSet> spContact;
            CVECICStr cFullName(&m_client);
            PersonDetailInfo perInfo = m_vecContact[i];

            cFullName.Format(_T("%s %s"), perInfo.firstName.c_str(), perInfo.lastName.c_str());
            std::string contactName = perInfo.firstName + " " + perInfo.lastName;
            std::string strFullName = *iter;
            if (strFullName == contactName) {
                VR_LOGI("equal");
                (void)m_client.CreateParameterSet(&spContact);
                if (NULL == spContact) {
                    return;
                }
                VR_LOGI("strId : %s", perInfo.strId.c_str());
                (void)spContact->AddParameter(_T("nId"), perInfo.strId.c_str(), NULL, NULL);
                (void)deleteContactList->AddItem(spContact);
                break;
            }
        }
    }
}

CVECIPtr<IVECIListItems>
VR_VoiceBoxCatalogPhone::GetAllVoiceTagInfo(const std::string &strDeviceAddress)
{
    VR_LOGD_FUNC();

    CVECIPtr<IVECIListItems> deleteContactNameList;
    (void)m_client.CreateListItems(&deleteContactNameList);
    if (NULL == deleteContactNameList.ptr()) {
        return deleteContactNameList;
    }
    VoiceList<std::string>::type nameList;
    GetDeleteContactNameList(strDeviceAddress, nameList);

    for (VoiceList<std::string>::iterator iter = nameList.begin(); iter != nameList.end(); ++iter) {
        for (size_t i = 0; i < m_vecContact.size(); ++i) {
            CVECIPtr<IVECIParameterSet> spContact;
            CVECICStr cFullName(&m_client);
            PersonDetailInfo perInfo = m_vecContact[i];

            cFullName.Format(_T("%s %s"), perInfo.firstName.c_str(), perInfo.lastName.c_str());
            std::string contactName = perInfo.firstName + " " + perInfo.lastName;
            std::string strFullName = *iter;
            if (strFullName == contactName) {
                (void)m_client.CreateParameterSet(&spContact);
                if (NULL == spContact.ptr()) {
                    return deleteContactNameList;
                }
                VR_LOGI("strId : %s", perInfo.strId.c_str());
                (void)spContact->AddParameter(_T("nId"), perInfo.strId.c_str(), NULL, NULL);
                (void)spContact->AddParameter(_T("cFullName"), cFullName.Get(), NULL, NULL);
                (void)deleteContactNameList->AddItem(spContact);
                break;
            }
        }
    }

    return deleteContactNameList;
}

void
VR_VoiceBoxCatalogPhone::ADDContactIDInfo(const std::string &strDeviceAddress,
    CVECIPtr<IVECIListItems>& contactList)
{
    VR_LOGD_FUNC();

    VoiceList<std::string>::type nameList;
    GetContactNameList(strDeviceAddress, nameList);

    for (size_t i = 0; i < m_vecContact.size(); ++i) {
        CVECIPtr<IVECIParameterSet> spContact;
        CVECICStr cFullName(&m_client);
        PersonDetailInfo perInfo = m_vecContact[i];

        cFullName.Format(_T("%s %s"), perInfo.firstName.c_str(), perInfo.lastName.c_str());
        std::string contactName = perInfo.firstName + " " + perInfo.lastName;
        for (VoiceList<std::string>::iterator iter = nameList.begin(); iter != nameList.end(); ++iter) {
            std::string strFullName = *iter;
            if (strFullName == contactName) {
                VR_LOGI("equal");
                (void)m_client.CreateParameterSet(&spContact);
                if (NULL == spContact) {
                    return;
                }
                VR_LOGI("strId : %s", perInfo.strId.c_str());
                (void)spContact->AddParameter(_T("nId"), perInfo.strId.c_str(), NULL, NULL);
                (void)spContact->AddParameter(_T("cFirstName"), perInfo.firstName.c_str(), NULL, NULL);
                (void)spContact->AddParameter(_T("cLastName"), perInfo.lastName.c_str(), NULL, NULL);
                (void)spContact->AddParameter(_T("cFullName"), cFullName.Get(), NULL, NULL);
                (void)spContact->AddParameter(_T("nPhoneType"), perInfo.phonetype.c_str(), NULL, NULL);
                (void)spContact->AddParameter(_T("cNumber"), perInfo.number.c_str(), NULL, NULL);
                (void)spContact->AddParameter(_T("cVoiceTag"), "", NULL, NULL);
                (void)contactList->AddItem(spContact);
            }
        }
    }
    return;
}

void
VR_VoiceBoxCatalogPhone::GetContactNodeInfo(std::string strFullName,
    CVECIPtr<IVECIListItems>& deleteContactList)
{
    VR_LOGD_FUNC();

    if ("" == strFullName) {
        strFullName = " ";
    }
    VR_LOGI("strFullName: %s", strFullName.c_str());
    // pugi::xml_node phoneItemNode = contactNode.first_child();
    for (size_t i = 0; i < m_vecContact.size(); ++i) {
        CVECIPtr<IVECIParameterSet> spContact;
        CVECICStr cFullName(&m_client);
        PersonDetailInfo perInfo = m_vecContact[i];

        cFullName.Format(_T("%s %s"), perInfo.firstName.c_str(), perInfo.lastName.c_str());
        std::string contactName = perInfo.firstName + " " + perInfo.lastName;
        // std::string strFullName = cFullName.Get();

        if (strFullName == contactName) {
            VR_LOGI("equal");
            (void)m_client.CreateParameterSet(&spContact);
            if (NULL == spContact) {
                return;
            }
            VR_LOGI("strId : %s", perInfo.strId.c_str());
            (void)spContact->AddParameter(_T("nId"), perInfo.strId.c_str(), NULL, NULL);
            (void)deleteContactList->AddItem(spContact);
            // (void)contactList->AddItem(spContact);
            return;
        }
    }
    return;
}

CVECIPtr<IVECIListItems>
VR_VoiceBoxCatalogPhone::GetOneVoiceTagInfo(std::string strFullName)
{
    VR_LOGD_FUNC();

    CVECIPtr<IVECIListItems> deleteContactGrammarList;
    (void)m_client.CreateListItems(&deleteContactGrammarList);
    if (NULL == deleteContactGrammarList.ptr()) {
        return deleteContactGrammarList;
    }
    if ("" == strFullName) {
        VR_LOGI("the name is null");
        strFullName = " ";
    }
    VR_LOGI("strFullName: %s", strFullName.c_str());
    for (size_t i = 0; i < m_vecContact.size(); ++i) {
        CVECIPtr<IVECIParameterSet> spContact;
        CVECICStr cFullName(&m_client);
        PersonDetailInfo perInfo = m_vecContact[i];

        cFullName.Format(_T("%s %s"), perInfo.firstName.c_str(), perInfo.lastName.c_str());
        std::string contactName = perInfo.firstName + " " + perInfo.lastName;
        if (strFullName == contactName) {
            (void)m_client.CreateParameterSet(&spContact);
            if (NULL == spContact.ptr()) {
                return deleteContactGrammarList;
            }
            VR_LOGI("strId : %s", perInfo.strId.c_str());
            (void)spContact->AddParameter(_T("nId"), perInfo.strId.c_str(), NULL, NULL);
            (void)spContact->AddParameter(_T("cFullName"), cFullName.Get(), NULL, NULL);
            (void)deleteContactGrammarList->AddItem(spContact);
            return deleteContactGrammarList;
        }
    }
    return deleteContactGrammarList;
}

void
VR_VoiceBoxCatalogPhone::GetContactNodeInfo(std::string strFullName, const std::string &phoneme,
    CVECIPtr<IVECIListItems>& contactList)
{
    VR_LOGD_FUNC();

    if ("" == strFullName) {
        strFullName = " ";
    }
    VR_LOGI("strFullName: %s", strFullName.c_str());
    VR_LOGI("phoneme: %s", phoneme.c_str());
    // pugi::xml_node phoneItemNode = contactNode.first_child();
    for (size_t i = 0; i < m_vecContact.size(); ++i) {
        CVECIPtr<IVECIParameterSet> spContact;
        CVECICStr cFullName(&m_client);
        PersonDetailInfo perInfo = m_vecContact[i];

        cFullName.Format(_T("%s %s"), perInfo.firstName.c_str(), perInfo.lastName.c_str());
        std::string contactName = perInfo.firstName + " " + perInfo.lastName;
        // std::string strFullName = cFullName.Get();
        if (strFullName == contactName) {
            VR_LOGI("equal");
            (void)m_client.CreateParameterSet(&spContact);
            if (NULL == spContact) {
                return;
            }
            VR_LOGI("strId : %s", perInfo.strId.c_str());
            (void)spContact->AddParameter(_T("nId"), perInfo.strId.c_str(), NULL, NULL);
            (void)spContact->AddParameter(_T("cFirstName"), perInfo.firstName.c_str(), NULL, NULL);
            (void)spContact->AddParameter(_T("cLastName"), perInfo.lastName.c_str(), NULL, NULL);
            (void)spContact->AddParameter(_T("cFullName"), cFullName.Get(), NULL, NULL);
            (void)spContact->AddParameter(_T("nPhoneType"), perInfo.phonetype.c_str(), NULL, NULL);
            (void)spContact->AddParameter(_T("cNumber"), perInfo.number.c_str(), NULL, NULL);
            (void)spContact->AddParameter(_T("cVoiceTag"), phoneme.c_str(), NULL, NULL);
            (void)contactList->AddItem(spContact);
        }
    }
    return;
}

VoiceVector<PersonDetailInfo>::type
VR_VoiceBoxCatalogPhone::GetContactInfo()
{
    return m_vecContact;
}

void
VR_VoiceBoxCatalogPhone::GetVoiceTagNodeInfo(VoiceList<std::string>::type NameList,
    CVECIPtr<IVECIListItems>& contactList)
{
    VR_LOGD_FUNC();

    for (size_t i = 0; i < m_vecContact.size(); ++i) {
        CVECIPtr<IVECIParameterSet> spContact;
        CVECICStr cFullName(&m_client);
        PersonDetailInfo perInfo = m_vecContact[i];

        cFullName.Format(_T("%s %s"), perInfo.firstName.c_str(), perInfo.lastName.c_str());
        std::string contactName = perInfo.firstName + perInfo.lastName;

        VoiceList<std::string>::const_iterator it;
        std::string tempName;
        for (it = NameList.cbegin(); it != NameList.cend(); ++it) {
            tempName = (*it);
            if (tempName == contactName) {
                VR_LOGI("equal");
                (void)m_client.CreateParameterSet(&spContact);
                if (NULL == spContact) {
                    return;
                }
                (void)spContact->AddParameter(_T("nId"), perInfo.strId.c_str(), NULL, NULL);
                (void)spContact->AddParameter(_T("cFirstName"), perInfo.firstName.c_str(), NULL, NULL);
                (void)spContact->AddParameter(_T("cLastName"), perInfo.lastName.c_str(), NULL, NULL);
                (void)spContact->AddParameter(_T("cFullName"), cFullName.Get(), NULL, NULL);
                (void)spContact->AddParameter(_T("nPhoneType"), perInfo.phonetype.c_str(), NULL, NULL);
                (void)spContact->AddParameter(_T("cNumber"), perInfo.number.c_str(), NULL, NULL);
                (void)spContact->AddParameter(_T("cVoiceTag"), "", NULL, NULL);
                (void)contactList->AddItem(spContact);
            }
        }
    }

    return;
}

void
VR_VoiceBoxCatalogPhone::LoadContactInfo(const VoiceVector<PersonDetailInfo>::type& vecContact,
    CVECIPtr<IVECIListItems>& contactList)
{
    VR_LOGD_FUNC();

    VoiceMap<std::string, std::string>::type mapNametoPhoneme;
    // VoiceVector<StructNode>::type tempVector;
    // grammar
    VR_LOGI("m_deviceAddress : %s", m_deviceAddress.c_str());
    std::string deviceVoiceTagValueStr;
    VR_VoiceBoxDataStorage storage;
    int key = 0;
    if ("" != m_deviceAddress) {
        key = VR_VoiceTagIDManager::getInstance()->getVoiceTagStorageKey(m_deviceAddress);
        VR_LOGI("key: %d", key);
        storage.GetValue(key, deviceVoiceTagValueStr);
    }
    VR_LOGI("VoiceTag Value: %s", deviceVoiceTagValueStr.c_str());
    pugi::xml_document deviceVoiceTagValueDoc;
    deviceVoiceTagValueDoc.load_string(deviceVoiceTagValueStr.c_str());

    pugi::xml_node voiceTagValueNode = deviceVoiceTagValueDoc.first_child();
    std::string phoneme;
    std::string tempData;
    std::string tempContactName;
    pugi::xml_node contactNode;
    while (!voiceTagValueNode.empty()) {
        phoneme.clear();
        tempData.clear();
        tempContactName.clear();

        std::string voiceTagID = voiceTagValueNode.name();
        VR_LOGI("voiceTagID: %s", voiceTagID.c_str());
        if (std::string::npos != voiceTagID.find(VR_VOICETAG_ID_PREFIX)) {
            contactNode = voiceTagValueNode.child("contact");
            if (contactNode) {
                tempContactName = contactNode.attribute("name").as_string();
                if ("" == tempContactName) {
                    tempContactName = " ";
                }
            }
            if (voiceTagValueNode.child(VR_VOICETAG_TEMPDATA)) {
                tempData = voiceTagValueNode.child(VR_VOICETAG_TEMPDATA).text().as_string();
            }
            VR_LOGI("tempContactName : %s", tempContactName.c_str());
            VR_LOGI("tempData : %s", tempData.c_str());
            if (voiceTagValueNode.child(VR_VOICETAG_PHONEME_NODE)) {
                phoneme = voiceTagValueNode.child(VR_VOICETAG_PHONEME_NODE).text().as_string();
            }
            VR_LOGI("phoneme : %s", phoneme.c_str());
            if ("false" == tempData) {
                mapNametoPhoneme.insert(std::make_pair(tempContactName, phoneme));
            }
        }
        else {
            deviceVoiceTagValueDoc.remove_child(voiceTagID.c_str());
        }
        voiceTagValueNode = voiceTagValueNode.next_sibling();
    }
    std::ostringstream oss;
    deviceVoiceTagValueDoc.print(oss);
    deviceVoiceTagValueStr = oss.str();
    VR_LOGI("new voicetag: %s", deviceVoiceTagValueStr.c_str());
    storage.PutValue(key, deviceVoiceTagValueStr);


    std::string contactName;
    for (size_t i = 0; i < vecContact.size(); ++i) {
        CVECIPtr<IVECIParameterSet> spContact;
        CVECICStr cFullName(&m_client);
        PersonDetailInfo perInfo = vecContact[i];

        cFullName.Format(_T("%s %s"), perInfo.firstName.c_str(), perInfo.lastName.c_str());
        contactName = perInfo.firstName + " " + perInfo.lastName;
        (void)m_client.CreateParameterSet(&spContact);
        if (NULL == spContact) {
            return;
        }
        (void)spContact->AddParameter(_T("nId"), perInfo.strId.c_str(), NULL, NULL);
        (void)spContact->AddParameter(_T("cFirstName"), perInfo.firstName.c_str(), NULL, NULL);
        (void)spContact->AddParameter(_T("cLastName"), perInfo.lastName.c_str(), NULL, NULL);
        (void)spContact->AddParameter(_T("cFullName"), cFullName.Get(), NULL, NULL);
        (void)spContact->AddParameter(_T("nPhoneType"), perInfo.phonetype.c_str(), NULL, NULL);
        (void)spContact->AddParameter(_T("cNumber"), perInfo.number.c_str(), NULL, NULL);
        for (VoiceMap<std::string, std::string>::iterator it = mapNametoPhoneme.begin();
            it != mapNametoPhoneme.end(); ++it) {
            if (contactName == it->first) {
                VR_LOGI("equal");
                (void)spContact->AddParameter(_T("cVoiceTag"), (it->second).c_str(), NULL, NULL);
                break;
            }
        }
        (void)contactList->AddItem(spContact);
    }

    return;
}

void
VR_VoiceBoxCatalogPhone::LoadMessageInfo(const VoiceVector<FormalTwoItem>::type& vecMessageInfo,
    CVECIPtr<IVECIListItems>& messageInfoList)
{
    VR_LOGD_FUNC();

    for (size_t i = 0; i < vecMessageInfo.size(); ++i) {
        CVECIPtr<IVECIParameterSet> spMessageType;
        (void)m_client.CreateParameterSet(&spMessageType);

        (void)spMessageType->AddParameter(_T("nId"), vecMessageInfo[i].strId.c_str(), NULL, NULL);
        (void)spMessageType->AddParameter(_T("value"), vecMessageInfo[i].name.c_str(), NULL, NULL);
        (void)messageInfoList->AddItem(spMessageType);
    }

    return;
}

void
VR_VoiceBoxCatalogPhone::LoadMsgOrPhoneType(const VoiceVector<FormalInfo>::type& vecMsgorPhoneType,
    CVECIPtr<IVECIListItems>& msgorPhoneTypeList)
{
    VR_LOGD_FUNC();

    for (size_t i = 0; i < vecMsgorPhoneType.size(); ++i) {

        for (size_t index = 0; index < vecMsgorPhoneType[i].aliasVector.size(); ++index) {
            CVECIPtr<IVECIParameterSet> spType;
            (void)m_client.CreateParameterSet(&spType);

            (void)spType->AddParameter(_T("nId"), vecMsgorPhoneType[i].formalItem.strId.c_str(), NULL, NULL);
            (void)spType->AddParameter(_T("cFormal"), vecMsgorPhoneType[i].formalItem.name.c_str(), NULL, NULL);
            (void)spType->AddParameter(_T("cAlias"), vecMsgorPhoneType[i].aliasVector[index].c_str(), NULL, NULL);
            (void)msgorPhoneTypeList->AddItem(spType);
        }
    }

    return;
}

bool
VR_VoiceBoxCatalogPhone::ContactGrammer(const VoiceVector<PersonDetailInfo>::type& vecPersonInfo)
{
    VR_LOGD_FUNC();
    VR_LOGP("DE: update phone grammar... case : 212-138 212-140");


    if (!vecPersonInfo.empty()) {
        CVECIPtr<IVECIListItems> contactList;
        (void)m_client.CreateListItems(&contactList);
        LoadContactInfo(vecPersonInfo, contactList);
        return UpdateHFDGrammer("HFDAgentContacts", contactList, "true");
    }
    else {
        VR_LOGI("the phonebook is null");
        std::string strGrammar = GetGrammarName();
        m_engineCallback.SendGrammarResult(strGrammar, "phone", "1", "0");
        return false;
    }

    return true;
}

bool
VR_VoiceBoxCatalogPhone::UpdateHFDQuickReplyMessages(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    VoiceVector<FormalTwoItem>::type vecMessageInfo;
    parser.getMessageInfo(vecMessageInfo);
    std::string strLastFlg = parser.getValueByKey("isLast");

    VR_LOGP("DE: update phone grammar... case : 212-138 212-140");
    if (!vecMessageInfo.empty()) {
        CVECIPtr<IVECIListItems> messageInfoList;
        (void)m_client.CreateListItems(&messageInfoList);
        LoadMessageInfo(vecMessageInfo, messageInfoList);

        return UpdateHFDGrammer("HFDAgentQuickReplyMessages", messageInfoList, strLastFlg);
    }
    else {
        return RemoveGrammer("HFDAgentQuickReplyMessages", strLastFlg);
    }
    return true;
}

bool
VR_VoiceBoxCatalogPhone::UpdateHFDPhoneTypes(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    VoiceVector<FormalInfo>::type vecPhoneType;
    parser.getPhoneType(vecPhoneType);
    std::string strLastFlg = parser.getValueByKey("isLast");

    VR_LOGP("DE: update phone grammar... case : 212-138 212-140");
    if (!vecPhoneType.empty()) {
        CVECIPtr<IVECIListItems> phoneTypeList;
        (void)m_client.CreateListItems(&phoneTypeList);
        LoadMsgOrPhoneType(vecPhoneType, phoneTypeList);

        return UpdateHFDGrammer("HFDAgentPhoneTypes", phoneTypeList, strLastFlg);
    }
    else {
        return RemoveGrammer("HFDAgentPhoneTypes", strLastFlg);
    }
    return true;
}

bool
VR_VoiceBoxCatalogPhone::UpdateHFDMessageTypes(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    VoiceVector<FormalInfo>::type vecMessageType;
    parser.getMessageType(vecMessageType);
    std::string strLastFlg = parser.getValueByKey("isLast");

    VR_LOGP("DE: update phone grammar... case : 212-138 212-140");


    if (!vecMessageType.empty()) {
        CVECIPtr<IVECIListItems> messageTypeList;
        (void)m_client.CreateListItems(&messageTypeList);
        LoadMsgOrPhoneType(vecMessageType, messageTypeList);

        return UpdateHFDGrammer("HFDAgentMessageTypes", messageTypeList, strLastFlg);
    }
    else {
        return RemoveGrammer("HFDAgentMessageTypes", strLastFlg);
    }
    return true;
}

bool
VR_VoiceBoxCatalogPhone::UpdateHFDGrammer(
    const std::string& strTableName, const CVECIPtr<IVECIListItems>& sourceList, const std::string& strLastFlg)
{
    VR_LOGD_FUNC();

    if (NULL == m_agentDataCommand.ptr() || "" == strTableName) {
        VR_LOGE("m_agentDataCommand is NULL");
        return false;
    }
    VR_LOGI("strTableName: %s", strTableName.c_str());

    HRESULT retCode = m_agentDataCommand->SetDataSynchronized(NULL, _T(""), strTableName.c_str(), _T(""), VBT_FALSE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    retCode = m_agentDataCommand->ReloadData(NULL, _T(""), strTableName.c_str(), sourceList);
    if (S_OK != retCode) {
        VR_LOGE("ReloadData: 0x%lx", retCode);
        return false;
    }

    CVECIPtr<IVECITransaction> spDataSyncTrans;
    retCode = m_agentDataCommand->SetDataSynchronized(&spDataSyncTrans, _T(""), strTableName.c_str(), _T(""), VBT_TRUE);
    if (S_OK != retCode || NULL == spDataSyncTrans.ptr()) {
        VR_LOGE("SetDataSynchronized: 0x%lx or spDataSyncTrans is NULL", retCode);
        return false;
    }

    CVECIOutStr strDataSyncTrans;
    retCode = spDataSyncTrans->GetTransactionId(&strDataSyncTrans);
    if (FAILED(retCode) || NULL == strDataSyncTrans.Get()) {
        VR_LOGE("GetTransactionId: 0x%lx or spDataSyncTrans.Get() is NULL", retCode);
        return false;
    }

    std::string strDataSyncTransation = strDataSyncTrans.Get();

    if ("HFDAgentContacts" == strTableName) {
        m_engineCallback.SetPhoneDataSyncTransId(strDataSyncTransation);
        m_engineCallback.PushFrontMessage("<grammar_reloadgrammar agent=\"phone\" name=\"contacts\"/>");
    }
    else {
        CVECIPtr<IVECITransaction> spTrans;
        retCode = m_agentDataCommand->ReloadGrammar(&spTrans, _T(""),  strTableName.c_str());
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
        m_engineCallback.SetCurrentTransaction("hfd", strTransaction);

        VR_LOGI("grammar : %s", strTransaction.c_str());
    }

    return true;
}

bool
VR_VoiceBoxCatalogPhone::UpdateHFDAgentContacts()
{
    VR_LOGD_FUNC();

    if (NULL == m_agentDataCommand.ptr()) {
        VR_LOGE("m_agentDataCommand is NULL");
        return false;
    }

    CVECIPtr<IVECITransaction> spTrans;
    HRESULT retCode = m_agentDataCommand->ReloadGrammar(&spTrans, _T(""),  "HFDAgentContacts");
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

    std::string strGrammar = GetGrammarName();
    VR_LOGI("strGrammar : %s", strGrammar.c_str());
    m_mapSaveTransactionId.insert(std::make_pair(strTransaction, strGrammar));
    m_engineCallback.SetCurrentTransaction("hfd", strTransaction);

    VR_LOGI("grammar : %s", strTransaction.c_str());

    return true;
}

bool
VR_VoiceBoxCatalogPhone::RemoveGrammer(const std::string& strTableName, const std::string& strLastFlg)
{
    VR_LOGD_FUNC();

    if (NULL == m_agentDataCommand.ptr() || "" == strTableName) {
        VR_LOGE("m_agentDataCommand is NULL");
        return false;
    }
    VR_LOGI("strTableName: %s", strTableName.c_str());

    HRESULT retCode = m_agentDataCommand->SetDataSynchronized(NULL, _T(""), strTableName.c_str(), _T(""), VBT_FALSE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    retCode = m_agentDataCommand->RemoveAllData(NULL, _T(""), strTableName.c_str());
    if (S_OK != retCode) {
        VR_LOGE("RemoveAllData: 0x%lx", retCode);
        return false;
    }

    retCode = m_agentDataCommand->SetDataSynchronized(NULL, _T(""), strTableName.c_str(), _T(""), VBT_TRUE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    CVECIPtr<IVECITransaction> spTrans;
    retCode = m_agentDataCommand->RemoveAllGrammars(&spTrans, _T(""), strTableName.c_str());
    if (S_OK != retCode || NULL == spTrans.ptr()) {
        VR_LOGE("RemoveAllGrammars: 0x%lx or spTrans is NULL", retCode);
        return false;
    }

    CVECIOutStr strTrans;
    retCode = spTrans->GetTransactionId(&strTrans);
    if (FAILED(retCode) || NULL == strTrans.Get()) {
        VR_LOGE("GetTransactionId: 0x%lx or strTrans.Get() is NULL", retCode);
        return false;
    }

    std::string strTransaction = strTrans.Get();

    if (!m_isDisactiveFromDE) {
        if ("HFDAgentContacts" == strTableName) {
            std::string strGrammar = GetGrammarName();
            VR_LOGI("strGrammar : %s", strGrammar.c_str());
            m_mapSaveTransactionId.insert(std::make_pair(strTransaction, strGrammar));
        }
    }
    m_engineCallback.SetCurrentTransaction("hfd", strTransaction);
    VR_LOGI("grammar : %s", strTransaction.c_str());

    return true;
}

bool
VR_VoiceBoxCatalogPhone::SetupPhoneBookData(
    VR_VoiceBoxXmlParser& parser
    )
{
    std::string path = parser.getValueByKey("path");
    m_strGrammar = parser.getXmlKey();
    VR_VoiceBoxPhoneData phoneData;
    m_vecContact.clear();
    phoneData.OpenPhoneBookDB(path, m_vecContact);
    return ContactGrammer(m_vecContact);
}

bool
VR_VoiceBoxCatalogPhone::PhoneGrammarDisActive(
    VR_VoiceBoxXmlParser& parser
    )
{
    m_strGrammar = parser.getXmlKey();
    std::string sender = parser.getValueByKey("sender");
    if ("DE" == sender) {
        m_isDisactiveFromDE = true;
    }
    else {
        m_isDisactiveFromDE = false;
    }
    m_vecContact.clear();
    return RemoveGrammer("HFDAgentContacts", "true");
}

void
VR_VoiceBoxCatalogPhone::InitialHFDPersonData()
{
    VR_LOGD_FUNC();

    m_isDisactiveFromDE = true;
    m_vecContact.clear();

    if (NULL == m_agentDataCommand.ptr()) {
        VR_LOGE("m_agentDataCommand is NULL");
        return;
    }

    HRESULT retCode = m_agentDataCommand->SetDataSynchronized(NULL, _T(""), "HFDAgentContacts", _T(""), VBT_FALSE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return;
    }
    retCode = m_agentDataCommand->RemoveAllData(NULL, _T(""), "HFDAgentContacts");
    if (S_OK != retCode) {
        VR_LOGE("RemoveAllData: 0x%lx", retCode);
        return;
    }
    retCode = m_agentDataCommand->SetDataSynchronized(NULL, _T(""), "HFDAgentContacts", _T(""), VBT_TRUE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return;
    }
    CVECIPtr<IVECITransaction> spTrans;
    retCode = m_agentDataCommand->RemoveAllGrammars(&spTrans, _T(""), "HFDAgentContacts");
    if (S_OK != retCode || NULL == spTrans.ptr()) {
        VR_LOGE("RemoveAllGrammars: 0x%lx or spTrans is NULL", retCode);
        return;
    }

    CVECIOutStr strTrans;
    retCode = spTrans->GetTransactionId(&strTrans);
    if (FAILED(retCode) || NULL == strTrans.Get()) {
        VR_LOGE("GetTransactionId: 0x%lx or strTrans.Get() is NULL", retCode);
        return;
    }

    std::string strTransaction = strTrans.Get();
    m_engineCallback.SetCurrentTransaction("hfd", strTransaction);

    VBT_BOOL bTimeout = VBT_FALSE;
    const long OVER_TIME = 20000;
    retCode = spTrans->WaitForCompletion(OVER_TIME, &bTimeout);
    if (FAILED(retCode) || bTimeout) {
        VR_LOGE("WaitForCompletion failed: %lx or RemoveAllGrammars over time", retCode);
        return;
    }

    m_engineCallback.SetUpdateGammarFlg(true);

    return;
}

VoiceMap<std::string, std::string>::type
VR_VoiceBoxCatalogPhone::GetTransactionIdMap()
{
    VR_LOGD_FUNC();

    return m_mapSaveTransactionId;
}

void
VR_VoiceBoxCatalogPhone::SetTransactionIdMap(VoiceMap<std::string, std::string>::type mapTransactionId)
{
    VR_LOGD_FUNC();

    m_mapSaveTransactionId = mapTransactionId;
}

void
VR_VoiceBoxCatalogPhone::SetPersonInfo(VoiceVector<PersonDetailInfo>::type vecContact)
{
    VR_LOGD_FUNC();
    m_vecContact.clear();
    m_vecContact = vecContact;
}

std::string
VR_VoiceBoxCatalogPhone::GetGrammarName()
{
    std::string strGrammar;
    if ("grammar_active" == m_strGrammar) {
        strGrammar = "active";
    }
    else if ("grammar_disactive" == m_strGrammar) {
        strGrammar = "disactive";
    }
    else if ("grammar_init" == m_strGrammar) {
        strGrammar = "grammar";
    }
    else {
        strGrammar = m_strGrammar;
    }
    return strGrammar;
}

void
VR_VoiceBoxCatalogPhone::SetDeviceAddress(const std::string deviceAddress)
{
    VR_LOGD_FUNC();

    VR_LOGI("deviceAddress: %s", deviceAddress.c_str());
    this->m_deviceAddress = deviceAddress;
}

void
VR_VoiceBoxCatalogPhone::PauseGrammarPhone()
{
    VR_LOGD_FUNC();
    if (NULL == m_agentDataCommand.ptr()) {
        VR_LOGE("m_agentDataCommand is NULL");
        return;
    }

    std::pair<std::string, std::string> pairAgent2TransId = m_engineCallback.GetCurrentTransaction();
    if (pairAgent2TransId.first.empty()) {
        VR_LOGI("grammar has updated finish");
        return;
    }

    VR_LOGI("PauseGrammarUpdate of phone, first = %s, second = %s", pairAgent2TransId.first.c_str(), pairAgent2TransId.second.c_str());

    CVECIPtr<IVECITransaction> spTrans;
    CVECIOutStr strTrans;
    HRESULT retCode =  m_agentDataCommand->PauseGrammarUpdate(&spTrans);
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

    retCode = spTrans->GetTransactionId(&strTrans);
    if (FAILED(retCode) || NULL == strTrans.Get()) {
        VR_LOGE("GetTransactionId: 0x%lx or strTrans.Get() is NULL", retCode);
        return;
    }

    std::string strTransaction = strTrans.Get();
    VR_LOGI("strTransaction = %s", strTransaction.c_str());
}

void
VR_VoiceBoxCatalogPhone::ResumeGrammarPhone()
{
    VR_LOGD_FUNC();
    if (NULL == m_agentDataCommand.ptr()) {
        VR_LOGE("m_agentDataCommand is NULL");
        return;
    }
    CVECIPtr<IVECITransaction> spTrans;
    CVECIOutStr strTrans;
    HRESULT retCode = m_agentDataCommand->ResumeGrammarUpdate(&spTrans);
    if (FAILED(retCode)  || NULL == spTrans.ptr()) {
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
VR_VoiceBoxCatalogPhone::CancelGrammarUpdate(bool bSync)
{
    if (NULL == m_agentDataCommand.ptr()) {
        VR_LOGE("m_agentDataCommand is NULL");
        return;
    }

    CVECIPtr<IVECITransaction> spTrans;
    CVECIOutStr strTrans;
    HRESULT retCode = m_agentDataCommand->CancelGrammarUpdate(&spTrans);
    if (FAILED(retCode) || NULL == spTrans.ptr()) {
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

    retCode = spTrans->GetTransactionId(&strTrans);
    if (FAILED(retCode) || NULL == strTrans.Get()) {
        VR_LOGE("GetTransactionId: 0x%lx or strTrans.Get() is NULL", retCode);
        return;
    }

    std::string strTransaction = strTrans.Get();
    VR_LOGI("strTransaction = %s", strTransaction.c_str());
}
/* EOF */
