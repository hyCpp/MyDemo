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
 * @file VR_VBT_VECIString_mock.h
 * @brief Mock class for IVECIString.
 *
 *
 * @attention used for C++ only.
 */

#ifndef CXX_VR_VBT_VECISTRING_MOCK_H
#define CXX_VR_VBT_VECISTRING_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "IVBTEngineClient.h"

/**
* MockVR_VBT_VECIString
*
* The class is a mock class for IVECIString.
*/
class MockVR_VBT_VECIString : public IVECIString
{
public:
    // Implementation for IUnknown's functions
    HRESULT QueryInterface(REFIID riid, void** ppvObj)
    {
        return S_OK;
    }

    VBT_ULONG AddRef()
    {
        return 1;
    }

    VBT_ULONG Release()
    {
        return 0;
    }

    // Mock methods
    MOCK_METHOD1(Reserve, HRESULT(VBT_ULONG ulSize));
    MOCK_METHOD1(GetString, HRESULT(VBT_TCHAR **pszString));
    MOCK_METHOD1(SetString, HRESULT(VBT_CSTR szString));
    MOCK_METHOD1(GetLength, HRESULT(VBT_ULONG *pulLength));
    MOCK_METHOD1(GetLengthCP, HRESULT(VBT_ULONG *pulLength));
    MOCK_METHOD3(IsEqual, HRESULT(
                VBT_CSTR szString,
                VBT_BOOL bCaseSensitive,
                VBT_BOOL *pbEqual));
    MOCK_METHOD1(Append, HRESULT(VBT_CSTR szString));
    MOCK_METHOD2(Replace, HRESULT(
                VBT_CSTR szOld,
                VBT_CSTR szNew));
    MOCK_METHOD2(Contains, HRESULT(
                VBT_CSTR szString,
                VBT_BOOL *pbContains));
    MOCK_METHOD2(AsNumber, HRESULT(
                VBT_ULONG ulBase,
                VBT_ULONG *pulNumber));
    MOCK_METHOD0(Trim, HRESULT());
    MOCK_METHOD0(ToLower, HRESULT());
    MOCK_METHOD0(ToUpper, HRESULT());
    MOCK_METHOD2(Split, HRESULT(
                VBT_CSTR szDelimiters,
                IVECIStringSet **ppTokens));
    MOCK_METHOD1(ToUnicode, HRESULT(VBT_ULONG ulCodePage));
    MOCK_METHOD1(FromUnicode, HRESULT(VBT_ULONG ulCodePage));
    MOCK_METHOD2(Insert, HRESULT(
                VBT_ULONG ulStartIndex,
                VBT_CSTR szValue));
    MOCK_METHOD0(Clear, HRESULT());
};

#endif  // CXX_VR_VBT_VECISTRING_MOCK_H
/* EOF */
