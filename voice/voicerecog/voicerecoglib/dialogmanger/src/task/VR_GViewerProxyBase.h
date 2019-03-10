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
 * @file VR_GViewerProxyBase.h
 * @brief Declaration file of class VR_GViewerProxyBase.
 *
 * This file includes the declaration of class VR_GViewerProxyBase.
 *
 * @attention used for C++ only.
 */
#ifndef VR_GVIEWERPROXYBASE_H
#define VR_GVIEWERPROXYBASE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"

/**
 * @brief The VR_GViewerProxyBase class
 *
 * class declaration
 */
class VR_GViewerProxyBase
{
public:
    VR_GViewerProxyBase();
    virtual  ~VR_GViewerProxyBase();

    virtual VOID OnMessage(const VR_ProtoMessageBase& msg);
};

#endif // VR_GVIEWERPROXYBASE_H
/* EOF */
