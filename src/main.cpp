#include <iostream>
#include "TCP_Server.h"
// for test
#include "time.h"

int main()
{
    time_t last, now;
    int i = 18;
    TCP_Server server;
    // while(server.receive_data != "q\n")
    last = time(NULL);
    while(i > -1)
    {
        server.recvMsg();
        if(1)
        {
            server.sendMsg(i);
            if((time(NULL) - last) >= 1)  // change move mode every 1s
            {
                last = time(NULL);
                i = i - 1;
            }
        }
    }
    server.sendMsg(18);
    server.release();
    return 0;
}
