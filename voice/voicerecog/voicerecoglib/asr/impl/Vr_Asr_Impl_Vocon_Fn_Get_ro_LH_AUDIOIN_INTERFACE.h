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
 * @file Vr_Asr_Impl_Fn_Get_ro_LH_AUDIOIN_INTERFACE.h
 * @brief interface define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Fn_Get_ro_LH_AUDIOIN_INTERFACE_h__
#define __Vr_Asr_Impl_Fn_Get_ro_LH_AUDIOIN_INTERFACE_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

#ifndef MACRO_Dynamic_Debug

int S_Fn_Audio_In_Start(void * pvoid_Audio_In);
int S_Fn_Audio_In_Stop(void * pvoid_Audio_In);
int S_Fn_Audio_In_Has_Data(void * pvoid_Audio_In, LH_BOOL * pb_Has_Data);
int S_Fn_Audio_In_Fetch_Data(void * pvoid_Audio_In, void ** ppvoid_Buffer, size_t * pi_Size);
int S_Fn_Audio_In_Return_Data(void * pvoid_Audio_In, void * pvoid_Buffer);

#endif

extern
LH_AUDIOIN_INTERFACE &
Fn_Get_ro_LH_AUDIOIN_INTERFACE();

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
