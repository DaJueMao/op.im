/**********************************************************
 *   Author          : Apriluestc
 *   Email           : 13669186256@163.com
 *   Last modified   : 2019-07-12 13:47
 *   Filename        : udp_message_packet.h
 *   Description     : udp 传送数据包格式，以及可靠性传输
 *   依据，包括包序号、发送者地址、接收者地址、最后一次发送
 *   的时间
 *   列化和反序列化接口：Serialize() 和 DeSerialize()
 * *******************************************************/

#ifndef INCLUDE_UDP_MESSAGE_PACKET_H
#define INCLUDE_UDP_MESSAGE_PACKET_H

#include <iostream>
#include <stdlib.h>

#include <jsoncpp/json/json.h>

class Api {
public:
    
    // UDP 消息包，包括用户 name、school、用户状态、message
    std::string user_name;
    std::string school;
    std::string message;
    // 消息的唯一序号
    std::string ack_id;

    // 最后一次发送的时间
    std::string last_time;

    // 接收者地址
    std::string recv_addr;
    
    // 发送者地址
    std::string send_addr;
    // 两种状态，在线 or 退出
    std::string command;
    // 回复序号即包确认 id
    std::string reply_id;

    // 状态
    std::string state = "true";

    // 应答方的发送时间
    std::string reply_time;
public:

    // 数据序列化
    void Serialize(std::string& str) {
        Json::Value js;
        js["user_name"] = user_name;
        js["school"] = school;
        js["message"] = message;
        js["ack_id"] = ack_id;
        js["last_time"] = last_time;
        js["recv_addr"] = recv_addr;
        js["send_addr"] = send_addr;
        js["command"] = command;
        js["reply_id"] = reply_id;
        js["state"] = state;
        js["reply_time"] = reply_time;
        Serialize(js, str);
    }

    // Json::Value 对象的反序列化
    void DeSerialize(std::string& str) {
        Json::Value js;
        DeSerialize(str, js);
        user_name = js["user_name"].asString();
        school = js["school"].asString();
        message = js["message"].asString();
        ack_id = js["ack_id"].asString();
        last_time = js["last_time"].asString();
        recv_addr = js["recv_addr"].asString();
        send_addr = js["send_addr"].asString();
        command = js["command"].asString();
        reply_id = js["reply_id"].asString();
        state = js["state"].asString();
        reply_time = js["reply_time"].asString();
    }
protected:
    // 真正的序列化接口，由 Json::Value 对象到 str
    void Serialize(Json::Value& js, std::string& str) {
#ifdef _FAST_
        Json::FastWriter w;
        str = w.write(js);
#else
        Json::StyledWriter w;
        str = w.write(js);
#endif
    }

    // 真正的反序列化接口
    void DeSerialize(std::string str, Json::Value& js) {
        Json::Reader r;
        r.parse(str, js, false);
    }
}; // Api

#endif // INCLUDE_UDP_MESSAGE_PACKET_H
