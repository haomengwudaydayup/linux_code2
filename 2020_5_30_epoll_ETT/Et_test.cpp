#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string>
#include <fcntl.h>
#include <errno.h>


int main(){
    
    int flag =  fcntl(0, F_GETFL);    
    fcntl(0, F_SETFL, flag | O_NONBLOCK);

    int epollfd = epoll_create(1);
    if(epollfd < 0){
        perror("epoll_create!\n");
        return 0;
    }

    //填充事件结构体
    struct epoll_event ev;
    //添加读入关心事件 并且将该fd采用边沿方式。
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = 0;

    epoll_ctl(epollfd, EPOLL_CTL_ADD, 0, &ev);

    while(1){
        struct epoll_event evs[10];
        
        //阻塞监控
        int ret = epoll_wait(epollfd, evs, 10, -1);
        if(ret < 0){
            //监控失败
            perror("epoll_wait");
            continue;
        }
        else if(0 == ret){
            perror("time out!");
            continue;
        }
        
        //遍历ret， ret为监控返回就绪的文件描述符个数
        for(int i = 0; i < ret; i++){
            
            if(evs[i].data.fd == 0){
                std::string read_data;
                while(1){
                    char buf[3] = {0};
                    ssize_t  read_size = read(0, buf, sizeof(buf) / sizeof(buf[0]) - 1);
                    if(read_size < 0){
                        //里面没有数据
                        if(errno == EAGAIN || errno == EWOULDBLOCK){
                            //unavliable value
                            break;
                        }
                        //读取失败
                        perror("read_size");
                        return 0;
                    }
                    read_data += buf;   
                    if(read_size < ssize_t(sizeof(buf) / sizeof(buf[0]) - 1)){
                        break;
                    }
                }
               printf("buf = %s\n", read_data.c_str());
            }
        }   
    }
    
    return 0;
}
