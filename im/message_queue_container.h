/**********************************************************
 *   Author          : Apriluestc
 *   Email           : 13669186256@163.com
 *   Last modified   : 2019-07-12 13:42
 *   Filename        : message_container.h
 *   Description     : 存放聊天消息的容器
 *   对外提供放数据和拿数据的接口
 * *******************************************************/

#ifndef INCLUDE_MESSAGE_QUEUE_CONTAINER_H
#define INCLUDE_MESSAGE_QUEUE_CONTAINER_H

#include <iostream>
#include <vector>
#include <string>
#include <semaphore.h>

class Container {
private:
    int capacity_;
    int product_;
    int consume_;
    std::vector<std::string> container_;
    sem_t blank_;
    sem_t data_;
    int size_;
public:
    Container(int capacity)
        :capacity_(capacity),
        product_(0),
        consume_(0),
        container_(capacity),
        size_(0)
    {
        sem_init(&blank_, 0, capacity);
        sem_init(&data_, 0, 0);
    }

    // 从网络中获取数据
    void getData(std::string& str) {
        sem_wait(&data_);
        str = container_[consume_];
        consume_++;
        size_--;
        consume_ %= capacity_;
        sem_post(&blank_);
    }

    // 发布消息至网络
    void putData(std::string& str) {
        sem_wait(&blank_);
        container_[product_] = str;
        product_++;
        size_++;
        product_ %= capacity_;
        sem_post(&data_);
    }

    bool isFull() {
        if (size_ == capacity_) {
            return true;
        }
        return false;
    }

    // 新增判断保存消息的容器状态，若 container 不空，注册回调函数
    bool isEmpty() {
        if (size_ == 0) {
            return true;
        }
        return false;
    }
    ~Container() {
        capacity_ = 0;
        size_ = 0;
        sem_destroy(&blank_);
        sem_destroy(&data_);
    }
}; // Container

#endif // INCLUDE_MESSAGE_QUEUE_CONTAINER_H
