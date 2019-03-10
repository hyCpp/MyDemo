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
#include "Vr_Asr_Impl_Fn_Get_string_Path.h"
//#include "VR_ConfigureIF.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

scoped_ptr<C_Get_Path> g_scp_o_Get_Path(MACRO_New C_Get_Path);

C_Get_Path::~C_Get_Path()
{
}

C_Get_Path::C_Get_Path()
{
}

string
C_Get_Path::M_Fn_Get_string_Path_VData()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    return Fn_Get_string_Path_VR() + "VDATA/ASR/";
#else
    //return VR_ConfigureIF::Instance()->getAsrDataPath();
    return "/data/asr/";
#endif
}

string
C_Get_Path::M_Fn_Get_string_Path_PData()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    return Fn_Get_string_Path_VR() + "PDATA/ASR/";
#else
    //return VR_ConfigureIF::Instance()->getAsrMapDataPath();
    return "/data/asr/";
#endif
}

string
C_Get_Path::M_Fn_Get_string_Path_Usr()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    return Fn_Get_string_Path_VR_Usr() + "VDATA/ASR/";
#else
    //return VR_ConfigureIF::Instance()->getAsrUsrPath();
    return "/data/asr/";
#endif
}

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
