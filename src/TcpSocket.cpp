#include "../include/TcpSocket.h"

TcpSocket::TcpSocket() {
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
}

TcpSocket::TcpSocket(int fd) : m_fd(fd) {}

TcpSocket::~TcpSocket() {
    close(m_fd);
}

int TcpSocket::connect_to_host(string ip, unsigned short port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    inet_pton(AF_INET, ip.data(), &addr.sin_addr.s_addr);
    int ret;
    if ((ret = connect(m_fd, (sockaddr*)&addr, sizeof(addr))) == -1) {
        perror("connect error");
        return -1;
    }
    return ret;
}

string TcpSocket::recv_msg() {
    int len;
    read_n((char*)&len, 4);  // 取出数据长度
    len = ntohl(len);        // 网络序转成主机序

    char* buf = new char[len + 1];
    if (read_n(buf, len) != len) {
        delete[] buf;
        return string();
    }
    string ret(buf);
    delete buf;
    return ret;
}

int TcpSocket::send_msg(string& str) {
    char* buf = new char[str.size() + 4];
    int sz = htonl(str.size());

    memcpy(buf, &sz, 4);
    memcpy(buf + 4, str.data(), str.size());

    int ret = write_n(buf, str.size() + 4);
    delete[] buf;
    return ret;
}

int TcpSocket::write_n(char* str, int len) {
    int write_num = 0, left = len;
    char* p = str;
    while (left > 0) {  // 一次可能写不完
        if ((write_num = write(m_fd, str, left)) > 0) {
            p += write_num;
            left -= write_num;
        }
        else {
            perror("write error");
            return -1;
        }
    }
    return len;
}

int TcpSocket::read_n(char* str, int len) {
    int read_num = 0, left = len;
    char* p = str;
    while (left > 0) {  // 一次可能读不完
        if ((read_num = read(m_fd, str, left)) > 0) {
            p += read_num;
            left -= read_num;
        }
        else {
            perror("read error");
            return -1;
        }
    }
    return len;
}
