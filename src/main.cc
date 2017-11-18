#include <iostream>
#include <boost/asio.hpp>
#include "server_tcpsrv.h"
#include "utility.h"

#define LOCAL_ADDR  "127.0.0.1"
#define SERV_PORT   1234



int main(int argc,char* argv[])
{
    ThreadPool *threadPool = new ThreadPool(4);

    TcpServer server(threadPool);
    server.Connect(NULL, SERV_PORT);
    server.Run();

    return 0;
}


