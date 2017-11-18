/*  TLVServer
 *  ===============
 *  Copyright (C) 2017 yaohuang2005@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <pthread.h>
#include <stdio.h>

#define RET_ERR -1
#define RET_OK  0

#define TRACE_FUNC_BEGIN
#define TRACE_FUNC_LEAVE
#define TRACE_FUNC_RET_D(RET)
#define TRACE_FUNC_RET_F(RET)
#define TRACE_FUNC_RET_S(RET)
#define TRACE_MAIN_BEGIN
#define TRACE_MAIN_LEAVE

int errexit(const char* format, ...);
void echo(const char* format, ...);

#endif
