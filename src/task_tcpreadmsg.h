/*  TLVServer
 *  ===============
 *  Copyright (C) 2017 yaohuang2005@gmail.com
 *
 *  Licensed under the GNU LESSER GENERAL PUBLIC LICENSE
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

#define MAXBTYE 1024

class TaskTcpReadMsg : public Task
{
    private:
        char *line;
        int readNBytes( int fd, char *bp, size_t len);
        int readVariableRec( int fd, char *bp, size_t len );
    public:
        TaskTcpReadMsg(void *arg);
        ~TaskTcpReadMsg();
        void run();
};

#endif
