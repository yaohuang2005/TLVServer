/*  TLVServer
 *  ===============
 *  Copyright (C) 2017 yaohuang2005@gmail.com
 *
 *  Licensed under the GNU LESSER GENERAL PUBLIC LICENSE;
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#include "task_factory.h"
#include "task_tcpreadmsg.h"
#include "utility.h"

TaskFactory* TaskFactory::m_ins = NULL;
auto_ptr<TaskFactory> TaskFactory::m_auto_ptr;
boost::mutex TaskFactory::inst_mutex;

TaskFactory::TaskFactory()
{
    m_auto_ptr = auto_ptr<TaskFactory>(this);
}

TaskFactory::~TaskFactory()
{
}

TaskFactory* TaskFactory::Ins()
{
    boost::mutex::scoped_lock lock(inst_mutex);
    if ( m_ins == NULL)
        m_ins = new TaskFactory();
    return m_ins;
}

Task* TaskFactory::CreateTask(int type, void *arg)
{
    switch (type)
    {
        case TASK_TYPE_TCP_READ_MSG:
            return new TaskTcpReadMsg(arg);
        default:
            return NULL;
    }
}

