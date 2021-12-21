#include "../include/TcpServer.h"

TcpServer::~TcpServer() {
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
}

TcpServer::~TcpServer() {
    close(m_fd);
}

int TcpServer::set_listen(unsigned int port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = 1234;
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret;
    if ((ret = bind(m_fd, (sockaddr*)&addr, sizeof(addr))) == -1) {
        perror("bind error");
        return -1;
    }
    if ((ret = listen(m_fd, 128)) == -1) {
        perror("listen error");
        return -1;
    }
    return ret;
}

void TcpServer::accept_conn(sockaddr_in* addr) {
}
