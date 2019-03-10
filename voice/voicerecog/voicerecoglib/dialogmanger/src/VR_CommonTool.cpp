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
 * @file VR_CommonTool.cpp
 *
 * @brief  Common Tool
 *
 * This class common tool
 *
 * @attention used for C++ only
 */
#include "VR_CommonTool.h"
#include "ncore/NCString.h"
#include "boost/format.hpp"

using namespace nutshell;
using namespace boost;

std::string VR_CommonTool::Encode(const std::string& rStrSrc)
{
    std::string strDest;
    strDest.reserve(rStrSrc.size());
    for (size_t pos = 0; pos != rStrSrc.size(); ++pos) {
        switch (rStrSrc[pos]) {
        case '&':
            {
                strDest.append("&amp;");
            }
            break;
        case '\"':
            {
                strDest.append("&quot;");
            }
            break;
        case '\'':
            {
                strDest.append("&apos;");
            }
            break;
        case '<':
            {
                strDest.append("&lt;");
            }
            break;
        case '>':
            {
                strDest.append("&gt;");
            }
            break;
        default:
            {
                strDest.append(&rStrSrc[pos], 1);
            }
            break;
        }
    }
    return strDest;
}

void VR_CommonTool::outputByByte(const char *name, const std::string &data)
{
    std::string output;
    for (UINT i = 0; i < data.length(); i++) {
        int tmp = data.at(i);
        tmp = tmp & 0xFF;
        output = output + (boost::format("%x ") % tmp).str();
    }
    VR_LOGD("field: %s data length: %lu, data: %s", name, data.length(), output.c_str());
    return;
}

void VR_CommonTool::outputByByte(const char* name, const int64_t data)
{
    union
    {
        int64_t iData;
        BYTE    iByte[sizeof(int64_t)];
    } unData;

    unData.iData = data;
    std::string output;
    int iSize = sizeof(int64_t);
    for (INT i = 0; i < iSize; i++) {
        int tmp = unData.iByte[i];
        tmp = tmp & 0xFF;
        output = output + (boost::format("%x ") % tmp).str();
    }
    VR_LOGD("field: %s data: %s", name, output.c_str());
    return;
}

bool VR_CommonTool::Convert2UCS2BE(const BL_String& strSource, VR_String& strDest)
{

    INT iLen = BL_StringLibIF::Instance()->GetCharCount(strSource);
    iLen = (iLen > 31) ? 31 : iLen;
    UCHAR16* pBuff = VR_new UCHAR16[iLen + 1];
    if (NULL == pBuff) {
        VR_LOGD("new pBuff failed");
        return false;
    }
    memset(pBuff, 0, sizeof(UCHAR16)*(iLen+1));
    VR_CommonTool::outputByByte("strSource", std::string(strSource.GetString(), strSource.GetLength()));
    strSource.GetUTF16BE(pBuff, iLen+1);
    strDest.assign(reinterpret_cast<char *>(pBuff), (iLen+1)*2);
    VR_CommonTool::outputByByte("strDest", strDest);
    delete[] pBuff;
    pBuff = NULL;
    return true;
}

bool VR_CommonTool::ParseUCS2_BE(const VOID *pAddr, const INT byteSize, BL_String &dst)
{
    CL_BOOL result = dst.SetUTF16BE(reinterpret_cast<const UCHAR16 *>(pAddr), byteSize / 2);
    dst += '\0';
    VR_CommonTool::outputByByte("dst", std::string(dst.GetString(), dst.GetLength()));
    return(result == CL_TRUE ? true : false);
}

/* EOF */
