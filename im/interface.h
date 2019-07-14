/**********************************************************
 *   Author          : Apriluestc
 *   Email           : 13669186256@163.com
 *   Last modified   : 2019-07-12 14:06
 *   Filename        : interface.h
 *   Description     : 聊天室界面化，将客户端发送的数据打
 *   印到聊天输出框内，绘制用户输入框，向用户展示在线好
 *   友列表
 * *******************************************************/

#ifndef INCLUDE_INTERFACE_H
#define INCLUDE_INTERFACE_H

#include <iostream>
#include <stdlib.h>
#include <ncurses.h>
#include <string>

class InterFace {
public:
    
    // 聊天窗口标题句柄
    WINDOW* header_;
    
    // 消息输出框句柄
    WINDOW* output_;
    
    // 用户输入框句柄
    WINDOW* input_;
    
    // 在线列表绘制句柄
    WINDOW* onlinelist_;
public:
    InterFace() {
        initscr();
        curs_set(0);
    }
    ~InterFace() {
        delwin(header_);
        delwin(output_);
        delwin(input_);
        delwin(onlinelist_);
        endwin();
    }

    // 标题窗口绘制
    void createHeader() {
        int y = 0;
        int x = 0;
        int h = LINES / 5;
        int w = COLS;
        header_ = createNewInterFace(h, w, y, x);
    }

    // 消息输出框绘制
    void createOutput() {
        int y = LINES / 5;
        int x = 0;
        int h = LINES * 3 / 5;
        int w = COLS * 2 / 3;
        output_ = createNewInterFace(h, w, y, x);
    }

    // 用户输入框绘制
    void createInput() {
        int y = LINES * 4 / 5;
        int x = 0;
        int h = LINES / 5;
        int w = COLS;
        input_ = createNewInterFace(h, w, y, x);
    }

    // 在线用户列表图形化绘制
    void createOnlineList() {
        int y = LINES / 5;
        int x = COLS * 2 / 3;
        int h = LINES * 3 / 5;
        int w = COLS * 1 / 3;
        onlinelist_ = createNewInterFace(h, w, y, x);
    }

    // 从出口中获取数据，并保存在缓冲 buffer 中
    std::string getStrFromInterFace(WINDOW* w) {
        char buf[1024] = {0};
        wgetnstr(w, buf, sizeof(buf));
        return buf;
    }

    // 向窗口中放置数据
    void putStrToInterFace(WINDOW* w, int startY, int startX, const std::string& message) {
        mvwaddstr(w, startY, startX, message.c_str());
    }
    void Refresh(WINDOW* w) {
        wrefresh(w);
    }
    WINDOW* createNewInterFace(int h, int w, int startY, int startX) {
        WINDOW* local = newwin(h, w, startY, startX);
        box(local, 0, 0);
        return local;
    }
    void clearInterFace(WINDOW* w, int begin, int line) {
        while (line-- > 0) {
            wmove(w, begin++, 0);
            wclrtoeol(w);
        }
    }
}; // InterFace

#endif // INCLUDE_INTERFACE_H
