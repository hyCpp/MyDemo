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
 * @file TTS_Comm.h 
 * @brief Declaration file of class 
 *
 * This file includes the declaration 
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#ifndef CXX_TTS_COMM_H
#define CXX_TTS_COMM_H

#ifdef TTS_ENABLE_TRACE
#	define TTS_TRACE TTS_Trace
#else
#	define TTS_TRACE
#endif

#define DECL_OBJ_LIST(cls) cls##ObjList
#define DECL_OBJ_LIST_ITER(cls) cls##ObjListIterator

// Get array size
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

// Disable class copy and assign constructor
#define DISABLE_COPY_AND_ASSIGN(Class) \
    Class(const Class&); \
    void operator=(const Class&)

// Log and then return error code if exp function call fail
#define RET_IF_FAIL2(err, exp) do { \
    err = (exp);\
    if (APL_ERR_TTS_NOERROR != err) { \
        RET_ERROR(err); \
    } }while (0);

// Log and then return error code if exp function call fail
#define RET_IF_FAIL(exp) do { \
    CL_ERROR err_ = (exp);\
    if (APL_ERR_TTS_NOERROR != err_) { \
        RET_ERROR(err_); \
    } }while (0);

// Log and then return error code if err is not APL_ERR_TTS_NOERROR
#define RET_IF_ERROR(err) do { \
    if (APL_ERR_TTS_NOERROR != (err)) { \
        RET_ERROR(err); \
    } }while (0)

// Log and then return error code if ptr is null
#define RET_IF_NULL(ptr, err) do { \
    if (NULL == (ptr)) { \
        RET_ERROR(err); \
    } }while (0);

// Log and then return error code
#define RET_ERROR(err) RET_ERROR_OPTION(err, AL_ERROR_NO_OPTION)

#define RET_ERROR_OPTION(err, opt) \
    AL_ErrorLog((err), (opt)); \
    RETURN(err);

// Invalid index value
const INT TTS_INVALID_INDEX = -1;

// Poll interval
const INT TTS_WAIT_INTERVAL = 10;

// Invalid prior conversion ID
const DWORD TTS_NON_PRIOR_ID = 0;

// User interrupt request type
enum TTS_Request 
{
    TTS_REQ_NONE = 0,	// /< None request
    TTS_REQ_PAUSE,		// /< Request to pause playback or interrupt current prior
    TTS_REQ_STOP,		// /< Request to stop playback or cancel current prior
    TTS_REQ_ABORT		// /< Request to abort all priors
};

// Event execute result code
enum TTS_Result 
{
    TTS_RES_DONE = 0,	// /< Event is done
    TTS_RES_INT,		// /< Event is interrupted
    TTS_RES_QUIT,		// /< Event thread will quit
};

// ThreadBase status
enum TTS_TaskStatus 
{
    TTS_TASK_NONE = 0,	// /< Task exit or not started
    TTS_TASK_IDLE,		// /< Task is idle
    TTS_TASK_PENDING,	// /< Task is pending
    TTS_TASK_RUNNING,	// /< Task is running
    TTS_TASK_PAUSED		// /< Task is paused
};

#endif /* CXX_TTS_COMM_H */
/* EOF */
