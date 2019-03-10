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
#include "Vr_Asr_Impl_Vocon_Heap.h"
#include "Vr_Asr_Impl_Vocon_Fn_Get_ro_LH_HEAP_INTERFACE.h"

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
void *
S_Fn_Heap_Malloc
(
    void * pvoid_Heap,
    size_t i_Size
)
{
    C_Heap * po_Heap = reinterpret_cast<C_Heap *>(pvoid_Heap);
    IF (!po_Heap) {
        RETURN_NULL;
    }

    return po_Heap->M_Fn_Malloc(i_Size);
}

#ifdef MACRO_Dynamic_Debug
static
#endif
void *
S_Fn_Heap_Calloc
(
    void * pvoid_Heap,
    size_t i_Number,
    size_t i_Size
)
{
    C_Heap * po_Heap = reinterpret_cast<C_Heap *>(pvoid_Heap);
    IF (!po_Heap) {
        RETURN_NULL;
    }

    return po_Heap->M_Fn_Calloc(i_Number, i_Size);
}

#ifdef MACRO_Dynamic_Debug
static
#endif
void *
S_Fn_Heap_Realloc
(
    void * pvoid_Heap,
    void * pvoid_Pointer,
    size_t i_Size
)
{
    C_Heap * po_Heap = reinterpret_cast<C_Heap *>(pvoid_Heap);
    IF (!po_Heap) {
        RETURN_NULL;
    }

    return po_Heap->M_Fn_Realloc(pvoid_Pointer, i_Size);
}

#ifdef MACRO_Dynamic_Debug
static
#endif
void
S_Fn_Heap_Free
(
    void * pvoid_Heap,
    void * pvoid_Pointer
)
{
    C_Heap * po_Heap = reinterpret_cast<C_Heap *>(pvoid_Heap);
    IF (!po_Heap) {
        return;
    }

    po_Heap->M_Fn_Free(pvoid_Pointer);
}

LH_HEAP_INTERFACE &
Fn_Get_ro_LH_HEAP_INTERFACE()
{
    static C_Mutex o_Mutex;
    C_Lock_Mutex o_Lock_Mutex(o_Mutex);

    static LH_HEAP_INTERFACE o_LH_HEAP_INTERFACE;
    static bool b_Init = false;
    IF (!b_Init) {
        o_LH_HEAP_INTERFACE.pmalloc = S_Fn_Heap_Malloc;
        o_LH_HEAP_INTERFACE.pcalloc = S_Fn_Heap_Calloc;
        o_LH_HEAP_INTERFACE.prealloc = S_Fn_Heap_Realloc;
        o_LH_HEAP_INTERFACE.pfree = S_Fn_Heap_Free;

        b_Init = true;
    }

    return o_LH_HEAP_INTERFACE;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
