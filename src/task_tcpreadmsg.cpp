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

#include "task_tcpreadmsg.h"
#include "server_tcpsrv.h"
#include "utility.h"

TaskTcpReadMsg::TaskTcpReadMsg(void *arg) : Task(arg)
{
    line = new char(MAXBTYE + 1);
    line[MAXBTYE] = '\0';
}

TaskTcpReadMsg::~TaskTcpReadMsg()
{
    delete line;
}

int TaskTcpReadMsg::readNBytes(int fd, char *bp, size_t len)
{
    int cnt;
    int rc;

    cnt = len;
    while (cnt > 0)
    {
        rc = recv(fd, bp, cnt, 0);
        if (rc < 0)				/* read error? */
        {
            if (errno == EINTR)	/* interrupted? */
                continue;			/* restart the read */
            return -1;				/* return error */
        }
        if (rc == 0)				/* EOF? */
            return len - cnt;		/* return short count */
        bp += rc;
        cnt -= rc;
    }
    return len;
}

int TaskTcpReadMsg::readVariableRec(int fd, char *bp, size_t len)
{
    u_int16_t rectype;
	u_int32_t reclen;

    int rc;

    // read header type
    rc = readNBytes(fd, (char *)&rectype, sizeof(u_int16_t));
    if (rc != sizeof(u_int16_t))
    	return rc < 0 ? -1 : 0;
    rectype = ntohs(rectype);

    // read header length
    rc = readNBytes(fd, (char *)&reclen, sizeof(u_int32_t));
    if (rc != sizeof(u_int32_t))
    	return rc < 0 ? -1 : 0;
    reclen = ntohl(reclen);

    if (reclen > len)
    {
    	// Ensure the buffer is big enough to hold the record,
    	// so read initial part of record data
        while (reclen > 0)
        {
            rc = readNBytes(fd, bp, len);
            if ( rc != (int)len )
                return rc < 0 ? -1 : 0;
            reclen -= len;
            if (reclen < len)
                len = reclen;
        }
        return -1;
    }

    /* Retrieve the rest part of record data */
    if (reclen > 0) {
    	rc = readNBytes(fd, bp, reclen);
    	if (rc != (int)reclen)
    		return rc < 0 ? -1 : 0;
    }


    TcpPkg* rdata = (TcpPkg*)arg;

    fprintf(stderr, "[%s:", rdata->src_ip);
    fprintf(stderr, "%d] ", rdata->src_port);


    if (rectype == 0xe110) {
    	fprintf(stderr, "[%s] ", "Hello");
    } else if (rectype == 0xDA7A) {
    	fprintf(stderr, "[%s] ", "Data");
    } else if (rectype == 0x0B1E){
    	fprintf(stderr, "[%s] ", "Goodbye");
    }

    fprintf(stderr, "[%d] ", reclen);
    u_int32_t maxPrintData = 4;
    fprintf(stderr, "[");

    for (u_int32_t i = 0; i < reclen && i < maxPrintData; i++) {
    	if (i != (maxPrintData - 1))
    		fprintf(stderr, "0x%x ", bp[i]);
    	else
    		fprintf(stderr, "0x%x", bp[i]);
    }

    fprintf(stderr, "]\n");

    return rc;
}

void TaskTcpReadMsg::run()
{
    int n, i;
    TcpPkg* rdata = (TcpPkg*)arg;
    int fd = rdata->fd;
    int size;

    line[0] = '\0';
    if ((n = readVariableRec(fd, line, MAXBTYE)) < 0)
    {
        if (errno == ECONNRESET)
            close(fd);
        if (rdata != NULL)
            delete rdata;
    }
    else if (n == 0)
    {
        close(fd);
        if (rdata != NULL)
            delete rdata;
    }
    else
    {
        size = n;
        for (i = 0; i < n; ++i)
        {
            if (line[i] == '\n')
            {
                line[i] = '\0';
                size = i + 1;
            }
        }
        if (line[0] != '\0')
        {
            if (!rdata->srv->TriggerSend(fd, line, size))
                echo("[TaskTcpReadMsg] Error: trigger send fail.\n");
        }
    }
}

