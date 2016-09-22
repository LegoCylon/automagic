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
#include <iterator>

//--------------------------------------------------------------------------------------------------
template <typename Container, typename Function, typename... Ts>
constexpr_fun_ decltype(auto) call_with_range (Container&& c, Function&& f, Ts&&... ts) {
    using std::begin;
    using std::end;
    return f(begin(c), end(c), std::forward<Ts>(ts)...);
}
