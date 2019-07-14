
#ifndef INCLUDE_UDP_HEART_HANDLER_H
#define INCLUDE_UDP_HEART_HANDLER_H

#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

#define PORT 8000
#define IP "127.0.0.1"

//class Live {
//private:
    char buf[1024] = {'\0'};
//public:
    void livesndhandler(void *arg) {
        int sfd =* (int *)arg;
        struct sockaddr_in saddr;
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(PORT);
        saddr.sin_addr.s_addr = inet_addr(IP);
        bzero(saddr.sin_zero,8);
        memset(buf, 0, 64); 
        while (1) {
            strcpy(buf, "want OK!");
            sendto(sfd, (void*)buf, sizeof(buf), 0, (struct sockaddr *)&saddr, sizeof(struct sockaddr));
            sleep(5); 
        }
    }
    void timeout(int signo) {
        if(signo==SIGALRM)
            printf("you are 超时 !\n");
        exit(-1);
    }

    void livercvhandler(void *arg){

        int sfd = *(int *)arg;
        struct sockaddr_in saddr;
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(PORT);
        saddr.sin_addr.s_addr = inet_addr(IP);
        bzero(saddr.sin_zero,8); 
        socklen_t len = sizeof(struct sockaddr);
        while(1) {
            memset(buf, 0, 64);
            signal(SIGALRM, timeout);
            recvfrom(sfd, (void *)buf, sizeof(buf), 0, (struct sockaddr *)&saddr, &len);
            while(0 != strncasecmp(buf,"OK",2)) {
                memset(buf, 0, 64);
                recvfrom(sfd, (void *)buf, sizeof(buf), 0, (struct sockaddr *)&saddr, &len);
            }
        }
        return;
    }
//}; // Live

#endif // INCLUDE_UDP_HEART_HANDLER_H
