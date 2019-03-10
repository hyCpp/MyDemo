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
 * @file VR_DialogEngineNull.h
 * @brief interface for DialogEngineNull.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_DIALOGENGINENULL_H
#define VR_DIALOGENGINENULL_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_DialogEngineIF.h"
#include <pugixml.hpp>
#include <sstream>

/**
 * @brief The VR_DialogEngineNull class
 *
 * class declaration
 */
class VR_DialogEngineNull : public VR_DialogEngineIF
{
public:
    virtual ~VR_DialogEngineNull()
    {
    }

    virtual bool IsNullEngine()
    {
        return true;
    }

    virtual bool Initialize(VR_DialogEngineListener *listener, const VR_Settings &settings)
    {
        return false;
    }

    virtual std::string getHints(const std::string& hintsParams)
    {
        return "NULL dialog engine has no hints";
    }

    virtual bool Start()
    {
        return false;
    }

    virtual void Stop()
    {
        return;
    }

    virtual bool SendMessage(const std::string& message, int actionSeqId = VR_ACTION_SEQ_ID_INVALID)
    {
        return false;
    }

    virtual void UnInitialize()
    {
        return;
    }
};

/**
 * @brief The VR_DialogEngineNullWithReply class
 *
 * class declaration
 */
class VR_DialogEngineNullWithReply : public VR_DialogEngineIF
{
public:
    VR_DialogEngineNullWithReply()
        :m_listener(nullptr)
    {
    }

    virtual ~VR_DialogEngineNullWithReply()
    {
    }

    virtual bool IsNullEngine()
    {
        return true;
    }

    virtual bool Initialize(VR_DialogEngineListener *listener, const VR_Settings &settings)
    {
        m_listener = listener;
        return false;
    }

    virtual std::string getHints(const std::string& hintsParams)
    {
        return "NULL dialog engine has no hints";
    }

    virtual bool Start()
    {
        if (m_listener) {
            m_listener->OnStarted();
        }
        return false;
    }

    virtual void Stop()
    {
        if (m_listener) {
            m_listener->OnStopped();
        }
        return;
    }

    virtual bool SendMessage(const std::string& message, int actionSeqId = VR_ACTION_SEQ_ID_INVALID)
    {
        if (!m_listener) {
            return false;
        }

        pugi::xml_document doc;
        if (!doc.load_string(message.c_str())) {
            return false;
        }
        pugi::xml_node msgNode = doc.first_child();
        std::string msgName = msgNode.name();



        size_t pos = msgName.find("grammar");
        if (0 == pos) {
            msgNode.set_name("grammar_result");
            if ("grammar_init" == msgName || "grammar_diff" == msgName) {
                msgNode.append_attribute("op").set_value("grammar");
            }
            else if ("grammar_active" == msgName) {
                msgNode.append_attribute("op").set_value("active");
            }
            else if ("grammar_disactive" == msgName) {
                msgNode.append_attribute("op").set_value("disactive");
            }
        }
        else if (msgName == "event") {
            msgNode.set_name("event-result");

            std::string eventName = msgNode.attribute("name").as_string();
            if (eventName == "startedNotify") {
                std::string startedFinishMsg = "<action agent=\"destatus\" op=\"notifyStartFinish\"></action>";
                std::string engineStateMsg = "<action agent=\"destatus\" op=\"notifyResourceState\">"
                                                 "<resourceStateType>engine</resourceStateType>"
                                                 "<resourceState>1</resourceState>"
                                             "</action>";
                m_listener->OnRequestAction(startedFinishMsg, VR_ACTION_SEQ_ID_INVALID);
                m_listener->OnRequestAction(engineStateMsg, VR_ACTION_SEQ_ID_INVALID);
            }
        }
        else {
            return false;
        }

        msgNode.remove_attribute("errcode");
        msgNode.append_attribute("errcode").set_value("1");

        std::ostringstream oss;
        msgNode.print(oss);
        m_listener->OnRequestAction(oss.str(), VR_ACTION_SEQ_ID_INVALID);
        return false;
    }

    virtual void UnInitialize()
    {
        return;
    }

private:
    VR_DialogEngineListener* m_listener;

    VR_DialogEngineNullWithReply(const VR_DialogEngineNullWithReply& nullEngine)
    {
        this->m_listener = nullEngine.m_listener;
    }

    void operator=(const VR_DialogEngineNullWithReply& nullEngine)
    {
        this->m_listener = nullEngine.m_listener;
    }
};

#endif // VR_DIALOGENGINENULL_H
/* EOF */
