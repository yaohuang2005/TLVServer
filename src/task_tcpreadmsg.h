/*  TLVServer
 *  ===============
 *  Copyright (C) 2017 yaohuang2005@gmail.com
 *
 *  Licensed under the GNU LESSER GENERAL PUBLIC LICENSE
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
#ifndef _TASK_TCP_READ_MSG_H_
#define _TASK_TCP_READ_MSG_H_

#include "task.h"

#define TASK_TYPE_TCP_READ_MSG 1

#define MAXBYTE 1024

/**
 * MsgHeader, totally has 6 bytes
 */
struct MsgHeader
{
    // msg tyep
    u_int16_t type;
    // real message length
    u_int32_t length;
};

class TaskTcpReadMsg : public Task
{
    private:
        char *line;
        int readn( int fd, char *bp, size_t len);
        int readvrec( int fd, char *bp, size_t len );
    public:
        TaskTcpReadMsg(void *arg);
        ~TaskTcpReadMsg();
        void run();
};

#endif
