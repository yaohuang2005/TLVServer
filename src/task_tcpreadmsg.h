#ifndef _TASK_TCP_READ_MSG_H_
#define _TASK_TCP_READ_MSG_H_

#include "task.h"

#define TASK_TYPE_TCP_READ_MSG 1

#define MAXBYTE 1024

/**
 * MsgHeader, totally has 6 bytes
 */
struct MsgHeader
{
    // msg tyep
    u_int16_t type;
    // real message length
    u_int32_t length;
};

class TaskTcpReadMsg : public Task
{
    private:
        char *line;
        int readn( int fd, char *bp, size_t len);
        int readvrec( int fd, char *bp, size_t len );
    public:
        TaskTcpReadMsg(void *arg);
        ~TaskTcpReadMsg();
        void run();
};

#endif
