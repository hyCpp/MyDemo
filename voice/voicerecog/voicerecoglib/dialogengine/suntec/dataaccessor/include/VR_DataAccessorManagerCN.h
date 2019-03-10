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
 * @file VR_DataAccessorManagerCN.h
 * @brief Declaration file of VR_DataAccessorManagerCN.
 *
 * This file includes the declaration of VR_DataAccessorManagerCN.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DATA_ACCESSOR_MANAGER_CN_H
#define VR_DATA_ACCESSOR_MANAGER_CN_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_DataAccessorManager.h"

/**
 * @brief The VR_DataAccessorManagerCN class
 *
 * VR_DataAccessorManager for CHINA region
 */

class VR_DataAccessorManagerCN : public VR_DataAccessorManager
{
public:
    VR_DataAccessorManagerCN(VR_AsrRequestor* asrRequestor, VR_DECommonIF* common, VR_ConfigureIF* config);
    virtual ~VR_DataAccessorManagerCN() {}

    void setMusicGrammarActive(const std::string &grammarID, bool enable, int songCount, int otherCount) override;
    void setPhoneContactGrammarActive(bool enable) override;

    void notifyGrammarCallback(const N_Vr::N_Asr::C_Event_Notify &notifyEvent, const std::string &category, const std::string &contextID, unsigned int totalCount = 0) override;
    void deletePhoneGrammarCallback(const N_Vr::N_Asr::C_Event_Phase &phaseEvent, const std::string &category, SaveGrammarTaskInfo &info);
    void requestUpdateGrammar(N_Vr::N_Asr::C_Request_Context_List_Update &updateMsg) override;
    void checkDirSizeForIFlytek() override;

    void onSaveGrammarToFile(const std::string &sourceID) override;
    void onUpdateGrammarCategoryFailed(const std::string &sourceID, const std::string &size) override;
    void handleGrammarSpaceNotEnough(const std::string &infoMsg) override;
    void handleGrammarSpaceNotEnough(SaveGrammarTaskInfo &info) override;

private:
    VoiceList<N_Vr::N_Asr::C_Request_Context_List_Update>::type m_updateMsgList;

    std::string getCategoryByContextID(const std::string &contextID);
};

#endif /* VR_DATA_ACCESSOR_MANAGER_CN_H */
/* EOF */
