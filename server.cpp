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

class Server
{
    public:
        Server();
        void repeat();
        void pitch();
        void release();
        char recieve[30];
        std::string recieve_data = "";
        int sockFD;
        int newFD;
        // private:
        addrinfo *res;
        std::string response;
        sockaddr_storage client_addr;
        socklen_t client_addr_size = sizeof(client_addr);

};

//initial the socket
Server::Server(void)
{
    auto &portNum = "9090";  // set local port number to 9090
    const unsigned int backLog = 8;  // number of connections allowed on the incoming queue

    // addrinfo hints, *res, *p;
    addrinfo hints, *p;    // we need 2 pointers, res to hold and p to iterate over
    memset(&hints, 0, sizeof(hints));

    // for more explanation, man socket
    hints.ai_family   = AF_UNSPEC;    // don't specify which IP version to use yet
    hints.ai_socktype = SOCK_STREAM;  // SOCK_STREAM refers to TCP, SOCK_DGRAM will be?
    hints.ai_flags    = AI_PASSIVE;


    // man getaddrinfo
    int gAddRes = getaddrinfo(NULL, portNum, &hints, &res);
    if (gAddRes != 0) {
        std::cerr << gai_strerror(gAddRes) << "\n";
    }

    // std::cout << "Detecting addresses" << std::endl;

    unsigned int numOfAddr = 0;
    char ipStr[INET6_ADDRSTRLEN];    // ipv6 length makes sure both ipv4/6 addresses can be stored in this variable


    // Now since getaddrinfo() has given us a list of addresses
    // we're going to iterate over them and ask user to choose one
    // address for program to bind to
    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        std::string ipVer;

        // if address is ipv4 address
        if (p->ai_family == AF_INET) {
            ipVer             = "IPv4";
            sockaddr_in *ipv4 = reinterpret_cast<sockaddr_in *>(p->ai_addr);
            addr              = &(ipv4->sin_addr);
            ++numOfAddr;
        }

        // if address is ipv6 address
        else {
            ipVer              = "IPv6";
            sockaddr_in6 *ipv6 = reinterpret_cast<sockaddr_in6 *>(p->ai_addr);
            addr               = &(ipv6->sin6_addr);
            ++numOfAddr;
        }

        // convert IPv4 and IPv6 addresses from binary to text form
        inet_ntop(p->ai_family, addr, ipStr, sizeof(ipStr));
        // std::cout << "(" << numOfAddr << ") " << ipVer << " : " << ipStr << std::endl;
    }

    // if no addresses found :(
    if (!numOfAddr) {
        std::cerr << "Found no host address to use\n";
    }

    // choose IPv4 address
    unsigned int choice = 0;
    bool madeChoice     = false;
    choice = 1;
    madeChoice = true;

    p = res;

    // let's create a new socket, socketFD is returned as descriptor
    // man socket for more information
    // these calls usually return -1 as result of some error
    sockFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockFD == -1) {
        std::cerr << "Error while creating socket\n";
        freeaddrinfo(res);
    }

    // only for situation that the port restarted right away while sockets are still active on its port
    // int opt = 1;
    // setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));

    // Let's bind address to our socket we've just created
    int bindR = bind(sockFD, p->ai_addr, p->ai_addrlen);
    if (bindR == -1) {
        std::cerr << "Error while binding socket\n";
        // if some error occurs, make sure to close socket and free resources
        close(sockFD);
        freeaddrinfo(res);
    }

    // finally start listening for connections on our socket
    int listenR = listen(sockFD, backLog);
    if (listenR == -1)
    {
        std::cerr << "Error while Listening on socket\n";
        // if some error occurs, make sure to close socket and free resources
        close(sockFD);
        freeaddrinfo(res);
    }
}

void Server::release(void)
{
    close(sockFD);
    freeaddrinfo(res);
}

void Server::repeat(void)
{
    newFD = accept(sockFD, (sockaddr *) &client_addr, &client_addr_size);
    if (newFD == -1)
    {
        std::cerr << "Error while Accepting on socket\n";
        return;
    }
    // std::cout << "got" << std::endl;  // for debug
    memset(recieve, 0, sizeof(recieve));
    auto bytes_recv = recv(newFD, recieve, 27, 0);
    std::stringstream ss;
    ss << "response is " << recieve;
    response = ss.str();
    std::cout << "response will be: " << recieve;// << std::endl;
    // send call sends the data you specify as second param and it's length as 3rd param, also returns how many bytes were actually sent
    auto bytes_sent = send(newFD, response.data(), response.length(), 0);
    close(newFD);
    recieve_data = recieve;  // I don't know why but read value of recieve during this socket is very possible to cause problem
    return;
}

int main()
{
    Server server;
    while(server.recieve_data != "q\n")
    {
        server.repeat();
    }
    server.release();
    return 0;
}
