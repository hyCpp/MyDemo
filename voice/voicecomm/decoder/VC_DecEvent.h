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
 * @file VC_DecEvent.h                                            
 * @brief Summary for VC_DecEvent.
 *
 * This file includes the declaration of class VC_DecEvent and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_DECEVENT_H
#define VC_DECEVENT_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once

/**
 * Summary for class VC_DecEvent
 *
 * There should be a detail description for
 * class VC_DecEvent
 */
class VC_DecEvent
{
public:
    virtual ~VC_DecEvent() {}
    virtual CL_ERROR DoAction() = 0;
};


#endif /* VC_DECEVENT_H */
/* EOF */