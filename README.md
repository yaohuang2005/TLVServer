## TLVServer
TLVServer is a tcp server that receive multiple clients connection
 to sending variable length of records.

## Design Consideration
The architecture of TLVServer is based on Boost thread library to 
create thread pool and using epoll to accept clients connection.

1. The TLVServer diagram is:
 main -> server_tcpsrv -> threadpool -> threadworker -> task_tcpreadmsg 

 like a tcp/ip stack implementation, the task_tcpreadmsg readVariableRec() method
works as an input function to read and parse incoming byte stream from tcp level to get type, length, data of records
 
2. Library dependency Linux:
       libpthread
       glibc++x86_64 (for getopt)
       libboost-1.59.0 (for multithread)
       epoll
       
3. Code in C++11

4. set the serveraddr.sin_addr.s_addr = INADDR_ANY to accept every interface connection

5. to handle long-lived connection, server periodically sends heartbeat back to alive client fd (for test, now in every 15 seconds), and also set cliend fd option as "SO_KEEPALIVE"

## To be improved:
add a session layer by monitoring client fd, and creating a thread task to send heartbeat to client fd based on its inactivated duration


## build step: (on Centos-6.4 x86-64 box)

```
In terminal:
$git clone git@github.com:yaohuang2005/TLVServer.git
$cd TLVServer

The directory structure is:
LICENSE  Makefile  README.md  src/  test/  TLV_server

$make

## test
```
test case 1: client send thread records of Hello, Data, Goodbye
On server: 
[140737337153280] [ThreadWorker] run task
[140737337153280] [TaskTcpReadMsg] handling 7
[127.0.0.1:1111] [Hello] [0] []
[140737337153280] [ThreadWorker] waiting task
[140737337169696] [ThreadPool] enqueue a task
[140737326663424] [ThreadWorker] got a task
[140737326663424] [ThreadWorker] run task
[140737326663424] [TaskTcpReadMsg] handling 7
[127.0.0.1:1111] [Data] [5] [0x1 0x2 0x3 0x4]
[140737326663424] [ThreadWorker] waiting task
[140737337169696] [server] finish sending heartbeat to all clients
[140737337169696] [TcpServer] finish send heartbeat in a loop
[140737337169696] [server] finish sending heartbeat to all clients
[140737337169696] [TcpServer] finish send heartbeat in a loop

On client:
[root@localhost test]# nc -p 1111 localhost 1234 < helloDataGoodbye 
heartbeat
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

