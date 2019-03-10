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
#ifndef GTEST_INCLUDE_GTEST_GTEST_H_
#    include "gtest/gtest.h"
#endif

#ifndef GMOCK_INCLUDE_GMOCK_GMOCK_H_
#    include "gmock/gmock.h"
#endif

#ifndef CXX_BL_ERROR_H
#    include "BL_Error.h"
#endif

#ifndef CXX_BL_STRING_H
#    include "BL_String.h"
#endif

#include "VP_Define.h"
#include "VC_Player.h"
#include "VP_DataAccessorList.h"
#include "VP_DataAccessorSound.h"
#include "VC_Player_mock.h"
#include "MEM_vector.h"

#ifndef CXX_VP_DATAACCESSOR_MOCK_H
#    include "VP_DataAccessor_mock.h"
#endif

using ::testing::Return;
using ::testing::_;
using namespace std;

/**
 * Summary for class VP_DataAccessorList_Test
 * 
 * This is a detail description for class VP_DataAccessorList_Test
 *
 */
class VP_DataAccessorList_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        m_pVP_DataAccessorList = make_shared<VP_DataAccessorList>(pDataList, "VPTest", 0);
        player = new(MEM_Voice)MockVC_Player();
    }

    virtual void TearDown() 
    {
        if (nullptr != player) {
            delete player;
            player = nullptr;
        }
    }

public:
    MockVC_Player *player;
    VoiceVector<shared_ptr<VP_DataAccessor>>::type pDataList;
    shared_ptr<VP_DataAccessorList> m_pVP_DataAccessorList;
};

TEST_F(VP_DataAccessorList_Test, Play)
{
    shared_ptr<MockVP_DataAccessor> pDataAccessor = make_shared<MockVP_DataAccessor>();
    pDataList.push_back(pDataAccessor);
    shared_ptr<VP_DataAccessorList> pDataAccessorList = make_shared<VP_DataAccessorList>(pDataList, "VPTest", 0);
    pDataAccessorList->m_curPlayIndex = 0;

    shared_ptr<MockVP_DataAccessor> pData = dynamic_pointer_cast<MockVP_DataAccessor>(
           (*pDataAccessorList)[0]); 
    EXPECT_CALL(*pData, Play(_, _, _, _, _)).WillOnce(Return(APL_ERR_NO_ERROR));
    CL_BOOL isTtsPlay;
    CL_ERROR err = pDataAccessorList->Play(player, nullptr, isTtsPlay, 0, 0);
    EXPECT_EQ(APL_ERR_NO_ERROR, err);
}

TEST_F(VP_DataAccessorList_Test, PlayEmpytElement)
{
    pDataList.push_back(nullptr);
    shared_ptr<VP_DataAccessorList> pDataAccessorList = make_shared<VP_DataAccessorList>(pDataList, "VPTest", 0);
    pDataAccessorList->m_curPlayIndex = 0;

    CL_BOOL isTtsPlay;
    CL_ERROR err = pDataAccessorList->Play(player, nullptr, isTtsPlay, 0, 0);
    EXPECT_EQ(APL_ERR_NO_ERROR, err);
}

TEST_F(VP_DataAccessorList_Test, PlayEmptyList)
{
    m_pVP_DataAccessorList->m_curPlayIndex = 0;

    CL_BOOL isTtsPlay;
    CL_ERROR err = m_pVP_DataAccessorList->Play(player, nullptr, isTtsPlay, 0, 0);
    EXPECT_EQ(APL_ERR_VP_UNKNOWN, err);
}

TEST_F(VP_DataAccessorList_Test, PlayAbortedList)
{
    pDataList.push_back(nullptr);
    shared_ptr<VP_DataAccessorList> pDataAccessorList = make_shared<VP_DataAccessorList>(pDataList, "VPTest", 0);
    pDataAccessorList->m_curPlayIndex = 0;
    pDataAccessorList->abort = true;
    
    CL_BOOL isTtsPlay;
    CL_ERROR err = pDataAccessorList->Play(player, nullptr, isTtsPlay, 0, 0);
    EXPECT_EQ(APL_ERR_VP_USER_STOP, err);
}
    
TEST_F(VP_DataAccessorList_Test, HasNext)
{
    shared_ptr<MockVP_DataAccessor> pDataAccessor = make_shared<MockVP_DataAccessor>();
    pDataList.push_back(pDataAccessor);
    shared_ptr<VP_DataAccessorList> pDataAccessorList = make_shared<VP_DataAccessorList>(pDataList, "VPTest", 0);
    pDataAccessorList->m_curPlayIndex = 0;
    EXPECT_TRUE(pDataAccessorList->HasNext());

    m_pVP_DataAccessorList->m_curPlayIndex = 0;
    EXPECT_FALSE(m_pVP_DataAccessorList->HasNext());
}

TEST_F(VP_DataAccessorList_Test, getSender)
{
    EXPECT_EQ(m_pVP_DataAccessorList->getSender(), "VPTest");
}

TEST_F(VP_DataAccessorList_Test, getUserData)
{
    EXPECT_EQ(m_pVP_DataAccessorList->getUserData(), 0);
}

TEST_F(VP_DataAccessorList_Test, isAbort)
{
    m_pVP_DataAccessorList->setAbort(true);
    EXPECT_TRUE(m_pVP_DataAccessorList->isAbort());

    m_pVP_DataAccessorList->setAbort(false);
    EXPECT_FALSE(m_pVP_DataAccessorList->isAbort());
}

TEST_F(VP_DataAccessorList_Test, getPlayResult)
{
    m_pVP_DataAccessorList->setPlayResult(VP_PLAYRESULT_OK);
    EXPECT_EQ(m_pVP_DataAccessorList->getPlayResult(), 0);
}

/* EOF */
