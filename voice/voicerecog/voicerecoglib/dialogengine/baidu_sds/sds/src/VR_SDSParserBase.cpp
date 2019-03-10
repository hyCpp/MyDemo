/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#include "VR_SDSParserBase.h"


VR_SDSParserBase::VR_SDSParserBase()
    : m_bHasError(CL_FALSE)
{

}

VR_SDSOperationParser::VR_SDSOperationParser(VR_SDSSERVER_OPERATION_TYPE eType)
    : m_operationType(eType)
{

}

VR_SDSOperationParser::VR_SDSOperationParser(VR_SDSSERVER_OPERATION_TYPE eType,
                                                     const BL_String& sParam)
    : m_operationType(eType), m_operationParam(sParam)
{
}
