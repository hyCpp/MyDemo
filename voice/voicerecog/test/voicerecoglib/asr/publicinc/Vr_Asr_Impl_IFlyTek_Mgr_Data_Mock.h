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
 * @file Voice_Asr_Impl_IFlyTek_Mgr_Data.h
 * @brief C_Mgr_Data class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_IFlyTek_Mgr_Data_Mock_h__
#define __Vr_Asr_Impl_IFlyTek_Mgr_Data_Mock_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_IFlyTek
{

class C_Engine_Concrete;

/**
 * @brief The C_Mgr_Data class
 *
 * C_Mgr_Data class
 */

class C_Mgr_Data_Mock : public C_Mgr_Data
{
  public:
    C_Mgr_Data_Mock(C_Engine_Concrete & ro_Engine_Concrete) : C_Mgr_Data(ro_Engine_Concrete) 
   {

    }

    MOCK_METHOD0(M_Fn_Init, E_Result());
    MOCK_METHOD0(M_Fn_Fina, E_Result());
    MOCK_METHOD0(M_Fn_On_Thread_Notified, void());
    MOCK_METHOD0(M_Fn_Start_Audio, E_Result());
    MOCK_METHOD0(M_Fn_Stop_Audio, E_Result());
    MOCK_METHOD0(_M_Fn_Thread_Init, E_Result());
    MOCK_METHOD0(_M_Fn_Thread_Fina, E_Result());
    MOCK_METHOD2(_M_Fn_Append_Audio_Data, E_Result(
        void* p_Buffer,
        unsigned int i_Buffer_Size
    ));

};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
