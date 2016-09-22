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

#include <utility>

//--------------------------------------------------------------------------------------------------
//  parameters_of allows you to extract the types of parameters from a callable object.
//--------------------------------------------------------------------------------------------------
template <typename T>
struct parameters_of; // Not defined

template <typename F, typename... Args>
struct parameters_of<F(Args...)> {
    using type = std::tuple<std::decay_t<Args>...>;
};

//--------------------------------------------------------------------------------------------------
//  pass_split_sequence is used to expand an integer sequence into individual parameters.
//  This allows you to use variadic generic lambdas to perform an operation N times without looping.
//
//  i.e. 
//  template <std::size_t N>
//  auto print_sequence () {
//      return pass_integer_sequence(
//          [] (auto&&... is) { std::cout << is...; }, 
//          std::make_index_sequence<N>{}
//      );
//  }
//--------------------------------------------------------------------------------------------------
template <typename F, typename I, std::size_t... Is>
decltype(auto) pass_split_sequence (F&& f, std::integer_sequence<I, Is...>) {
    return f(Is...);
}
