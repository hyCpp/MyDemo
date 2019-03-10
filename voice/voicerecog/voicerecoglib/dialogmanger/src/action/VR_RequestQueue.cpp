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

#include <google/protobuf/message_lite.h>

#ifndef VR_ACTIONBASE_H
#   include "VR_ActionBase.h"
#endif

#ifndef VR_REQUESTQUEUE_H
#   include "VR_RequestQueue.h"
#endif

VoiceList<int>::type VR_RequestQueue::m_VoiceTagActionIDs;

VR_RequestQueue::VR_RequestQueue()
{
    VR_LOGD_FUNC();
}

VR_RequestQueue::~VR_RequestQueue()
{
    VR_LOGD_FUNC();
}

void VR_RequestQueue::Push(spVR_ActionBase spReq, bool bTtsAction)
{
    VR_LOGI("actionID=[%d], spReq=[%p]", spReq->GetActionId(), spReq.get());
    if (spReq) {
        if (bTtsAction && !IsTtsActionExist(spReq->GetActionId())) {
            m_vecTtsIDs.push_back(spReq->GetActionId());
        }
        m_Reqs[spReq->GetActionId()] = spReq;
        spReq->Run();
        if (spReq->IsActionCompleted()) {
            Remove(spReq->GetActionId());
        }
    }
}

void VR_RequestQueue::OnMessage(int actionId, const VR_ProtoMessageBase& proto, bool bTtsAction)
{
    VR_LOGI("actionID=[%d], proto=[%s]", actionId, proto.GetTypeName().c_str());
    spVR_ActionBase spReq = Find(actionId);
    VR_LOGI("spReq=[%p]", spReq.get());
    if (spReq) {
        spReq->OnReceive(proto);
        VR_LOGI("spReq->IsActionCompleted()=[%d]", spReq->IsActionCompleted());
        if (spReq->IsActionCompleted()) {
            Remove(actionId);
            if (bTtsAction) {
                RemoveTtsId(actionId);
            }
        }
    }
}

spVR_ActionBase VR_RequestQueue::Find(int actionId)
{
    VR_LOGI("actionID=[%d]", actionId);
    VoiceMap<int, spVR_ActionBase>::type::iterator
    req = m_Reqs.find(actionId);
    return ((req != m_Reqs.end()) ? req->second : spVR_ActionBase());
}

void VR_RequestQueue::ClearTtsAction()
{
    VR_LOGI("before clear m_vecTtsIDs=[%lu]", m_vecTtsIDs.size());
    for (VoiceVector<int>::iterator it = m_vecTtsIDs.begin();
        it != m_vecTtsIDs.end(); ++it) {
        Remove(*it);
    }
    m_vecTtsIDs.clear();
    VR_LOGI("after clear m_vecTtsIDs=[%lu]", m_vecTtsIDs.size());
}

bool VR_RequestQueue::IsTtsActionExist(const int actionId)
{
    bool bExist = false;
    if (m_vecTtsIDs.end() != find(m_vecTtsIDs.begin(), m_vecTtsIDs.end(), actionId)) {
        bExist = true;
    }
    VR_LOGI("bExist=%s", bExist ? "true" : "false");
    return bExist;
}

void VR_RequestQueue::RemoveTtsId(int actionId)
{
    VoiceVector<int>::iterator it = find(m_vecTtsIDs.begin(), m_vecTtsIDs.end(), actionId);
    if (m_vecTtsIDs.end() != it) {
        VR_LOGI("remove actionID=[%d] OK", actionId);
        m_vecTtsIDs.erase(it);
    }
}

void VR_RequestQueue::Remove(int actionId)
{
    VR_LOGI("actionID=[%d]", actionId);
    m_Reqs.erase(actionId);
}
/* EOF */

