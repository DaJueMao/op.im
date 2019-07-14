/**********************************************************
 *   Author          : Apriluestc
 *   Email           : 13669186256@163.com
 *   Last modified   : 2019-07-12 18:31
 *   Filename        : Main.cc
 *   Description     : im 服务端主函数，收到数据，立即广
 *   播数据至客户端，以轮询的方式
 *
 *   服务端主要维护四个线程，即：消息发送线程、消息接收线程
 *   消息重发线程，I/O线程
 * *******************************************************/

#include "udp_server.h"

void Usage(char* usage) {
    printf("usage: %s [server_ip] [server-port]\n", usage);
}

static void* proData(void* arg) {
    Server* ser = (Server*)arg;
    while (1) {
        std::string str;
        ser->recvData(str);
    }
    return nullptr;
}

static void* conData(void* arg) {
    Server* ser = (Server*)arg;
    while (1) {
        ser->broadcast();
    }
    return nullptr;
}
int main(int argc, char* argv[])
{
    if (argc != 3) {
        Usage(argv[0]);
    }
    daemon(0, 0);
    std::string ip = argv[1];
    int port = atoi(argv[2]);
    Server ser(ip, port);
    ser.initServer();
    pthread_t product;
    pthread_t consumer;
    pthread_create(&product, nullptr, proData, &ser);
    pthread_create(&consumer, nullptr, conData, &ser);
    pthread_join(product, nullptr);
    pthread_join(consumer, nullptr);
    return 0;
}
