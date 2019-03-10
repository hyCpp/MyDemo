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

/**
 * @file Vr_Asr_Impl_Fn_Iostream.h
 * @brief interface define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Vocon_Fn_Iostream_h__
#define __Vr_Asr_Impl_Vocon_Fn_Iostream_h__

#include "Vr_Asr_Impl_File_Info.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

#ifndef MACRO_Dynamic_Debug

int S_Fn_Reader_File_Seek(void *stream, long offset, LH_STREAM_ORIGIN origin);
int S_Fn_Reader_File_Error(void *stream);
size_t S_Fn_Reader_File_Read(void *buffer, size_t size, size_t count, void *stream);
int S_Fn_Reader_File_Finished(void *stream);
int S_Fn_Writer_File_Seek(void *stream, long offset, LH_STREAM_ORIGIN origin);
int S_Fn_Writer_File_Error(void *stream);
size_t S_Fn_Writer_File_Write(const void *buffer, size_t size, size_t count, void *stream);
int S_Fn_Writer_File_Finished(void *stream, LH_BOOL onErrorPath);

#endif

extern
ST_ERROR
Fn_ST_CreateStreamWriterToFile(const char *szFilename, struct LH_OSTREAM_INTERFACE_S *pOStreamInterface, C_File_Info& ro_File_Info);

extern
ST_ERROR
Fn_ST_CreateStreamReaderFromFile(const char *szFilename, struct LH_ISTREAM_INTERFACE_S *pIStreamInterface, void **ppIStreamInst);

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
