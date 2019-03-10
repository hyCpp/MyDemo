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

#ifndef CXX_VR_VBT_VECIPARAMETERSET_MOCK_H
#define CXX_VR_VBT_VECIPARAMETERSET_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "IVBTEngineClient.h"
#include "VECITypes.h"
/**
* MockVR_VBT_VECIParameterSet
*
* The class is a mock class for IVECIParameterSet.
*/
class MockVR_VBT_VECIParameterSet : public IVECIParameterSet
{
public:
    // Implementation for IVECIIterable's functions
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

    HRESULT GetIterator(IVECIIterator **ppIterator)
    {
        return S_OK;
    }

    HRESULT LockList()
    {
        return S_OK;
    }

    HRESULT UnlockList()
    {
        return S_OK;
    }

    // Mock methods
    MOCK_METHOD1(GetSize, HRESULT(VBT_ULONG *pulSize));
    MOCK_METHOD3(GetParameter, HRESULT(VBT_ULONG ulIndex,
                VeciParamAttribute enAttrib,
                VBT_TCHAR **pszValue));
    MOCK_METHOD3(GetParameterInfo, HRESULT(VBT_CSTR szName,
                VeciParamAttribute enAttrib,
                VBT_TCHAR **pszValue));
    MOCK_METHOD4(AddParameter, HRESULT(VBT_CSTR szName,
                VBT_CSTR szValue,
                VBT_CSTR szDisplay,
                VBT_CSTR szRequired));
    MOCK_METHOD4(UpdateParameter, HRESULT(VBT_CSTR szName,
                VBT_CSTR szValue,
                VBT_CSTR szDisplay,
                VBT_CSTR szRequired));
};

#endif  // CXX_VR_VBT_VECIPARAMETERSET_MOCK_H
/* EOF */
