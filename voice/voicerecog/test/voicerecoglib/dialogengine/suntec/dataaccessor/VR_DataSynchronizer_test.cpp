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

#include "gtest/gtest.h"
#include "VR_DataSynchronizer.h"
#include "VR_AsrRequestor_mock.h"
#include "VR_AsrRequestor.h"
#include "Vr_Asr_Engine.h"
#include "Vr_Asr_Event_Phase_mock.h"
#include "Vr_Asr_Event_Notify_mock.h"

using ::testing::_;
using ::testing::Return;

using namespace N_Vr::N_Asr;

/**
* VR_DataSynchronizer_Test
*
* The class is just for VR_DataSynchronizer test.
*/
class VR_DataSynchronizer_Test : public testing::Test
{
public:
    VR_DataSynchronizer_Test()
    {
    }

    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}

private:
    VR_DataSynchronizer_Test(const VR_DataSynchronizer_Test& other);
    void operator= (const VR_DataSynchronizer_Test& other);
};

TEST_F(VR_DataSynchronizer_Test, getCTerm)
{
    const std::string strPron = "pron";
    VR_AsrRequestor_mock asrRequestor;
    VR_DataSynchronizer dataSynchronizer(&asrRequestor);
    spC_Term spCitem = dataSynchronizer.getCTerm(100, "", strPron);
    C_Buffer &cbuf = spCitem->m_vector_buffer_Transcriptions[0];
    EXPECT_EQ(100, spCitem->m_i_User_Data_Lo);
    EXPECT_EQ(strPron.size(), cbuf.m_i_Size);
}

TEST_F(VR_DataSynchronizer_Test, getCTermWithPronID)
{
    VR_AsrRequestor_mock asrRequestor;
    VR_DataSynchronizer dataSynchronizer(&asrRequestor);
    dataSynchronizer.getCTermWithPronID(100, "", 3);
}

TEST_F(VR_DataSynchronizer_Test, deleteGrammar_param1)
{
    const std::string strContextID = "10";
    std::string type = "";
    VR_AsrRequestor_mock asrRequestor;
    VR_DataSynchronizer dataSynchronizer(&asrRequestor);
    EXPECT_FALSE(dataSynchronizer.deleteGrammar(strContextID, type));
}

TEST_F(VR_DataSynchronizer_Test, deleteGrammar_param2)
{
    const std::string strContextID = "10";
    std::string type = "";
    VR_AsrRequestor_mock asrRequestor;
    VR_DataSynchronizer dataSynchronizer(&asrRequestor); 
    boost::function<void(N_Vr::N_Asr::C_Event_Phase const &)> phaseCallbackFuncPoint = boost::bind(&VR_DataSynchronizer::phaseCallback, &dataSynchronizer, _1);
    EXPECT_FALSE(dataSynchronizer.deleteGrammar(strContextID, type, phaseCallbackFuncPoint));
}

TEST_F(VR_DataSynchronizer_Test, deleteGrammar_param3)
{
    const std::string strContextID = "10";
    std::string type = "";
    VR_AsrRequestor_mock asrRequestor;
    VR_DataSynchronizer dataSynchronizer(&asrRequestor);
    VoiceList<spC_Term>::type delList;
    boost::function<void(N_Vr::N_Asr::C_Event_Phase const &)> phaseCallbackFuncPoint = boost::bind(&VR_DataSynchronizer::phaseCallback, &dataSynchronizer, _1);
    EXPECT_FALSE(dataSynchronizer.deleteGrammar(strContextID, type, delList, phaseCallbackFuncPoint));
}

TEST_F(VR_DataSynchronizer_Test, updateGrammar_param4_bool)
{
    const std::string strContextID = "10";
    std::string type = "";
    VR_AsrRequestor_mock asrRequestor;
    VR_DataSynchronizer dataSynchronizer(&asrRequestor);
    VoiceList<spC_Term>::type delList;
    EXPECT_FALSE(dataSynchronizer.updateGrammar(strContextID, type, delList, true));
}

TEST_F(VR_DataSynchronizer_Test, updateGrammar_param3_list2)
{
    const std::string strContextID = "10";
    std::string type = "";
    VoiceList<spC_Term>::type delList;
    VoiceList<spC_Term>::type addList;
    VR_AsrRequestor_mock asrRequestor;
    VR_DataSynchronizer dataSynchronizer(&asrRequestor);
    EXPECT_FALSE(dataSynchronizer.updateGrammar(strContextID, type, addList, delList));
}

TEST_F(VR_DataSynchronizer_Test, genVoiceTagPhoneme)
{
    VR_AsrRequestor_mock asrRequestor;
    VR_DataSynchronizer dataSynchronizer(&asrRequestor);
    dataSynchronizer.genVoiceTagPhoneme();
    SUCCEED();
}

TEST_F(VR_DataSynchronizer_Test, updateGrammar_param6)
{
    const std::string strContextID = "10";
    std::string type = "";
    VoiceList<spC_Term>::type delList;
    VoiceList<spC_Term>::type addList;
    VR_AsrRequestor_mock asrRequestor;
    VR_DataSynchronizer dataSynchronizer(&asrRequestor);
    boost::function<void(N_Vr::N_Asr::C_Event_Phase const &)> phaseCallbackFuncPoint;
    boost::function<void(N_Vr::N_Asr::C_Event_Notify const &)> notifyCallbackFuncPoint;
    EXPECT_FALSE(dataSynchronizer.updateGrammar(strContextID, type, addList, delList, phaseCallbackFuncPoint, notifyCallbackFuncPoint, true));
}

TEST_F(VR_DataSynchronizer_Test, updateGrammar_param3_bool)
{
    const std::string strContextID = "10";
    std::string type = "";
    VR_AsrRequestor_mock asrRequestor;
    ON_CALL(asrRequestor, updateGrammar(_)).WillByDefault(Return(true));
    VR_DataSynchronizer dataSynchronizer(&asrRequestor);
    VoiceList<spC_Term>::type delList;
    EXPECT_FALSE(dataSynchronizer.updateGrammar(strContextID, type, delList, true));
}

TEST_F(VR_DataSynchronizer_Test, stopGrammar)
{
    VR_AsrRequestor_mock asrRequestor;
    VR_DataSynchronizer dataSynchronizer(&asrRequestor);
    dataSynchronizer.m_asrRequestIdList.push_back(2);
    dataSynchronizer.stopGrammar();
    SUCCEED();
}


TEST_F(VR_DataSynchronizer_Test, updateGrammarCategoryFinish)
{
    VR_AsrRequestor_mock asrRequestor;
    VR_DataSynchronizer dataSynchronizer(&asrRequestor);
    dataSynchronizer.m_asrRequestIdList.push_back(2);
    dataSynchronizer.updateGrammarCategoryFinish("pengjialing");
    SUCCEED();
}

TEST_F(VR_DataSynchronizer_Test, phaseCallback)
{
    VR_AsrRequestor_mock asrRequestor;
    VR_DataSynchronizer dataSynchronizer(&asrRequestor);
    C_Event_Phase_mock phaseEvent;
    dataSynchronizer.phaseCallback(phaseEvent);
    SUCCEED();
}

TEST_F(VR_DataSynchronizer_Test, notifyCallback)
{
    VR_AsrRequestor_mock asrRequestor;
    VR_DataSynchronizer dataSynchronizer(&asrRequestor);
    C_Event_Notify_mock notify;
    dataSynchronizer.notifyCallback(notify);
    SUCCEED();
}


/* EOF */
