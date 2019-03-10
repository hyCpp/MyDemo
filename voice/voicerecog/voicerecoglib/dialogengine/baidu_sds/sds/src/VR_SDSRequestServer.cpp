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

#include "VR_SDSRequestServer.h"
#include "DSPUtility.h"
#include "VR_Log.h"

using namespace std;


BL_String VR_SDSRequestServerBase::s_cHost;
BL_String VR_SDSRequestServerBase::s_cSID;
DWORD VR_SDSRequestServerBase::s_dwDID = 1;

VR_SDSMessageServerStart::VR_SDSMessageServerStart()
{
    s_dwDID = 1;
    m_cRequest.Format("%s/msg?did=%d&type=start", s_cHost.GetString(), s_dwDID);
    ++s_dwDID;
}

VR_SDSMessageServerStart::VR_SDSMessageServerStart(const string& rcUserID)
{
    s_dwDID = 1;
    m_cRequest.Format("%s/msg?uid=%s&did=%d&type=start", s_cHost.GetString(), rcUserID.c_str(), s_dwDID);
    ++s_dwDID;
}

VR_SDSMessageServerStop::VR_SDSMessageServerStop()
{
    m_cRequest.Format("%s/msg?sid=%s&did=%d&type=stop", s_cHost.GetString(), s_cSID.GetString(), s_dwDID);
}

VR_SDSMessageServerSelect::VR_SDSMessageServerSelect(DWORD dwIndex)
{
    m_cRequest.Format("%s/msg?sid=%s&did=%d&type=select&index=%d", s_cHost.GetString(), s_cSID.GetString(), s_dwDID, dwIndex);
    ++s_dwDID;
}

VR_SDSResponseServerVR::VR_SDSResponseServerVR(VR_SDSSERVER_ERROR eError)
{
    m_cRequest.Format("%s/rsp?sid=%s&did=%d&type=vr&error=%d", s_cHost.GetString(), s_cSID.GetString(), s_dwDID, eError);
    ++s_dwDID;
}

VR_SDSResponseServerVR::VR_SDSResponseServerVR(const string& rcVRResult, LONG lLon, LONG lLat)
{
    BL_String codedResult;
    CL_BOOL result = DSPUtility::EncodeURL(rcVRResult.c_str(), codedResult, BL_CP_UTF8);

    if (!result) {
        codedResult = rcVRResult.c_str();
    }
    m_cRequest.Format("%s/rsp?sid=%s&did=%d&type=vr&q=%s&lon=%d&lat=%d&uid=moonwalker", s_cHost.GetString(), s_cSID.GetString(), s_dwDID, codedResult.GetString(), lLon, lLat);
    ++s_dwDID;
}

VR_SDSResponseServerRPT::VR_SDSResponseServerRPT(VR_SDSSERVER_ERROR eError)
{
    m_cRequest.Format("%s/rsp?sid=%s&did=%d&type=commit&param=report&error=%d", s_cHost.GetString(), s_cSID.GetString(), s_dwDID, eError);
    ++s_dwDID;
}

VR_SDSResponseServerWB::VR_SDSResponseServerWB(VR_SDSSERVER_ERROR eError)
{
    m_cRequest.Format("%s/rsp?sid=%s&did=%d&type=commit&param=weibo&error=%d", s_cHost.GetString(), s_cSID.GetString(), s_dwDID, eError);
    ++s_dwDID;
}

VR_SDSResponseServerPIC::VR_SDSResponseServerPIC(VR_SDSSERVER_ERROR eError)
{
    m_cRequest.Format("%s/rsp?sid=%s&did=%d&type=picture&error=%d", s_cHost.GetString(), s_cSID.GetString(), s_dwDID, eError);
    ++s_dwDID;
}

VR_SDSResponseServerHomeStatus::VR_SDSResponseServerHomeStatus(CL_BOOL bSet)
{
    m_cRequest.Format("%s/rsp?sid=%s&did=%d&type=check&param=home&error=%d", s_cHost.GetString(), s_cSID.GetString(), s_dwDID, bSet);
    ++s_dwDID;
}

VR_SDSResponseServerCorpStatus::VR_SDSResponseServerCorpStatus(CL_BOOL bSet)
{
    m_cRequest.Format("%s/rsp?sid=%s&did=%d&type=check&param=corporation&error=%d", s_cHost.GetString(), s_cSID.GetString(), s_dwDID, bSet);
    ++s_dwDID;
}

VR_SDSResponseServerWBStatus::VR_SDSResponseServerWBStatus(CL_BOOL bSet)
{
    m_cRequest.Format("%s/rsp?sid=%s&did=%d&type=check&param=weibo&error=%d", s_cHost.GetString(), s_cSID.GetString(), s_dwDID, bSet);
    ++s_dwDID;
}


VR_SDSResponseServerPlayMusic::VR_SDSResponseServerPlayMusic(DWORD dwResult)
{
    m_cRequest.Format("%s/rsp?sid=%s&did=%d&type=musicplay&error=%d", s_cHost.GetString(), s_cSID.GetString(), s_dwDID, dwResult);
    ++s_dwDID;
}

VR_SDSResponseServerAppControl::VR_SDSResponseServerAppControl(DWORD dwResult, string cAppType)
{
    m_cRequest.Format("%s/rsp?sid=%s&did=%d&type=%s&error=%d", s_cHost.GetString(), s_cSID.GetString(), s_dwDID, cAppType.c_str(), dwResult);
    ++s_dwDID;
}
