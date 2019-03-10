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

#if defined(_WIN32) || defined(_WIN32_WCE)
#include "stdafx.h"
#else
#endif

#include <stdio.h>
#include <string.h>

#include "Vr_Asr_Impl_Afx.h"
#include "Record_Log.h"
#include "Win_Linux_Interface.h"
#include "AudioIn_Adapter.h"
#include "Wav_File.h"


C_AudioIn_Adapter::~C_AudioIn_Adapter()
{
    M_Fn_Clear_Buffer();
}

C_AudioIn_Adapter::C_AudioIn_Adapter()
{
}

void
C_AudioIn_Adapter::M_Fn_Get_Data_Type_Audio_SSE
(
    void ** ppvoid_Data_Type_Audio_SSE,
    size_t * pi_Size_Data_Type_Audio_SSE
)
{
}

int C_AudioIn_Adapter::M_Fn_Open_WavFile(const std::string& string_wave_file_name, bool b_new)
{    
    if (!b_new && (m_string_wave_file_name == string_wave_file_name)) {
        return 0;
    }

    M_Fn_Clear_Buffer();

    C_Wav_File wave_info_obj;

    bool b_oper_flag = wave_info_obj.Open(string_wave_file_name, 16000, false);
    if (!b_oper_flag) {
        Fn_Log("Failed to open wave file, path %s ", string_wave_file_name.c_str());
        MACRO_Log_Error
        return -1;
    }

    BYTE* py_temp_buffer = NULL;
    py_temp_buffer = new BYTE[4096];
    if (NULL == py_temp_buffer) {
        Fn_Log("Failed to get buffer");
        MACRO_Log_Error
        return -1;
    }

    for (;;) {
        int int_read_len = wave_info_obj.Read(py_temp_buffer, 4096);
        if (int_read_len <= 0) {
            break;
        }

        BYTE* py_buffer = new BYTE[int_read_len];
        if (NULL == py_buffer) {
            Fn_Log("Failed to get buffer");
            MACRO_Log_Error
            return -1;
        }

        memcpy(py_buffer, py_temp_buffer, int_read_len);
        m_vector_wave_buffer.push_back(std::pair<BYTE *, size_t>(py_buffer, static_cast<size_t>(int_read_len)));
    }

    delete[] py_temp_buffer;
    py_temp_buffer = NULL;

    wave_info_obj.Close();

    m_string_wave_file_name = string_wave_file_name;

    return 0;
}

int C_AudioIn_Adapter::M_Fn_Prepare()
{
    return 0;
}

int C_AudioIn_Adapter::M_Fn_Start()
{
    m_wave_buffer_index = 0;

    return 0;
}

int C_AudioIn_Adapter::M_Fn_Stop()
{
    return 0;
}

int C_AudioIn_Adapter::M_Fn_Data_Available_Check(bool * pb_Data_Available)
{
    *pb_Data_Available = !(m_wave_buffer_index == m_vector_wave_buffer.size());

    return 0;
}

int C_AudioIn_Adapter::M_Fn_Data_Fetch(void ** ppvoid_Buffer, size_t * pi_Size)
{
    if (m_wave_buffer_index != m_vector_wave_buffer.size()) {
        *ppvoid_Buffer = m_vector_wave_buffer[m_wave_buffer_index].first;
        *pi_Size = m_vector_wave_buffer[m_wave_buffer_index].second;
        ++m_wave_buffer_index;
    }
    else {
        *ppvoid_Buffer = NULL;
        *pi_Size = 0;
    }

    return 0;
}

int C_AudioIn_Adapter::M_Fn_Data_Return(void * const & rpvoid_Buffer)
{
    return 0;
}

void C_AudioIn_Adapter::M_Fn_Clear_Buffer()
{
    typedef std::vector<std::pair<BYTE *, size_t> >::size_type Vec_Index;

    Vec_Index vec_buffer_size = m_vector_wave_buffer.size();
    for (
        Vec_Index vec_index = 0;
        vec_index != vec_buffer_size;
        ++vec_index
    ) {
        delete[] m_vector_wave_buffer[vec_index].first;
    }

    m_vector_wave_buffer.clear();
    m_wave_buffer_index = 0;

}


/* EOF */
