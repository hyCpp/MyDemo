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
 * @file VP_BlockingQueue.hpp
 * @brief This file defines a template of blocking queue 
 *
 * This blocking queue is used to store events received by
 * ReceiverCb 
 */

#ifndef _VP_BLOCKING_QUEUE_H
#define _VP_BLOCKING_QUEUE_H

#pragma once

#include <mutex>
#include <condition_variable>
#include <deque>

using namespace std;

template <typename T>
class VP_BlockingQueue
{
private:
    std::mutex              m_mutex;
    std::condition_variable m_condition;
    deque<T>     m_queue;

public:
    void EnQueue(T const& value) {
        /* adding this scope is used to auto-release the lock
         * see http://en.cppreference.com/w/cpp/thread/unique_lock/~unique_lock
         * unique_lock will block the current thread if the mutex has been managed by other unique_locks
         */
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_queue.push_back(value);
        }

        m_condition.notify_one();
    }

    T DeQueue() {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [=]{ 
                bool is_empty = m_queue.empty();
                RETURN(!is_empty);
                });
        T rc(std::move(m_queue.front()));
        m_queue.pop_front();

        RETURN(rc);
    }

    int Size() const {
        int size = m_queue.size();
        RETURN(size);
    }
};

#endif
/* EOF */
