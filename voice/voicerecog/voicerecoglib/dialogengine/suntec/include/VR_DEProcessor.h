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
 * @file VR_DEProcessor.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_DEPROCESSOR_H
#define VR_DEPROCESSOR_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/* Standard Header */
#include <string>

/**
 * @brief The VR_DEProcessor class
 *
 * class declaration
 */
class VR_DEProcessor
{
public:
    /**
     * Destructor
     *
     */
    virtual ~VR_DEProcessor() {}

    /**
     * ProcessMessage
     *
     * Process Voice Recognition related messages
     *
     * @param none
     * @return none
     * @attention none
     * @see none
     */
    virtual void ProcessMessage(const std::string& message, int actionSeqId  = -1) = 0;
};

#endif // VR_DEPROCESSOR_H
/* EOF */