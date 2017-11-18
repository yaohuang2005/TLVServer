/*  TLVServer
 *  ===============
 *  Copyright (C) 2017 yaohuang2005@gmail.com
 *
 *  Licensed under the GNU LESSER GENERAL PUBLIC LICENSE
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#include "threadworker.h"
#include "threadpool.h"
#include "utility.h"
#include "task.h"

void ThreadWorker::operator()()
{
    Task *task;
    while(true)
    {
        {   // acquire lock
            boost::mutex::scoped_lock lock(pool->queue_mutex);

            echo("[ThreadWorker] waiting task\n");
            // look for a work item
            while(!pool->stop && pool->tasks.empty())
            {
                // if there are none wait for notification
                pool->queue_cond.wait(lock);
            }
            echo("[ThreadWorker] got a task\n");

            if(pool->stop) // exit if the pool is stopped
            {
                return;
            }

            // get the task from the queue
            task = pool->tasks.front();
            pool->tasks.pop_front();

        }   // release lock

        // execute the task
        echo("[ThreadWorker] run task\n");
        task->run();
        delete task;
    }
}


