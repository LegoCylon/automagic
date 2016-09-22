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

#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
#define constexpr_fun_ constexpr
#define constexpr_var_ constexpr
#else
#define constexpr_fun_ 
#define constexpr_var_ const
#endif
