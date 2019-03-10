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

/* Suntec Header */
#ifndef VR_LOG_H
#    include "VR_Log.h"
#endif

#include "VR_ConsolePrinter.h"

#include <iostream>

#include <AVSCommon/Utils/Logger/LoggerUtils.h>

std::shared_ptr<std::mutex> VR_ConsolePrinter::m_globalMutex = std::make_shared<std::mutex>();

VR_ConsolePrinter::VR_ConsolePrinter() :
        avsCommon::utils::logger::Logger(avsCommon::utils::logger::Level::UNKNOWN),
        m_mutex(m_globalMutex) {
}

void VR_ConsolePrinter::simplePrint(const std::string& stringToPrint) {
    auto mutex = m_globalMutex;
    if (!mutex) {
        return;
    }

    std::lock_guard<std::mutex> lock{*mutex};
    // std::cout << stringToPrint << std::endl;
    VR_LOGI("%s\n", stringToPrint.c_str());
}

void VR_ConsolePrinter::prettyPrint(const std::string& stringToPrint) {
    const std::string line(stringToPrint.size() + 16, '#');
    std::ostringstream oss;
    oss << line << std::endl;
    oss << "#       " << stringToPrint << "       #" << std::endl;
    oss << line << std::endl;

    simplePrint(oss.str());
}

void VR_ConsolePrinter::emit(
    avsCommon::utils::logger::Level level,
    std::chrono::system_clock::time_point time,
    const char* threadMoniker,
    const char* text) {
    std::lock_guard<std::mutex> lock{*m_mutex};
    VR_LOGI("%s\n", avsCommon::utils::logger::formatLogString(level, time, threadMoniker, text).c_str());
}

/* EOF */
