#include <cstring>    // sizeof()
#include <iostream>
#include <string>
#include <sstream>  // stringstream
// headers for socket(), getaddrinfo() and friends
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>    // close()

#define SEND_LENGTH 30
#define RECIEVE_LENGTH 30
#define LOCAL_PORT "9090"

class TCP_Server
{
    public:
        std::string recieve_data = "";

        TCP_Server();
        void repeat();
        void pitch();
        void release();
    private:
        int newFD;
        int sockFD;
        addrinfo *res;
        char recieve[RECIEVE_LENGTH];
        std::string response;
        sockaddr_storage client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
};
