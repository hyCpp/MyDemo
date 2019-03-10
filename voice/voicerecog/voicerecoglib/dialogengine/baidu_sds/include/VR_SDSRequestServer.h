
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

#ifndef VR_SDSREQUESTSERVER_H
#define VR_SDSREQUESTSERVER_H

#include <string>
#include "BL_String.h"
#include "VR_SDSServerDEF.h"

class VR_SDSRequestServerBase
{
public:
    VR_SDSRequestServerBase() {}
    virtual ~VR_SDSRequestServerBase() {}

    virtual const BL_String& GetRequest() const { return m_cRequest; }
    virtual DWORD GetDID() { return s_dwDID; }

    static VOID SetHost(const BL_String& rcHost) { s_cHost = rcHost; }
    static VOID SetSID(const BL_String& rcSID) { s_cSID = rcSID; }

protected:
    static BL_String s_cHost;
    static BL_String s_cSID;
    static DWORD s_dwDID;

    BL_String m_cRequest;
};

typedef VR_SDSRequestServerBase VR_SDSResponseServerBase, VR_SDSResponseServerStatus;

// Message: Client -> Server actively
class VR_SDSMessageServerStart : public VR_SDSRequestServerBase
{
public:
    VR_SDSMessageServerStart();
    VR_SDSMessageServerStart(const std::string& rcUserID);
};

class VR_SDSMessageServerStop : public VR_SDSRequestServerBase
{
public:
    VR_SDSMessageServerStop();
};

class VR_SDSMessageServerSelect : public VR_SDSRequestServerBase
{
public:
    VR_SDSMessageServerSelect(DWORD dwIndex);
};

// Response: Client -> Server passively
class VR_SDSResponseServerVR : public VR_SDSResponseServerBase
{
public:
    VR_SDSResponseServerVR(VR_SDSSERVER_ERROR eError);
    VR_SDSResponseServerVR(const std::string& rcVRResult, LONG lLon, LONG lLat);
};

class VR_SDSResponseServerRPT : public VR_SDSResponseServerBase
{
public:
    VR_SDSResponseServerRPT(VR_SDSSERVER_ERROR eError);
};

class VR_SDSResponseServerWB : public VR_SDSResponseServerBase
{
public:
    VR_SDSResponseServerWB(VR_SDSSERVER_ERROR eError);
};

class VR_SDSResponseServerPIC : public VR_SDSResponseServerBase
{
public:
    VR_SDSResponseServerPIC(VR_SDSSERVER_ERROR eError);
};

class VR_SDSResponseServerHomeStatus : public VR_SDSResponseServerStatus
{
public:
    VR_SDSResponseServerHomeStatus(CL_BOOL bSet);
};

class VR_SDSResponseServerCorpStatus : public VR_SDSResponseServerStatus
{
public:
    VR_SDSResponseServerCorpStatus(CL_BOOL bSet);
};

class VR_SDSResponseServerWBStatus : public VR_SDSResponseServerStatus
{
public:
    VR_SDSResponseServerWBStatus(CL_BOOL bSet);
};

class VR_SDSResponseServerPlayMusic : public VR_SDSResponseServerBase
{
public:
    VR_SDSResponseServerPlayMusic(DWORD dwResult);
};

class VR_SDSResponseServerAppControl : public VR_SDSResponseServerBase
{
public:
    VR_SDSResponseServerAppControl(DWORD dwResult, std::string cAppType);
};

#endif // VR_SDSREQUESTSERVER_H
