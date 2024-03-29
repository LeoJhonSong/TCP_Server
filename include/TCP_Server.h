//
// Created by SONGZhuHeng on 2019/7/30.
//
#ifndef TCP_SERVER_TCP_SERVER_H
#define TCP_SERVER_TCP_SERVER_H

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
// #include <algorithm>  // remove_if()

#define RECEIVE_LENGTH 30
#define LOCAL_PORT "9090"

//                            *       *       *       *       *       *       *       *       *       *       *   *   *   *   *   *
//                            帧头    信息字  LED1    LED2    舵机1   舵机2   舵预1   舵预2   舵预3   舵预3   前后侧移方向上下校验帧尾
#define MOVE_FORWARD         "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x7f\x7f\x7c\xfd\xfd"
#define MOVE_BACKWARD        "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\x7f\x7f\x7f\x83\xfd\xfd"
#define MOVE_LEFT            "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x00\x7f\x7f\x7c\xfd\xfd"
#define MOVE_RIGHT           "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\xff\x7f\x7f\x83\xfd\xfd"
#define MOVE_TURN_LEFT       "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x7f\x00\x7f\x7c\xfd\xfd"
#define MOVE_TURN_RIGHT      "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x7f\xff\x7f\x83\xfd\xfd"
#define MOVE_UP              "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x7f\x7f\x00\x7c\xfd\xfd"
#define MOVE_DOWN            "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x7f\x7f\xff\x83\xfd\xfd"
#define MOVE_HALF_FORWARD    "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x3f\x7f\x7f\x7f\x7c\xfd\xfd"
#define MOVE_HALF_BACKWARD   "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xbf\x7f\x7f\x7f\x83\xfd\xfd"
#define MOVE_HALF_LEFT       "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x3f\x7f\x7f\x7c\xfd\xfd"
#define MOVE_HALF_RIGHT      "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\xbf\x7f\x7f\x83\xfd\xfd"
#define MOVE_HALF_TURN_LEFT  "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x7f\x3f\x7f\x7c\xfd\xfd"
#define MOVE_HALF_TURN_RIGHT "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x7f\xbf\x7f\x83\xfd\xfd"
#define MOVE_HALF_UP         "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x7f\x7f\x3f\x7c\xfd\xfd"
#define MOVE_HALF_DOWN       "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x7f\x7f\xbf\x83\xfd\xfd"
#define MOVE_SLEEP           "\xfe\xfe\x03\x00\x03\xb6\x03\xb6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x7f\x7f\x7f\x03\xfd\xfd"

#define SEND_FORWARD            0
#define SEND_BACKWARD           1
#define SEND_LEFT               2
#define SEND_RIGHT              3
#define SEND_TURN_LEFT          4
#define SEND_TURN_RIGHT         5
#define SEND_UP                 6
#define SEND_DOWN               7
#define SEND_HALF_FORWARD       8
#define SEND_HALF_BACKWARD      9
#define SEND_HALF_LEFT          10
#define SEND_HALF_RIGHT         11
#define SEND_HALF_TURN_LEFT     12
#define SEND_HALF_TURN_RIGHT    13
#define SEND_HALF_UP            14
#define SEND_HALF_DOWN          15
#define SEND_SLEEP              16

class TCP_Server {
    public:
        int isOneLeak = 0;
        int isTwoLeak = 0;
        float depth = 0;

        TCP_Server();
        ~TCP_Server();
        void recvMsg();
        void sendMsg(int move);
    private:
        int is_new = 0;
        int sockFD;
        int newFD;
        char receive[RECEIVE_LENGTH];
        addrinfo *res;
        sockaddr_storage client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
};

#endif //TCP_SERVER_TCP_SERVER_H
