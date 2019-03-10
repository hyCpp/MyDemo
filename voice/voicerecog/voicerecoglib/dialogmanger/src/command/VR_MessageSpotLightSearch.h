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
 * @file VR_MessageSpotLightSearch.h
 * @brief Declaration file of class VR_MessageSpotLightSearch.
 *
 * This file includes the declaration of class VR_MessageSpotLightSearch.
 *
 * @attention used for C++ only.
 */
#ifndef VR_MESSAGESPOTLIGHTSEARCH_H
#define VR_MESSAGESPOTLIGHTSEARCH_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "VR_Message.h"

/**
 * @brief The VR_MessageSpotLightSearch class
 *
 * class declaration
 */
class VR_MessageSpotLightSearch : public VR_Message
{
public:
    VR_MessageSpotLightSearch(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual ~VR_MessageSpotLightSearch();

    virtual void perform();
    virtual void replay();
};

/**
 * @brief The VR_MessageSearchModuleStatus class
 *
 * class declaration
 */
class VR_MessageSearchModuleStatus : public VR_Message
{
public:
    VR_MessageSearchModuleStatus(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual ~VR_MessageSearchModuleStatus();

    virtual void perform();
    virtual void replay();
};

#endif // VR_MESSAGESPOTLIGHTSEARCH_H
/* EOF */
