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
 * @file VR_Macro.h
 * @brief Declaration file of macro.
 *
 * This file includes the declaration of macro.
 *
 * @attention used for C++ only.
 */

#ifndef VR_MACRO_H
#define VR_MACRO_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#ifndef VR_DECLARE_CLASS_WITH_SMART_PTR
#define VR_DECLARE_CLASS_WITH_SMART_PTR(CLASS_NAME)         \
    class CLASS_NAME;                                       \
    typedef boost::shared_ptr<CLASS_NAME> sp##CLASS_NAME;   \
    typedef boost::weak_ptr<CLASS_NAME>   wp##CLASS_NAME
#endif

#ifndef VR_DEFINE_CLASS_WITH_SMART_PTR
#define VR_DEFINE_CLASS_WITH_SMART_PTR(CLASS_NAME)          \
    class CLASS_NAME;                                       \
    typedef boost::shared_ptr<CLASS_NAME> sp##CLASS_NAME;   \
    typedef boost::weak_ptr<CLASS_NAME>   wp##CLASS_NAME;   \
    class CLASS_NAME
#endif

#ifndef VR_DISALLOW_EVIL_CONSTRUCTORS
#define VR_DISALLOW_EVIL_CONSTRUCTORS(TypeName)             \
    private:                                                \
        TypeName(const TypeName&);                          \
        void operator=(const TypeName&)
#endif

#endif // VR_MACRO_H
/* EOF */
