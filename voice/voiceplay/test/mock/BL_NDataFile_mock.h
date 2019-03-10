/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
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
 * @file BL_NDataFile_mock.h
 * @brief Summary for MockBL_NDataFile.
 *
 * This file includes the declaration of class MockBL_NDataFile and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef MOCK_BL_NDATA_FILE_H 
#define MOCK_BL_NDATA_FILE_H 

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "BL_NDataFile.h"

/**
 * This is a mock class for BL_NDataFile
 *
 * This class is used to mock a object for class 
 * BL_NDataFile
 */
// class MockBL_NDataFile : public BL_NDataFile
// {
// public:
//     MockBL_NDataFile()
//         : BL_PathResolver(BL_FILE_PREFIX_NDATA)
//         , BL_NDataFile()
//     {
//     }
// 
//     virtual ~MockBL_NDataFile();
// 
//     MOCK_METHOD2(Open, CL_BOOL(const BL_String& strFileName, BL_FileOpenMode eMode));
//     MOCK_METHOD3(Read, CL_BOOL(VOID* const pReadBuff, DWORD dwSize, DWORD *pdwLength));
//     MOCK_METHOD2(Seek, CL_BOOL(LONG lOffset, BL_FileSeekOrigin eOrigin));
//     MOCK_METHOD0(Close, CL_BOOL());
// };
// 
// MockBL_NDataFile::~MockBL_NDataFile()
// {
// }

#endif
/* EOF */

