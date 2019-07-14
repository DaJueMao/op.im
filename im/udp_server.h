/**********************************************************
 *   Author          : Apriluestc
 *   Email           : 13669186256@163.com
 *   Last modified   : 2019-07-12 14:58
 *   Filename        : udp_server.h
 *   Description     : 聊天室 server 端，利用 map 维护在线用户列表
 *   消息转发
 * *******************************************************/

#ifndef INCLUDE_UDP_SERVER_H
#define INCLUDE_UDP_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <utility>
#include <map>

#include "lib/log.h"
#include "message_container.h"
#include "udp_message_packet.h"

#define SIZE 1024
typedef int socket_t;

class Server {
private:
    socket_t sock_;
    std::string ip_;
    int port_;
    
    // 用户列表
    std::map<in_addr_t, struct sockaddr_in> userList_;

    // 存放消息的容器
    Container container_;
protected:
    void addUser(struct sockaddr_in& remote) {
        userList_.insert(std::pair<in_addr_t, struct sockaddr_in>(remote.sin_addr.s_addr, remote));
    }
    void delUser(struct sockaddr_in& remote) {
        std::map<in_addr_t, struct sockaddr_in>::iterator it = userList_.find(remote.sin_addr.s_addr);
        if (it != userList_.end()) {
            userList_.erase(it);
        }
    }
public:
    Server(const std::string ip, const int port)
        :ip_(ip),
        port_(port),
        container_(1024)
    {}
    ~Server() {
        close(sock_);
    }
    void initServer() {
        sock_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock_ < 0) {
            print("socket error", FATAL);
            exit(1);
        }
        struct sockaddr_in local;
        local.sin_family = AF_INET;
        local.sin_port = htons(port_);
        local.sin_addr.s_addr = inet_addr(ip_.c_str());
        if (bind(sock_, (struct sockaddr*)&local, sizeof(local)) < 0) {
            print("socket error", FATAL);
            exit(2);
        }
    }

    // 服务器收数据放至 container 中
    int recvData(std::string& str) {
        struct sockaddr_in remote;
        socklen_t len = sizeof(remote);
        char buf[SIZE];
        ssize_t s = recvfrom(sock_, buf, sizeof(buf), 0, (struct sockaddr*)&remote, &len);
        if (s < 0) {
            print("recvfrom error", WARNING);
            return s;
        }
        buf[s] = '\0';
        str = buf;

        // 放置消息
        container_.putData(str);
        Api data;
        data.DeSerialize(str);
        if (data.command == "QUIT") {
            // 用户按下 ctrl + c 退出，从在线用户列表中移除该用户，若非如此则添加至 onlineList 中
            delUser(remote);
        } else {
            addUser(remote);
        }
        return s;
    }

    // 服务器发数据
    int sendData(std::string& str, struct sockaddr_in& remote) {
        ssize_t s = sendto(sock_, str.c_str(), str.size(), 0, (struct sockaddr*)&remote, sizeof(remote));
        if (s < 0) {
            print("sendto error", WARNING);
            return s;
        }
        return s;
    }

    // 广播数据，遍历在线用户列表 map<in_addr_t, struct sockaddr_in>，轮询发送
    int broadcast() {
        std::string str;
        container_.getData(str);
        std::map<in_addr_t, struct sockaddr_in>::iterator it = userList_.begin();
        for (; it != userList_.end(); it++) {
            sendData(str, it->second);
        }
        return 1;
    }
}; // Server

#endif // INCLUDE_UDP_SERVER_H
