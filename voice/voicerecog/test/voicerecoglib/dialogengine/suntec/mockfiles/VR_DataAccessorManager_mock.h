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
 * @file VR_AsrRequestor_mock.h
 * @brief Declaration file of VR_AsrRequestor_mock.
 *
 * This file includes the declaration of VR_AsrRequestor_mock.
 *
 * @attention used for C++ only.
 */

#ifndef VR_VR_DATAACCESSORMANAGER_MOCK_H
#define VR_VR_DATAACCESSORMANAGER_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VR_DataAccessorManager.h"

/**
* VR_AsrRequestor_mock
*
* The class is a mock class for VR_AsrRequestor.
*/
class VR_DataAccessorManager_mock : public VR_DataAccessorManager
{
public:
    VR_DataAccessorManager_mock(VR_AsrRequestor* asrRequestor, VR_DECommonIF* common, VR_ConfigureIF* config)
    : VR_DataAccessorManager(asrRequestor, common, config)
    {

    }

    MOCK_METHOD1(updateState, void(const std::string &stateMsg));
    MOCK_METHOD1(getUpdateState, void(std::string& stateMsg));
    MOCK_METHOD1(setUpdateStateCallback, void(boost::function<void(const std::string &)> &callback));
    MOCK_METHOD1(updateGrammar, void(const std::string &grammarMsg));
    MOCK_METHOD4(saveVoiceTagGrammar, bool(const std::string &voiceTagID, const std::string &name, const std::string &phoneme, const std::string &deviceAddress));
    MOCK_METHOD2(deleteVoiceTagGrammar, bool(const VoiceList<std::pair<std::string, std::string>>::type &deleteList, const std::string &deviceAddress));
    MOCK_METHOD1(deleteAllVoiceTagGrammar, bool(const std::string &deviceAddress));
    MOCK_METHOD0(clearVoiceTagGrammar, bool());
    MOCK_METHOD3(getInfo, void(const std::string &operation, const std::string &reqMsg, std::string &response));
    MOCK_METHOD1(setCountryID, void(const std::string &countryID));

    MOCK_METHOD6(saveVoiceTag, bool(const std::string &deviceAddress, const std::string &voiceTagID, const std::string &pcmPath, const std::string &phoneme, const std::string &contactMsg, bool update));
    MOCK_METHOD1(getVoiceTagPCMPath, std::string(const std::string &voiceTagID));
    MOCK_METHOD2(deleteVoiceTag, bool(const std::string &deviceAddress, const std::string &voiceTagID));
    MOCK_METHOD1(deleteAllVoiceTag, bool(const std::string &deviceAddress));
    MOCK_METHOD0(clearVoiceTag, bool());
    MOCK_METHOD2(syncVoiceTag, bool(const std::string &deviceAddress, VoiceList<std::string>::type &btVoiceTagIDList));
    MOCK_METHOD2(updateVoiceTag, bool(const std::string &voiceTagID, const pugi::xml_node &contactNode));
    MOCK_METHOD1(setCurrentDevice, void(const std::string &deviceAddress));
    MOCK_METHOD0(clearCurrentDevice, void());
    MOCK_METHOD1(getContactInfo, std::string(const std::string &voiceTagID));
    MOCK_METHOD0(getVoiceTagCount, int());
    MOCK_METHOD1(getVoiceTagGrammarParam, void(VoiceList<VoiceTagGrammarParam>::type &paramList));

};

#endif
/* EOF */
