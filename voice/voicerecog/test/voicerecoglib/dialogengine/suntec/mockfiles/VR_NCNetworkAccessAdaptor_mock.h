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
 * @file VR_NCNetworkAccessAdaptor_mock.h
 * @brief mock definition for VR_INCNetworkAccessAdaptor
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_NCNETWORKACCESSADAPTOR_MOCK_H
#define VR_NCNETWORKACCESSADAPTOR_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VR_NCNetworkAccessAdaptor.h"

/**
 * VR_NCNetworkAccessAdaptor_mock
 *
 * Mock class definition for VR_NCNetworkAccessAdaptor
 */
class VR_NCNetworkAccessAdaptor_mock : public VR_INCNetworkAccessAdaptor
{
public:
    MOCK_METHOD1(setReplyReceiver, VOID(ncsp<NEReplyReceiver>::sp));
    MOCK_METHOD0(bindService,      NC_BOOL());
    MOCK_METHOD0(unbindService,    VOID());
    MOCK_METHOD3(request,          INT32(const NCString&, const NCString&, const AccessMode&));
    MOCK_METHOD1(release,          VOID(const NCString&));
};

#endif /* VR_NCNETWORKACCESSADAPTOR_MOCK_H */
/* EOF */
