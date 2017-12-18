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
#ifndef _TASK_TCP_WRITE_MSG_H_
#define _TASK_TCP_WRITE_MSG_H_

#include "task.h"

#define TASK_TYPE_TCP_WRITE_MSG 2

class TaskTcpWriteMsg : public Task
{
    public:
        TaskTcpWriteMsg(void *arg) : Task(arg) {}
        void run();
};

#endif
