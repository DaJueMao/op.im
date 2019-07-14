/**********************************************************
 *   Author          : Apriluestc
 *   Email           : 13669186256@163.com
 *   Last modified   : 2019-07-12 18:31
 *   Filename        : client.cpp
 *   Description     : 客户端开启四个线程，分别绘制四个窗
 *   口，
 * *******************************************************/

#include <set>
#include <signal.h>
#include <pthread.h>
#include <stdio.h>

#include "udp_client.h"
#include "udp_message_packet.h"
#include "interface.h"
#include "jarvis.h"

void Usage(char* usage) {
    printf("Usage: %s [clinet_ip] [client_port]\n", usage);
}

InterFace ie;
Api send_data;
Api recv_data;
std::set<std::string> circle_friend;
Client* cli = nullptr;

void* headerRun(void* arg) {
    (void)arg;
    int y, x;
    std::string message = "welcome to chat sys";
    int step = 0;
    while (1) {
        ie.createHeader();
        ie.Refresh(ie.header_);
        getmaxyx(ie.header_, y, x);
        int startY = y * 4 / 10;
        int startX = x;
        ie.putStrToInterFace(ie.header_, startY, step++, message);
        ie.Refresh(ie.header_);
        usleep(200000);
        step %= (startX - 1);
        ie.clearInterFace(ie.header_, startY, 1);
        ie.Refresh(ie.header_);
    }
    return nullptr;
}

void* outputRun(void* arg) {
    Client* client = (Client*)arg;
    int step = 1;
    int y = 0;
    int x = 0;
    int err_code = -1;
    SpeechRec sc;
    // 创建聊天输出窗口
    ie.createOutput();
    // 刷新窗口
    ie.Refresh(ie.output_);
    while (1) {
        std::string message;
        // 接收消息
        client->recvData(message);
        // 反序列化 Json 字符串
        recv_data.DeSerialize(message);
        message.clear();
        // 定义在线用户列表标识符格式为 name-school，比如：zhangjian-uestc
        std::string name_school = recv_data.user_name + "-" + recv_data.school;
        message = name_school + ": " + recv_data.message;
        if (recv_data.command == "QUIT") {
            // 处理客户端退出指令，如果退出，就从在线列表中删除该用户，以保证列表中的用户都是在线的
            circle_friend.erase(name_school);
        } else {
            // 在线用户，插入到 Map 中
            circle_friend.insert(name_school);
            // 将从消息队列中获取到的数据打印到聊天输出框
            ie.putStrToInterFace(ie.output_, step++, 1, message);
            // 调用语音合成接口，将聊天消息以语音形式播放给用户
            sc.ASR(err_code, message);
            // 若屏幕沾满，则 clear
            ie.Refresh(ie.output_);
            getmaxyx(ie.output_, y, x);
            // 处理输出框内文字格式，防止超出边界
            int startY = y;
            step %= startY;
            if (step == 0) {
                ie.createOutput();
                ie.Refresh(ie.output_);
                step = 1;
                ie.putStrToInterFace(ie.output_, step++, 1, message);
                // 调用语音合成接口，将聊天消息以语音形式播放给用户
                sc.ASR(err_code, message);
                
                ie.Refresh(ie.output_);
            }
        }
    }
    return nullptr;
}

void* inputRun(void* arg) {
    Client* client = (Client*)arg;
    std::string point = "Please Enter: ";
    while (1) {
        // 创建聊天输入窗口
        ie.createInput();
        // 刷新窗口
        ie.Refresh(ie.input_);
        // 向聊天输入框中放置消息
        ie.putStrToInterFace(ie.input_, 1, 1, point);
        // 刷新窗口
        ie.Refresh(ie.input_);
        // 将聊天输入框中的消息保存至 std::string 中
        std::string message = ie.getStrFromInterFace(ie.input_);
        // 设定聊天消息内容
        send_data.message = message;
        // 序列化之后发送的是 Json 字符串
        send_data.Serialize(message);
        // 聊天内容发送至消息队列中
        client->sendData(message);
    }
    return nullptr;
}

void* onlineListRun(void* arg) {
    (void)arg;
    ie.createOnlineList();
    ie.Refresh(ie.onlinelist_);
    int x, y;
    int step = 1;
    while (1) {
        std::string message;
        std::set<std::string>::iterator it = circle_friend.begin();
        ie.createOnlineList();
        ie.Refresh(ie.onlinelist_);
        step = 1;
        for (; it != circle_friend.end(); it++) {
            message = *it;
            ie.putStrToInterFace(ie.onlinelist_, step++, 1, message);
            ie.Refresh(ie.onlinelist_);
            getmaxyx(ie.onlinelist_, y, x);
            int startY = y;
            step %= startY;
            if (step == 0) {
                sleep(3);
                ie.createOnlineList();
                ie.Refresh(ie.onlinelist_);
                step = 1;
                ie.putStrToInterFace(ie.onlinelist_, step++, 1, message);
                ie.Refresh(ie.onlinelist_);
            }
        }
        sleep(1);
    }
    return nullptr;
}

void hander(int sig) {
    (void)sig;
    std::string message;
    send_data.command = "QUIT";
    send_data.message = "None";
    send_data.Serialize(message);
    cli->sendData(message);
    exit(1);
}

int main(int argc, char* argv[])
{
    if (argc != 5) {
        // std::cout << "Usage : [target] [object]" << std::endl;
        Usage(argv[0]);
    }
    send_data.user_name = argv[3];
    send_data.school = argv[4];
    send_data.command = "None";
    std::string ip = argv[1];
    int port = atoi(argv[2]);
    Client client(ip, port);
    client.initClient();
    cli = &client;
    struct sigaction sigSet;
    sigSet.sa_handler = hander;
    sigemptyset(&sigSet.sa_mask);
    sigSet.sa_flags = 0;
    sigaction(2, &sigSet, nullptr);
    
    pthread_t headerID;
    pthread_t outputID;
    pthread_t onlineListID;
    pthread_t inputID;
    
    pthread_create(&headerID, nullptr, headerRun, nullptr);
    pthread_create(&outputID, nullptr, outputRun, (void*)&client);
    pthread_create(&onlineListID, nullptr, onlineListRun, nullptr);
    pthread_create(&inputID, nullptr, inputRun, (void*)&client);

    pthread_join(headerID, nullptr);
    pthread_join(outputID, nullptr);
    pthread_join(onlineListID, nullptr);
    pthread_join(inputID, nullptr);
    return 0;
}
