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

#include "stdafx.h"
#include "VC_DecoderOgg.h"
#include "BL_NDataFile.h"
#include "VC_CommDef.h"

static size_t ogg_read(void* data_ptr,      // A pointer to the data
                size_t byteSize,     // Byte size on this particular system
                size_t sizeToRead,   // Maximum number of items to be read
                void* data_src)      // A pointer to the data we passed into
{
    size_t readSize, left;
    VC_OggState* vorbisData = static_cast<VC_OggState*>(data_src);
    if (NULL == vorbisData) {
        return -1;
    }

    left = vorbisData->dataSize - vorbisData->dataRead;
    if (sizeToRead * byteSize < left) {
        readSize = sizeToRead * byteSize;
    }
    else {
        readSize = left;
    }

    if (0 == readSize) { // EOF
        return 0;
    }

    if (!vorbisData->isFile) { // Read from memory
        memcpy(data_ptr, static_cast<char*>(vorbisData->dataHandle) + vorbisData->dataRead, readSize);
        vorbisData->dataRead += readSize;
    } 
    else {
        DWORD dwRead = 0;
        BL_NDataFile* pFile = reinterpret_cast<BL_NDataFile*>(vorbisData->dataHandle);
        if (!pFile->Read(data_ptr, readSize, &dwRead)) { // File read error
            pFile->Close();
            return 0;
        }

        vorbisData->dataRead += dwRead;
        if (vorbisData->dataSize == vorbisData->dataRead) { // EOF
             pFile->Close();
        }
        readSize = dwRead;
    }

    return readSize;
}

static int ogg_seek(void* data_src, ogg_int64_t offset, int origin)
{
    return -1; // Not need seek
}

static int ogg_close(void* data_src)
{
    return 0;
}

static long ogg_tell(void* data_src)
{
    VC_OggState* vorbisData = static_cast<VC_OggState*>(data_src);
    if (NULL == vorbisData) {
        return -1L;
    }
    return vorbisData->dataRead;
}

static ov_callbacks s_callbacks = {
    (size_t (*)(void *, size_t, size_t, void *))  ogg_read,
    (int (*)(void *, ogg_int64_t, int))           ogg_seek,
    (int (*)(void *))                             ogg_close,
    (long (*)(void *))                            ogg_tell
};

VC_DecoderOgg::VC_DecoderOgg() : m_bStop(CL_FALSE), m_sampleRate(22050)
{
    m_oggState.dataHandle = NULL;
    m_oggState.vf.datasource = NULL;
}

VC_DecoderOgg::~VC_DecoderOgg()
{

}

CL_ERROR VC_DecoderOgg::Init(const VOID* param)
{
    // do nothing
    return APL_ERR_NO_ERROR;
}

CL_ERROR VC_DecoderOgg::Uninit()
{
    return APL_ERR_NO_ERROR;
}

CL_ERROR VC_DecoderOgg::Decode(const BL_String& fileName, LONG lOffset, DWORD dwLen, Listener* pListener) 
{
    // Setup ogg decoder
    VC_LOGD("Decode, filename=%s, offset=%ld\n", fileName.GetString(), lOffset);
    if (m_dataFile.IsOpen()) {
        VC_LOGE("Decode, filename not be closed\n");
        m_dataFile.Close();
    }

    if (!m_dataFile.Open(fileName, BL_FILE_OPEN_MODE_R)) {
        VC_LOGE("Decode, Open file failed\n");
        return APL_ERR_VC_DECODER;
    }

    if (!m_dataFile.Seek(lOffset, BL_FILE_SEEK_SET)) {
        m_dataFile.Close();
        VC_LOGE("Decode, Seek file failed\n");
        return APL_ERR_VC_DECODER;
    }
    
    m_bStop = CL_FALSE;
    m_oggState.dataHandle = &m_dataFile;
    m_oggState.dataSize = dwLen;
    m_oggState.isFile = true;
    m_oggState.dataRead = 0;
    return DoDecode(pListener);
}

CL_ERROR VC_DecoderOgg::Decode(const VOID* pData, DWORD dwLen, Listener* pListener)
{
    // Setup ogg decoder
    m_oggState.dataHandle = const_cast<VOID*>(pData);
    m_oggState.dataSize = dwLen;
    m_oggState.isFile = false;
    m_oggState.dataRead = 0;
    return DoDecode(pListener);
}

CL_ERROR VC_DecoderOgg::DoDecode(Listener* pListener) 
{
    if (NULL != m_oggState.vf.datasource) {
        VC_LOGE("*** VC_DecoderOgg::Decode, not close ogg\n");
        ov_clear(&m_oggState.vf);
    }

    VC_ERROR_TYPE err = APL_ERR_VC_NO_ERROR;
    int iErrr = ov_open_callbacks(reinterpret_cast<void *>(&m_oggState), &m_oggState.vf, 0, 0, s_callbacks);
    if (0 != iErrr) {
        VC_LOGE("VC_DecoderOgg::Decode, Open ogg file failed, err=%d\n", err);
        return APL_ERR_VC_DECODER;
    }

    long ret = 0;
    do {
        VC_AudioBuffer* pBuffer = pListener->OnDecBuffer();
        if (NULL == pBuffer) {
            err = APL_ERR_VC_USERSTOP;
            break;
        }

        char* addr = reinterpret_cast<char*>(pBuffer->addr());
        long size = pBuffer->size();
        do {
            ret = ov_read(&m_oggState.vf, addr, size, NULL);
            if (OV_HOLE == ret || OV_EBADLINK == ret) {
                err = APL_ERR_VC_DECODER;
                break; // error
            } 
            else if (0 == ret) {
                break; // EOF
            }
            else {
            }

            addr += ret;
            size -= ret;
        } while (size > 0 && !m_bStop);

        pBuffer->setLen(pBuffer->size() - size);
        pListener->OnDecReady(pBuffer);
    } while (ret > 0 && !m_bStop);

    // Clear OGG vorbis file handle
    ov_clear(&m_oggState.vf);

    return m_bStop ? APL_ERR_VC_USERSTOP : err;
}

CL_ERROR VC_DecoderOgg::Stop()
{
    m_bStop = CL_TRUE;
    return APL_ERR_NO_ERROR;
}

CL_ERROR VC_DecoderOgg::GetParam(DEC_PARAM_TYPE type, VOID* param) const
{
    return APL_ERR_NO_ERROR;
}

CL_ERROR VC_DecoderOgg::SetParam(DEC_PARAM_TYPE type, const VOID* param)
{
    return APL_ERR_NO_ERROR;
}

VOID VC_DecoderOgg::SetSampleRate(const BL_String& fileName, LONG lOffset)
{
    if (m_dataFile.IsOpen()) {
        m_dataFile.Close();
    }

    if (!m_dataFile.Open(fileName, BL_FILE_OPEN_MODE_R)) {
        VC_LOGE("VC_DecoderOgg::GetSampleRate, Open file failed\n");
        return;
    }

    LONG offset = lOffset + 0x28;       // read sample rate from file
    if (!m_dataFile.Seek(offset, BL_FILE_SEEK_SET)) {
        m_dataFile.Close();
        VC_LOGE("VC_DecoderOgg::GetSampleRate, Seek file failed\n");
        return;
    }

    m_dataFile.Read(&m_sampleRate, 2);
    VC_LOGD("Sample Rate is: [%d]\n", m_sampleRate);

    m_dataFile.Close();
}

INT VC_DecoderOgg::GetSampleRate() const 
{
    return m_sampleRate;
}

/* EOF */
