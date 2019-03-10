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
 * @file VR_VBT_VECIEngineClient_mock.h
 * @brief Mock class for IVECIEngineClient.
 *
 *
 * @attention used for C++ only.
 */

#ifndef CXX_VR_VBT_VECIENGINECLIENT_MOCK_H
#define CXX_VR_VBT_VECIENGINECLIENT_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#ifndef IVBTENGINECLIENT_H
#include "IVBTEngineClient.h"
#endif
/**
* MockVR_VBT_VECIEngineClient
*
* The class is a mock class for IVECIEngineClient.
*/
class MockVR_VBT_VECIEngineClient : public IVECIEngineClient
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
    MOCK_METHOD3(Init, HRESULT(VBT_CSTR szName,
                VBT_CSTR szRuntimeDir,
                VBT_CSTR szCfgFileName));
    MOCK_METHOD0(Shutdown, HRESULT());
    MOCK_METHOD3(RegisterEventSink, HRESULT(VBT_INT nMask,
                VBT_BOOL bEavesdrop,
                IVECIEventSink *pEventSink));
    MOCK_METHOD1(UnregisterEventSink, HRESULT(IVECIEventSink *pEventSink));
    MOCK_METHOD1(RegisterLogger, HRESULT(IVECILogger *pLogger));
    MOCK_METHOD1(RegisterWorkItemHandler, HRESULT(IVECIWorkItemHandler *pHandler));
    MOCK_METHOD2(SetProperty, HRESULT(VeciClientProperty type, VARIANT *pValue));
    MOCK_METHOD2(GetProperty, HRESULT(VeciClientProperty type, VARIANT *pValue));
    MOCK_METHOD1(CreateEngineDispatcher, HRESULT(IVECIEngineCommand **ppCmd));
    MOCK_METHOD1(CreateAgentDispatcher, HRESULT(IVECIAgentDataCommand **ppCmd));
    MOCK_METHOD1(CreateMessageWriter, HRESULT(IVECIMessageWriter **ppWriter));
    MOCK_METHOD1(CreateParameterSet, HRESULT(IVECIParameterSet **ppParams));
    MOCK_METHOD1(CreateListItems, HRESULT(IVECIListItems **ppItems));
    MOCK_METHOD1(CreateString, HRESULT(IVECIString **ppString));
    MOCK_METHOD1(CreateStringSet, HRESULT(IVECIStringSet **ppStringSet));
    MOCK_METHOD1(ProcessNextItem, HRESULT(VeciQueueType type));
    MOCK_METHOD2(SendClientMessage, HRESULT(VBT_CSTR szTarget, VBT_CSTR szMessage));
    MOCK_METHOD2(CopyDatabase, HRESULT(VBT_CSTR szSource, VBT_CSTR szTarget));
    MOCK_METHOD1(LogMessage, HRESULT(VBT_CSTR szMessage));
    MOCK_METHOD3(QueryMappingDb, HRESULT(VBT_CSTR szTable,
                VBT_CSTR szVal,
                IVECIParameterSet **ppResult));
    MOCK_METHOD1(InitEx, HRESULT(IVECIInitParams *pInitParams));
};

#endif  // CXX_VR_VBT_VECIENGINECLIENT_MOCK_H
/* EOF */
