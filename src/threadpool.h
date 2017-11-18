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
#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <vector>
#include <deque>
#include <boost/function.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <stddef.h>
#include "task.h"

class ThreadWorker;

class ThreadPool
{
private:
    friend class ThreadWorker;
    // need to keep track of threads so we can join them
    std::vector< boost::thread* > workers;
    // the task queue
    std::deque< Task* > tasks;
    // synchronization
    boost::mutex queue_mutex;
    boost::condition queue_cond;
    bool stop;
public:
    ThreadPool(size_t);
    //template<class F> void enqueue(F f);
    void enqueue(Task*);
    ~ThreadPool();
};

#endif
