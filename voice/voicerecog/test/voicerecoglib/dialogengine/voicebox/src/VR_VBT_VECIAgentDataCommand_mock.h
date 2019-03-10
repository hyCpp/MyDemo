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

#ifndef VR_VBT_VECIAGENTDATACOMMAND_MOCK_H
#define VR_VBT_VECIAGENTDATACOMMAND_MOCK_H

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
class MockVR_VBT_VECIAgentDataCommand : public IVECIAgentDataCommand
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

    // mock methods
    MOCK_METHOD1(Init, HRESULT(VBT_CSTR szAgentName));
    MOCK_METHOD4(GetDataConfiguration, HRESULT(
                    IVECITransaction **ppTrans,
                    VBT_CSTR szSourceId,
                    VBT_CSTR szHandlerName,
                    VBT_CSTR szTableName));

    MOCK_METHOD5(SetDataSynchronized, HRESULT(
                    IVECITransaction **ppTrans,
                    VBT_CSTR szSourceId,
                    VBT_CSTR szTableName,
                    VBT_CSTR szHandlerName,
                    VBT_BOOL bDataSynchronized));
    MOCK_METHOD4(SetDataActiveSource, HRESULT(
                    IVECITransaction **ppTrans,
                    VBT_CSTR szActiveSourceId,
                    VBT_CSTR szTableName,
                    VBT_CSTR szHandlerName));
    MOCK_METHOD4(AddData, HRESULT(
                    IVECITransaction **ppTrans,
                    VBT_CSTR szSourceId,
                    VBT_CSTR szTableName,
                    IVECIListItems *pData));
    MOCK_METHOD4(ReloadData, HRESULT(
                    IVECITransaction **ppTrans,
                    VBT_CSTR szSourceId,
                    VBT_CSTR szTableName,
                    IVECIListItems *pData));
    MOCK_METHOD4(RemoveData, HRESULT(
                    IVECITransaction **ppTrans,
                    VBT_CSTR szSourceId,
                    VBT_CSTR szTableName,
                    IVECIListItems *pData));
    MOCK_METHOD3(RemoveAllData, HRESULT(
                    IVECITransaction **ppTrans,
                    VBT_CSTR szSourceId,
                    VBT_CSTR szTableName));
    MOCK_METHOD5(AddGrammar, HRESULT(
                    IVECITransaction **ppTrans,
                    VBT_CSTR szSourceId,
                    VBT_CSTR szTableName,
                    VBT_CSTR szHandlerName,
                    IVECIListItems *pData));
    MOCK_METHOD3(ReloadGrammar, HRESULT(
                    IVECITransaction **ppTrans,
                    VBT_CSTR szSourceId,
                    VBT_CSTR szTableName));
    MOCK_METHOD5(RemoveGrammar, HRESULT(
                      IVECITransaction **ppTrans,
                    VBT_CSTR szSourceId,
                    VBT_CSTR szTableName,
                    VBT_CSTR szHandlerName,
                    IVECIListItems *pData));
    MOCK_METHOD3(RemoveAllGrammars, HRESULT(
                    IVECITransaction **ppTrans,
                    VBT_CSTR szSourceId,
                    VBT_CSTR szTableName));
    MOCK_METHOD1(PauseGrammarUpdate, HRESULT(
                    IVECITransaction **ppTrans));
    MOCK_METHOD1(ResumeGrammarUpdate, HRESULT(
                    IVECITransaction **ppTrans));
    MOCK_METHOD1(CancelGrammarUpdate, HRESULT(
                    IVECITransaction **ppTrans));
};

#endif  // VR_VBT_VECIAGENTDATACOMMAND_MOCK_H
/* EOF */
