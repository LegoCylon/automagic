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

#include <random>
#include <type_traits>

//--------------------------------------------------------------------------------------------------
//  In lieu of C++17 template argument deduction, make_uniform_distribution uses the common type of A & B 
//  rather than requiring them to be the same type and selects the appropriate distribution based
//  on the underlying common type.
//--------------------------------------------------------------------------------------------------
template <
    typename A, 
    typename B,
    typename C = typename std::common_type<A, B>::type,
    typename D = std::uniform_int_distribution<C>
>
auto make_uniform_distribution (
    const A& minimum, 
    const B& maximum
) -> typename std::enable_if<std::is_integral<C>::value, D>::type {
    return D{static_cast<C>(minimum), static_cast<C>(maximum)};
}

template <
    typename A, 
    typename B,
    typename C = typename std::common_type<A, B>::type,
    typename D = std::uniform_real_distribution<C>
>
auto make_uniform_distribution (
    const A& minimum, 
    const B& maximum
) -> typename std::enable_if<std::is_floating_point<C>::value, D>::type {
    return D{static_cast<C>(minimum), static_cast<C>(maximum)};
}

//--------------------------------------------------------------------------------------------------
//  In lieu of C++17 std::sample, this is also just intended to select one random element from the
//  range.
//--------------------------------------------------------------------------------------------------
template <typename InputIt, typename UniformRandomBitGenerator>
auto random_element (InputIt first, InputIt last, UniformRandomBitGenerator&& g) {
    auto&& dis = make_uniform_distribution(0, std::distance(first, last) - 1);
    return std::next(first, dis(g));
}
