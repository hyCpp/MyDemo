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
 * @file AudioIn_Adapter_gtest.h
 * @brief Declaration file of class C_AudioIn_Adapter_gtest.
 *
 * This file includes the declaration of class C_AudioIn_Adapter_gtest.
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __AudioIn_Adapter_gtest_h__
#define __AudioIn_Adapter_gtest_h__

#include <list>
#include <string>
#include "Vr_Asr_Engine.h"

// namespace N_Vr
// {

// namespace N_Asr
// {

// class C_Audio_In;

// } // name N_Common

// } // name N_Vr


#define BYTE unsigned char

/**
 * @brief The C_AudioIn_Adapter_gtest class
 *
 * C_AudioIn_Adapter_gtest class
 */
class C_AudioIn_Adapter_gtest : public N_Vr::N_Asr::C_Audio_In
{
public:
    explicit C_AudioIn_Adapter_gtest();

    // Destructor:
    virtual ~C_AudioIn_Adapter_gtest();

    virtual int M_Fn_Prepare();

    virtual int M_Fn_Start();

    virtual int M_Fn_Stop();

    virtual int M_Fn_Data_Available_Check(bool * pb_Data_Available);

    virtual int M_Fn_Data_Fetch(void ** ppvoid_Buffer, size_t * pi_Size);

    virtual int M_Fn_Data_Return(void * const & rpvoid_Buffer);

    virtual int M_Fn_Open_WavFile(const std::string& string_wave_file_name);

protected:
    // Constructor: Copy
    C_AudioIn_Adapter_gtest(C_AudioIn_Adapter_gtest const &);
    // Operator: =
    C_AudioIn_Adapter_gtest & operator = (C_AudioIn_Adapter_gtest const &);

private:
    void M_Fn_Clear_Buffer();

private:

    std::string m_string_wave_file_name;
    std::vector<std::pair<BYTE *, size_t> >::size_type m_wave_buffer_index;
    std::vector<std::pair<BYTE *, size_t> > m_vector_wave_buffer;
};

#endif // __AudioIn_Adapter_gtest_h__
/* EOF */

