#include "TCP_Server.h"

int main()
{
    TCP_Server server;
    while(server.recieve_data != "q\n")
    {
        server.repeat();
    }
    server.release();
    return 0;
}
