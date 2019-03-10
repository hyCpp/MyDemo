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

#include "VR_SDSEvent.h"
#include "VR_Log.h"

using namespace std;

VR_SDSEvent::VR_SDSEvent(DWORD dwType, DWORD dwID, DWORD dwReqID, DWORD dwParam)
    : m_dwType(dwType), m_dwID(dwID), m_dwReqID(dwReqID), m_dwParam(dwParam)
{

}

VR_SDSEvent::~VR_SDSEvent()
{

}

VOID VR_SDSEvent::SetID(DWORD dwID)
{
    m_dwID = dwID;
}

DWORD VR_SDSEvent::GetID() const
{
    return m_dwID;
}

VOID VR_SDSEvent::SetReqID(DWORD dwReqID)
{
    m_dwReqID = dwReqID;
}

DWORD VR_SDSEvent::GetReqID() const
{
    return m_dwReqID;
}

VOID VR_SDSEvent::SetParam(DWORD dwParam)
{
    m_dwParam = dwParam;
}

DWORD VR_SDSEvent::GetParam() const
{
    return m_dwParam;
}

VOID VR_SDSEvent::SetText(const string& rcText)
{
    m_cText = rcText;
}

const string& VR_SDSEvent::GetText() const
{
    return m_cText;
}

VOID VR_SDSEvent::SetType(DWORD dwType)
{
    m_dwType = dwType;
}

DWORD VR_SDSEvent::GetType() const
{
    return m_dwType;
}


