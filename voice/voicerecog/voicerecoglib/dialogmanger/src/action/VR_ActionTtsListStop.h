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
 * @file VR_ActionTtsListStop.h
 * @brief Declaration file of class VR_ActionTtsListStop.
 *
 * This file includes the declaration of class VR_ActionTtsListStop.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ACTIONTTSLISTSTOP_H
#define VR_ACTIONTTSLISTSTOP_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_ActionBase.h"

/**
 * @brief The VR_ActionTtsListStop class
 *
 * class declaration
 */
class VR_ActionTtsListStop : public VR_ActionBase
{
public:
    VR_ActionTtsListStop(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase& proto);
    ~VR_ActionTtsListStop();

    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase& proto);

protected:
    int m_ActionType;
    int m_EngineReqId;
    int m_EngineTtsPlayReqId;
    VR_String m_Text;
};


#endif // VR_ACTIONTTSLISTSTOP_H
/* EOF */
