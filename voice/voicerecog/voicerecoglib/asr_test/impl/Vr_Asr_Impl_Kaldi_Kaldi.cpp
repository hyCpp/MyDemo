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
#include "Vr_Asr_Impl_Kaldi_Kaldi.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Kaldi
{

scoped_ptr<C_Kaldi> g_scp_o_Kaldi(MACRO_New C_Kaldi);

C_Kaldi::~C_Kaldi()
{
}

C_Kaldi::C_Kaldi()
{
}

} // name N_Kaldi

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
