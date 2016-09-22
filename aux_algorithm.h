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

#include <type_traits>

//--------------------------------------------------------------------------------------------------
//  choose - evaluates each choice using each If/Then pair. The first If unary op to not
//  return false will return the result of calling its Then unary op.
//  If all choices fail, then the result will be a value-initialized result of Then.
//--------------------------------------------------------------------------------------------------
template <typename If, typename Then>
decltype(auto) choose (If&& choiceIf, Then&& choiceThen) {
    return choiceIf() ? choiceThen() : std::result_of_t<Then()>();
}

template <typename If, typename Then, typename... Elses>
decltype(auto) choose (If&& choiceIf, Then&& choiceThen, Elses&&... choiceElses) {
    return choiceIf() ? choiceThen() : choose(std::forward<Elses>(choiceElses)...);
}

//--------------------------------------------------------------------------------------------------
//  recurse takes a function object and passes it to itself as the first parameter.
//  This avoids needing to explicitly assign a lambda to a variable.
//--------------------------------------------------------------------------------------------------
template <typename F, typename... Ts>
decltype(auto) recurse (F&& f, Ts&&... ts) {
    return f(std::forward<F>(f), std::forward<Ts>(ts)...);
}
