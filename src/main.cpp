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

#include <vector>
#include <iostream>
#include <chrono>
#include <getopt.h>

#include <iostream>
#include <boost/asio.hpp>
#include "server_tcpsrv.h"
#include "utility.h"

#define LOCAL_ADDR  "127.0.0.1"
#define SERV_PORT   1234
// Global config
int port = 1234;
int number = 4;
ThreadPool *threadPool;


void printHelp(int rc)
{
    std::cout <<
              "-p --port <port>:       Set port to wait\n"
              "-n --number <count>:    Set maximum clients to wait\n"
              "-h --help:              Print this help\n";
    exit(rc);
}

void processArgs(int argc, char** argv)
{
    const char* const short_opts = "p:n:h";
    const option long_opts[] = {
            {"port",      0, nullptr, 'p'},  // optional
            {"number",    0, nullptr, 'n'},  // optional
            {"help",      0, nullptr, 'h'},
            {nullptr,     0, nullptr, 0}
    };

    while (true)
    {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt)
        {
            case 'p':
                port = std::atoi(optarg);
                std::cout << "port set to: " << port << std::endl;
                break;
            case 'n':
                number = std::atoi(optarg);
                std::cout << "number clients to wait: " << number << std::endl;
                break;
            case 'h':
                printHelp(EXIT_SUCCESS);
                break;
            case '?': // Unrecognized option
            default:
                printHelp(EXIT_FAILURE);
                break;
        }
    }


    if (port == 0 || number == 0) {
        printHelp(EXIT_FAILURE);
    }
}

static void shutdown()
{
    //clean up, join threads
    std::cout << "shutdown" << std::endl;
    delete threadPool;  // will shutdown all threads. and close client fd
}

static void signalHandler(int signal)
{
    write(STDERR_FILENO, "[engine] caught signal\n", 23);
    switch (signal) {
        case SIGINT:
        case SIGQUIT:
        case SIGHUP:
        case SIGTERM:
            shutdown();
            _exit(EXIT_SUCCESS);
        default:
            break;
    }
}

static void signalInit()
{
    signal(SIGINT,  &signalHandler);
    signal(SIGQUIT, &signalHandler);
    signal(SIGHUP,  &signalHandler);
    signal(SIGTERM, &signalHandler);
}

int main(int argc,char* argv[])
{
    processArgs(argc, argv);

    threadPool = new ThreadPool(number);

    TcpServer server(threadPool);
    /* Signal handler */
    signalInit();

    server.Connect(NULL, port);
    server.Run();

    return 0;
}

