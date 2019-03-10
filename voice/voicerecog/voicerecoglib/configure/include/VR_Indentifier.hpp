/**
 * Copyright @ 2013 - 2014 Suntec Software(Shanghai) Co., Ltd.
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
 * @file VR_AudioSourceListener.h
 * @brief Declaration file of class VR_AudioSourceListener.
 *
 * This file includes the declaration of class VR_AudioSourceListener, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#ifndef VR_INDENTIFIER_HPP
#define VR_INDENTIFIER_HPP

#include <atomic>

template<typename T>
class VR_Indentifier
{
public:
    const static T INVALID = 0;
    VR_Indentifier()
    {
        if(m_sRequestID <= INVALID)
        {
            m_sRequestID = INVALID;
        }
        m_RequestID = ++m_sRequestID;
    }

    T GetID() const
    {
        return m_RequestID;
    }

    void SetID(T id)
    {
        m_RequestID = id;
    }

protected:
    T m_RequestID;
    static std::atomic<T> m_sRequestID;
};

template<typename T> std::atomic<T> VR_Indentifier<T>::m_sRequestID(0);


typedef VR_Indentifier<uint32_t> VR_IdGenerator;

#endif /* VR_INDENTIFIER_HPP */
/* EOF */
