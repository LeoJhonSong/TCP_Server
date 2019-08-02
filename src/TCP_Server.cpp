//
// Created by leo on 2019/7/30.
//

#include "TCP_Server.h"

TCP_Server::TCP_Server(void)
// initial the socket
{
    auto &portNum = LOCAL_PORT;  // set local port number to 9090
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

    sockFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    // only for situation that the port restarted right away while sockets are still active on its port
    int opt = 1;
    setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
    if (sockFD == -1) {
        std::cerr << "Error while creating socket\n";
        freeaddrinfo(res);
    }

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

void TCP_Server::release(void)
// release the socket
{
    close(newFD);
    close(sockFD);
    freeaddrinfo(res);
}

void TCP_Server::recvMsg(void)
// receive message from the client (ROV)
{
    if(!is_new)
    {
        newFD = accept(sockFD, (sockaddr *) &client_addr, &client_addr_size);
        is_new = 1;
    }
    if (newFD == -1)
    {
        std::cerr << "Error while Accepting on socket\n";
        return;
    }
    // std::cout << "got" << std::endl;  // for debug
    memset(receive, 0, sizeof(receive));
    auto bytes_recv = recv(newFD, receive, 27, 0);
    if(receive[4] == '\xaa'){ isOneLeak = 1; }
    if(receive[7] == '\xaa'){ isTwoLeak = 1; }
    depth = (int(receive[8]) * 256 + int(receive[9])) / 100.0;  // the unit is meter
    receive_data = receive;  // since receive_data is a string but not char array, the length of receive_data is changeable
    std::cout << depth << std::endl;
}

void TCP_Server::sendMsg(int move)
// move:
// FORWARD          0
// BACKWARD         1
// LEFT             2
// RIGHT            3
// TURN_LEFT        4
// TURN_RIGHT       5
// UP               6
// DOWN             7
// HALF_FORWARD     8
// HALF_BACKWARD    9
// HALF_LEFT        10
// HALF_RIGHT       11
// HALF_TURN_LEFT   12
// HALF_TURN_RIGHT  13
// HALF_UP          14
// HALF_DOWN        15
{
    // switch(move)
    // {
    //     case 0:
    //         char response[28] = FORWARD;
    //         break;
    //     case 1:
    //         char response[28] = BACKWARD;
    //         break;
    //     case 2:
    //         char response[28] = LEFT;
    //         break;
    //     case 3:
    //         char response[28] = RIGHT;
    //         break;
    //     case 4:
    //         char response[28] = TURN_LEFT;
    //         break;
    //     case 5:
    //         char response[28] = TURN_RIGHT;
    //         break;
    //     case 6:
    //         char response[28] = UP;
    //         break;
    //     case 7:
    //         char response[28] = DOWN;
    //         break;
    //     case 8:
    //         char response[28] = HALF_FORWARD;
    //         break;
    //     case 9:
    //         char response[28] = HALF_BACKWARD;
    //         break;
    //     case 10:
    //         char response[28] = HALF_LEFT;
    //         break;
    //     case 11:
    //         char response[28] = HALF_RIGHT;
    //         break;
    //     case 12:
    //         char response[28] = HALF_TURN_LEFT;
    //         break;
    //     case 13:
    //         char response[28] = HALF_TURN_RIGHT;
    //         break;
    //     case 14:
    //         char response[28] = HALF_UP;
    //         break;
    //     case 15:
    //         char response[28] = HALF_DOWN;
    //         break;
    //     default:
    //         char response[28] = SLEEP;
    //         break;
    // }
    // response.erase(std::remove_if(response.begin(), response.end(), ::isspace), response.end());  // remove spaces
    // send call sends the data you specify as second param and it's length as 3rd param, also returns how many bytes were actually sent
    // auto bytes_sent = send(newFD, response.data(), response.length(), 0);
    unsigned char response[28] = SLEEP;
    unsigned char resp[28] = "\xfe fe03";
    char recw[28] = "\x14w";
    auto bytes_sent = send(newFD, response, 27, 0);
    std::cout << isOneLeak << std::endl;
    std::cout << isTwoLeak << std::endl;
}