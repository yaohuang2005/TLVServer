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


