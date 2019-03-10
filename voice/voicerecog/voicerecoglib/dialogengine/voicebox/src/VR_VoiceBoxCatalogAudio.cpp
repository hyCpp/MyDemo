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
#ifndef VR_VOICEBOXCATALOGAUDIO_H
#    include "VR_VoiceBoxCatalogAudio.h"
#endif

#ifndef VR_VOICEBOXXMLPARSER_H
#    include "VR_VoiceBoxXmlParser.h"
#endif

#ifndef VR_VOICEBOXLOG_H
#    include "VR_VoiceBoxLog.h"
#endif

#ifndef VR_CONFIGUREIF_H
#    include "VR_ConfigureIF.h"
#endif

using namespace nutshell;

std::string VR_VoiceBoxCatalogAudio::m_strSourceId = "";
std::string VR_VoiceBoxCatalogAudio::m_strDBPath = "";
bool VR_VoiceBoxCatalogAudio::m_bIPodUSB2 = false;
bool VR_VoiceBoxCatalogAudio::m_bUSBIPod2 = false;
bool VR_VoiceBoxCatalogAudio::m_bIpodActive = false;

VR_VoiceBoxCatalogAudio::VR_VoiceBoxCatalogAudio(
    IVECIEngineClient& client,
    IVECIEngineCommand& engineCommand,
    VR_VoiceBoxEngineCallback& engineCallback
    )
: m_client(client)
, m_engineCommand(engineCommand)
, m_engineCallback(engineCallback)
{
}

VR_VoiceBoxCatalogAudio::~VR_VoiceBoxCatalogAudio()
{
}

bool
VR_VoiceBoxCatalogAudio::Initialize()
{
    HRESULT result = m_client.CreateAgentDispatcher(&m_agentDataCommandMusic);
    if (S_OK != result) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "CreateAgentDispatcher: 0x%lx", result);
        return false;
    }
    if (NULL == m_agentDataCommandMusic.ptr()) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, 0, "m_agentDataCommandMusic is NULL");
        return false;
    }

    result = m_client.CreateAgentDispatcher(&m_agentDataCommandRadio);
    if (S_OK != result) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "CreateAgentDispatcher: 0x%lx", result);
        return false;
    }
    if (NULL == m_agentDataCommandRadio.ptr()) {
       VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, 0, "m_agentDataCommandRadio is NULL");
        return false;
    }

    result = m_agentDataCommandMusic->Init(VBT_AGENT_MUSIC);
    if (S_OK != result) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "Init Music: 0x%lx", result);
        return false;
    }

    result = m_agentDataCommandRadio->Init(VBT_AGENT_RADIO);
    if (S_OK != result) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, result, "Init Radio: 0x%lx", result);
        return false;
    }

    m_mapUSENAudioSource.clear();
    m_mapUSESAudioSource.clear();
    m_mapUSFRAudioSource.clear();

    m_mapUSENAudioSource["AM"].push_back("AM");
    m_mapUSENAudioSource["AM"].push_back("AM Radio");
    m_mapUSENAudioSource["FM"].push_back("FM");
    m_mapUSENAudioSource["FM"].push_back("FM Radio");
    m_mapUSENAudioSource["FM"].push_back("Radio");
    m_mapUSENAudioSource["XM"].push_back("XM");
    m_mapUSENAudioSource["XM"].push_back("Syrius");
    m_mapUSENAudioSource["XM"].push_back("Syrius XM");
    m_mapUSENAudioSource["XM"].push_back("Sat");
    m_mapUSENAudioSource["XM"].push_back("Satellite");
    m_mapUSENAudioSource["XM"].push_back("Satellite Radio");
    m_mapUSENAudioSource["CD"].push_back("CD");
    m_mapUSENAudioSource["CD"].push_back("CD Player");
    m_mapUSENAudioSource["CD"].push_back("CD Changer");
    m_mapUSENAudioSource["CD"].push_back("Disc");
    m_mapUSENAudioSource["CD"].push_back("MP3 CD");
    m_mapUSENAudioSource["iPod"].push_back("iPod");
    m_mapUSENAudioSource["iPod"].push_back("iPod 1");
    m_mapUSENAudioSource["iPod2"].push_back("iPod 2");
    m_mapUSENAudioSource["USB"].push_back("USB");
    m_mapUSENAudioSource["USB"].push_back("USB Audio");
    m_mapUSENAudioSource["USB"].push_back("USB 1");
    m_mapUSENAudioSource["USB"].push_back("MP3 Player");
    m_mapUSENAudioSource["USB"].push_back("MP3 Player 1");
    m_mapUSENAudioSource["USB"].push_back("MP3 1");
    m_mapUSENAudioSource["USB"].push_back("MP3");
    m_mapUSENAudioSource["USB2"].push_back("USB Audio 2");
    m_mapUSENAudioSource["USB2"].push_back("USB 2");
    m_mapUSENAudioSource["USB2"].push_back("MP3 Player 2");
    m_mapUSENAudioSource["USB2"].push_back("MP3 2");
    m_mapUSENAudioSource["Bluetooth Audio"].push_back("Bluetooth");
    m_mapUSENAudioSource["Bluetooth Audio"].push_back("Bluetooth Audio");
    m_mapUSENAudioSource["Bluetooth Audio"].push_back("BT Audio");
    m_mapUSENAudioSource["Auxiliary"].push_back("AUX");
    m_mapUSENAudioSource["Auxiliary"].push_back("Aux jack");
    m_mapUSENAudioSource["Auxiliary"].push_back("Auxiliary");
    m_mapUSENAudioSource["Auxiliary"].push_back("Audio jack");
    m_mapUSENAudioSource["Auxiliary"].push_back("Stereo plug");
    m_mapUSENAudioSource["Auxiliary"].push_back("Headphone jack");
    m_mapUSENAudioSource["Auxiliary"].push_back("A V");
    m_mapUSENAudioSource["Auxiliary"].push_back("Composite video ");
    m_mapUSENAudioSource["Auxiliary"].push_back("Video");
    m_mapUSENAudioSource["Auxiliary"].push_back("Auxiliary video");
    m_mapUSENAudioSource["Rear Entertainment System"].push_back("Rear");
    m_mapUSENAudioSource["Rear Entertainment System"].push_back("Back Seat");
    m_mapUSENAudioSource["Rear Entertainment System"].push_back("Rear Seat Entertainment");
    m_mapUSENAudioSource["Rear Entertainment System"].push_back("Rear System");
    m_mapUSENAudioSource["Rear Entertainment System"].push_back("R S E");
    m_mapUSENAudioSource["Rear Entertainment System"].push_back("R E S");
    m_mapUSENAudioSource["Rear Entertainment System"].push_back("Rear Entertainment System");
    m_mapUSENAudioSource["Rear Entertainment System"].push_back("DVD");
    m_mapUSENAudioSource["Rear Entertainment System"].push_back("DVD Player");

    m_mapUSESAudioSource["AM"].push_back("AM");
    m_mapUSESAudioSource["AM"].push_back("Radio AM");
    m_mapUSESAudioSource["FM"].push_back("FM");
    m_mapUSESAudioSource["FM"].push_back("Radio FM");
    m_mapUSESAudioSource["XM"].push_back("XM");
    m_mapUSESAudioSource["XM"].push_back("Sirius");
    m_mapUSESAudioSource["XM"].push_back("Sirius XM");
    m_mapUSESAudioSource["XM"].push_back("Sat");
    m_mapUSESAudioSource["XM"].push_back("Satélite");
    m_mapUSESAudioSource["XM"].push_back("Radio Satelital");
    m_mapUSESAudioSource["CD"].push_back("CD");
    m_mapUSESAudioSource["CD"].push_back("Reproductor de CD");
    m_mapUSESAudioSource["CD"].push_back("Reproductor de Disco Compacto");
    m_mapUSESAudioSource["CD"].push_back("Disco");
    m_mapUSESAudioSource["iPod"].push_back("iPod");
    m_mapUSESAudioSource["iPod"].push_back("iPod 1");
    m_mapUSESAudioSource["iPod2"].push_back("iPod 2");
    m_mapUSESAudioSource["USB"].push_back("USB");
    m_mapUSESAudioSource["USB"].push_back("Memoria USB");
    m_mapUSESAudioSource["USB"].push_back("USB 1");
    m_mapUSESAudioSource["USB"].push_back("Tarjeta de memoria");
    m_mapUSESAudioSource["USB"].push_back("Llavero USB");
    m_mapUSESAudioSource["USB"].push_back("Llave de memoria");
    m_mapUSESAudioSource["USB"].push_back("Unidad flash USB");
    m_mapUSESAudioSource["USB"].push_back("MP3");
    m_mapUSESAudioSource["USB"].push_back("MP3 1");
    m_mapUSESAudioSource["USB2"].push_back("USB 2");
    m_mapUSESAudioSource["USB2"].push_back("MP3 2");
    m_mapUSESAudioSource["Bluetooth Audio"].push_back("Bluetooth");
    m_mapUSESAudioSource["Bluetooth Audio"].push_back("Aparato de Bluetooth");
    m_mapUSESAudioSource["Bluetooth Audio"].push_back("Dispositivo de Bluetooth");
    m_mapUSESAudioSource["Auxiliary"].push_back("Auxiliar");
    m_mapUSESAudioSource["Auxiliary"].push_back("Entrada auxiliar");
    m_mapUSESAudioSource["Auxiliary"].push_back("Puerto auxiliar");
    m_mapUSESAudioSource["Auxiliary"].push_back("Entrada de audífonos");
    m_mapUSESAudioSource["Auxiliary"].push_back("Entrada de estéreo");
    m_mapUSESAudioSource["Auxiliary"].push_back("Aux");
    m_mapUSESAudioSource["Auxiliary"].push_back("A V");
    m_mapUSESAudioSource["Auxiliary"].push_back("Video Compuesto ");
    m_mapUSESAudioSource["Auxiliary"].push_back("Audio/Video");
    m_mapUSESAudioSource["Auxiliary"].push_back("Video Auxiliar");
    m_mapUSESAudioSource["Auxiliary"].push_back("Entrada de audio/video");
    m_mapUSESAudioSource["Rear Entertainment System"].push_back("DVD Trasero");
    m_mapUSESAudioSource["Rear Entertainment System"].push_back("Sistema Trasero");
    m_mapUSESAudioSource["Rear Entertainment System"].push_back("Sistema de entretenimiento Trasero ");

    m_mapUSFRAudioSource["AM"].push_back("AM");
    m_mapUSFRAudioSource["AM"].push_back("AM Radio");
    m_mapUSFRAudioSource["FM"].push_back("FM");
    m_mapUSFRAudioSource["FM"].push_back("FM Radio");
    m_mapUSFRAudioSource["FM"].push_back("Radio");
    m_mapUSFRAudioSource["XM"].push_back("XM");
    m_mapUSFRAudioSource["XM"].push_back("Syrius");
    m_mapUSFRAudioSource["XM"].push_back("Syrius XM");
    m_mapUSFRAudioSource["XM"].push_back("Sat");
    m_mapUSFRAudioSource["XM"].push_back("Satellite");
    m_mapUSFRAudioSource["XM"].push_back("Satellite Radio");
    m_mapUSFRAudioSource["CD"].push_back("CD");
    m_mapUSFRAudioSource["CD"].push_back("CD Player");
    m_mapUSFRAudioSource["CD"].push_back("CD Changer");
    m_mapUSFRAudioSource["CD"].push_back("Disc");
    m_mapUSFRAudioSource["CD"].push_back("MP3 CD");
    m_mapUSFRAudioSource["iPod"].push_back("iPod");
    m_mapUSFRAudioSource["iPod"].push_back("iPod 1");
    m_mapUSFRAudioSource["iPod2"].push_back("iPod 2");
    m_mapUSFRAudioSource["USB"].push_back("USB");
    m_mapUSFRAudioSource["USB"].push_back("USB Audio");
    m_mapUSFRAudioSource["USB"].push_back("USB 1");
    m_mapUSFRAudioSource["USB"].push_back("MP3 Player");
    m_mapUSFRAudioSource["USB"].push_back("MP3 Player 1");
    m_mapUSFRAudioSource["USB"].push_back("MP3 1");
    m_mapUSFRAudioSource["USB"].push_back("MP3");
    m_mapUSFRAudioSource["USB2"].push_back("USB Audio 2");
    m_mapUSFRAudioSource["USB2"].push_back("USB 2");
    m_mapUSFRAudioSource["USB2"].push_back("MP3 Player 2");
    m_mapUSFRAudioSource["USB2"].push_back("MP3 2");
    m_mapUSFRAudioSource["Bluetooth Audio"].push_back("Bluetooth");
    m_mapUSFRAudioSource["Bluetooth Audio"].push_back("Bluetooth Audio");
    m_mapUSFRAudioSource["Bluetooth Audio"].push_back("BT Audio");
    m_mapUSFRAudioSource["Auxiliary"].push_back("AUX");
    m_mapUSFRAudioSource["Auxiliary"].push_back("Aux jack");
    m_mapUSFRAudioSource["Auxiliary"].push_back("Auxiliary");
    m_mapUSFRAudioSource["Auxiliary"].push_back("Audio jack");
    m_mapUSFRAudioSource["Auxiliary"].push_back("Stereo plug");
    m_mapUSFRAudioSource["Auxiliary"].push_back("Headphone jack");
    m_mapUSFRAudioSource["Auxiliary"].push_back("A V");
    m_mapUSFRAudioSource["Auxiliary"].push_back("Composite video ");
    m_mapUSFRAudioSource["Auxiliary"].push_back("Video");
    m_mapUSFRAudioSource["Auxiliary"].push_back("Auxiliary video");
    m_mapUSFRAudioSource["Rear Entertainment System"].push_back("Rear");
    m_mapUSFRAudioSource["Rear Entertainment System"].push_back("Back Seat");
    m_mapUSFRAudioSource["Rear Entertainment System"].push_back("Rear Seat Entertainment");
    m_mapUSFRAudioSource["Rear Entertainment System"].push_back("Rear System");
    m_mapUSFRAudioSource["Rear Entertainment System"].push_back("R S E");
    m_mapUSFRAudioSource["Rear Entertainment System"].push_back("R E S");
    m_mapUSFRAudioSource["Rear Entertainment System"].push_back("Rear Entertainment System");
    m_mapUSFRAudioSource["Rear Entertainment System"].push_back("DVD");
    m_mapUSFRAudioSource["Rear Entertainment System"].push_back("DVD Player");

    return true;
}

bool
VR_VoiceBoxCatalogAudio::LoadAudioSource(
    const VoiceVector<AudioSourceInfo>::type& vecAudioSource,
    CVECIPtr<IVECIListItems>& audioSourceList)
{
    VR_LOGD_FUNC();

    if (vecAudioSource.empty()) {
        return true;
    }

    std::string strCulture =  m_engineCallback.GetCultureName();
    auto mapUSAudioSource = GetUSAudioSource(strCulture);
    if (mapUSAudioSource.empty()) {
        return false;
    }

    bool bHasUSB = false;
    bool bHasiPod = false;
    std::string striPodName;
    std::string striPod2Name;
    GetUSBIPodInfo(vecAudioSource, bHasUSB, bHasiPod, striPodName, striPod2Name);

    m_bUSBIPod2 = false;
    m_bIPodUSB2 = false;

    for (size_t i = 0; i < vecAudioSource.size(); ++i) {
        std::string strFormal = vecAudioSource[i].formalItem.name;
        if (strFormal.empty()) {
            continue;
        }

        if (bHasUSB && bHasiPod) {
            ChangeUSBIPodFormal(strFormal);
        }

        const std::string strSourceId = std::to_string(i + 1);
        if (std::string::npos != strFormal.find("iPod")) {
            std::string strDeviceName = ("iPod" == vecAudioSource[i].formalItem.name) ? striPodName : striPod2Name;
            if (!strDeviceName.empty()) {
                AddAudioSourceItem(strSourceId, strFormal, strDeviceName, vecAudioSource[i].formalItem.threeItem, "", audioSourceList);
            }
        }

        AddAudioSourceItem(strSourceId, strFormal, vecAudioSource[i].formalItem.threeItem, mapUSAudioSource, audioSourceList);
    }

    return true;
}

void
VR_VoiceBoxCatalogAudio::GetUSBIPodInfo(
        const VoiceVector<AudioSourceInfo>::type& vecAudioSource,
        bool& bHasUSB, bool& bHasiPod, std::string& striPodName, std::string& striPod2Name)
{
    striPodName.clear();
    striPod2Name.clear();


    for (size_t i = 0; i < vecAudioSource.size(); ++i) {
        if ("USB" == vecAudioSource[i].formalItem.name || "USB2" == vecAudioSource[i].formalItem.name) {
            bHasUSB = true;
        }
        else if ("iPod" == vecAudioSource[i].formalItem.name) {
            bHasiPod = true;
            auto iter = vecAudioSource[i].aliasMap.begin();
            for (; vecAudioSource[i].aliasMap.end() != iter; ++iter) {
                if ("iPod" != iter->first || "iPod 1" != iter->first) {
                    striPodName = iter->first;
                    break;
                }
            }
        }
        else if ("iPod2" == vecAudioSource[i].formalItem.name) {
            bHasiPod = true;
            auto iter = vecAudioSource[i].aliasMap.begin();
            for (; vecAudioSource[i].aliasMap.end() != iter; ++iter) {
                if ("iPod 2" != iter->first) {
                    striPod2Name = iter->first;
                    break;
                }
            }
        }
        else {
            // this is normal case. example: vecAudioSource[i].formalItem.name = AM
        }
    }
}

void
VR_VoiceBoxCatalogAudio::ChangeUSBIPodFormal(std::string& strFormal)
{
    if ("USB2" == strFormal) {
        m_bIPodUSB2 = true;
        strFormal = "USB";
    }
    else if ("iPod2" == strFormal) {
        m_bUSBIPod2 = true;
        strFormal = "iPod";
    }
    else {
        // this is normal case. when strFormal = USB or strFormal = iPod
    }
}

VoiceMap<std::string, VoiceList<std::string >::type >::type
VR_VoiceBoxCatalogAudio::GetUSAudioSource(const std::string& strCulture)
{
    if ("en-US" == strCulture) {
        return m_mapUSENAudioSource;
    }
    else if ("es-MX" == strCulture) {
        return m_mapUSESAudioSource;
    }
    else if ("fr-CA" == strCulture) {
        return m_mapUSFRAudioSource;
    }
    else {
        // if the strCulture is not valiad.
    }

    VoiceMap<std::string, VoiceList<std::string >::type >::type mapUSAudioSource;
    return mapUSAudioSource;
}

void
VR_VoiceBoxCatalogAudio::AddAudioSourceItem(
    const std::string& strId, const std::string& strFormal, const std::string& strGrammarId,
    const VoiceMap<std::string, VoiceList<std::string >::type >::type& mapUSAudioSource,
    CVECIPtr<IVECIListItems>& audioSourceList)
{
    auto iter = mapUSAudioSource.find(strFormal);
    if (mapUSAudioSource.end() != iter) {
        auto iterAlias = iter->second.begin();
        for (; iter->second.end() != iterAlias; ++iterAlias) {
            AddAudioSourceItem(strId, strFormal, *iterAlias, strGrammarId, "", audioSourceList);
        }
    }
    else {
        AddAudioSourceItem(strId, strFormal, strFormal, strGrammarId, "", audioSourceList);
    }
}

bool
VR_VoiceBoxCatalogAudio::LoadSatChannelNumber(
    const VoiceVector<FormalTwoItem>::type& vecSatChannelNumber,
    CVECIPtr<IVECIListItems>& satChannelNumberList)
{
    if (vecSatChannelNumber.empty()) {
        return true;
    }

    if (NULL == satChannelNumberList.ptr()) {
        return true;
    }

    VR_LOGI("VR_VoiceBoxCatalogAudio LoadSatChannelNumber");

    for (size_t i = 0; i < vecSatChannelNumber.size(); ++i) {
        CVECIPtr<IVECIParameterSet> spSatChannelNumber;
        (void)m_client.CreateParameterSet(&spSatChannelNumber);
        if (NULL == spSatChannelNumber.ptr()) {
            continue;
        }

        (void)spSatChannelNumber->AddParameter(_T("nId"), vecSatChannelNumber[i].strId.c_str(), NULL, NULL);
        (void)spSatChannelNumber->AddParameter(_T("cFormal"), vecSatChannelNumber[i].name.c_str(), NULL, NULL);
        (void)satChannelNumberList->AddItem(spSatChannelNumber);
    }

    return true;
}

bool
VR_VoiceBoxCatalogAudio::LoadFormalPronInfo(
    const VoiceVector<FormalPronInfo>::type& vecFormalPronInfo,
    CVECIPtr<IVECIListItems>& listItems)
{
    if (vecFormalPronInfo.empty() || NULL == listItems.ptr()) {
        return true;
    }

    for (size_t i = 0; i < vecFormalPronInfo.size(); ++i) {

        VoiceMap<std::string, std::string>::const_iterator iter = vecFormalPronInfo[i].aliasMap.begin();
        for (; vecFormalPronInfo[i].aliasMap.end() != iter; ++iter) {
            CVECIPtr<IVECIParameterSet> spItems;
            (void)m_client.CreateParameterSet(&spItems);
            if (NULL == spItems.ptr()) {
                continue;
        }

            (void)spItems->AddParameter(_T("nId"), vecFormalPronInfo[i].formalItem.strId.c_str(), NULL, NULL);
            (void)spItems->AddParameter(_T("cFormal"), vecFormalPronInfo[i].formalItem.name.c_str(), NULL, NULL);
            (void)spItems->AddParameter(_T("cAlias"), iter->first.c_str(), NULL, NULL);
            (void)spItems->AddParameter(_T("cPronunciation"), iter->second.c_str(), NULL, NULL);
            (void)listItems->AddItem(spItems);
        }
    }

    return true;
}

bool
VR_VoiceBoxCatalogAudio::UpdateRadioFMGenres(VR_VoiceBoxXmlParser& parser)
{
    VoiceVector<FormalPronInfo>::type vecFmGenre;
    parser.getFmGenre(vecFmGenre);
    std::string strLastFlg = parser.getLastFlg();
    if (vecFmGenre.empty()) {
        return RadioRemoveGrammer("RadioAgentFMGenres", "fmgenres", strLastFlg);
    }
    else {
        return RadioAgentFMGenres(vecFmGenre, strLastFlg);
    }

    return true;
}

bool
VR_VoiceBoxCatalogAudio::UpdateRadioSatelliteChannelNames(VR_VoiceBoxXmlParser& parser)
{
    VoiceVector<FormalPronInfo>::type vecSatChannelName;
    parser.getSatChannelName(vecSatChannelName);
    std::string strLastFlg = parser.getLastFlg();
    if (vecSatChannelName.empty()) {
        return RadioRemoveGrammer("RadioAgentSatelliteChannelNames", "satchannelnames", strLastFlg);
    }
    else {
        return RadioAgentSatelliteChannelNames(vecSatChannelName, strLastFlg);
    }

    return true;
}

bool
VR_VoiceBoxCatalogAudio::UpdateRadioSatelliteChannelNumbers(VR_VoiceBoxXmlParser& parser)
{
    VoiceVector<FormalTwoItem>::type vecSatChannelNumber;
    parser.getSatChannelNumber(vecSatChannelNumber);
    std::string strLastFlg = parser.getLastFlg();
    if (vecSatChannelNumber.empty()) {
        return RadioRemoveGrammer("RadioAgentSatelliteChannelNumbers", "satchannelnumbers", strLastFlg);
    }
    else {
        return RadioAgentSatelliteChannelNumbers(vecSatChannelNumber, strLastFlg);
    }

    return true;
}

bool
VR_VoiceBoxCatalogAudio::UpdateRadioSatelliteGenres(VR_VoiceBoxXmlParser& parser)
{
    VoiceVector<FormalPronInfo>::type vecSatGenre;
    parser.getSatGenre(vecSatGenre);
    std::string strLastFlg = parser.getLastFlg();
    if (vecSatGenre.empty()) {
        return RadioRemoveGrammer("RadioAgentSatelliteGenres", "satgenres", strLastFlg);
    }
    else {
        return RadioAgentSatelliteGenres(vecSatGenre, strLastFlg);
    }

    return true;
}

bool
VR_VoiceBoxCatalogAudio::RadioAgentFMGenres(const VoiceVector<FormalPronInfo>::type& vecFmGenre, const std::string& strLastFlg)
{
    if (NULL == m_agentDataCommandRadio.ptr()) {
        VR_LOGE("m_agentDataCommandRadio is NULL");
        return false;
    }

    HRESULT retCode = m_agentDataCommandRadio->SetDataSynchronized(NULL, _T(""), "RadioAgentFMGenres", "fmgenres", VBT_FALSE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    CVECIPtr<IVECIListItems> lstItems;
    (void)m_client.CreateListItems(&lstItems);
    LoadFormalPronInfo(vecFmGenre, lstItems);

    retCode = m_agentDataCommandRadio->ReloadData(NULL, _T(""), "RadioAgentFMGenres", lstItems);
    if (S_OK != retCode) {
        VR_LOGE("ReloadData: 0x%lx", retCode);
        return false;
    }

    retCode = m_agentDataCommandRadio->SetDataSynchronized(NULL, _T(""), "RadioAgentFMGenres", "fmgenres", VBT_TRUE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    CVECIPtr<IVECITransaction> spTrans;
    retCode = m_agentDataCommandRadio->ReloadGrammar(&spTrans, _T(""),  "RadioAgentFMGenres");
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
    m_engineCallback.SetCurrentTransaction("radio", strTransaction);

    VR_LOGI("grammar : %s", strTransaction.c_str());

    return true;
}

bool
VR_VoiceBoxCatalogAudio::RadioAgentSatelliteChannelNames(const VoiceVector<FormalPronInfo>::type& vecSatChannelName, const std::string& strLastFlg)
{
    if (NULL == m_agentDataCommandRadio.ptr()) {
        VR_LOGE("m_agentDataCommandRadio is NULL");
        return false;
    }

    HRESULT retCode = m_agentDataCommandRadio->SetDataSynchronized(NULL, _T(""), "RadioAgentSatelliteChannelNames", "satchannelnames", VBT_FALSE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    CVECIPtr<IVECIListItems> lstItems;
    (void)m_client.CreateListItems(&lstItems);

    LoadFormalPronInfo(vecSatChannelName, lstItems);

    retCode = m_agentDataCommandRadio->ReloadData(NULL, _T(""), "RadioAgentSatelliteChannelNames", lstItems);
    if (S_OK != retCode) {
        VR_LOGE("ReloadData: 0x%lx", retCode);
        return false;
    }

    retCode = m_agentDataCommandRadio->SetDataSynchronized(NULL, _T(""), "RadioAgentSatelliteChannelNames", "satchannelnames", VBT_TRUE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    CVECIPtr<IVECITransaction> spTrans;
    retCode = m_agentDataCommandRadio->ReloadGrammar(&spTrans, _T(""),  "RadioAgentSatelliteChannelNames");
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
    m_engineCallback.SetCurrentTransaction("radio", strTransaction);

    VR_LOGI("grammar : %s", strTransaction.c_str());
    return true;
}

bool
VR_VoiceBoxCatalogAudio::RadioAgentSatelliteChannelNumbers(const VoiceVector<FormalTwoItem>::type& vecSatChannelNumber, const std::string& strLastFlg)
{
    if (NULL == m_agentDataCommandRadio.ptr()) {
        VR_LOGE("m_agentDataCommandRadio is NULL");
        return false;
    }

    HRESULT retCode = m_agentDataCommandRadio->SetDataSynchronized(NULL, _T(""), "RadioAgentSatelliteChannelNumbers", "satchannelnumbers", VBT_FALSE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    CVECIPtr<IVECIListItems> lstItems;
    (void)m_client.CreateListItems(&lstItems);

    LoadSatChannelNumber(vecSatChannelNumber, lstItems);

    retCode = m_agentDataCommandRadio->ReloadData(NULL, _T(""), "RadioAgentSatelliteChannelNumbers", lstItems);
    if (S_OK != retCode) {
        VR_LOGE("ReloadData: 0x%lx", retCode);
        return false;
    }

    retCode = m_agentDataCommandRadio->SetDataSynchronized(NULL, _T(""), "RadioAgentSatelliteChannelNumbers", "satchannelnumbers", VBT_TRUE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    CVECIPtr<IVECITransaction> spTrans;
    retCode = m_agentDataCommandRadio->ReloadGrammar(&spTrans, _T(""),  "RadioAgentSatelliteChannelNumbers");
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
    m_engineCallback.SetCurrentTransaction("radio", strTransaction);

    VR_LOGI("grammar : %s", strTransaction.c_str());
    return true;
}

bool
VR_VoiceBoxCatalogAudio::RadioAgentSatelliteGenres(const VoiceVector<FormalPronInfo>::type& vecSatGenre, const std::string& strLastFlg)
{
    VR_LOGD_FUNC();

    if (NULL == m_agentDataCommandRadio.ptr()) {
        VR_LOGE("m_agentDataCommandRadio is NULL");
        return false;
    }

    HRESULT retCode = m_agentDataCommandRadio->SetDataSynchronized(NULL, _T(""), "RadioAgentSatelliteGenres", "satgenres", VBT_FALSE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    CVECIPtr<IVECIListItems> lstItems;
    (void)m_client.CreateListItems(&lstItems);

    LoadFormalPronInfo(vecSatGenre, lstItems);

    retCode = m_agentDataCommandRadio->ReloadData(NULL, _T(""), "RadioAgentSatelliteGenres", lstItems);
    if (S_OK != retCode) {
        VR_LOGE("ReloadData: 0x%lx", retCode);
        return false;
    }

    retCode = m_agentDataCommandRadio->SetDataSynchronized(NULL, _T(""), "RadioAgentSatelliteGenres", "satgenres", VBT_FALSE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    CVECIPtr<IVECITransaction> spTrans;
    retCode = m_agentDataCommandRadio->ReloadGrammar(&spTrans, _T(""),  "RadioAgentSatelliteGenres");
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
    m_engineCallback.SetCurrentTransaction("radio", strTransaction);

    VR_LOGI("grammar : %s", strTransaction.c_str());
    return true;
}

bool
VR_VoiceBoxCatalogAudio::RadioRemoveGrammer(
    const std::string& strTableName, const std::string& strHandlerName, const std::string& strLastFlg)
{
    VR_LOGD_FUNC();

    if (NULL == m_agentDataCommandRadio.ptr() || "" == strTableName) {
        VR_LOGE("m_agentDataCommandRadio is NULL");
        return false;
    }

    if (strTableName.empty() || strHandlerName.empty()) {
        VR_LOGE("strTableName or strHandlerName is empty");
        return false;
    }

    VR_LOGI("strTableName: %s", strTableName.c_str());

    HRESULT retCode = m_agentDataCommandRadio->SetDataSynchronized(NULL, _T(""), strTableName.c_str(), strHandlerName.c_str(), VBT_FALSE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }
    retCode = m_agentDataCommandRadio->RemoveAllData(NULL, _T(""), strTableName.c_str());
    if (S_OK != retCode) {
        VR_LOGE("RemoveAllData: 0x%lx", retCode);
        return false;
    }
    retCode = m_agentDataCommandRadio->SetDataSynchronized(NULL, _T(""), strTableName.c_str(), strHandlerName.c_str(), VBT_TRUE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }
    CVECIPtr<IVECITransaction> spTrans;
    retCode = m_agentDataCommandRadio->RemoveAllGrammars(&spTrans, _T(""), strTableName.c_str());
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
    m_engineCallback.SetCurrentTransaction("radio", strTransaction);

     VR_LOGI("grammar : %s", strTransaction.c_str());

     return true;
}

bool
VR_VoiceBoxCatalogAudio::UpdateMusicAudioSources(
    VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    if (NULL == m_agentDataCommandMusic.ptr()) {
        VR_LOGE("m_agentDataCommandMusic is NULL");
        return false;
    }

    VoiceVector<AudioSourceInfo>::type vecAudioSource;
    parser.getAudioSource(vecAudioSource);
    HRESULT retCode = m_agentDataCommandMusic->SetDataSynchronized(NULL, _T(""), _T("MusicAgentAudioSources"), _T(""), VBT_FALSE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    CVECIPtr<IVECIListItems> audioSourceList;
    (void)m_client.CreateListItems(&audioSourceList);

    // Music AudioSource
    LoadAudioSource(vecAudioSource, audioSourceList);

    retCode = m_agentDataCommandMusic->ReloadData(NULL, _T(""), _T("MusicAgentAudioSources"), audioSourceList);
    if (S_OK != retCode) {
        VR_LOGE("ReloadData: 0x%lx", retCode);
        return false;
    }

    retCode = m_agentDataCommandMusic->SetDataSynchronized(NULL, _T(""), _T("MusicAgentAudioSources"), _T(""), VBT_TRUE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    CVECIPtr<IVECITransaction> spTrans;
    retCode = m_agentDataCommandMusic->ReloadGrammar(&spTrans, _T(""),  _T("MusicAgentAudioSources"));
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
    m_engineCallback.SetCurrentTransaction("music", strTransaction);

    VR_LOGI("grammar : %s", strTransaction.c_str());

    return true;
}

void
VR_VoiceBoxCatalogAudio::AddAudioSourceItem(const std::string& stdId,
    const std::string& strFormal, const std::string& strAlias,
    const std::string& strGrammarId, const std::string& strIspermanent,
    CVECIPtr<IVECIListItems>& audioSourceList)
{
    CVECIPtr<IVECIParameterSet> spItems;
    (void)m_client.CreateParameterSet(&spItems);
    if (NULL == spItems.ptr() || NULL == audioSourceList.ptr()) {
        VR_LOGE("spItems is NULL or audioSourceList is NULL");
        return;
    }

    VR_LOGI("nId = %s, cFormal = %s, cAlias = %s, cTts = %s, cGrammarId = %s, cIspermanent = %s", \
        stdId.c_str(), strFormal.c_str(), strAlias.c_str(), strFormal.c_str(), strGrammarId.c_str(), strIspermanent.c_str());

    (void)spItems->AddParameter(_T("nId"), stdId.c_str(), NULL, NULL);
    (void)spItems->AddParameter(_T("cFormal"), strFormal.c_str(), NULL, NULL);
    (void)spItems->AddParameter(_T("cAlias"), strAlias.c_str(), NULL, NULL);
    (void)spItems->AddParameter(_T("cTts"), strFormal.c_str(), NULL, NULL);
    (void)spItems->AddParameter(_T("cGrammarId"), strGrammarId.c_str(), NULL, NULL);
    if (!strIspermanent.empty()) {
        (void)spItems->AddParameter(_T("cIspermanent"), strIspermanent.c_str(), NULL, NULL);
    }

    (void)audioSourceList->AddItem(spItems);
}

bool
VR_VoiceBoxCatalogAudio::MusicGrammarAudioSourceOC(VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    if (NULL == m_agentDataCommandMusic.ptr()) {
        VR_LOGE("m_agentDataCommandMusic is NULL");
        return false;
    }

    std::string strWithDisc = parser.getValueByKey("withdisc");
    VR_LOGI("strWithDisc = %s", strWithDisc.c_str());
    HRESULT retCode = m_agentDataCommandMusic->SetDataSynchronized(NULL, _T(""), _T("MusicAgentAudioSources"), _T(""), VBT_FALSE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    CVECIPtr<IVECIListItems> audioSourceList;
    (void)m_client.CreateListItems(&audioSourceList);

    // id formal alias tts grammarid ispermanent
    AddAudioSourceItem("1", "USB", "USB", "", "0", audioSourceList);
    AddAudioSourceItem("2", "iPod", "iPod", "", "0", audioSourceList);
    AddAudioSourceItem("3", "USB2", "USB2", "", "0", audioSourceList);
    AddAudioSourceItem("4", "iPod2", "iPod2", "", "0", audioSourceList);
    AddAudioSourceItem("5", "Bluetooth Audio", "bluetooth audio", "", "0", audioSourceList);
    AddAudioSourceItem("5", "Bluetooth Audio", "bluetooth", "", "0", audioSourceList);
    AddAudioSourceItem("6", "AM", "AM radio", "", "1", audioSourceList);
    AddAudioSourceItem("6", "AM", "AM", "", "1", audioSourceList);
    AddAudioSourceItem("7", "FM", "FM radio", "", "1", audioSourceList);
    AddAudioSourceItem("7", "FM", "FM", "", "1", audioSourceList);
    AddAudioSourceItem("8", "Auxiliary", "auxiliary", "", "1", audioSourceList);
    AddAudioSourceItem("8", "Auxiliary", "line in", "", "1", audioSourceList);
    AddAudioSourceItem("9", "DAB", "DAB", "", "1", audioSourceList);
    AddAudioSourceItem("9", "DAB", "dab", "", "1", audioSourceList);

    if ("true" == strWithDisc) {
        AddAudioSourceItem("10", "CD", "disc", "", "1", audioSourceList);
        AddAudioSourceItem("10", "CD", "Compact Disc", "", "1", audioSourceList);
        AddAudioSourceItem("10", "CD", "CD", "", "1", audioSourceList);
    }

    retCode = m_agentDataCommandMusic->ReloadData(NULL, _T(""), _T("MusicAgentAudioSources"), audioSourceList);
    if (S_OK != retCode) {
        VR_LOGE("ReloadData: 0x%lx", retCode);
        return false;
    }

    retCode = m_agentDataCommandMusic->SetDataSynchronized(NULL, _T(""), _T("MusicAgentAudioSources"), _T(""), VBT_TRUE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    CVECIPtr<IVECITransaction> spTrans;
    retCode = m_agentDataCommandMusic->ReloadGrammar(&spTrans, _T(""),  _T("MusicAgentAudioSources"));
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
    m_engineCallback.SetCurrentTransaction("music", strTransaction);

    VR_LOGI("grammar : %s", strTransaction.c_str());

    return true;
}

bool
VR_VoiceBoxCatalogAudio::MusicGrammarActive(
    VR_VoiceBoxXmlParser& parser)
{
    m_strSourceId = parser.getValueByKey("grammarid");
    m_strDBPath = parser.getValueByKey("path");
    std::string strReply = parser.getValueByKey("reply");
    std::string strSourceType = parser.getValueByKey("sourcetype");
    if ("ipod" == strSourceType) {
        m_bIpodActive = true;
    }

    std::string strDMActiveSourceId = m_engineCallback.GetMDActiveSourceId();
    VR_LOGI("strDMActiveSourceId = %s, m_strSourceId = %s, m_strDBPath = %s", strDMActiveSourceId.c_str(), m_strSourceId.c_str(), m_strDBPath.c_str());
    if (strDMActiveSourceId != m_strSourceId) {
        if ("true" == strReply) {
            m_engineCallback.SendGrammarResult("active", "media", m_strSourceId.c_str(), "0");
        }
        m_engineCallback.SetUpdateGammarFlg(true);

        return true;
    }

    std::string strAudioSourceConnected = m_engineCallback.GetAudioConnected();
    HRESULT retCode = m_engineCommand.SetPreference(NULL, _T("Music"),
        VBT_USR_PREF_MUSIC_AUDIOSOURCECONNECTED, strAudioSourceConnected.c_str());
    if (S_OK != retCode) {
        VR_LOGE("SetPreference: 0x%lx", retCode);
        return false;
    }

    std::string strPath = m_strDBPath;
    SetMusicDBPathByPath(strPath);

    SetMusicEnable(parser);

    if (NULL == m_agentDataCommandMusic.ptr()) {
        VR_LOGE("m_agentDataCommandMusic is NULL");
        return false;
    }

    CVECIPtr<IVECITransaction> spTrans;
    retCode = m_agentDataCommandMusic->SetDataActiveSource(&spTrans, m_strSourceId.c_str(), NULL, NULL);
    if (S_OK != retCode || NULL == spTrans.ptr()) {
        VR_LOGE("SetDataActiveSource: 0x%lx or spTrans is NULL", retCode);
        return false;
    }

    CVECIOutStr strTrans;
    retCode = spTrans->GetTransactionId(&strTrans);
    if (FAILED(retCode) || NULL == strTrans.Get()) {
        VR_LOGE("GetTransactionId: 0x%lx or spTrans.Get() is NULL", retCode);
        return false;
    }

    std::string strTranstmp = strTrans.Get();
    if ("true" == strReply) {
        m_engineCallback.SetActiveSouceTrans(strTranstmp, "active", m_strSourceId);
    }

    return true;
}

void
VR_VoiceBoxCatalogAudio::SetDataActiveSource(
    const std::string& strActiveSourceId)
{
    VR_LOGD_FUNC();

    if (NULL == m_agentDataCommandMusic.ptr()) {
        VR_LOGE("m_agentDataCommandMusic is NULL");
        return;
    }
    CVECIPtr<IVECITransaction> spTrans;
    HRESULT retCode = m_agentDataCommandMusic->SetDataActiveSource(&spTrans, strActiveSourceId.c_str(), NULL, NULL);
    if (S_OK != retCode || NULL == spTrans.ptr()) {
        return;
    }

    CVECIOutStr strTrans;
    retCode = spTrans->GetTransactionId(&strTrans);
    if (FAILED(retCode)) {
        return;
    }

    if (NULL != strTrans.Get()) {
        VR_LOGI("strTrans = %s", strTrans.Get());
    }
}

bool
VR_VoiceBoxCatalogAudio::MusicGrammarDisActive(
    VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();

    m_strDBPath.clear();
    m_strSourceId.clear();
    m_bIpodActive = false;

    std::string strSourceId = parser.getValueByKey("grammarid");
    std::string strReply = parser.getValueByKey("reply");

    std::string strDMActiveSourceId = m_engineCallback.GetMDActiveSourceId();
    VR_LOGI("strDMActiveSourceId = %s", strDMActiveSourceId.c_str());
    if (strDMActiveSourceId != "5") {
        if ("true" == strReply) {
            m_engineCallback.SendGrammarResult("disactive", "media", m_strSourceId.c_str(), "0");
        }
        m_engineCallback.SetUpdateGammarFlg(true);

        return true;
    }

    if (NULL == m_agentDataCommandMusic.ptr()) {
        VR_LOGE("m_agentDataCommandMusic is NULL");
        return false;
    }

    CVECIPtr<IVECITransaction> transaction;
    HRESULT retCode = m_engineCommand.SetPreference(&transaction, _T("Music"), VBT_USR_PREF_MUSIC_AUDIOSOURCECONNECTED, "False");
    if (FAILED(retCode) || (NULL == transaction.ptr())) {
        VR_LOGE("SetPreference: 0x%lx or transaction is NULL", retCode);
        return false;
    }

    retCode = m_engineCommand.SetPreference(NULL, _T("Music"), _T("DBPath"), "/data/vr/data/MusicCatalog.db");
    if (S_OK != retCode) {
        return false;
    }

    CVECIPtr<IVECITransaction> spTrans;
    retCode = m_agentDataCommandMusic->SetDataActiveSource(&spTrans, "5", NULL, NULL);
    if (S_OK != retCode || NULL == spTrans.ptr()) {
        return false;
    }

    CVECIOutStr strTrans;
    retCode = spTrans->GetTransactionId(&strTrans);
    if (FAILED(retCode) || NULL == strTrans.Get()) {
        VR_LOGE("GetTransactionId: 0x%lx or strTrans.Get() is NULL", retCode);
        return false;
    }

    std::string strTranstmp = strTrans.Get();
    if ("true" == strReply) {
        m_engineCallback.SetActiveSouceTrans(strTranstmp, "disactive", strSourceId);
    }

    return true;
}

bool
VR_VoiceBoxCatalogAudio::SetupMusicData(
    VR_VoiceBoxXmlParser& parser)
{
    VR_LOGD_FUNC();
    VR_LOGP("DE: update music grammar... case : 212-137-00 212-139-00");

    if (NULL == m_agentDataCommandMusic.ptr()) {
        VR_LOGE("m_agentDataCommandMusic is NULL");
        return false;
    }

    std::string strAudioSourceConnected = m_engineCallback.GetAudioConnected();
    HRESULT retCode = m_engineCommand.SetPreference(NULL, _T("Music"), VBT_USR_PREF_MUSIC_AUDIOSOURCECONNECTED, strAudioSourceConnected.c_str());
    if (S_OK != retCode) {
        return false;
    }

    std::string strPath = parser.getValueByKey("path");
    SetMusicDBPathByPath(strPath);

    SetMusicEnable(parser);

    std::string strGrammarId = parser.getValueByKey("grammarid");
    m_engineCallback.SetGrammarInitSourceId(strGrammarId);

    VR_LOGI("begin to reload music grammar");
    retCode = m_agentDataCommandMusic->SetDataSynchronized(NULL, strGrammarId.c_str(), _T(""), _T(""), VBT_FALSE);
    if (S_OK != retCode) {
        VR_LOGI("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    retCode = m_agentDataCommandMusic->SetDataSynchronized(NULL, strGrammarId.c_str(), _T(""), _T(""), VBT_TRUE);
    if (S_OK != retCode) {
        VR_LOGI("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    CVECIPtr<IVECITransaction> spTrans;
    retCode = m_agentDataCommandMusic->ReloadGrammar(&spTrans, strGrammarId.c_str(), NULL);
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
    m_engineCallback.InsertMusicUpdateGrammar(strTransaction, strGrammarId, "true");
    m_engineCallback.SetCurrentTransaction("music", strTransaction);

    VR_LOGI("music grammar strTransaction : %s", strTransaction.c_str());

    std::string strDMActiveSourceId = m_engineCallback.GetMDActiveSourceId();
    if (strGrammarId == strDMActiveSourceId) {
        CVECIPtr<IVECITransaction> spTransActive;
        m_agentDataCommandMusic->SetDataActiveSource(&spTransActive, strGrammarId.c_str(), NULL, NULL);
        if (S_OK != retCode || NULL == spTransActive.ptr()) {
            return false;
        }
    }

    return true;
}

void
VR_VoiceBoxCatalogAudio::SetMusicDBPathByPath(std::string& strPath)
{
    if (strPath.empty()) {
        return;
    }

    size_t ipos1 = strPath.rfind(".db");
    size_t ipos2 = strPath.rfind("MusicCatalog");

    if (std::string::npos != ipos1 && std::string::npos != ipos2) {
        strPath = strPath.replace(ipos2 + 12, (ipos1 - (ipos2 + 12)), "");
    }

    HRESULT retCode = m_engineCommand.SetPreference(NULL, _T("Music"), _T("DBPath"), strPath.c_str());
    if (S_OK != retCode) {
        return;
    }
}

void
VR_VoiceBoxCatalogAudio::SetMusicEnable(
    VR_VoiceBoxXmlParser& parser)
{
    VR_ConfigureIF * pcConfig = VR_ConfigureIF::Instance();
    if (NULL == pcConfig) {
        return;
    }

    std::string strSongItemCount = parser.getValueByKey("songitemcount");
    std::string strGeneralItemcount = parser.getValueByKey("generalitemcount");
    int iSongItem = std::atoi(strSongItemCount.c_str());
    int iOtherItem = std::atoi(strGeneralItemcount.c_str());
    std::string  strEnableSongs;
    std::string  strEnableOthers;
    VR_REGION_TYPE country = pcConfig->getVRContry();
    if (VR_REGION_OC == country) {
        VR_LOGI("iSongItem = %d, iOtherItem = %d", iSongItem, iOtherItem);
        strEnableSongs = (iSongItem > 15000) ? "False" : "True";
        strEnableOthers = (iOtherItem > 15000) ? "False" : "True";
    }
    else if (VR_REGION_US == country) {
        std::string strUCItemCount = parser.getValueByKey("ucitemcount");
        std::string strGenreItemCount = parser.getValueByKey("genreitemcount");
        int iUCItemCount = std::atoi(strUCItemCount.c_str());
        int iGenreItemCount = std::atoi(strGenreItemCount.c_str());
        iOtherItem = iOtherItem + iUCItemCount + iGenreItemCount;
        VR_LOGI("iSongItem = %d, iOtherItem = %d", iSongItem, iOtherItem);
        strEnableSongs = ((iSongItem + iOtherItem) > 15000) ? "False" : "True";
        strEnableOthers = (iOtherItem > 15000) ? "False" : "True";
    }
    else {
        return;
    }

    HRESULT retCode = m_engineCommand.SetPreference(NULL, _T("Music"), VBT_USR_PREF_MUSIC_ENABLESONGS, strEnableSongs.c_str());
    if (S_OK != retCode) {
        VR_LOGI("set the enablesongs failed");
        return;
    }

    retCode = m_engineCommand.SetPreference(NULL, _T("Music"), VBT_USR_PREF_MUSIC_ENABLEOTHERS, strEnableOthers.c_str());
    if (S_OK != retCode) {
        VR_LOGI("set the enableothers failed");
        return;
    }
}

bool
VR_VoiceBoxCatalogAudio::UpdateMusicDiffInfo(
    VR_VoiceBoxXmlParser& parser, const std::string& strMusicDiffType)
{
    VR_LOGD_FUNC();
    VR_LOGP("DE: differential update of music grammar... case : 212-12 212-14");

    if ("add" != strMusicDiffType && "delete" != strMusicDiffType) {
        VR_LOGI("the type of music grammar_diff is error");
        return false;
    }

    if (NULL == m_agentDataCommandMusic.ptr()) {
        VR_LOGE("m_agentDataCommandMusic is NULL");
        return false;
    }

    auto pairDiffInfo = parser.getMusicDiffInfo();
    if (pairDiffInfo.first.empty() || pairDiffInfo.second.empty()) {
        VR_LOGI("no data to update");
        return false;
    }
    std::string strLastFlg = parser.getLastFlg();
    CVECIPtr<IVECIListItems> spListDiff;
    m_client.CreateListItems(&spListDiff);
    if (NULL == spListDiff.ptr()) {
        return false;
    }

    auto iter = pairDiffInfo.second.begin();
    for (; pairDiffInfo.second.end() != iter; ++iter) {
        CVECIPtr<IVECIParameterSet> spParams;
        m_client.CreateParameterSet(&spParams);
        if (NULL == spParams.ptr()) {
            continue;
        }

        spParams->AddParameter(_T("nId"), (*iter).c_str(), NULL, NULL);
        VR_LOGI("nId = %s", (*iter).c_str());
        spListDiff->AddItem(spParams);
    }

    std::string strTableName;
    std::string strHandlerName;
    GetTableHandlerName(pairDiffInfo.first, strTableName, strHandlerName);

    HRESULT retCode = m_agentDataCommandMusic->SetDataSynchronized(NULL, m_strSourceIdDiff.c_str(), strTableName.c_str(), strHandlerName.c_str(), VBT_FALSE);
    if (S_OK != retCode) {
        VR_LOGI("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }

    retCode = m_agentDataCommandMusic->SetDataSynchronized(NULL, m_strSourceIdDiff.c_str(), strTableName.c_str(), strHandlerName.c_str(), VBT_TRUE);
    if (S_OK != retCode) {
        VR_LOGI("SetDataSynchronized: 0x%lx", retCode);
        return false;
    }
    CVECIPtr<IVECITransaction> spTrans;
    if ("add" == strMusicDiffType) {
        retCode = m_agentDataCommandMusic->AddGrammar(&spTrans, m_strSourceIdDiff.c_str(), strTableName.c_str(), strHandlerName.c_str(), spListDiff);
    }
    else if ("delete" == strMusicDiffType) {
        retCode = m_agentDataCommandMusic->RemoveGrammar(&spTrans, m_strSourceIdDiff.c_str(), strTableName.c_str(), strHandlerName.c_str(), spListDiff);
    }
    else {

    }

    if (S_OK != retCode || NULL == spTrans.ptr()) {
        return false;
    }

    CVECIOutStr strTrans;
    retCode = spTrans->GetTransactionId(&strTrans);
    if (FAILED(retCode) || NULL == strTrans.Get()) {
        return false;
    }

    std::string strTransaction = strTrans.Get();
    m_engineCallback.InsertMusicUpdateGrammar(strTransaction, m_strSourceIdDiff, strLastFlg);
    m_engineCallback.SetCurrentTransaction("music", strTransaction);

    VR_LOGI("music add grammar  strTransaction: %s", strTransaction.c_str());

    return true;
}

bool
VR_VoiceBoxCatalogAudio::MusicGrammarDiff(
    VR_VoiceBoxXmlParser& parser)
{
    VR_LOGP("DE: differential update of music grammar... case : 212-12-00 212-13-00 212-14-00 212-15-00");

    std::string strAudioSourceConnected = m_engineCallback.GetAudioConnected();
    HRESULT retCode = m_engineCommand.SetPreference(NULL, _T("Music"), VBT_USR_PREF_MUSIC_AUDIOSOURCECONNECTED, strAudioSourceConnected.c_str());
    if (S_OK != retCode) {
        return false;
    }

    m_strSourceIdDiff = parser.getValueByKey("grammarid");
    VR_LOGI("m_strSourceIdDiff = %s", m_strSourceIdDiff.c_str());
    std::string strPath = parser.getValueByKey("path");
    SetMusicDBPathByPath(strPath);

    SetMusicEnable(parser);

    m_engineCallback.SetUpdateGammarFlg(true);

    return true;
}

void
VR_VoiceBoxCatalogAudio::GetTableHandlerName(const std::string& strGrammarName,
    std::string& strTableName, std::string& strHandlerName)
{
    if (strGrammarName.empty()) {
        return;
    }

    if ("song" == strGrammarName) {
        strTableName = "MusicAgentSongs";
        strHandlerName = "songs";
    }
    else if ("album" == strGrammarName) {
        strTableName = "MusicAgentAlbums";
        strHandlerName = "albums";
    }
    else if ("artist" == strGrammarName) {
        strTableName = "MusicAgentArtists";
        strHandlerName = "artists";
    }
    else if ("genre" == strGrammarName) {
        strTableName = "MusicAgentGenres";
        strHandlerName = "genres";
    }
    else if ("audiobook" == strGrammarName) {
        strTableName = "MusicAgentAudiobooks";
        strHandlerName = "audiobooks";
    }
    else if ("composer" == strGrammarName) {
        strTableName = "MusicAgentComposers";
        strHandlerName = "composers";
    }
    else if ("playlist" == strGrammarName) {
        strTableName = "MusicAgentPlaylists";
        strHandlerName = "playlists";
    }
    else if ("podcast" == strGrammarName) {
        strTableName = "MusicAgentPodcasts";
        strHandlerName = "podcasts";
    }
    else {

    }
}

void
VR_VoiceBoxCatalogAudio::PauseGrammarMusic()
{
    if (NULL == m_agentDataCommandMusic.ptr()) {
        VR_LOGE("m_agentDataCommandMusic is NULL");
        return;
    }

    std::pair<std::string, std::string> pairAgent2TransId = m_engineCallback.GetCurrentTransaction();
    if (pairAgent2TransId.first.empty()) {
        VR_LOGI("grammar has updated finish");
        return;
    }

    VR_LOGI(" PauseGrammarUpdate of music, first = %s, second = %s", pairAgent2TransId.first.c_str(), pairAgent2TransId.second.c_str());

    CVECIPtr<IVECITransaction> spTrans;
    HRESULT retCode =  m_agentDataCommandMusic->PauseGrammarUpdate(&spTrans);
    if (S_OK != retCode || NULL == spTrans.ptr()) {
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
VR_VoiceBoxCatalogAudio::PauseGrammarRadio()
{
    if (NULL == m_agentDataCommandRadio.ptr()) {
        VR_LOGE("m_agentDataCommandRadio is NULL");
        return;
    }
    std::pair<std::string, std::string> pairAgent2TransId = m_engineCallback.GetCurrentTransaction();
    if (pairAgent2TransId.first.empty()) {
        VR_LOGI("grammar has updated finish");
        return;
    }

    VR_LOGI("PauseGrammarUpdate of radio, first = %s, second = %s", pairAgent2TransId.first.c_str(), pairAgent2TransId.second.c_str());

    CVECIPtr<IVECITransaction> spTrans;
    HRESULT retCode =  m_agentDataCommandRadio->PauseGrammarUpdate(&spTrans);
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
VR_VoiceBoxCatalogAudio::ResumeGrammarMusic()
{
    if (NULL == m_agentDataCommandMusic.ptr()) {
        VR_LOGE("m_agentDataCommandMusic is NULL");
        return;
    }
    CVECIPtr<IVECITransaction> spTrans;
    HRESULT retCode = m_agentDataCommandMusic->ResumeGrammarUpdate(&spTrans);
    if (FAILED(retCode) || NULL == spTrans.ptr()) {
        VR_LOGE("ResumeGrammarUpdate: 0x%lx or spTrans is NULL", retCode);
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

    m_engineCallback.SetGrammarInitSourceId("5");
}

void
VR_VoiceBoxCatalogAudio::ResumeGrammarRadio()
{
    if (NULL == m_agentDataCommandRadio.ptr()) {
        VR_LOGE("m_agentDataCommandRadio is NULL");
        return;
    }
    CVECIPtr<IVECITransaction> spTrans;
    HRESULT retCode = m_agentDataCommandRadio->ResumeGrammarUpdate(&spTrans);
    if (FAILED(retCode) || NULL == spTrans.ptr()) {
        VR_LOGE("ResumeGrammarUpdate: 0x%lx or spTrans is NULL", retCode);
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
VR_VoiceBoxCatalogAudio::InitialMusicPersonData()
{
    VR_LOGD_FUNC();
    const int MAXOURCEIDNUM = 4;
    for (int i = 1; i <= MAXOURCEIDNUM; ++i) {
        MusicRemoveAllGrammars(std::to_string(i));
    }
}

void
VR_VoiceBoxCatalogAudio::MusicRemoveAllGrammars(const std::string& strSourceId)
{
    if (NULL == m_agentDataCommandMusic.ptr() || strSourceId.empty()) {
        VR_LOGE("m_agentDataCommandMusic is NULL");
        return;
    }

    HRESULT retCode = m_agentDataCommandMusic->SetDataSynchronized(NULL, _T(""), _T(""), _T(""), VBT_TRUE);
    if (S_OK != retCode) {
        VR_LOGE("SetDataSynchronized: 0x%lx", retCode);
        return;
    }

    CVECIPtr<IVECITransaction> spTrans;
    retCode = m_agentDataCommandMusic->RemoveAllGrammars(&spTrans, strSourceId.c_str(), "");
    if (FAILED(retCode) || NULL == spTrans.ptr()) {
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
    VR_LOGI("strTransaction = %s", strTransaction.c_str());
    m_engineCallback.SetCurrentTransaction("music", strTransaction);

    VBT_BOOL bTimeout = VBT_FALSE;
    const long OVER_TIME = 20000;
    retCode = spTrans->WaitForCompletion(OVER_TIME, &bTimeout);
    if (FAILED(retCode) || bTimeout) {
        VR_LOGE("WaitForCompletion failed: %lx or RemoveAllGrammars over time", retCode);
        return;
    }

    VR_LOGI("MusicRemoveAllGrammars end : strTransaction = %s, strSourceId = %s", strTransaction.c_str(), strSourceId.c_str());
}

void
VR_VoiceBoxCatalogAudio::CancelMusicGrammarUpdate(bool bSync)
{
    VR_LOGD_FUNC();
    if (NULL == m_agentDataCommandMusic.ptr()) {
        VR_LOGE("m_agentDataCommandMusic is NULL");
        return;
    }
    CVECIPtr<IVECITransaction> spTrans;
    CVECIOutStr strTrans;
    HRESULT retCode = m_agentDataCommandMusic->CancelGrammarUpdate(&spTrans);
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

    retCode = spTrans->GetTransactionId(&strTrans);
    if (FAILED(retCode) || NULL == strTrans.Get()) {
        VR_LOGE("GetTransactionId: 0x%lx or strTrans.Get() is NULL", retCode);
        return;
    }

    std::string strTransaction = strTrans.Get();
    VR_LOGI("strTransaction = %s", strTransaction.c_str());
}

void
VR_VoiceBoxCatalogAudio::CancelRadioGrammarUpdate(bool bSync)
{
    if (NULL == m_agentDataCommandRadio.ptr()) {
        VR_LOGE("m_agentDataCommandRadio is NULL");
        return;
    }

    CVECIPtr<IVECITransaction> spTrans;
    CVECIOutStr strTrans;
    HRESULT retCode = m_agentDataCommandRadio->CancelGrammarUpdate(&spTrans);
    if (S_OK != retCode || NULL == spTrans.ptr()) {
        VR_LOGE("CancelGrammarUpdate: 0x%lx", retCode);
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

std::string
VR_VoiceBoxCatalogAudio::GetSourceId()
{
    return m_strSourceId;
}

std::string
VR_VoiceBoxCatalogAudio::GetDBPath()
{
    return m_strDBPath;
}

bool
VR_VoiceBoxCatalogAudio::GetIsIPodUSB2()
{
    return m_bIPodUSB2;
}

bool
VR_VoiceBoxCatalogAudio::GetIsUSBIPod2()
{
    return m_bUSBIPod2;
}

bool
VR_VoiceBoxCatalogAudio::GetIsIpodActive()
{
    return m_bIpodActive;
}
/* EOF */
