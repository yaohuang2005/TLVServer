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

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads)
{
    stop = false;
    for(size_t i = 0;i < threads;++i)
    {
        boost::thread *t = new boost::thread(ThreadWorker(this));
        workers.push_back(t);
    }
}

// the destructor joins all threads
ThreadPool::~ThreadPool()
{
    // stop all threads
    stop = true;
    queue_cond.notify_all();

    // join them
    for(size_t i = 0; i < workers.size();++i)
        workers[i]->join();
}

// add new work item to the pool
//template<class F>
//void ThreadPool::enqueue(F f)
void ThreadPool::enqueue(Task *task)
{
    {
        // acquire lock
        boost::mutex::scoped_lock lock(queue_mutex);

        echo("[ThreadPool] enqueue a task\n");
        // add the task
        tasks.push_back(task);
    } // release lock

    // wake up one thread
    queue_cond.notify_one();
}
