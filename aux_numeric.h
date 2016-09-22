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

//--------------------------------------------------------------------------------------------------
//  In lieu of std::transform_reduce and std::accumulate which specifies that the range must not
//  be modified.
//--------------------------------------------------------------------------------------------------
template <
    typename InputIt, 
    typename UnaryOp, 
    typename BinaryOp, 
    typename T = decltype(std::declval<UnaryOp>()(*std::declval<InputIt>()))
>
T accumutate (InputIt first, InputIt last, UnaryOp&& unaryOp, BinaryOp&& binaryOp, T&& init = T{}) {
    for (; first != last; ++first)
        init = binaryOp(std::forward<T>(init), unaryOp(*first));
    return init;
}

//--------------------------------------------------------------------------------------------------
//  modulo will calculate the remainder for an integral or floating point type so the caller may
//  perform the operation generically.
//--------------------------------------------------------------------------------------------------
template <typename A, typename B, typename C = std::common_type_t<A, B>>
auto modulo (A&& a, B&& b) -> std::enable_if_t<std::is_floating_point<C>::value, C> {
    return std::remainder(a, b);
}

template <typename A, typename B, typename C = std::common_type_t<A, B>>
auto modulo (A&& a, B&& b) -> std::enable_if_t<std::is_integral<C>::value, C> {
    return a % b;
}
