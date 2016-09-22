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
#include <random>

//--------------------------------------------------------------------------------------------------
//  No AAA
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Initial features
//--------------------------------------------------------------------------------------------------
namespace Version0_0 {

struct Game {
    typedef unsigned int Life;

    static const Life c_lifeMax = std::numeric_limits<Life>::max();
    std::mt19937 m_engine;
    Life m_life;

    Game () : m_engine(), m_life(c_lifeMax) { }
    Life& CastHeal (Life& life) {
        std::uniform_int_distribution<Life> dis(0, c_lifeMax - life);
        return life += dis(m_engine);
    }
    Life& CastHurt (Life& life) {
        std::uniform_int_distribution<Life> dis(0, life);
        return life -= dis(m_engine);
    }
    bool Turn () {
        using Spells = Life& (Game::*[])(Life&);
        static const Spells c_spells = { &Game::CastHeal, &Game::CastHurt, };
        std::uniform_int_distribution<size_t> dis(0, size_array(c_spells) - 1);
        return (this->*c_spells[dis(m_engine)])(m_life) > 0;
    }
};

} // namespace Version0_0

//--------------------------------------------------------------------------------------------------
//  Maim spell
//--------------------------------------------------------------------------------------------------
namespace Version1_0 {

struct Game {
    typedef unsigned int Life;

    static const Life c_lifeMax = std::numeric_limits<Life>::max();
    std::mt19937 m_engine;
    Life m_life;

    Game () : m_engine(), m_life(c_lifeMax) { }
    Life& CastHeal (Life& life) {
        std::uniform_int_distribution<Life> dis(0, c_lifeMax - life);
        return life += dis(m_engine);
    }
    Life& CastHurt (Life& life) {
        std::uniform_int_distribution<Life> dis(0, life);
        return life -= dis(m_engine);
    }
    Life& CastMaim (Life& life) {
        Life change;
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
    bool Turn () {
        using Spells = Life& (Game::*[])(Life&);
        static const Spells c_spells = { &Game::CastHeal, &Game::CastHurt, &Game::CastMaim };
        std::uniform_int_distribution<size_t> dis(0, size_array(c_spells) - 1);
        return (this->*c_spells[dis(m_engine)])(m_life) > 0;
    }
};

} // namespace Version1_0

//--------------------------------------------------------------------------------------------------
//  Rend spell
//--------------------------------------------------------------------------------------------------
namespace Version2_0 {

struct Game {
    typedef unsigned int Life;

    static const Life c_lifeMax = std::numeric_limits<Life>::max();
    std::mt19937 m_engine;
    Life m_life;

    static Life CalcGCD (Life a, Life b) {
        return (b == Life()) ? a : CalcGCD(b, modulo(a, b));
    }

    Game () : m_engine(), m_life(c_lifeMax) { }
    Life& CastHeal (Life& life) {
        std::uniform_int_distribution<Life> dis(0, c_lifeMax - life);
        return life += dis(m_engine);
    }
    Life& CastHurt (Life& life) {
        std::uniform_int_distribution<Life> dis(0, life);
        return life -= dis(m_engine);
    }
    Life& CastMaim (Life& life) {
        Life change;
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
    Life& CastRend (Life& life) {
        std::uniform_int_distribution<Life> dis(0, c_lifeMax);
        return life /= CalcGCD(life, dis(m_engine));
    }
    bool Turn () {
        using Spells = Life& (Game::*[])(Life&);
        static const Spells c_spells = {
            &Game::CastHeal, 
            &Game::CastHurt, 
            &Game::CastMaim,
            &Game::CastRend,
        };
        std::uniform_int_distribution<size_t> dis(0, size_array(c_spells) - 1);
        return (this->*c_spells[dis(m_engine)])(m_life) > 0;
    }
};

} // namespace Version2_0

//--------------------------------------------------------------------------------------------------
//  Multiplayer
//--------------------------------------------------------------------------------------------------
namespace Version3_0 {

struct Game {
    enum {
        c_playerCount = 4u,
    };

    typedef unsigned int Life;
    typedef std::array<Life, c_playerCount> LifeArray;

    static const Life c_lifeMax = std::numeric_limits<Life>::max();
    std::mt19937 m_engine;
    LifeArray m_life;

    static Life CalcGCD (Life a, Life b) {
        return (b == Life()) ? a : CalcGCD(b, modulo(a, b));
    }

    Game () : m_engine() {
        m_life.fill(c_lifeMax);
    }
    Life& CastHeal (Life& life) {
        std::uniform_int_distribution<Life> dis(0, c_lifeMax - life);
        return life += dis(m_engine);
    }
    Life& CastHurt (Life& life) {
        std::uniform_int_distribution<Life> dis(0, life);
        return life -= dis(m_engine);
    }
    Life& CastMaim (Life& life) {
        Life change;
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
    Life& CastRend (Life& life) {
        std::uniform_int_distribution<Life> dis(0, c_lifeMax);
        return life /= CalcGCD(life, dis(m_engine));
    }
    bool Turn () {
        using Spells = Life& (Game::*[])(Life&);
        static const Spells c_spells = {
            &Game::CastHeal, 
            &Game::CastHurt, 
            //&Game::CastMaim,
            //&Game::CastRend,
        };
        std::uniform_int_distribution<size_t> dis(0, size_array(c_spells) - 1);

        bool anyAlive = false;
        for (LifeArray::value_type& i : m_life) {
            if (i > 0)
                anyAlive = (this->*c_spells[dis(m_engine)])(i) > 0 || anyAlive;
        }
        return anyAlive;
    }
};

} // namespace Version3_0
