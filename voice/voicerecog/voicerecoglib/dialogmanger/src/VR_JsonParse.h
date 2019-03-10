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
 * @file VR_JsonParse.h
 * @brief Declaration file of class VR_JsonParse.
 *
 * This file includes the declaration of class VR_JsonParse.
 *
 * @attention used for C++ only.
 */

#ifndef VR_JSONPARSE_H
#define VR_JSONPARSE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

/**
 * @brief The VR_JsonParse class
 *
 * class declaration
 */
class VR_JsonParse
{
public:
    VR_JsonParse();
    virtual ~VR_JsonParse();

    VR_String ParseJson(const VR_String &strMsg);
    VR_String getStringFromJson(const VR_String &strMsg, const VR_String &strKey);

private:
    bool LoadString(const VR_String &strMsg);
    void NotifyCommandAction(const VR_String &strMsg);

private:
    rapidjson::Document m_docJson;
};

#endif // VR_JsonParse
/* EOF */
