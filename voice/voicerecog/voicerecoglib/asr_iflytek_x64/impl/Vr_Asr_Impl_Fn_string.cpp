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
#include "Vr_Asr_Impl_Fn_string.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

string
Fn_Get_string_Without_Blank
(
    string const & rstring_I
)
{
    string rstring_O(rstring_I);

    IF (rstring_O.empty()) {
        return rstring_O;
    }

    rstring_O.erase(0, rstring_O.find_first_not_of(" \t\r\n"));
    rstring_O.erase(rstring_O.find_last_not_of(" \t\r\n") + 1);

    return rstring_O;
}

bool
Fn_Convert_String_To_Integer
(
    string const & rstring_I,
    long & ri_I
)
{
    char* cp_end_ptr = NULL;
    errno = 0;

    ri_I = strtol(rstring_I.c_str(), &cp_end_ptr, 10);

    IF ((cp_end_ptr == rstring_I.c_str())
        || (*cp_end_ptr != '\0')
        || ((ri_I == LONG_MIN || ri_I == LONG_MAX) && errno == ERANGE)) {
        RETURN(false);
    }

    return true;
}

int
Fn_Is_Utf8_String
(
    string const & rstring_I
)
{
    bool b_Check_Range = false;
    size_t i_Patten = 0;
    size_t i_Following = 0;
    size_t const i_Num = rstring_I.size();

    for (
        size_t i_Pos = 0;
        i_Pos < i_Num;
        ++i_Pos
    ) {

        unsigned char i_Char = rstring_I[i_Pos];

        IF (i_Following) {

            // 10XX XXXX

            IF (0x80 != (i_Char & 0xC0)) {
                return false;
            }

            IF (b_Check_Range) {

                b_Check_Range = false;

                switch (i_Patten) {
                case 3:
                    // 1110 ####
                    // 10#X XXXX
                    // 10XX XXXX
                    // 0800~FFFF
                    //     100000000000
                    // 1111111111111111
                {
                    IF (0 == (i_Char & 0x20)) {
                        return false;
                    }

                    break;
                }
                case 4:
                    // 1111 0###
                    // 10## XXXX
                    // 10XX XXXX
                    // 10XX XXXX
                    // 10000~1FFFFF
                    //     10000000000000000
                    // 111111111111111111111
                {
                    IF (0 == (i_Char & 0x30)) {
                        return false;
                    }

                    break;
                }
                case 5:
                    // 1111 10##
                    // 10## #XXX
                    // 10XX XXXX
                    // 10XX XXXX
                    // 10XX XXXX
                    // 200000~3FFFFFF
                    //     1000000000000000000000
                    // 11111111111111111111111111
                {
                    IF (0 == (i_Char & 0x38)) {
                        return false;
                    }

                    break;
                }
                case 6:
                    // 1111 110#
                    // 10## ##XX
                    // 10XX XXXX
                    // 10XX XXXX
                    // 10XX XXXX
                    // 10XX XXXX
                    // 4000000~7FFFFFFF
                    //     100000000000000000000000000
                    // 1111111111111111111111111111111
                {
                    IF (0 == (i_Char & 0x3C)) {
                        return false;
                    }

                    break;
                }
                default:
                {
                    MACRO_Log_Error;
                    return false;
                }
                }
            }

            --i_Following;

            continue;
        }

        b_Check_Range = false;
        i_Patten = 0;

        while (i_Char & 0x80) {
            i_Char = i_Char << 1;

            ++i_Patten;
        }

        switch (i_Patten) {
        case 0:
            // 0XXX XXXX
            // 0000~007F
        {
            break;
        }
        case 2:
            // 110# ###X
            // 10XX XXXX
            // 0080~07FF
            //    10000000
            // 11111111111
        {
            IF (0 == (i_Char & 0x78)) {
                return false;
            }

            i_Following = 1;

            break;
        }
        case 3:
            // 1110 ####
            // 10#X XXXX
            // 10XX XXXX
            // 0800~FFFF
            //     100000000000
            // 1111111111111111
        case 4:
            // 1111 0###
            // 10## XXXX
            // 10XX XXXX
            // 10XX XXXX
            // 10000~1FFFFF
            //     10000000000000000
            // 111111111111111111111
        case 5:
            // 1111 10##
            // 10## #XXX
            // 10XX XXXX
            // 10XX XXXX
            // 10XX XXXX
            // 200000~3FFFFFF
            //     1000000000000000000000
            // 11111111111111111111111111
        case 6:
            // 1111 110#
            // 10## ##XX
            // 10XX XXXX
            // 10XX XXXX
            // 10XX XXXX
            // 10XX XXXX
            // 4000000~7FFFFFFF
            //     100000000000000000000000000
            // 1111111111111111111111111111111
        {
            IF (0 == i_Char) {
                b_Check_Range = true;
            }

            i_Following = i_Patten - 1;

            break;
        }
        default:
        {
            return false;
        }
        }
    }

    IF (i_Following) {

        return false;
    }

    return true;
}


} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
