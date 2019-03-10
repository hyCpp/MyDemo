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
 * @file VR_DataAccessorManagerEU.h
 * @brief Declaration file of VR_DataAccessorManagerEU.
 *
 * This file includes the declaration of VR_DataAccessorManagerEU.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DATA_ACCESSOR_MANAGER_EU_H
#define VR_DATA_ACCESSOR_MANAGER_EU_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_DataAccessorManager.h"
#include "VR_DialogEngineAsrDataManager.h"

enum class VR_StationType;

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DataAccessorContactEU);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DataAccessorMediaEU);

namespace N_Vr
{
namespace N_Asr
{
class C_Event_Phase;
class C_Event_Notify;
}
}

/**
 * @brief The VR_DataAccessorManagerEU class
 *
 * provide interface for VR_DataAccessorManagerEU
 */

class VR_DataAccessorManagerEU : public VR_DataAccessorManager
{
public:
    VR_DataAccessorManagerEU(VR_AsrRequestor* asrRequestor, VR_DECommonIF* common, VR_ConfigureIF* config);
    virtual ~VR_DataAccessorManagerEU();

    std::string getStationBand(int stationGrammarID);
    int getStationID(int stationGrammarID);
    int getStationGrammarID(int stationID, VR_StationType type);

    void handleMusicOneshotResult(pugi::xml_node &result);

    bool checkPhoneTypeAvailable(int phoneTypeID);
    void processChangeVrLanguage(std::string language);
    void handleAsrMusicStepByStepResult(pugi::xml_node &result);

    virtual void updateGrammar(const std::string &grammarMsg) override;
    virtual void resetGrammarGeneration() override;

    bool getGracenoteSupportedEnable();
    void resetAsrVMPStatus(const std::string& language);
    void getGracenoteMagicString(const std::string &path, std::string &magicString);
    spC_Term getGracenoteAsrCterm(std::string& musicCategory, std::string& mediaName);
    void notifyLoadGracenoteFinish(const std::string &magicString, bool loadStatus);

protected:
    spVR_DataAccessorContactEU m_accessorContactEU;
    spVR_DataAccessorMediaEU m_accessorMediaEU;
    VR_DialogEngineAsrDataManager m_asrDataManager;

    VoiceList<std::pair<pugi_vr::sp_xml_document, std::string>>::type m_gracenoteMsgList;

    static bool m_gracenoteSupported;

    void processRadioGrammarNew(VoiceList<spC_Term>::type &addList);

    virtual void handleGrammarNew(pugi::xml_node &category) override;
    virtual void updateOtherGrammarState(const std::string &categoryName) override;

    virtual void setMusicGrammarActive(const std::string &grammarID, bool enable, int songCount, int otherCount) override;
    virtual void setPhoneContactGrammarActive(bool enable) override;

    void saveRadioData(pugi::xml_node &itemNode, VoiceMap<int, std::string>::type & radioMap);
    void preprocessRadioMsg(const std::string &grammarMsg);
    bool isNeedWaitLoadVmp(pugi_vr::sp_xml_document spGrammarMsgDoc);

    virtual void getMusicCTermList(VoiceList<spC_Term>::type &cTermList, const unsigned int &id, const std::string &name, const std::string &pronIDStr = std::string(), const std::string &pronNameStr = std::string());

    static int genMusicCTermListCallbackEU(void* addList, int columnNum, char **columnValue, char **columnName);
    static void generateMusicCTermListEU(VoiceList<spC_Term>::type &cTermList, const unsigned int &id, const std::string &name, const std::string &pronIDStr, const std::string &pronNameStr);
    static int queryMagicStringCallback(void* magicString, int columnNum, char **columnValue, char **columnName);

    void Asr_Load_GracenoteClcCallback(
        const N_Vr::N_Asr::C_Event_Notify &notify,
        std::string magicString);

private:
    std::string selectMusicClcDomainType(const std::string &categoryName);
    void processGracenoteMsgQueue(const std::string callBackMagicString);
    void pushBackMsgToGracenoteMsgList(pugi_vr::sp_xml_document spGrammarMsgDoc, const std::string &magicString);

};

#endif /* VR_DATA_ACCESSOR_MANAGER_EU_H */
/* EOF */
