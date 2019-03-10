/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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
 * @file VR_DialogEngine.h
 * @brief dialog engine class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_DIALOGENGINE_H
#define VR_DIALOGENGINE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/* Suntec Header */
#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

#ifndef VR_DIALOGENGINEIF_H
#    include "VR_DialogEngineIF.h"
#endif

/* Forward Declaration*/
class VR_AlexaEngine;
class VR_MsgController;

/**
 * @brief The VR_DialogEngine class
 *
 * dialog controller class
 */
class VR_API VR_DialogEngine : public VR_DialogEngineIF
{
public:
    VR_DialogEngine();
    virtual ~VR_DialogEngine();

    virtual bool Initialize(VR_DialogEngineListener* listerner, const VR_Settings &settings);
    virtual std::string getHints(const std::string& hintsParams) override;
    virtual bool Start();
    virtual void Stop();
    virtual bool SendMessage(const std::string& event, int actionSeqId = VR_ACTION_SEQ_ID_INVALID);
    virtual void UnInitialize();

private:
	VR_AlexaEngine*           m_engine;
	VR_MsgController*         m_msgController;
    VR_DialogEngineListener*  m_listener;
};

#endif
/* EOF */
