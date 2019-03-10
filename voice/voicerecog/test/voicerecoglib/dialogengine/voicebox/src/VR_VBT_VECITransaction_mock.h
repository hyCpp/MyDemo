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

#ifndef CXX_VR_VBT_VECITRANSACTION_MOCK_H
#define CXX_VR_VBT_VECITRANSACTION_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "IVBTEngineClient.h"

/**
* MockVR_VBT_VECITransaction
*
* The class is a mock class for IVECITransaction.
*/
class MockVR_VBT_VECITransaction : public IVECITransaction
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
    MOCK_METHOD2(WaitForCompletion, HRESULT(VBT_ULONG ulTimeoutMs, VBT_BOOL *pbTimeout));
    MOCK_METHOD1(GetNumericId, HRESULT(VBT_ULONG *pulId));
    MOCK_METHOD1(GetTransactionId, HRESULT(VBT_TCHAR **pszTransId));
    MOCK_METHOD1(GetResultErrorCode, HRESULT(VBT_ULONG *pulCode));
    MOCK_METHOD1(GetResultCachedContextId, HRESULT(VBT_ULONG *pulId));
    MOCK_METHOD1(GetResultSpeakList, HRESULT(IVECIStringSet **ppList));
    MOCK_METHOD1(GetResultMessage, HRESULT(IVECIParsedMessage **ppMessage));
    MOCK_METHOD2(CompleteWorkItemCmd, HRESULT(IVECITransaction *pParentTrans,
                VBT_ULONG ulTimeoutMs));
    MOCK_METHOD1(GetUserData, HRESULT(IUnknown **ppUnk));
    MOCK_METHOD1(SetUserData, HRESULT(IUnknown *pUnk));
    MOCK_METHOD0(SpeakDone, HRESULT());
    MOCK_METHOD0(ClientRenderDone, HRESULT());
    MOCK_METHOD0(SetBackTag, HRESULT());
};

#endif  // CXX_VR_VBT_VECITRANSACTION_MOCK_H
/* EOF */
