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


#include "VR_Platform.h"
#include "VR_RequestServerDialog.h"
#include "VR_Log.h"
#include "BL_Internet.h"

VR_Platform::VR_Platform()
{

}

CL_BOOL VR_Platform::Initialize(const TiXmlElement* pcXmlElem)
{
    VR_LOGD_FUNC();

    return CL_TRUE;
}


void VR_Platform::Destroy()
{
}

void VR_Platform::Start()
{
}

void VR_Platform::Stop()
{
}

CL_ERROR VR_Platform::GetLocation(LONG &lLon, LONG &lLat)
{
    lLon = 0;
    lLat = 0;

    return 0;
}

CL_BOOL VR_Platform::IsNetworkConnected()
{
    BL_Internet inet;
    CL_BOOL connected = CL_FALSE;
    CL_BOOL ok = inet.IsConnected(connected);
    return ok && connected;
}

