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
#include "Vr_Asr_Impl_Fn_Get_string_Line_Fixed.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

void
Fn_Get_string_Line_Fixed
(
    std::ifstream & rifstream_In,
    string & rstring_Out
)
{
    std::getline(rifstream_In, rstring_Out);

    while (!rstring_Out.empty()) {
        char i_Char = rstring_Out[rstring_Out.size() - 1];

        IF ((i_Char != 0xd)
            && (i_Char != 0xa)
        ) {
            break;
        }

        rstring_Out = rstring_Out.substr(0, rstring_Out.size() - 1);
    }
}

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
