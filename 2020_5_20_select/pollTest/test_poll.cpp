#include<stdio.h>
#include<unistd.h>
#include<poll.h>

int main(){

    //1、定义事件数组并初始化
    struct pollfd fd_arr[10];
    //填充事件结构
    fd_arr[0].fd = 0;
    //关心0号文件描述符的可读时间
    fd_arr[0].events = POLLIN;

    //2、监视
    int vaildnum = 1;

    while(1){

        //3000毫秒
        int ret = poll(fd_arr, vaildnum, 3000);
        if(ret < 0){
            perror("poll errer!");
            return 0;
        }
        else if(0 == ret){
            printf("超时了！\n");
            continue;
        }

        for(int i = 0; i < vaildnum; i++){
            if(fd_arr[i].revents = POLLIN){
                //产生事件
                char buf[1024] = {0};
                read(fd_arr[i].fd, buf, sizeof(buf) - 1);
                printf("read content: [%s]", buf);
            }
        }
    }

    return 0;
}
