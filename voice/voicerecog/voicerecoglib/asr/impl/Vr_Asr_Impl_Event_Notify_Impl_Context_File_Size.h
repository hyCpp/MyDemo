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
 * @file Vr_Asr_Impl_Event_Notify_Impl_Context_File_Size.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Event_Notify_Impl_Context_File_Size_h__
#define __Vr_Asr_Impl_Event_Notify_Impl_Context_File_Size_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

/**
 * @brief The C_Event_Notify_Impl_Context_File_Size class
 *
 * C_Event_Notify_Impl_Context_File_Size class
 */
class C_Event_Notify_Impl_Context_File_Size : public C_Event_Notify
{
  public:

    unsigned int m_i_File_Size;

    virtual
    ~C_Event_Notify_Impl_Context_File_Size(); // Destructor

    explicit
    C_Event_Notify_Impl_Context_File_Size // Constructor of Overload
    (
        unsigned int i_File_Size
    );

    virtual
    E_Event_Notify_Type // [R] Result string XML.
    Get_Event_Notify_Type() const; // [F] Get result.

    virtual
    unsigned int // [r] result unsigned int context file size
    Get_Context_File_Size() const; // [f] get context file size

  PROTECTED:

    C_Event_Notify_Impl_Context_File_Size // Constructor: Copy
    (
        C_Event_Notify_Impl_Context_File_Size const & // [I]
    );

    C_Event_Notify_Impl_Context_File_Size & // [R]
    operator = // Operator: =
    (
        C_Event_Notify_Impl_Context_File_Size const & // [I]
    );
};

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
