/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
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

#include "stdafx.h"

#ifndef CXX_BL_ERROR_H
#	include "BL_Error.h"
#endif

#ifndef CXX_BL_STRING_H
#	include "BL_String.h"
#endif

#include <list>
#include "VP_DataAccessorList.h"
using namespace std;

VP_DataAccessorList::VP_DataAccessorList()
    : sender("")
    , userdata(0)
    , m_bNotify(true)
    , abort(false)
    , m_curPlayIndex(0)
    , m_pControl(nullptr)
{
}

VP_DataAccessorList::VP_DataAccessorList(const VoiceVector<shared_ptr<VP_DataAccessor>>::type &pDataList, string sdr, int ud)
    : sender(sdr)
    , userdata(ud)
    , m_bNotify(true)
    , abort(false)
    , m_curPlayIndex(0)
    , m_pControl(nullptr)
    , m_pDataList(pDataList)
{
}

VP_DataAccessorList::~VP_DataAccessorList() 
{
}

CL_BOOL VP_DataAccessorList::HasNext() const 
{
    return (m_curPlayIndex < static_cast<DWORD>(m_pDataList.size()));
}

string VP_DataAccessorList::getSender() const
{
    return sender;
}

int VP_DataAccessorList::getUserData() const
{
    return userdata;
}

void VP_DataAccessorList::setAbort(bool val)
{
    abort = val;
}

bool VP_DataAccessorList::isAbort() const
{
    return abort;
}

void VP_DataAccessorList::setPlayResult(VP_PLAY_RESULT val)
{
    playResult = val;
}

VP_PLAY_RESULT VP_DataAccessorList::getPlayResult() const
{
    return playResult;
}

const shared_ptr<VP_DataAccessor>& VP_DataAccessorList::operator[](int idx) const
{
    return m_pDataList[idx];
}

void VP_DataAccessorList::setPlayIndex(DWORD playIndex)
{
    m_curPlayIndex = playIndex;
}

DWORD VP_DataAccessorList::getPlayIndex() const
{
    return m_curPlayIndex;
}

void VP_DataAccessorList::setChannelId(VP_SMALL_SIGNAL_ID ssid)
{
    m_ssid = ssid;
}

VP_SMALL_SIGNAL_ID VP_DataAccessorList::getChannelId() const
{
    return m_ssid;
}

void VP_DataAccessorList::setPlayEndNotify(bool notify)
{
    m_bNotify = notify;
}

bool VP_DataAccessorList::getPlayEndNotify() const
{
    return m_bNotify;
}

void VP_DataAccessorList::setPlayControl(shared_ptr<VP_VoicePlayControl> playControl)
{
    m_pControl = playControl;
}

shared_ptr<VP_VoicePlayControl> VP_DataAccessorList::getPlayControl() const
{
    return m_pControl;
}

CL_ERROR VP_DataAccessorList::Play(VC_Player* pPlayer, VC_Player::Listener *pListener,
        CL_BOOL& isTtsPlay, int reqId, int tokenid)
{
    VP_LOGI("VP_DataAccessorList::Play() enter!");
    if (static_cast<DWORD>(m_pDataList.size()) <= m_curPlayIndex) {
        VP_DEBUG_ERROR("Current index:[%ld] is greater than the length[%d]\n", m_curPlayIndex, m_pDataList.size());
        return APL_ERR_VP_UNKNOWN;
    }

    if (abort) {
        VP_LOGD("User has cancelled this playlist!\n");
        return APL_ERR_VP_USER_STOP;
    }

    shared_ptr<VP_DataAccessor> pAccessor = m_pDataList[m_curPlayIndex];

    CL_ERROR error = APL_ERR_NO_ERROR;
    if (nullptr != pAccessor) {
        pAccessor->setPlayControl(m_pControl);
        pAccessor->setLastPhrase(m_curPlayIndex == m_pDataList.size() - 1);
        error = pAccessor->Play(pPlayer, pListener, isTtsPlay, reqId, tokenid);
    }
    m_curPlayIndex++;

    return error;
}

void VP_DataAccessorList::playFinished()
{
    VP_LOGD_FUNC();

    int iListSize = m_pDataList.size();
    for (int index = 0; index < iListSize; ++index) {

        shared_ptr<VP_DataAccessor> pAccessor = m_pDataList[index];        
        if (nullptr != pAccessor) {
            pAccessor->playFinished();
        }
    }

}

/* EOF */
