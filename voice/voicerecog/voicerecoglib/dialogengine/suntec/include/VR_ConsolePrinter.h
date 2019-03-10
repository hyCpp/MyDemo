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
 * @file VR_MsgController.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_CONSOLEPRINTER_H
#define VR_CONSOLEPRINTER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <mutex>
#include <string>

#include <AVSCommon/Utils/Logger/Logger.h>

using namespace alexaClientSDK;

/**
 * A simple class that prints to the screen.
 */
class VR_ConsolePrinter : public avsCommon::utils::logger::Logger {
public:
    /**
     * Constructor.
     */
    VR_ConsolePrinter();

    /**
     * Prints a simple message along with an \n.
     *
     * @param stringToPrint The string to print.
     */
    static void simplePrint(const std::string& stringToPrint);

    /**
     * Prints a message with a pretty format with a \n after.
     *
     * @param stringToPrint The string to print.
     */
    static void prettyPrint(const std::string& stringToPrint);

    void emit(
        avsCommon::utils::logger::Level level,
        std::chrono::system_clock::time_point time,
        const char* threadMoniker,
        const char* text) override;

private:
    /// Used to serialize access to std::cout.
    static std::shared_ptr<std::mutex> m_globalMutex;

    /**
     * Holding a shared pointer to the mutex
     * to make sure the mutex is not already destroyed
     * when called from global's destructor
     */
    std::shared_ptr<std::mutex> m_mutex;
};

#endif // VR_CONSOLEPRINTER_H
/* EOF */
