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
 * @file VR_EngineMessageRegerIF.h
 * @brief Declaration file of class VR_EngineMessageRegerIF.
 *
 * This file includes the declaration of class VR_EngineMessageRegerIF.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ENGINEMESSAGEREGERIF_H
#define VR_ENGINEMESSAGEREGERIF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Macro.h"
#include "VR_Def.h"

#include <boost/function.hpp>

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EngineMessageRegerIF);

/**
 * @brief The VR_EngineMessageRegerIF class
 *
 * class declaration
 */
class VR_EngineMessageRegerIF
{
public:
    typedef boost::function<bool(int id, const VR_String&)> xmlCallBack;

    virtual ~VR_EngineMessageRegerIF();
    virtual int RegisterNotifyMessageCallBack(xmlCallBack) = 0;
    virtual int RegisterReqMessageCallBack(xmlCallBack) = 0;
    virtual int RegisterReplyMessageCallBack(xmlCallBack) = 0;
    virtual bool UnRegisterMessageTypeCallBack(int) = 0;

};

#endif /* VR_ENGINEMESSAGEREGERIF_H */
/* EOF */
