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
 * @file Vr_Asr_Impl_File_Info.h
 * @brief C_File_Info class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_File_Info_h__
#define __Vr_Asr_Impl_File_Info_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

/**
 * @brief The C_File_Info class
 *
 * C_File_Info class
 */

class C_File_Info
{
  public:
    int m_b_File_Write;
    FILE* m_p_File;
    long m_i_File_Pos;
    long m_i_File_Len;

    ~C_File_Info() // [F]Destructor
    {
    }

    explicit
    C_File_Info() // [F]Constructor of Default
    : m_b_File_Write(false)
    , m_p_File(NULL)
    , m_i_File_Pos(0)
    , m_i_File_Len(0)
    {
    }

    C_File_Info // [F]Constructor: Copy
    (
        C_File_Info const &
    );

    C_File_Info &
    operator = // [F]Operator: =
    (
        C_File_Info const &
    );
};

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
