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
 * @file VR_ProtoMessageRegerIF.h
 * @brief Declaration file of class VR_ProtoMessageRegerIF.
 *
 * This file includes the declaration of class VR_ProtoMessageRegerIF.
 *
 * @attention used for C++ only.
 */

#ifndef VR_PROTOMESSAGEREGERIF_H
#define VR_PROTOMESSAGEREGERIF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Macro.h"
#include "VR_Def.h"

#include <boost/function.hpp>

class EV_EventContext;
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ProtoMessageRegerIF);

/**
 * @brief The VR_ProtoMessageRegerIF class
 *
 * class declaration
 */
class VR_ProtoMessageRegerIF
{
public:
    typedef boost::function<void(const VR_ProtoMessageBase*, const EV_EventContext*)> protoCallBack;
    virtual ~VR_ProtoMessageRegerIF();
    virtual int RegisterMessageCallBack(const VR_ProtoMessageBase*, protoCallBack) = 0;
    virtual int RegisterMessageCallBack(const VR_String&, protoCallBack) = 0;
    virtual bool UnRegisterMessageTypeCallBack(int) = 0;
};

#endif /* VR_PROTOMESSAGEREGERIF_H */
/* EOF */
