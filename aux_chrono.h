//--------------------------------------------------------------------------------------------------
//  Copyright 2016 Andy Bond
// 
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//  
//  http://www.apache.org/licenses/LICENSE-2.0
//  
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//--------------------------------------------------------------------------------------------------
#pragma once

#include <chrono>
#include <utility>

//--------------------------------------------------------------------------------------------------
template <
    typename Clock = std::chrono::steady_clock,
    typename Duration = std::chrono::nanoseconds,
    typename F, // Any callable object
    typename... Ts // Parameters to forward to F
>
inline decltype(auto) timed_call (Duration& duration, F&& function, Ts&&... ts) {
    struct Timer {
        Duration& m_duration;
        const std::chrono::time_point<Clock> m_start;

        explicit Timer (Duration& duration) : m_duration(duration), m_start(Clock::now()) { }
        ~Timer () {
            m_duration = std::chrono::duration_cast<Duration>(Clock::now() - m_start);
        }
    } timer(duration);
    return function(std::forward<Ts>(ts)...);
}
