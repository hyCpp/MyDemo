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


namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

C_Heap::~C_Heap()
{
}

C_Heap::C_Heap()
: m_o_Mutex()
, m_o_LH_HEAP_INTERFACE()
, m_pvoid_Instance(NULL)
{
}

void *
C_Heap::M_Fn_Malloc
(
    size_t i_Size
)
{
    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex);

    return m_o_LH_HEAP_INTERFACE.pmalloc(m_pvoid_Instance, i_Size);
}

void *
C_Heap::M_Fn_Calloc
(
    size_t i_Number,
    size_t i_Size
)
{
    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex);

    return m_o_LH_HEAP_INTERFACE.pcalloc(m_pvoid_Instance, i_Number, i_Size);
}

void *
C_Heap::M_Fn_Realloc
(
    void * pvoid_Pointer,
    size_t i_Size
)
{
    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex);

    return m_o_LH_HEAP_INTERFACE.prealloc(m_pvoid_Instance, pvoid_Pointer, i_Size);
}

void
C_Heap::M_Fn_Free
(
    void * pvoid_Pointer
)
{
    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex);

    m_o_LH_HEAP_INTERFACE.pfree(m_pvoid_Instance, pvoid_Pointer);
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */