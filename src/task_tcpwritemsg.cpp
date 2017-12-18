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
#include <sys/socket.h>      // recv() and send()
#include <sys/epoll.h>       // epoll interface
#include <unistd.h>
#include <stdio.h>
#include <string.h>          // strerror()
#include <errno.h>

#include "server_tcpsrv.h"
#include "task_tcpwritemsg.h"
#include "utility.h"

void TaskTcpWriteMsg::run()
{
    unsigned int n;
    // data to wirte back to client
    TcpPkg *rdata = (TcpPkg*)arg;

    // send responce to client
    if ((n = send(rdata->fd, "server\n", 7, 0)) < 0)
    {
        if (errno == ECONNRESET)
            close(rdata->fd);
    }
    else if (n == 0)
    {
        close(rdata->fd);
    }
    else
    {
        rdata->srv->ContinueRecv(rdata->fd);
    }
    // delete data
    delete rdata->msg;
    delete rdata;
}
