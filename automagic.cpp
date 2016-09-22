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

#include "gameV_0.h"
#include "gameV_1.h"
#include "gameV_2.h"
#include "aux_chrono.h"
#include "aux_iterator.h"

#include <iostream>
#include <numeric>

//--------------------------------------------------------------------------------------------------
struct ProfileInfo {
    using Duration = std::chrono::milliseconds;
    using TurnCount = std::size_t;

    Duration m_duration{};
    TurnCount m_turnCount{};

    ProfileInfo () = default;
    ProfileInfo (const Duration& duration, const TurnCount& turnCount) :
        m_duration{duration},
        m_turnCount{turnCount}
    { }
};

//--------------------------------------------------------------------------------------------------
struct ProfileSummary {
    ProfileInfo m_maximum{
        ProfileInfo::Duration{std::numeric_limits<ProfileInfo::Duration::rep>::min()},
        std::numeric_limits<ProfileInfo::TurnCount>::min()
    };
    ProfileInfo m_average{};
    ProfileInfo m_minimum{
        ProfileInfo::Duration{std::numeric_limits<ProfileInfo::Duration::rep>::max()}, 
        std::numeric_limits<ProfileInfo::TurnCount>::max()
    };
};

//--------------------------------------------------------------------------------------------------
template <typename G>
void ProfileGame (const char* label) {

    static const auto& profile = [] (auto& i) {
        i.m_turnCount = timed_call(
            i.m_duration,
            [] () {
                G game;
                int turnCount = 0;
                while (game.Turn())
                    ++turnCount;
                return turnCount;
            }
        );
    };
    static const auto& output = [] (const auto& i, const char* label) {
        std::cout << label;
        std::cout << " Turns: " << i.m_turnCount;
        std::cout << " Time: " << i.m_duration.count();
        std::cout << std::endl;
    };

    std::array<ProfileInfo, 100> info;
    std::for_each(std::begin(info), std::end(info), profile);

    ProfileSummary summary{};
    (void)std::accumulate(
        std::cbegin(info), 
        std::cend(info),
        std::ref(summary),
        [] (auto& s, const auto& i) {
            auto& summary = s.get();
            summary.m_maximum.m_duration = std::max(summary.m_maximum.m_duration, i.m_duration);
            summary.m_maximum.m_turnCount = std::max(summary.m_maximum.m_turnCount, i.m_turnCount);
            summary.m_average.m_duration += i.m_duration;
            summary.m_average.m_turnCount += i.m_turnCount;
            summary.m_minimum.m_duration = std::min(summary.m_minimum.m_duration, i.m_duration);
            summary.m_minimum.m_turnCount = std::min(summary.m_minimum.m_turnCount, i.m_turnCount);
            return s;
        }
    );

    std::cout << label << std::endl;
    output(summary.m_average, "Tot");
    summary.m_average.m_duration /= info.size();
    summary.m_average.m_turnCount /= info.size();
    output(summary.m_maximum, "Max");
    output(summary.m_average, "Avg");
    output(summary.m_minimum, "Min");
    std::cout << std::endl;
}

//--------------------------------------------------------------------------------------------------
int main () {
    static const struct {
        void (*m_function)(const char*);
        const char* m_label;
    } c_games[] = {
#if 0
        { &ProfileGame<Version0_0::Game>, "V0.0", },
        { &ProfileGame<Version0_1::Game>, "V0.1", },
        { &ProfileGame<Version0_2::Game>, "V0.2", },
        { &ProfileGame<Version1_0::Game>, "V1.0", },
        { &ProfileGame<Version1_1::Game>, "V1.1", },
        { &ProfileGame<Version1_2::Game>, "V1.2", },
        { &ProfileGame<Version2_0::Game>, "V2.0", },
        { &ProfileGame<Version2_1::Game>, "V2.1", },
        { &ProfileGame<Version2_2::Game>, "V2.2", },
#endif
        { &ProfileGame<Version3_0::Game>, "V3.0", },
        { &ProfileGame<Version3_1::Game>, "V3.1", },
        { &ProfileGame<Version3_2::Game>, "V3.2", },
#if 0
#endif
    };
    call_with_range(
        c_games, 
        [] (auto&&... args) { return std::for_each(std::forward<decltype(args)>(args)...); }, 
        [] (const auto& g) { g.m_function(g.m_label); }
    );

    return 0;
}
