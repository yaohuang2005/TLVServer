
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
#ifndef _THREAD_WORKER_H_
#define _THREAD_WORKER_H_

class ThreadPool;

class ThreadWorker
{
private:
    ThreadPool *pool;
public:
    ThreadWorker(ThreadPool *p) : pool(p) {}
    void operator()();
};

#endif
