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
 * @file VP_DataAccessor_mock.h                                            
 * @brief Summary for MockVP_DataAccessor.
 *
 * This file includes the declaration of class MockVP_DataAccessor and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef CXX_VP_DATAACCESSOR_MOCK_H
#define CXX_VP_DATAACCESSOR_MOCK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "VP_DataAccessorSound.h"
#include "VP_DataAccessorList.h"

/**
 * Summary for class MockVP_DataAccessor
 * 
 * This is a detail description for class MockVP_DataAccessor
 *
 */
class MockVP_DataAccessor : public VP_DataAccessor 
{
public:
    MOCK_METHOD5(Play, CL_ERROR(VC_Player* pPlayer, VC_Player::Listener *pListener,
                CL_BOOL& isTtsPlay, int reqId, int tokenid));
};

/**
 * Summary for class MockVP_DataAccessorList
 *
 * This is a detail description for class MockVP_DataAccessorList
 *
 */
class MockVP_DataAccessorList : public VP_DataAccessorList
{
public:
    MOCK_METHOD5(Play, CL_ERROR(VC_Player* pPlayer, VC_Player::Listener *pListener, 
            CL_BOOL& isTtsPlay, int reqId, int tokenid));
    MOCK_CONST_METHOD0(HasNext, CL_BOOL()); 
};

#endif // CXX_VP_DATAACCESSOR_MOCK_H
/* EOF */
