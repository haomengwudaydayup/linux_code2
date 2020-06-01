#include<stdio.h>
#include<unistd.h>
#include<sys/epoll.h>

int main(){

    //创建epoll事件结构 
    struct epoll_event ev;
    //输入关心事件
    ev.events = EPOLLIN;
    //关心的文件描述符
    ev.data.fd = 0;
    
    //step one - 在内核当中创建结构体 - 返回epoll操作句柄
    int epollfd = epoll_create(10);
    if(epollfd < 0){
        perror("epoll_create error!");
        return 0;
    }
    
    //step two - 添加事件结构到epoll中
    epoll_ctl(epollfd, EPOLL_CTL_ADD, 0, &ev);

    //step three
    while(1){
        
        //定义事件结构数组 - 用于结构链表当中就绪的事件结构
        struct epoll_event fd_arr[10];
        //超时 300ns = 3s
        int ret = epoll_wait(epollfd, fd_arr, sizeof(fd_arr) / sizeof(fd_arr[0]), 3000);
        if(ret < 0){
            //监控出错
            perror("epoll_wait error!");
            continue;
        }
        else if(0 == ret){
            //超时
            perror("time out!\n");
            continue;
        }

        for(int i = 0; i < ret; ++i){
            if(fd_arr[i].data.fd == 0){
                //说明0号文件描述符就绪
                char buf[1024] = {0};
                read(fd_arr[i].data.fd, buf, sizeof(buf) - 1);
                printf("buf = [%s]\n", buf);
            }
        }
    }
    return 0;
}
