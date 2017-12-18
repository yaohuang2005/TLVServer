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
#ifndef _THREAD_WORKER_FACTORY_H_
#define _THREAD_WORKER_FACTORY_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <boost/thread/mutex.hpp>
#include "task.h"
using namespace std;

class TaskFactory
{
    private:
        static auto_ptr<TaskFactory> m_auto_ptr;
        static TaskFactory *m_ins;
        static boost::mutex inst_mutex;
    protected:
        TaskFactory();
        TaskFactory(const TaskFactory&);
        virtual ~TaskFactory();
        friend class auto_ptr<TaskFactory>; 
    public:
        static TaskFactory* Ins();
        Task* CreateTask(int type, void *arg);
};

#endif

