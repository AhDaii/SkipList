#ifndef __SERVER_H__
#define __SERVER_H__

#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

class TcpServer {
private:
    int m_fd;  // 用于监听的fd
public:
    TcpServer();
    ~TcpServer();
    /**
     * @brief tcp服务器监听设置
     *
     * @param port 监听的端口
     * @return int 监听失败返回-1，否则返回[0, +∞]
     */
    int set_listen(unsigned int port);
    /**
     * @brief tcp服务器处理连接请求
     *
     * @param addr 请求连接端的套接字信息
     */
    void accept_conn(sockaddr_in* addr = nullptr);
};

#endif  // __SERVER_H__