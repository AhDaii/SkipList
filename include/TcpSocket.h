#ifndef __TCPSOCKET_H__
#define __TCPSOCKET_H__

#include <arpa/inet.h>
#include <cstring>
#include <memory>
#include <string>
#include <unistd.h>
using std::string;
using std::unique_ptr;

class TcpSocket {
private:
    int m_fd;
    int read_n(char* str, int len);
    int write_n(char* str, int len);

public:
    TcpSocket();
    TcpSocket(int fd);
    ~TcpSocket();
    /**
     * @brief 连接到指定地址
     *
     * @param ip ip
     * @param port 端口号
     * @return int -1表示连接失败，否则表示成功
     */
    int connect_to_host(string ip, unsigned short port);

    // send_msg和recv_msg用于解决沾包问题
    int send_msg(string& msg);
    string recv_msg();
};

#endif  // __TCPSOCKET_H__