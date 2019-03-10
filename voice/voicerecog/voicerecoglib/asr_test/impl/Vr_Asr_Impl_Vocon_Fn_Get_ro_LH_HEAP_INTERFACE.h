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
 * @file Vr_Asr_Impl_Fn_Get_ro_LH_HEAP_INTERFACE.h
 * @brief interface define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Fn_Get_ro_LH_HEAP_INTERFACE_h__
#define __Vr_Asr_Impl_Fn_Get_ro_LH_HEAP_INTERFACE_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

#ifndef MACRO_Dynamic_Debug

void * S_Fn_Heap_Malloc(void * pvoid_Heap, size_t i_Size);
void * S_Fn_Heap_Calloc(void * pvoid_Heap, size_t i_Number, size_t i_Size);
void * S_Fn_Heap_Realloc(void * pvoid_Heap, void * pvoid_Pointer, size_t i_Size);
void S_Fn_Heap_Free(void * pvoid_Heap, void * pvoid_Pointer);

#endif

extern
LH_HEAP_INTERFACE &
Fn_Get_ro_LH_HEAP_INTERFACE();

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
