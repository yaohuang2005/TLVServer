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
#include "server_tcpsrv.h"
#include "utility.h"

TcpServer::TcpServer(ThreadPool* pool)
{
    threadPool = pool;

    // epoll descriptor, for handling accept
    epfd = epoll_create(256);
    listenfd = socket(PF_INET, SOCK_STREAM, 0);
    // set the descriptor as non-blocking
    setnonblocking(listenfd);
    // event related descriptor
    ev.data.fd = listenfd;
    // monitor in message, edge trigger
    ev.events = EPOLLIN | EPOLLET;
    // register epoll event
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
}

void TcpServer::Connect(char *host, uint16_t port)
{
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    if (host)
        inet_aton(host, &(serveraddr.sin_addr));
    else
        serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(port);
    bind(listenfd, (sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);
}

TcpServer::~TcpServer()
{
    delete threadPool;
}

void TcpServer::setnonblocking(int sock)
{
    int opts;
    if ((opts = fcntl(sock, F_GETFL)) < 0)
        errexit("GETFL %d failed", sock);
    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0)
        errexit("SETFL %d failed", sock);
}

void TcpServer::eraseClosedClientfd(int fd){
	boost::mutex::scoped_lock lock(clientfds_mutex);
	clientfds.erase(fd);
}

void TcpServer::sendHeartbeatToClient()
{
	long n = 0;
	for ( auto fd : clientfds) {
		if (fd != 0) {
			if ((n = send(fd, "heartbeat\n", 10, 0)) < 0)
			{
				if (errno == ECONNRESET)
					close(fd);
				eraseClosedClientfd(fd);
			}
			else if (n == 0)
			{
				close(fd);
				eraseClosedClientfd(fd);
			}
		}
	}
	echo("[server] finish sending heartbeat to all clients\n");
}

void TcpServer::sendHeartbeatThreadWorker()
{
    struct itimerspec timer = {
        .it_interval = {15, 0},  /* 15 second */
        .it_value    = {15, 0},
    };
    uint64_t count;
    int fd = timerfd_create(CLOCK_MONOTONIC, 0);
    timerfd_settime(fd, 0, &timer, NULL);
    for (;;) {
        read(fd, &count, sizeof(count));
        sendHeartbeatToClient();
    }
}

void TcpServer::sendHeartbeatThreadKickstart() {
	// only kick out one timer thread
	boost::thread heartbeatSenderThread(&TcpServer::sendHeartbeatThreadWorker, this);
	heartbeatSenderThread.detach();
}

void TcpServer::Run()
{
    int optval = 1;
    sendHeartbeatThreadKickstart();
    for(;;)
    {
        // waiting for epoll event
        nfds = epoll_wait(epfd, events, LISTENQ, TIMEOUT);

        // In case of edge trigger, must go over each event
        for (i = 0; i < nfds; ++i)
        {
            // Get new connection
            if (events[i].data.fd == listenfd)
            {
                // accept the client connection
                connfd = accept(listenfd, (sockaddr*)&clientaddr, &clilen);
                if (connfd < 0)
                    errexit("connfd < 0");
                setnonblocking(connfd);
                setsockopt(connfd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval));
                echo("[TcpServer] connect from %s \n", inet_ntoa(clientaddr.sin_addr));
                ev.data.fd = connfd;
                // monitor in message, edge trigger
                ev.events = EPOLLIN | EPOLLET;
                // add fd to epoll queue
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
                clientfds.insert(connfd);
            }
            // Received data
            else if (events[i].events & EPOLLIN)
            {
                if (events[i].data.fd < 0)
                    continue;
                TcpPkg* pkg= new TcpPkg;
                pkg->fd = events[i].data.fd;
                pkg->srv = this;


                strcpy(pkg->src_ip, inet_ntoa(clientaddr.sin_addr));
                pkg->src_port = (int)ntohs(clientaddr.sin_port);


                Task *task = TaskFactory::Ins()->CreateTask(TASK_TYPE_TCP_READ_MSG, (void*)pkg);
                threadPool->enqueue(task);
            }
            // Have data to send
            else if (events[i].events & EPOLLOUT)
            {
                if (events[i].data.ptr == NULL)
                    continue;
                if (sendingQueue.size() == 0)
                    continue;
                Task *task = TaskFactory::Ins()->CreateTask(TASK_TYPE_TCP_WRITE_MSG, 
                        (void*)sendingQueue.front());
                sendingQueue.pop();
                threadPool->enqueue(task);
            }
        }
    }
}

bool TcpServer::TriggerSend(int fd, char* msg, int len)
{
    try
    {
        TcpPkg *pkg = new TcpPkg();
        pkg->fd = fd;
        pkg->size = len;
        pkg->msg = new string(msg);
        pkg->srv = this;
        sendingQueue.push(pkg);
        ContinueSend(fd);
    }
    catch(...)
    {
        return false;
    }
    return true;
}

void TcpServer::ContinueSend(int fd)
{
    // Modify monitored event to EPOLLOUT, wait next loop to send data
    ev.events = EPOLLOUT | EPOLLET;
    // modify moditored fd event
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}

void TcpServer::ContinueRecv(int fd)
{
    // Modify monitored event to EPOLLIN, wait next loop to receive data
    ev.events = EPOLLIN | EPOLLET;
    // modify moditored fd event
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}
