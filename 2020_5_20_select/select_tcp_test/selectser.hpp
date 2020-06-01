#pragma once

/*
 *   封装select接口功能 - 为了更加方便使用tcp编程
 */

#include"tcpser.hpp"

#include<stdio.h>
#include<unistd.h>
#include<sys/select.h>
#include<vector>

class SelectSvr{
    public:
        
        //构造函数
        SelectSvr(){
            //清空readfds集合
            FD_ZERO(&readfds_);
            //初始化maxfd_
            maxfd_ = -1;
        }

        //添加管理文件描述符到可读事件集合
        void AddFdInSet(int fd){

            FD_SET(fd, &readfds_); 
            //如果插入的文件大， 需要更新maxfd_
            if(fd > maxfd_){
                maxfd_ = fd;
            }
        }
        
        //从集合删除fd
        void DeleteFdForSet(int fd){
            
            if(FD_ISSET(fd, &readfds_)){
                //fd在集合当中
                FD_CLR(fd, &readfds_);
                if(maxfd_ == fd){
                    //删除的最后一个fd， 需要更新maxfd_
                    maxfd_--;
                }
            }
        }
        
        int SelectWait(std::vector<TcpSer>& output){
            
            //定义超时时间
            struct timeval tv;
            //秒
            tv.tv_sec = 0;
            //毫秒 3000ns = 3s
            tv.tv_usec = 300000;

            fd_set tmp = readfds_;
            //监听 - 超时时间
            int fdnum = select(maxfd_ + 1, &tmp, NULL, NULL, &tv);
            if(fdnum < 0){
                //监控出错
                perror("select error!\n");
                return false;
            }
            else if(0 == fdnum){
                //超时
             //   perror("timeout!\n");
                return false;
            }

            //正常情况
            for(int i = 0; i <= maxfd_; i++){
                
                if(FD_ISSET(i, &tmp)){
                    //i号fd发生事件
                    TcpSer ts;
                    ts.Setfd(i);
                    output.push_back(ts);
                }
            }
            
            return true;
        }
    private:
        //最大文件描述符数指
        int maxfd_;
        //定义管理数组 - 可读
        fd_set readfds_;
};
