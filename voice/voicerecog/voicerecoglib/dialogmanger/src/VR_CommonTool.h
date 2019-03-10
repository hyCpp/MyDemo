/**
 * Copyright @ 2013 - 2015 Suntec Software(Shanghai) Co., Ltd.
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
 * @file VR_CommonTool.h
 *
 * @brief common tool
 *
 * This class VR_CommonTool
 *
 * @attention used for C++ only
 */

#ifndef VR_COMMONTOOL_H
#define VR_COMMONTOOL_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <sstream>

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include "VR_Log.h"
#include "BL_StringLibIF.h"
#include "BL_String.h"

/**
 *  value to string
 */
template<class T> void toString(std::string& result, const T& param)
{
    std::stringstream ss;
    ss << param;
    result = ss.str();
}


/**
 *  template for switch endian
 */
template <typename T>
T EndianSwitch(T Data)
{
    union
    {
              T iData;
        uint8_t iByte[sizeof(T)];
    } unSrc, unDest;

    if (1 == sizeof(T)) {
        return Data;
    }
    unSrc.iData = Data;
    for (int i = 0; i < sizeof(T); i++) {
        unDest.iByte[i] = unSrc.iByte[sizeof(T) - i - 1];
    }
    return unDest.iData;
}

/**
 * @brief The VR_CommonTool class
 *
 * class declaration
 */
class VR_CommonTool
{
public:

    /*
     * Escapes or unescapes an XML file removing traces of offending characters that could be wrongfully interpreted as markup.
     * The following characters are reserved in XML and must be replaced with their corresponding XML entities:
     *  ' is replaced with &apos;
     *  " is replaced with &quot;
     *  & is replaced with &amp;
     *  < is replaced with &lt;
     *  > is replaced with &gt;
    */

    static std::string Encode(const std::string& rStrSrc);

    /**
     * @brief outputByByte
     * print UCS2 info by byte
     * @param u
     */
    static void outputByByte(const char* name, const std::string& data);

    static void outputByByte(const char* name, const int64_t data);

    /**
     * @brief Convert2UCS2BE
     * convert BL_String to UCS2 Big endian type
     * @param strSource
     * @param strDest
     * @return
     */
    static bool Convert2UCS2BE(const BL_String& strSource, VR_String& strDest);

    /**
     * @brief ParseUCS2_BE
     * Parse form avclan to BL_String
     * @param pAddr
     * @param byteSize
     * @param dst
     * @return
     */
    static bool ParseUCS2_BE(const VOID *pAddr, const INT byteSize, BL_String &dst);

};

#endif /* VR_COMMONTOOL_H */
/* EOF */
