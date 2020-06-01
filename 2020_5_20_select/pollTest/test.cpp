#include<stdio.h>
#include<unistd.h>
#include<sys/select.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/select.h>

int main(){

    //socket
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd < 0){
        perror("socket failed!");
        return 0;
    }
    
    //bind
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(17777);
    addr.sin_addr.s_addr = inet_addr("192.168.48.129");
    bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
        

    //定义一个可读的结构体
    fd_set readfds;
    
    //初始化
    FD_ZERO(&readfds);

    //添加 - 将标准输入文件描述符添加到set中
    FD_SET(0, &readfds);
    FD_SET(sockfd, &readfds);

            printf("FD_ISSET(0, &readfds) = %d\n", FD_ISSET(0, &readfds));
    //监控 - 关心输入， 阻塞监控
    int ret = select(sockfd + 1, &readfds, NULL, NULL, NULL);
    
    if(ret){
        printf("ret : %d\n", ret);

        if(FD_ISSET(0, &readfds) != 0){
                
            printf("FD_ISSET(sockfd, &readfds) = %d\n", FD_ISSET(sockfd, &readfds));

            char buf[1024] = {0};
            read(0, buf, sizeof(buf) - 1);
            printf("buf: %s\n", buf);
        }
        else if(FD_ISSET(sockfd, &readfds) != 0){
            
            printf("FD_ISSET(0, &readfds) = %d\n", FD_ISSET(0, &readfds));
            
            printf("%d\n", sockfd);
            
            struct sockaddr_in peeraddr;
            
            socklen_t addrlen = sizeof(peeraddr);
            int newfd = accept(sockfd, (struct sockaddr*)&peeraddr, &addrlen);
            if(newfd < 0){
                perror("accept failed!");
                return 0;
            } 

            printf("host has a new connection IP:[%s] and PORT:[%d]\n", inet_ntoa(peeraddr.sin_addr), 
                    ntohs(addrlen));

        }
    }
    
    //如果不想监控他
    FD_CLR(0, &readfds);
    FD_CLR(sockfd, &readfds);

    close(sockfd);

    return 0;

}
