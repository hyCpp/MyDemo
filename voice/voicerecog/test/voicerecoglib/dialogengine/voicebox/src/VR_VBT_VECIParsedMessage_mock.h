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
 * @file VR_VBT_VECITransaction_mock.h
 * @brief Mock class for IVECITransaction.
 *
 *
 * @attention used for C++ only.
 */

#ifndef CXX_VR_VBT_VECIPARSEDMESSAGE_MOCK_H
#define CXX_VR_VBT_VECIPARSEDMESSAGE_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "IVBTEngineClient.h"

/**
* MockVR_VBT_VECIParsedMessage
*
* The class is a mock class for IVECIParsedMessage.
*/
class MockVR_VBT_VECIParsedMessage : public IVECIParsedMessage
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

    ~MockVR_VBT_VECIParsedMessage()
    {
    }

    // Mock methods
    MOCK_METHOD1(GetString, HRESULT(VBT_TCHAR **pszString));
    MOCK_METHOD1(GetMsgType, HRESULT(VeciMessageType *pType));
    MOCK_METHOD1(GetMsgSource, HRESULT(VBT_TCHAR **pszAttribute));
    MOCK_METHOD1(GetMsgClass, HRESULT(VBT_TCHAR **pszClass));
    MOCK_METHOD1(GetActionType, HRESULT(VBT_TCHAR **pszActionType));
    MOCK_METHOD1(GetActionClass, HRESULT(VBT_TCHAR **pszActionType));
    MOCK_METHOD2(GetActionParameterValue, HRESULT(
                VBT_CSTR szName,
                VBT_TCHAR **pszValue));
    MOCK_METHOD1(GetActionParameters, HRESULT(IVECIParameterSet **ppParams));
    MOCK_METHOD1(GetUCID, HRESULT(VBT_TCHAR **pszUCID));
    MOCK_METHOD1(GetAgentName, HRESULT(VBT_TCHAR **pszName));
    MOCK_METHOD1(GetListType, HRESULT(VBT_TCHAR **pszListType));
    MOCK_METHOD1(GetListClass, HRESULT(VBT_TCHAR **pszListClass));
    MOCK_METHOD1(GetListSize, HRESULT(VBT_ULONG *pulSize));
    MOCK_METHOD2(GetListItem, HRESULT(VBT_ULONG ulItemIndex, IVECIParameterSet **ppParams));
    MOCK_METHOD3(GetListItemParameterValue, HRESULT(
                VBT_ULONG ulItemIndex,
                VBT_CSTR szParamName,
                VBT_TCHAR **pszValue));
    MOCK_METHOD2(GetListItemDisplayValue, HRESULT(
                VBT_ULONG ulItemIndex,
                VBT_TCHAR **pszValue));
    MOCK_METHOD2(GetValueByKey, HRESULT(
                VBT_CSTR szPath,
                VBT_TCHAR **pszValue));
    MOCK_METHOD2(GetMessageParameterValue, HRESULT(
                VBT_CSTR szName,
                VBT_TCHAR **pszValue));
};

#endif  // CXX_VR_VBT_VECIPARSEDMESSAGE_MOCK_H
/* EOF */
