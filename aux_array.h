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

#include "aux_constexpr.h"
#include "aux_utility.h"

#include <array>
#include <initializer_list>
#include <type_traits>

//--------------------------------------------------------------------------------------------------
//  In lieu of C++17 std::experimental::make_array.
//  Note, unlike the official version, this doesn't deal with reference wrappers.
//  The static_casts avoid warnings about narrowing conversions.
//--------------------------------------------------------------------------------------------------
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1800)
template <typename... Ts, typename CommonType = std::common_type_t<Ts...>>
constexpr_fun_ auto make_array (Ts&&... ts) {
    return std::array<CommonType, sizeof...(Ts)>{ { std::forward<Ts>(ts)... } };
}

template <typename ExplicitType, typename... Ts>
constexpr_fun_ auto make_array (Ts&&... ts) {
    return std::array<ExplicitType, sizeof...(Ts)>{ { std::forward<Ts>(ts)... } };
}

//--------------------------------------------------------------------------------------------------
//  make_filled_array will initialize an array of size N with N copies of 't' to assit with member
//  initializers.
//--------------------------------------------------------------------------------------------------
template <std::size_t N, typename T>
constexpr_fun_ auto make_filled_array (const T& t) {
    return pass_split_sequence(
        [&t] (auto&&... is) { return std::array<T, N>{ { (void(is), t)... } }; },
        std::make_index_sequence<N>{}
    );
}

template <typename C, typename T, std::size_t N = std::tuple_size<C>::value>
constexpr_fun_ auto make_filled_array (const C&, const T& t) {
    return pass_split_sequence(
        [&t] (auto&&... is) { return std::array<T, N>{ { (void(is), t)... } }; },
        std::make_index_sequence<N>{}
    );
}
#endif

//--------------------------------------------------------------------------------------------------
//  In lieu of C++17 std::experimental::to_array.
//--------------------------------------------------------------------------------------------------
#if __cplusplus >= 201402L || (defined(_MSC_VER) && _MSC_VER >= 1900)
template <typename T, std::size_t N>
constexpr_fun_ auto to_array (const T(&arr)[N]) {
    return pass_split_sequence(
        [&arr] (auto&&... is) { return std::array<std::remove_cv_t<T>, N>{ {arr[is]...} }; },
        std::make_index_sequence<N>{}
    );
}
#endif

//--------------------------------------------------------------------------------------------------
//  In lieu of C++17 std::size.
//--------------------------------------------------------------------------------------------------
template <typename T, std::size_t N>
constexpr_fun_ std::size_t size_array (const T (&)[N]) noexcept {
    return N;
}

template <typename T, std::size_t N>
constexpr_fun_ std::size_t size_array (const std::array<T, N>&) noexcept {
    return N;
}

template <typename T>
constexpr_fun_ std::size_t size_array (const std::initializer_list<T>& list) noexcept {
    return list.size();
}
