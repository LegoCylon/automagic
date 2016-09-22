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

#include "aux_algorithm.h"
#include "aux_array.h"
#include "aux_iterator.h"
#include "aux_numeric.h"
#include "aux_random.h"

//--------------------------------------------------------------------------------------------------
//  C++14 AAA
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Initial features
//--------------------------------------------------------------------------------------------------
namespace Version0_2 {

struct Game {
    using Life = unsigned int;

    static constexpr auto c_lifeMax = std::numeric_limits<Life>::max();
    std::mt19937 m_engine{};
    Life m_life{c_lifeMax};

    auto Turn () {
        using Spell = decltype(m_life)& (*)(decltype(m_life)&, decltype(m_engine)&);
        static const auto c_spells = make_array<Spell>(
            [] (auto& life, auto& engine) -> auto& { //  Heal
                auto&& dis = make_uniform_distribution(0, c_lifeMax - life);
                return life += dis(engine);
            },
            [] (auto& life, auto& engine) -> auto& { //  Hurt
                auto&& dis = make_uniform_distribution(0, life);
                return life -= dis(engine);
            }
        );
        auto&& spell = random_element(std::begin(c_spells), std::end(c_spells), m_engine);
        return (*spell)(m_life, m_engine) > 0;
    }
};

} // namespace Version0_2

//--------------------------------------------------------------------------------------------------
//  Maim spell
//--------------------------------------------------------------------------------------------------
namespace Version1_2 {

struct Game {
    using Life = unsigned int;

    static constexpr auto c_lifeMax = std::numeric_limits<Life>::max();
    std::mt19937 m_engine{};
    Life m_life{c_lifeMax};

    auto Turn () {
        using Spell = decltype(m_life)& (*)(decltype(m_life)&, decltype(m_engine)&);
        static const auto c_spells = make_array<Spell>(
            [] (auto& life, auto& engine) -> auto& {    //  Heal
                auto&& dis = make_uniform_distribution(0, c_lifeMax - life);
                return life += dis(engine);
            }, 
            [] (auto& life, auto& engine) -> auto& {    //  Hurt
                auto&& dis = make_uniform_distribution(0, life);
                return life -= dis(engine);
            }, 
            [] (auto& life, auto&) -> auto& {           //  Maim
                auto&& change = choose(
                    [&life] { return life > c_lifeMax / 100 * 80; },    // (80, 100]%
                    [] { return c_lifeMax / 100 * 25; },
                    [&life] { return life > c_lifeMax / 100 * 60; },    // (60, 80]%
                    [] { return c_lifeMax / 100 * 20; },
                    [&life] { return life > c_lifeMax / 100 * 40; },    // (40, 60]%
                    [] { return c_lifeMax / 100 * 15; },
                    [&life] { return life > c_lifeMax / 100 * 20; },    // (20, 40]%
                    [] { return c_lifeMax / 100 * 10; }
                );                                                      // [0, 20]%
                return life -= change;
            }
        );
        auto&& spell = random_element(std::begin(c_spells), std::end(c_spells), m_engine);
        return (*spell)(m_life, m_engine) > 0;
    }
};

} // namespace Version1_2

//--------------------------------------------------------------------------------------------------
//  Rend spell
//--------------------------------------------------------------------------------------------------
namespace Version2_2 {

struct Game {
    using Life = unsigned int;

    static constexpr auto c_lifeMax = std::numeric_limits<Life>::max();
    std::mt19937 m_engine{};
    Life m_life{c_lifeMax};

    auto Turn () {
        using Spell = decltype(m_life)& (*)(decltype(m_life)&, decltype(m_engine)&);
        static const auto c_spells = make_array<Spell>(
            [] (auto& life, auto& engine) -> auto& {    //  Heal
                auto&& dis = make_uniform_distribution(0, c_lifeMax - life);
                return life += dis(engine);
            }, 
            [] (auto& life, auto& engine) -> auto& {    //  Hurt
                auto&& dis = make_uniform_distribution(0, life);
                return life -= dis(engine);
            }, 
            [] (auto& life, auto&) -> auto& {           //  Maim
                auto&& change = choose(
                    [&life] { return life > c_lifeMax / 100 * 80; },    // (80, 100]%
                    [] { return c_lifeMax / 100 * 25; },
                    [&life] { return life > c_lifeMax / 100 * 60; },    // (60, 80]%
                    [] { return c_lifeMax / 100 * 20; },
                    [&life] { return life > c_lifeMax / 100 * 40; },    // (40, 60]%
                    [] { return c_lifeMax / 100 * 15; },
                    [&life] { return life > c_lifeMax / 100 * 20; },    // (20, 40]%
                    [] { return c_lifeMax / 100 * 10; }
                );                                                      // [0, 20]%
                return life -= change;
            },
            [] (auto& life, auto& engine) -> auto& {    // Rend
                auto&& dis = make_uniform_distribution(0, c_lifeMax);
                return life /= recurse(
                    [] (auto&& gcd, auto&& a, auto&& b) {
                        if (b == decltype(b){})
                            return a;
                        return gcd(std::forward<decltype(gcd)>(gcd), b, modulo(a, b));
                    },
                    life, 
                    dis(engine)
                );
            }
        );
        auto&& spell = random_element(std::begin(c_spells), std::end(c_spells), m_engine);
        return (*spell)(m_life, m_engine) > 0;
    }
};

} // namespace Version2_2

//--------------------------------------------------------------------------------------------------
//  Multiplayer
//--------------------------------------------------------------------------------------------------
namespace Version3_2 {

struct Game {
    static constexpr auto c_playerCount = 4u;
    using Life = unsigned int;
    using LifeArray = std::array<Life, c_playerCount>;

    static constexpr auto c_lifeMax{std::numeric_limits<Life>::max()};
    std::mt19937 m_engine{};
    LifeArray m_life{make_filled_array(m_life, c_lifeMax)};

    auto Turn () {
        using Spell = decltype(m_life[0])& (*)(decltype(m_life[0])&, decltype(m_engine)&);
        static const auto c_spells = make_array<Spell>(
            [] (auto& life, auto& engine) -> auto& {    //  Heal
                auto&& dis = make_uniform_distribution(0, c_lifeMax - life);
                return life += dis(engine);
            }, 
            [] (auto& life, auto& engine) -> auto& {    //  Hurt
                auto&& dis = make_uniform_distribution(0, life);
                return life -= dis(engine);
            }/*, 
            [] (auto& life, auto&) -> auto& {           //  Maim
                auto&& change = choose(
                    [&life] { return life > c_lifeMax / 100 * 80; },    // (80, 100]%
                    [] { return c_lifeMax / 100 * 25; },
                    [&life] { return life > c_lifeMax / 100 * 60; },    // (60, 80]%
                    [] { return c_lifeMax / 100 * 20; },
                    [&life] { return life > c_lifeMax / 100 * 40; },    // (40, 60]%
                    [] { return c_lifeMax / 100 * 15; },
                    [&life] { return life > c_lifeMax / 100 * 20; },    // (20, 40]%
                    [] { return c_lifeMax / 100 * 10; }
                );                                                      // [0, 20]%
                return life -= change;
            },
            [] (auto& life, auto& engine) -> auto& {    // Rend
                auto&& dis = make_uniform_distribution(0, c_lifeMax);
                return life /= recurse(
                    [] (auto&& gcd, auto a, auto b) {
                        if (b == decltype(b){})
                            return a;
                        return gcd(std::forward<decltype(gcd)>(gcd), b, modulo(a, b));
                    },
                    life, 
                    dis(engine)
                );
            }*/
        );
        return accumutate(
            std::begin(m_life),
            std::end(m_life),
            [&engine = m_engine] (auto& life) {
                if (life > 0) {
                    auto&& spell = random_element(std::begin(c_spells), std::end(c_spells), engine);
                    (*spell)(life, engine);
                }
                return life > 0;
            },
            [] (auto&& anyAlive, auto&& result) { return anyAlive || result; }
        );
    }
};

} // namespace Version3_2
