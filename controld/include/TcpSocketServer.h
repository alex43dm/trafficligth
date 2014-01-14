#ifndef TCPSOCKETSERVER_H
#define TCPSOCKETSERVER_H

#include <string>

#include "Thread.h"

class TcpSocketServer : public Thread
{
    public:
        std::string port;
        bool isInited;

        TcpSocketServer(const std::string &port);
        virtual ~TcpSocketServer();
        bool run();
        bool init();
        void *threadLoop();
    protected:
        virtual bool cmd() = 0;
        unsigned char buf[255];
    private:
        int sockfd;
};

#endif // TCPSOCKETSERVER_H
