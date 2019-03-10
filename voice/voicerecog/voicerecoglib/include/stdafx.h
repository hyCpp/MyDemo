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
 * @file stdafx.h
 * @brief include file for standard system include files,
 *
 * This file include file for standard system include files,
 * or project specific include files that are used frequently,
 * but are changed infrequently
 *
 * @attention not include in any head file.
 */

#ifndef STDAFX_H
#define STDAFX_H

#if defined(_WIN32) || defined(_WIN32_WCE)

#   ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.
#       define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#   endif
#   include "windows.h"
#else
#   include "BL_Base.h"
#endif

#ifndef CXX_VR_LOG_H
#   include "VR_Log.h"
#endif

#   include "ncore/NCTypesDefine.h"

#endif /* STDAFX_H */
/* EOF */
