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
 *   创建守护进程步骤：
 *   1、首先成为后台进程
 *   2、脱离父进程的控制终端，登录会话和进程组
 *   3、禁止重新开启控制终端
 *   4、关闭文件描述符
 *   5、重定向 0、1、2 标准文件描述符
 *   6、改变工作目录和文件掩码
 * *******************************************************/

#include "udp_server.h"
#include "lib/log.h"

void Usage(char* usage) {
    printf("usage: %s [server_ip] [server-port]\n", usage);
}

static void* print_log(void* arg) {
    (void)arg;
    std::string message;
    print(message, FATAL);
    return nullptr;
}

// 从数据池拿消息
static void* proData(void* arg) {
    Server* ser = (Server*)arg;
    while (1) {
        std::string str;
        ser->recvData(str);
    }
    return nullptr;
}

// 拿到消息广播消息指在线用户列表 map
static void* conData(void* arg) {
    Server* ser = (Server*)arg;
    while (1) {
        // 广播消息
        ser->broadcast();
    }
    return nullptr;
}
int main(int argc, char* argv[])
{
    if (argc != 3) {
        Usage(argv[0]);
    }
    // 设置服务进程位守护进程
    daemon(0, 0);
    std::string ip = argv[1];
    int port = atoi(argv[2]);
    Server ser(ip, port);
    ser.initServer();
    // 生产者线程
    pthread_t product;
    // 消费者线程
    pthread_t consumer;
    // io 线程
    pthread_t log;
    // 创建生产者线程
    pthread_create(&product, nullptr, proData, &ser);
    // 创建消费者线程
    pthread_create(&consumer, nullptr, conData, &ser);
    // 创建 io 线程，这里指日志打印线程
    pthread_create(&log, nullptr, print_log, &ser);
    // 线程等待
    pthread_join(product, nullptr);
    pthread_join(consumer, nullptr);
    pthread_join(log, nullptr);
    return 0;
}
