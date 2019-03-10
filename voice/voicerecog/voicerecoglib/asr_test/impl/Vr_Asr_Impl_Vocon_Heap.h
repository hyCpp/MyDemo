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
 * @file Voice_Asr_Impl_Heap.h
 * @brief C_Heap class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Heap_h__
#define __Vr_Asr_Impl_Heap_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

/**
 * @brief The C_Heap class
 *
 * C_Heap class
 */

class C_Heap
{
  public:

    C_Mutex m_o_Mutex;
    LH_HEAP_INTERFACE m_o_LH_HEAP_INTERFACE; // The heap interface.
    void * m_pvoid_Instance; // Pointer to the heap.

    virtual
    ~C_Heap(); // [F]Destructor

    explicit
    C_Heap(); // [F]Constructor of Default

    void *
    M_Fn_Malloc
    (
        size_t i_Size
    );

    void *
    M_Fn_Calloc
    (
        size_t i_Number,
        size_t i_Size
    );

    void *
    M_Fn_Realloc
    (
        void * pvoid_Pointer,
        size_t i_Size
    );

    void
    M_Fn_Free
    (
        void * pvoid_Pointer
    );

  PROTECTED:

    C_Heap // [F]Constructor: Copy
    (
        C_Heap const & // [I]
    );

    C_Heap & // [R]
    operator = // [F]Operator: =
    (
        C_Heap const & // [I]
    );
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */