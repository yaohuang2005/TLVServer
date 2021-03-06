## TLVServer
TLVServer is a tcp server that receive multiple clients connection
 to sending variable length of records.

## Design Consideration
The architecture of TLVServer is based on Boost thread library to 
create thread pool and using epoll to accept clients connection.

1. The TLVServer diagram is:
         
    (1) main -> server_tcpsrv -> threadpool -> threadworker -> task_tcpreadmsg
    
    (2)         server_tcpsrv -> heartbeatSenderThreadWorker 
                (sends heart beat to all client fd to make long-lived connection alive)
                
 Like a tcp/ip stack implementation, the task_tcpreadmsg readVariableRec() method
works as an input function to read and parse incoming byte stream from tcp level to get type, length, data of records
 
2. Library dependency Linux:
       glibc++x86_64,
       libboost-1.59.0  (for thread pool) and libpthread (for thread),
       epoll  (for socket fd events)
       
3. Code in C++11

4. set the serveraddr.sin_addr.s_addr = INADDR_ANY to accept every interface connection

5. to handle long-lived connection, tcp server periodically sends heartbeat back to all 
alive client fd (for test, now in every 15 seconds), and of course also set cliend fd option as "SO_KEEPALIVE"


## To be improved:
Add a session level by monitoring client fd, inactivated duration, and creating a thread task to send heartbeat to client fd based on idle duration

1. raplace Boost thread with C++11 Concurrency
2. repalce epoll with libevent


## Build and test step: (on Centos-6.4 x86-64 box)

```
In terminal:
$git clone git@github.com:yaohuang2005/TLVServer.git
$cd TLVServer

The directory structure is:
LICENSE  Makefile  README.md  src/  test/  TLV_server

$make

## test
test case 1: client send thread records of Hello, Data, Goodbye
On server: 
...
[140737337169696] [TcpServer] connect from 127.0.0.1 
[140737337169696] [ThreadPool] enqueue a task
[127.0.0.1:4444] [Hello] [0] []
[140737316181760] [ThreadWorker] waiting task
[140737337169696] [ThreadPool] enqueue a task
[127.0.0.1:4444] [Data] [5] [0x1 0x2 0x3 0x4]
[140737305691904] [ThreadWorker] waiting task
[140737337169696] [ThreadPool] enqueue a task
[140737337161472] [ThreadWorker] waiting task
[140737337169696] [ThreadPool] enqueue a task
[127.0.0.1:4444] [Goodbye] [0] []
[140737326671616] [ThreadWorker] waiting task
[140737337169696] [server] finish sending heartbeat to all clients
[140737337169696] [TcpServer] finish send heartbeat in a loop
...

On client:
[root@localhost test]# nc -p 4444 localhost 1234 < full
server
heartbeat
heartbeat


We can see the client also received heartbeat in every 15 seconds.

Note: for easy test, I prepare out the binary file HelloDataGoodbyte by:
echo 'E11000000000DA7A0000000501020304050B1E00000000' | \
xxd -r -p > HelloDataGoodbyte



test case 2: client send wrong msg record
on the server terminal:
[140737337153280] [ThreadWorker] waiting task
[140737326663424] [ThreadWorker] waiting task
[140737316173568] [ThreadWorker] waiting task
[140737305683712] [ThreadWorker] waiting task
[140737337169696] [ThreadPool] enqueue a task
[140737337153280] [ThreadWorker] got a task
[140737337153280] [ThreadWorker] run task
[140737337153280] [TaskTcpReadMsg] handling 7
[140737337153280] [TaskTcpReadMsg] Error: readline failed: [err 11] - Resource temporarily unavailable
[140737337153280] [ThreadWorker] waiting task
[140737337169696] [server] finish sending heartbeat to all clients
[140737337169696] [TcpServer] finish send heartbeat in a loop


On the client terminal:
[root@localhost test]# nc -p 1111 localhost 1234 
kkkkkkkkkkkk 
heartbeat
heartbeat
heartbeat

In this case we can see that server cannot parse the "kkkkkkkkkkk" byte stream, but the client still receive heartbeat in every 15 seconds

