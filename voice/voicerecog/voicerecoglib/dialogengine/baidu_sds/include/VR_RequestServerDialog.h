/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#ifndef VR_REQUESTSERVERDIALOG_H
#define VR_REQUESTSERVERDIALOG_H

#ifndef CXX_PNETASYNCREQUEST_H
#   include "PNetASyncRequest.h"
#endif

#include <memory>
#include "BL_String.h"
#include "BL_Base.h"
#include "VR_Platform.h"
#include "VR_SDSStateMachine.h"

class ServerDialogNetRequest;
class VR_RequestServerDialog
{
public:
    VR_RequestServerDialog(std::shared_ptr<VR_SDSStateMachine> listener,
                           DWORD dwReqID, const BL_String &szUrl);
    virtual ~VR_RequestServerDialog();
    virtual VOID DoAction();
    virtual CL_BOOL CancelAction();
    virtual VOID DoCallBackAction();

    VOID SetData(BYTE* bData, DWORD dwSize) { m_bData = bData; m_dwSize = dwSize; }
    VOID SetErrCode(DWORD dwErrCode) { m_dwErrCode = dwErrCode; }

    virtual const XCHAR* GetClassTypeName()const
    {
        return ClassTypeName();
    }

    static const XCHAR* ClassTypeName()
    {
        return "VR_RequestServerDialog";
    }

    DWORD GetReqID() const { return m_dwReqID; }

protected:
    virtual const XCHAR* GetReqName() const;

private:
    std::shared_ptr<VR_SDSStateMachine> m_pListener;
    DWORD   m_dwReqID;
    BL_String m_szUrl;
    ServerDialogNetRequest* m_pcRequest;
    BYTE* m_bData;
    DWORD m_dwSize;
    DWORD m_dwErrCode;
};

class ServerDialogNetRequest : public PNetASyncRequest
{
public:
    ServerDialogNetRequest(BL_String szUrl, VR_RequestServerDialog* pcRequest);
    virtual ~ServerDialogNetRequest() {}

private:
    class ServerDialogNetCallBack : public PNetASyncReqCallback
    {
    public:
        ServerDialogNetCallBack(){}
        ServerDialogNetCallBack(VR_RequestServerDialog* pcRequest):m_pcRequest(pcRequest),m_bTried(CL_FALSE){}
        VOID SetRequest(VR_RequestServerDialog* pcRequest)
        {
            m_pcRequest = pcRequest;
        }
    protected:
        virtual VOID DoAction(PNetASyncRequest* pcReqeust);

    private:
        VR_RequestServerDialog* m_pcRequest;
        CL_BOOL m_bTried;
    };
    ServerDialogNetCallBack m_cServerDialogNetCB;
};

#endif // VR_REQUESTSERVERDIALOG_H
