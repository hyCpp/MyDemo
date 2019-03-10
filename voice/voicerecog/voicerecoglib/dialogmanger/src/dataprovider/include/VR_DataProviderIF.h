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
 * @file VR_DataProviderIF.h
 * @brief Declaration file of class VR_DataProviderIF.
 *
 * This file includes the declaration of class VR_DataProviderIF.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DATAPROVIDERIF_H
#define VR_DATAPROVIDERIF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

namespace navi
{
namespace dataprovider
{

/**
 * @brief The VR_DataProviderIF class
 *
 * declare class VR_DataProviderIF
 */
class VR_DataProviderIF
{
public:
    virtual ~VR_DataProviderIF()
    {

    }

public:
    virtual bool RegistObserver() = 0;
    virtual void UnRegistObserver() = 0;
};

}

}

#endif /* VR_DATAPROVIDERIF_H */
/* EOF */
