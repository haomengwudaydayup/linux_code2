#include"selectser.hpp"

int main(){
    
    //创建侦听套接字
    TcpSer listen_fd;
    if(!listen_fd.Socket()) { return 0; }
    if(!listen_fd.Bind("192.168.48.129", 17777)) { return 0; }
    if(!listen_fd.Listen(5)) { return 0; }

    //创建select集合
    SelectSvr  ss;
    //添加侦听套接字fd
    ss.AddFdInSet(listen_fd.Getfd());
    
    while(1){
        
        //监控 
        //监控失败，超时 - continue
        
        //定义事件准备就绪管理数组
        std::vector<TcpSer> output;
        if(!ss.SelectWait(output)) { continue; }
        
        //轮次遍历就绪事件数组 - 并处理
        for(size_t i = 0; i < output.size(); i++){
            
            //1、侦听套接字事件就绪
            if(listen_fd.Getfd() == output[i].Getfd()){
                //即有新的连接请求， 需要使用accept函数建立一个新的连接
                
                //创建TcpSer对象管理新的socketfd & 用户的地址信息
                TcpSer tmp;
                struct sockaddr_in cliaddr;
                if(!listen_fd.Accept(tmp, cliaddr)) { continue; }   
                
                printf("Ser has a new connect IP:[%s] --> PORT:[%d]\n", inet_ntoa(cliaddr.sin_addr),
                        ntohs(cliaddr.sin_port));
                //添加new套接字fd到set中
                ss.AddFdInSet(tmp.Getfd());
            }
            else{  
                //2、非侦听套接字、创建连接的套接字
                //即：客户端发送来数据， 需要处理
                std::string buf;
                if(!output[i].Recv(buf)){
                    continue;
                }
                printf("client says: [%s]\n", buf.c_str());

                //处理 - 暂未实现
            }
        }
    }

    return 0;
}
