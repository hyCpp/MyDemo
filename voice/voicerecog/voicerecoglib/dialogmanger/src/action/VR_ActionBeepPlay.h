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
 * @file VR_ActionBeepPlay.h
 * @brief Declaration file of class VR_ActionBeepPlay.
 *
 * This file includes the declaration of class VR_ActionBeepPlay.
 *
 * @attention used for C++ only.
 */


#ifndef VR_ACTIONBEEPPLAY_H
#define VR_ACTIONBEEPPLAY_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_ActionBase.h"

/**
 * @brief The VR_ActionBeepPlay class
 *
 * class declaration
 */
class VR_ActionBeepPlay : public VR_ActionBase
{
public:
    VR_ActionBeepPlay(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto);
    ~VR_ActionBeepPlay();

    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase& proto);
    static int getLastActionId();
    static int getLastEngineId();

protected:
    int m_ActionType;
    int m_EngineReqId;
    static int m_EngineBeepPlayId;
    VR_String m_FilePath;
    bool m_isBargein;
    static int m_lastActionId;
};

#endif // VR_ACTIONBEEPPLAY_H
/* EOF */
