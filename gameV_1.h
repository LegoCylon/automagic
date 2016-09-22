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

#include "aux_array.h"
#include "aux_numeric.h"
#include "aux_random.h"

//--------------------------------------------------------------------------------------------------
//  C++11 AAA (MSVC2010 compatible)
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Initial features
//--------------------------------------------------------------------------------------------------
namespace Version0_1 {

struct Game {
    typedef unsigned int Life;

    static const auto c_lifeMax = std::numeric_limits<Life>::max();
    std::mt19937 m_engine;
    Life m_life;

    Game () : m_engine(), m_life(c_lifeMax) { }
    auto CastHeal (Life& life) -> decltype(life) {
        auto dis = make_uniform_distribution(0, c_lifeMax - life);
        return life += dis(m_engine);
    }
    auto CastHurt (Life& life) -> decltype(life) {
        auto dis = make_uniform_distribution(0, life);
        return life -= dis(m_engine);
    }
    auto Turn () -> decltype(m_life > 0) {
        static const auto& c_spells = { &Game::CastHeal, &Game::CastHurt, };
        auto dis = make_uniform_distribution(0, size_array(c_spells) - 1);
        return (this->*(c_spells.begin())[dis(m_engine)])(m_life) > 0;
    }
};

} // namespace Version0_1

//--------------------------------------------------------------------------------------------------
//  Maim spell
//--------------------------------------------------------------------------------------------------
namespace Version1_1 {

struct Game {
    typedef unsigned int Life;

    static const auto c_lifeMax = std::numeric_limits<Life>::max();
    std::mt19937 m_engine;
    Life m_life;

    Game () : m_engine(), m_life(c_lifeMax) { }
    auto CastHeal (Life& life) -> decltype(life) {
        auto dis = make_uniform_distribution(0, c_lifeMax - life);
        return life += dis(m_engine);
    }
    auto CastHurt (Life& life) -> decltype(life) {
        auto dis = make_uniform_distribution(0, life);
        return life -= dis(m_engine);
    }
    auto CastMaim (Life& life) -> decltype(life) {
        typename std::remove_reference<decltype(life)>::type change;
        if (life > c_lifeMax / 100 * 80)            // (80, 100]%
            change = c_lifeMax / 100 * 25;
        else if (life > c_lifeMax / 100 * 60)       // (60, 80]%
            change = c_lifeMax / 100 * 20;
        else if (life > c_lifeMax / 100 * 40)       // (40, 60]%
            change = c_lifeMax / 100 * 15;
        else if (life > c_lifeMax / 100 * 20)       // (20, 40]%
            change = c_lifeMax / 100 * 10;
        else                                        // [0, 20]%
            change = 0;
        return life -= change;
    }
    auto Turn () -> decltype(m_life > 0) {
        static const auto& c_spells = { &Game::CastHeal, &Game::CastHurt, &Game::CastMaim, };
        auto dis = make_uniform_distribution(0, size_array(c_spells) - 1);
        return (this->*(c_spells.begin())[dis(m_engine)])(m_life) > 0;
    }
};

} // namespace Version1_1

//--------------------------------------------------------------------------------------------------
//  Rend spell
//--------------------------------------------------------------------------------------------------
namespace Version2_1 {

struct Game {
    typedef unsigned int Life;

    static const auto c_lifeMax = std::numeric_limits<Life>::max();
    std::mt19937 m_engine;
    Life m_life;

    static auto CalcGCD (Life a, Life b) -> decltype(a) {
        return (b == decltype(b)()) ? a : CalcGCD(b, modulo(a, b));
    }

    Game () : m_engine(), m_life(c_lifeMax) { }
    auto CastHeal (Life& life) -> decltype(life) {
        auto dis = make_uniform_distribution(0, c_lifeMax - life);
        return life += dis(m_engine);
    }
    auto CastHurt (Life& life) -> decltype(life) {
        auto dis = make_uniform_distribution(0, life);
        return life -= dis(m_engine);
    }
    auto CastMaim (Life& life) -> decltype(life) {
        typename std::remove_reference<decltype(life)>::type change;
        if (life > c_lifeMax / 100 * 80)            // (80, 100]%
            change = c_lifeMax / 100 * 25;
        else if (life > c_lifeMax / 100 * 60)       // (60, 80]%
            change = c_lifeMax / 100 * 20;
        else if (life > c_lifeMax / 100 * 40)       // (40, 60]%
            change = c_lifeMax / 100 * 15;
        else if (life > c_lifeMax / 100 * 20)       // (20, 40]%
            change = c_lifeMax / 100 * 10;
        else                                        // [0, 20]%
            change = 0;
        return life -= change;
    }
    auto CastRend (Life& life) -> decltype(life) {
        auto dis = make_uniform_distribution(0, c_lifeMax);
        return life /= CalcGCD(life, dis(m_engine));
    }
    auto Turn () -> decltype(m_life > 0) {
        static const auto& c_spells = {
            &Game::CastHeal,
            &Game::CastHurt,
            &Game::CastMaim,
            &Game::CastRend,
        };
        auto dis = make_uniform_distribution(0, size_array(c_spells) - 1);
        return (this->*(c_spells.begin())[dis(m_engine)])(m_life) > 0;
    }
};

} // namespace Version2_1

//--------------------------------------------------------------------------------------------------
//  Multiplayer
//--------------------------------------------------------------------------------------------------
namespace Version3_1 {

struct Game {
    typedef unsigned int Life;
    static const auto c_playerCount = 4u;
    typedef std::array<Life, c_playerCount> LifeArray;

    static const auto c_lifeMax = std::numeric_limits<Life>::max();
    std::mt19937 m_engine;
    LifeArray m_life;

    static auto CalcGCD (Life a, Life b) -> decltype(a) {
        return (b == decltype(b)()) ? a : CalcGCD(b, modulo(a, b));
    }

    Game () : m_engine() {
        m_life.fill(c_lifeMax);
    }
    auto CastHeal (Life& life) -> decltype(life) {
        auto dis = make_uniform_distribution(0, c_lifeMax - life);
        return life += dis(m_engine);
    }
    auto CastHurt (Life& life) -> decltype(life) {
        auto dis = make_uniform_distribution(0, life);
        return life -= dis(m_engine);
    }
    auto CastMaim (Life& life) -> decltype(life) {
        typename std::remove_reference<decltype(life)>::type change;
        if (life > c_lifeMax / 100 * 80)            // (80, 100]%
            change = c_lifeMax / 100 * 25;
        else if (life > c_lifeMax / 100 * 60)       // (60, 80]%
            change = c_lifeMax / 100 * 20;
        else if (life > c_lifeMax / 100 * 40)       // (40, 60]%
            change = c_lifeMax / 100 * 15;
        else if (life > c_lifeMax / 100 * 20)       // (20, 40]%
            change = c_lifeMax / 100 * 10;
        else                                        // [0, 20]%
            change = 0;
        return life -= change;
    }
    auto CastRend (Life& life) -> decltype(life) {
        auto dis = make_uniform_distribution(0, c_lifeMax);
        return life /= CalcGCD(life, dis(m_engine));
    }
    auto Turn () -> decltype(m_life[0] > 0) {
        static const auto& c_spells = {
            &Game::CastHeal,
            &Game::CastHurt,
            //&Game::CastMaim,
            //&Game::CastRend,
        };
        auto dis = make_uniform_distribution(0, size_array(c_spells) - 1);

        auto anyAlive = false;
        for (auto i = m_life.begin(), c = m_life.end(); i != c; ++i) {
            if (*i > 0)
                anyAlive = (this->*(c_spells.begin())[dis(m_engine)])(*i) > 0 || anyAlive;
        }
        return anyAlive;
    }
};

} // namespace Version3_1
