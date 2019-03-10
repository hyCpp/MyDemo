/**
 * Copyright @ 2014 - 2015 Suntec Software(Shanghai) Co., Ltd.
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
#include "VR_AvcUnit.h"

#define VR_HTTPFILE_SUCCESS

namespace nutshell
{

static INT sloop_times = 3;

#ifdef VR_HTTPFILE_SUCCESS
NC_ERROR NCHttpClient::connectClient()
{
    static int cnt = 0;
    if (sloop_times <= ++cnt) {
        cnt = 0;
        return NC_HTTP_SUCCESS;
    }

    return NC_HTTP_ERROR;
}

NC_ERROR NCHttpFile::writeFile(const NCString &srcFile, const NCString &dstFile, NCHttpStatusCallBack *statusCallBack)
{
    if (dstFile == "//Internal/VR/EmptyPath") {
        return NC_HTTP_ERROR;
    }
    return NC_HTTP_SUCCESS;
}

NC_ERROR NCHttpFile::copyFile(const NCString &srcFile, const NCString &dstFile, NC_BOOL overwrite, NCHttpStatusCallBack *statusCallBack)
{
    if (dstFile == "EmptyPath") {
        return NC_HTTP_ERROR;
    }
    return NC_HTTP_SUCCESS;
}

NC_ERROR NCHttpFile::close()
{
    return NC_HTTP_SUCCESS;
}

NC_ERROR NCHttpClient::disconnectClient()
{
    return NC_HTTP_SUCCESS;
}

NC_ERROR NCHttpFile::getLastError()
{
    return NC_HTTP_ERROR;
}

void set_loop_time(int times)
{
    sloop_times = times;
}

#endif
}

/* EOF */
