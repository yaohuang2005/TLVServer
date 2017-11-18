## TLVServer
TLVServer is a tcp server that receive multiple clients connection
 to sending variable length of records.

## Design Consideration
The architecture of TLVServer is based on Boost thread library to 
create thread pool and using epoll to accept clients connections.

2. Library dependency Linux:
       libpthread
       glibc++ (for getopt)
       libboost(for multithread)
       epoll

So the diagram is:

 TLVServer(main) -> Server(mutilthreaded) -> ReadTask 



## build step: (on Centos-6.4 x86-64 box)

```
In terminal:

$git clone git@github.com:yaohuang2005/TLVServer.git
$cd TLVServer


The directory structure is:
LICENSE  Makefile  README.md  src/  test/  TLV_server



$make

## test

