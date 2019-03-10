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

#ifndef __Vr_Asr_Impl_Fn_Get_string_Line_Fixed_h__
#define __Vr_Asr_Impl_Fn_Get_string_Line_Fixed_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

extern
void
Fn_Get_string_Line_Fixed
(
    std::ifstream & rifstream_In,
    string & rstring_Out
);

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
