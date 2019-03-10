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

#include <string>
#include <iostream>
#include "stdafx.h"
#include "DemoRun.h"

using namespace std;

int main(int argc, char* argv[]) 
{
    DemoRun demoRun;
    demoRun.initialize();
    demoRun.start();

    vector<string> commands;
    string cmd;

    std::cout << "Please input a sequence of event name and end with a \"NULL\" command:" << std::endl;
    while (cmd != "NULL") {
        std::cin >> cmd;
        if (cmd != "NULL") {
            commands.push_back(cmd);
        }
    }

    demoRun.Run(commands);

    char ch;
    std::cout << "Press q to quit" << std::endl;
    std::cin >> ch;
    while (ch != 'q' && ch != 'Q') {
        sleep(1);
        std::cin >> ch;
    } 

    demoRun.stop();
    demoRun.deinitialize();

    return 0;
}

/* EOF */
