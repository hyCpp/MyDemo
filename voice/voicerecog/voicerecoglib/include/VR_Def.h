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
 * @file VR_Def.h
 * @brief Class declaration file.
 *
 * This file includes the declaration of class.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DEF_H
#define VR_DEF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <string>

#include "MEM_new.h"
#include "MEM_map.h"
#include "MEM_set.h"
#include "MEM_list.h"
#include "MEM_deque.h"
#include "MEM_queue.h"
#include "MEM_stack.h"
#include "MEM_vector.h"

#define VR_new    new(MEM_Voice)

#ifdef __GNUC__
# define VR_API __attribute__((visibility("default")))
#else
# define VR_API
#endif

namespace google
{
namespace protobuf
{
    class MessageLite;
}
}

class VR_EngineMessage;
typedef google::protobuf::MessageLite VR_ProtoMessageBase;
typedef std::string VR_String;
typedef __int32_t   VR_INT32;
typedef __uint32_t  VR_UINT32;

#endif /* VR_DEF_H */
/* EOF */
