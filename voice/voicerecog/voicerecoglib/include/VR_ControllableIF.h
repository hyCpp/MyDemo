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
 * @file VR_ControllableIF.h
 * @brief Declaration file of class VR_ControllableIF.
 *
 * This file includes the declaration of class VR_ControllableIF.
 *
 * @attention used for C++ only.
 */

#ifndef VR_CONTROLLABLEIF_H
#define VR_CONTROLLABLEIF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif
/**
 * @brief The VR_ControllableIF class
 *
 * class VR_ControllableIF declaration
 */
class VR_ControllableIF
{
public:
    virtual ~VR_ControllableIF()
    {
    }

    virtual void Initialize() = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Destory() = 0;
};

#endif /* VR_CONTROLLABLEIF_H */
/* EOF */
