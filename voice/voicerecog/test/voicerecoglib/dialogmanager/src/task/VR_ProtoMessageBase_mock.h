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
 * @file VR_VBT_EngineCallback_mock.h
 * @brief Mock class for IVECIEngineClient.
 *
 *
 * @attention used for C++ only.
 */

#ifndef CXX_VR_ProtoMessageBase_MOCK_H
#define CXX_VR_ProtoMessageBase_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "message_lite.h"
#include <google/protobuf/stubs/common.h>
#include "VR_Def.h"
#include <string>

/**
* VR_ProtoMessageBase_Mock
*
* The class is a mock class for MessageLite.
*/
class VR_ProtoMessageBase_Mock: public  VR_ProtoMessageBase
{
public:
    // Mock methods
    MOCK_CONST_METHOD0(GetTypeName, std::string());
    MOCK_CONST_METHOD0(New, MessageLite*());
    MOCK_CONST_METHOD0(Clear, void());
    MOCK_CONST_METHOD0(IsInitialized, bool());
    MOCK_CONST_METHOD1(CheckTypeAndMergeFrom, void(const MessageLite&));
    MOCK_CONST_METHOD1(MergePartialFromCodedStream, bool(io::CodedInputStream*));
    MOCK_CONST_METHOD1(SerializeWithCachedSizes, void(io::CodedOutputStream*));
    MOCK_METHOD0(GetCachedSize, int());

};

#endif /* CXX_VR_ProtoMessageBase_MOCK_H */
/* EOF */