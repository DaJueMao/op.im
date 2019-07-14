/**********************************************************
 *   Author          : Apriluestc
 *   Email           : 13669186256@163.com
 *   Last modified   : 2019-07-12 16:27
 *   Filename        : udp_client.h
 *   Description     : 
 * *******************************************************/

#ifndef INCLUDE_UDP_CLIENT_H
#define INCLUDE_UDP_CLIENT_H

#include <unistd.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "lib/log.h"

#define SIZE 1024
typedef int socket_t;

class Client {
private:
    socket_t sock_;
    struct sockaddr_in server_;
public:
    Client(const std::string ip, const int port) {
        server_.sin_family = AF_INET;
        server_.sin_port = htons(port);
        server_.sin_addr.s_addr = inet_addr(ip.c_str());
    }
    void initClient() {
        sock_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock_ < 0) {
            print("socket error", FATAL);
            exit(1);
        }
    }

    // 客户端接收数据
    int recvData(std::string& str) {
        struct sockaddr_in remote;
        socklen_t len = sizeof(remote);
        char buf[SIZE] = {0};
        ssize_t s = recvfrom(sock_, buf, sizeof(buf), 0, (struct sockaddr*)&remote, &len);
        if (s < 0) {
            print("recvfrom error", WARNING);
            return s;
        }
        buf[s-1] = '\0';
        int i = 0;
        while (buf[i] != '\0') {
            str += buf[i];
            i++;
        }
        return s;
    }

    // 客户端发数据
    int sendData(std::string& str) {
        ssize_t s = sendto(sock_, str.c_str(), str.size(), 0, (struct sockaddr*)&server_, sizeof(server_));
        if (s < 0) {
            print("sendto error", WARNING);
            return s;
        }
        return s;
    }
    ~Client() {
        close(sock_);
    }
}; // Client

#endif // INCLUDE_UDP_CLIENT_H
