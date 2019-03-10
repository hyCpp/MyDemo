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

#include "Vr_Asr_Impl_Afx.h"
#include "Vr_Asr_Impl_Vocon_Fn_Iostream.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

#ifdef MACRO_Dynamic_Debug
static
#endif
int
S_Fn_Writer_File_Seek
(
    void *stream,
    long offset,
    LH_STREAM_ORIGIN origin
)
{
    IF (NULL == stream) {
        MACRO_Log_Error
        return -1;
    }

    C_File_Info* po_File_Info = reinterpret_cast<C_File_Info*>(stream);
    IF (NULL == po_File_Info) {
        MACRO_Log_Error
        return -1;
    }

    offset += sizeof(DWORD);

    IF (!po_File_Info->m_b_File_Write) {
        IF (LH_STREAM_SEEK_SET == origin) {
            po_File_Info->m_i_File_Pos = offset;
        }
        ELSE IF (LH_STREAM_SEEK_CUR == origin) {
            po_File_Info->m_i_File_Pos += offset;
        }
        ELSE { // LH_STREAM_SEEK_END == origin:
            po_File_Info->m_i_File_Pos = po_File_Info->m_i_File_Len + offset;
        }

       return 0;
    }

    FILE* p_File = NULL;
    p_File = po_File_Info->m_p_File;
    IF (NULL == p_File) {
        MACRO_Log_Error
        return -1;
    }

    int i_Pos_Flag = 0;
    IF (LH_STREAM_SEEK_SET == origin) {
        i_Pos_Flag = SEEK_SET;
    }
    ELSE IF (LH_STREAM_SEEK_CUR == origin) {
        i_Pos_Flag = SEEK_CUR;
    }
    ELSE { // LH_STREAM_SEEK_END == origin:
        i_Pos_Flag = SEEK_END;
    }

    return fseek(p_File, offset, i_Pos_Flag);
}

#ifdef MACRO_Dynamic_Debug
static
#endif
int
S_Fn_Writer_File_Error
(
    void *stream
)
{
    IF (NULL == stream) {
        MACRO_Log_Error
        return -1;
    }

    C_File_Info* po_File_Info = reinterpret_cast<C_File_Info*>(stream);
    IF (NULL == po_File_Info) {
        MACRO_Log_Error
        return -1;
    }

    IF (!po_File_Info->m_b_File_Write) {
        return 0;
    }

    FILE* p_File = po_File_Info->m_p_File;
    IF (NULL == p_File) {
        MACRO_Log_Error
        return -1;
    }

    return ferror(p_File);
}

#ifdef MACRO_Dynamic_Debug
static
#endif
int
S_Fn_Reader_File_Seek
(
    void *stream,
    long offset,
    LH_STREAM_ORIGIN origin
)
{
    IF (NULL == stream) {
        MACRO_Log_Error
        return -1;
    }

    FILE* p_File = reinterpret_cast<FILE*>(stream);
    IF (NULL == p_File) {
        MACRO_Log_Error
        return -1;
    }

    int i_Pos_Flag = 0;
    IF (LH_STREAM_SEEK_SET == origin) {
        i_Pos_Flag = SEEK_SET;
    }
    ELSE IF (LH_STREAM_SEEK_CUR == origin) {
        i_Pos_Flag = SEEK_CUR;
    }
    ELSE { // LH_STREAM_SEEK_END == origin:
        i_Pos_Flag = SEEK_END;
    }

    offset += sizeof(DWORD);

    return fseek(p_File, offset, i_Pos_Flag);
}

#ifdef MACRO_Dynamic_Debug
static
#endif
int
S_Fn_Reader_File_Error
(
    void *stream
)
{
    IF (NULL == stream) {
        MACRO_Log_Error
        return -1;
    }

    FILE* p_File = reinterpret_cast<FILE*>(stream);
    IF (NULL == p_File) {
        MACRO_Log_Error
        return -1;
    }

    return ferror(p_File);
}

#ifdef MACRO_Dynamic_Debug
static
#endif
size_t
S_Fn_Reader_File_Read
(
    void *buffer,
    size_t size,
    size_t count,
    void *stream
)
{
    IF (NULL == stream) {
        MACRO_Log_Error
        return -1;
    }

    FILE* p_File = reinterpret_cast<FILE*>(stream);
    IF (NULL == p_File) {
        MACRO_Log_Error
        return -1;
    }

    return fread(buffer, size, count, p_File);
}

#ifdef MACRO_Dynamic_Debug
static
#endif
int
S_Fn_Reader_File_Finished
(
    void *stream
)
{
    IF (NULL == stream) {
        MACRO_Log_Error
        return -1;
    }

    FILE* p_File = reinterpret_cast<FILE*>(stream);
    IF (NULL == p_File) {
        MACRO_Log_Error
        return -1;
    }

    return fclose(p_File);
}

#ifdef MACRO_Dynamic_Debug
static
#endif
size_t
S_Fn_Writer_File_Write
(
    const void *buffer,
    size_t size,
    size_t count,
    void *stream
)
{
    IF (NULL == stream) {
        MACRO_Log_Error
        return -1;
    }

    C_File_Info* po_File_Info = reinterpret_cast<C_File_Info*>(stream);
    IF (NULL == po_File_Info) {
        MACRO_Log_Error
        return -1;
    }

    IF (!po_File_Info->m_b_File_Write) {
        size_t i_Write_Size = size * count;
        po_File_Info->m_i_File_Pos += static_cast<long>(i_Write_Size);
        IF (po_File_Info->m_i_File_Len < po_File_Info->m_i_File_Pos) {
            po_File_Info->m_i_File_Len = po_File_Info->m_i_File_Pos;
        }

        return count;
    }

    FILE* p_File = po_File_Info->m_p_File;
    IF (NULL == p_File) {
        MACRO_Log_Error
        return -1;
    }

    return fwrite(buffer, size, count, p_File);
}

#ifdef MACRO_Dynamic_Debug
static
#endif
int
S_Fn_Writer_File_Finished
(
    void *stream,
    LH_BOOL onErrorPath
)
{
    IF (NULL == stream) {
        MACRO_Log_Error
        return -1;
    }

    C_File_Info* po_File_Info = reinterpret_cast<C_File_Info*>(stream);
    IF (NULL == po_File_Info) {
        MACRO_Log_Error
        return -1;
    }

    IF (!po_File_Info->m_b_File_Write) {
        return 0;
    }

    FILE* p_File = po_File_Info->m_p_File;
    IF (NULL == p_File) {
        MACRO_Log_Error
        return -1;
    }

    IF (LH_TRUE == onErrorPath) {
        return fclose(p_File);
    }

    int i_Ret_Value = 0;
    DWORD i_Crc_Value = 0;
    C_Crc32Calc o_Crc32Calc;
    boost::scoped_array<unsigned char> scp_File_Buffer;

    do {
        long i_File_Len = 0;
        size_t i_Read_Write_Len = 0;

        i_Ret_Value = fseek(p_File, 0, SEEK_END);
        IF (0 != i_Ret_Value) {
            MACRO_Log_Error
            break;
        }

        i_File_Len = ftell(p_File);
        IF ((long)sizeof(DWORD) >= i_File_Len) {
            MACRO_Log_Error
            break;
        }

        i_Ret_Value = fseek(p_File, 0, SEEK_SET);
        IF (0 != i_Ret_Value) {
            MACRO_Log_Error
            break;
        }

        scp_File_Buffer.reset(MACRO_New unsigned char[i_File_Len]);
        i_Read_Write_Len = fread(scp_File_Buffer.get(), 1, i_File_Len, p_File);
        IF (i_File_Len != static_cast<long>(i_Read_Write_Len)) {
            i_Ret_Value = -1;
            MACRO_Log_Error
            break;
        }

        i_Crc_Value = o_Crc32Calc.Calc(scp_File_Buffer.get() + sizeof(DWORD), i_File_Len - sizeof(DWORD));

        i_Ret_Value = fseek(p_File, 0, SEEK_SET);
        IF (0 != i_Ret_Value) {
            MACRO_Log_Error
            break;
        }

        i_Read_Write_Len = fwrite(&i_Crc_Value, 1, sizeof(DWORD), p_File);
        IF (sizeof(DWORD) != i_Read_Write_Len) {
            i_Ret_Value = -1;
            MACRO_Log_Error
            break;
        }
    } while (0);

    fclose(p_File);

    return i_Ret_Value;
}

ST_ERROR
Fn_ST_CreateStreamWriterToFile
(
    const char *szFilename,
    struct LH_OSTREAM_INTERFACE_S *pOStreamInterface,
    C_File_Info& ro_File_Info
)
{
    IF ((NULL == szFilename)
        || (NULL == pOStreamInterface)) {
        MACRO_Log_Error
        return ST_E_ARG;
    }

    pOStreamInterface->pfwrite = S_Fn_Writer_File_Write;
    pOStreamInterface->pfseek = S_Fn_Writer_File_Seek;
    pOStreamInterface->pferror = S_Fn_Writer_File_Error;
    pOStreamInterface->pffinished = S_Fn_Writer_File_Finished;

    IF (!ro_File_Info.m_b_File_Write) {
        return ST_OK;
    }

    FILE* p_File = fopen(szFilename, "wb+");
    IF (NULL == p_File) {
        MACRO_Log_Error
        return ST_E_ARG;
    }

    size_t i_Write_Len = 0;
    DWORD i_Crc_Value = 0;
    i_Write_Len = fwrite(&i_Crc_Value, 1, sizeof(DWORD), p_File);
    IF (sizeof(DWORD) != i_Write_Len) {
        MACRO_Log_Error
        fclose(p_File);
        return ST_E_RESOURCE;
    }

    ro_File_Info.m_p_File = p_File;

    return ST_OK;
}

ST_ERROR
Fn_ST_CreateStreamReaderFromFile
(
    const char *szFilename,
    struct LH_ISTREAM_INTERFACE_S *pIStreamInterface,
    void **ppIStreamInst
)
{
    IF ((NULL == szFilename)
        || (NULL == pIStreamInterface)
        || (NULL == ppIStreamInst)) {
        MACRO_Log_Error
        return ST_E_ARG;
    }

    pIStreamInterface->pfread = S_Fn_Reader_File_Read;
    pIStreamInterface->pfseek = S_Fn_Reader_File_Seek;
    pIStreamInterface->pferror = S_Fn_Reader_File_Error;
    pIStreamInterface->pffinished = S_Fn_Reader_File_Finished;

    FILE* p_File = fopen(szFilename, "rb");
    IF (NULL == p_File) {
        MACRO_Log_Error
        return ST_E_ARG;
    }

    ST_ERROR o_ST_ERROR = ST_OK;
    DWORD i_Crc_Value = 0;
    DWORD i_Crc_Value_Calc = 0;
    C_Crc32Calc o_Crc32Calc;
    boost::scoped_array<unsigned char> scp_File_Buffer;

    do {
        int i_Ret_Value = 0;
        long i_File_Len = 0;
        size_t i_Read_Len = 0;

        i_Ret_Value = fseek(p_File, 0, SEEK_END);
        IF (0 != i_Ret_Value) {
            o_ST_ERROR = ST_E_RESOURCE;
            MACRO_Log_Error
            break;
        }

        i_File_Len = ftell(p_File);
        IF ((long)sizeof(DWORD) >= i_File_Len) {
            o_ST_ERROR = ST_E_RESOURCE;
            MACRO_Log_Error
            break;
        }

        i_Ret_Value = fseek(p_File, 0, SEEK_SET);
        IF (0 != i_Ret_Value) {
            o_ST_ERROR = ST_E_RESOURCE;
            MACRO_Log_Error
            break;
        }

        scp_File_Buffer.reset(MACRO_New unsigned char[i_File_Len]);
        i_Read_Len = fread(scp_File_Buffer.get(), 1, i_File_Len, p_File);
        IF (i_File_Len != static_cast<long>(i_Read_Len)) {
            o_ST_ERROR = ST_E_RESOURCE;
            MACRO_Log_Error
            break;
        }

        i_Crc_Value = *((DWORD*)scp_File_Buffer.get());
        i_Crc_Value_Calc = o_Crc32Calc.Calc(scp_File_Buffer.get() + sizeof(DWORD), i_File_Len - sizeof(DWORD));
        IF (i_Crc_Value_Calc != i_Crc_Value) {
            o_ST_ERROR = ST_E_RESOURCE;
            MACRO_Log_Error
            break;
        }

        i_Ret_Value = fseek(p_File, sizeof(DWORD), SEEK_SET);
        IF (0 != i_Ret_Value) {
            o_ST_ERROR = ST_E_RESOURCE;
            MACRO_Log_Error
            break;
        }
    } while (0);

    IF (ST_OK != o_ST_ERROR) {
        fclose(p_File);
    }
    ELSE {
        *ppIStreamInst = reinterpret_cast<void*>(p_File);
    }

    return o_ST_ERROR;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
