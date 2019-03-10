/**
 * Copyright @ 2015 - 2017 Suntec Software(Shanghai) Co., Ltd.
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
 * @file VR_FSMState.h
 * @brief state machine state
 *
 * @attention used for C++ only.
 */

#ifndef VR_FSMSTATE_H
#define VR_FSMSTATE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

class VR_FSMStateIF
{
public:
    virtual sendEvent(destination) = 0;
};

class VR_FSMState : public VR_FSMStateIF
{
public:
    VR_FSMState(const std::string &stateName, VR_FSMMachine* machine/*, VR_FSMState* parent*/);
    virtual ~VR_FSMState();

    void onEntry();
    void onExit();
    void onEvent() {
        processEvent();
        // if (m_parent) {
        //     m_parent->onEvent();
        // }
    }
    void registerEvent(const std::string &eventName, func* callback);

    void sendEvent(destination);

private:
    VR_FSMMachine* m_machine;
    std::map<std::string, func*> m_eventMap;
    func* m_entryFunc;
    func* m_exitFunc;
    // VR_FSMState* m_parent;

    void processEvent();
};

class VR_FSMMachine
{
    VR_FSMMachine(const std::string &machineName);
    virtual ~VR_FSMMachine();

    void addState(const std::string &stateName);
    void registerEntry(const std::string &stateName, func* callback);
    void registerExit(const std::string &stateName, func* callback);
    void registerEvent(const std::string &stateName, const std::string &eventName);
    void registerEvent(const std::string &stateName, const std::string &eventName, const std::string &transitionTarget)
    {
        m_transitionMap.insert(std::pair<std::string, std::string>(eventName, transitionTarget));
        registerEvent(stateName, eventName);
    }
    // void transition(const std::string &stateName);
    void onEvent(event)
    {
        m_currentState->onEvent(event);
        getTransitionTarget(event);
        if (target) {
            transition(target);
        }
    }

    void sendEvent(destination);

private:
    std::map<std::string, std::string> m_transitionMap;
    std::map<std::string, VR_FSMState*> m_stateMap;
    EventListener* m_listener;

    void transition(target)
    {
        m_currentState->onExit();
        m_currentState = target;
        m_currentState->onEntry();
    }
};

#endif // VR_FSMSTATE_H
/* EOF */