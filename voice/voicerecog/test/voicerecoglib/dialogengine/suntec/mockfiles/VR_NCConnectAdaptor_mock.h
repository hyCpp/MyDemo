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
 * @file VR_NCConnectAdaptor_mock.h
 * @brief mock definition for VR_INCConnectAdaptor
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_NCCONNECTADAPTOR_MOCK_H
#define VR_NCCONNECTADAPTOR_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VR_NCConnectAdaptor.h"

/**
 * VR_NCConnectAdaptor_mock
 *
 * Mock class definition for VR_NCConnectAdaptor
 */
class VR_NCConnectAdaptor_mock : public VR_INCConnectAdaptor
{
public:
    MOCK_METHOD1(setReplyReceiver, VOID(ncsp<NEReplyReceiver>::sp));
    MOCK_METHOD0(bindService,      NC_BOOL());
    MOCK_METHOD0(unbindService,    VOID());
    MOCK_METHOD4(request,          VOID(const NCString&, const NCString&
                                        , const NCString&, NCString&));
    MOCK_METHOD5(request,          VOID(const NCString&, const NCString&
                                        , const NCString&, const NCString&, NCString&));
    MOCK_METHOD1(release,          VOID(const NCString&));
    MOCK_METHOD2(replyInterrupt,   VOID(const NCString&, const NCString&));
    MOCK_METHOD1(stopWait,         VOID(const NCString&));
};

#endif /* VR_NCCONNECTADAPTOR_MOCK_H */
/* EOF */
