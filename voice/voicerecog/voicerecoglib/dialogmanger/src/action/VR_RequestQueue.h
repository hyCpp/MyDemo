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
/**
 * @file VR_RequestQueue.h
 * @brief Declaration file of class VR_RequestQueue.
 *
 * This file includes the declaration of class VR_RequestQueue.
 *
 * @attention used for C++ only.
 */

#ifndef VR_REQUESTQUEUE_H
#define VR_REQUESTQUEUE_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ActionBase);
/**
 * @brief The VR_RequestQueue class
 *
 * class declaration
 */
class VR_RequestQueue
{
public:
    VR_RequestQueue();
    ~VR_RequestQueue();
    void Push(spVR_ActionBase spReq, bool bTtsAction = false);
    void OnMessage(int actionId, const VR_ProtoMessageBase& proto, bool bTtsAction = false);
    spVR_ActionBase Find(int actionId);
    void ClearTtsAction();
    
protected:
    void Remove(int actionId);
    bool IsTtsActionExist(const int actionId);
    void RemoveTtsId(int actionId);

protected:
    VoiceMap<int, spVR_ActionBase>::type    m_Reqs;
    static VoiceList<int>::type             m_VoiceTagActionIDs;
    VoiceVector<int>::type                  m_vecTtsIDs;
};

#endif /* VR_REQUESTQUEUE_H */
/* EOF */
