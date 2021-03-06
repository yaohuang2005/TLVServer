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
#ifndef _SERVER_TCP_SERVER_H_
#define _SERVER_TCP_SERVER_H_

#include <sys/socket.h>      // socket interface
#include <sys/epoll.h>       // epoll interface
#include <netinet/in.h>      // struct sockaddr_in
#include <arpa/inet.h>       // IP addr convertion
#include <fcntl.h>           // File descriptor controller
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>          // bzero()
#include <stdlib.h>          // malloc(), delete 
#include <errno.h>
#include <queue>
#include <string>
#include <unordered_set>
#include <inttypes.h>
#include <sys/timerfd.h>

#include "utility.h"
#include "threadpool.h"
#include "threadworker.h"
#include "task_factory.h"
#include "task_tcpreadmsg.h"
#include "task_tcpwritemsg.h"
#include "server.h"

#define OPEN_MAX    100
#define LISTENQ     20
#define INFTIM      1000
#define TIMEOUT     500

class TcpServer;

struct TcpPkg
{
    // fd to TCP connection
    int fd;
    // real message length
    int size;
    // message to send
    string *msg;
    // TcpServer
    TcpServer *srv;

    char src_ip[40];
    int  src_port;
};

class TcpServer : public Server
{
    private:
        // message to send
        queue<TcpPkg*> sendingQueue;
        // epoll descriptor from epoll_create()
        int epfd;
        // register epoll_ctl()
        epoll_event ev;
        // store queued events from epoll_wait()
        epoll_event events[LISTENQ];
        // Set socket as non-blocking
        void setnonblocking(int sock);

        // nfds is number of events (number of returned fd)
        int i, nfds;
        int listenfd, connfd;
        // thread pool
        ThreadPool *threadPool;
        // socket struct
        socklen_t clilen;
        sockaddr_in clientaddr;
        sockaddr_in serveraddr;

        void sendHeartbeatToClient();
        std::unordered_set<int> clientfds;
        boost::mutex clientfds_mutex;
        void sendHeartbeatThreadKickstart();
        void sendHeartbeatThreadWorker();

    public:
        TcpServer(ThreadPool*);
        virtual ~TcpServer();
        void Connect(char *host, uint16_t port);
        void Run();
        bool TriggerSend(int fd, char* data, int len);
        void ContinueSend(int fd);
        void ContinueRecv(int fd);

        void eraseClosedClientfd(int fd);
};

#endif
