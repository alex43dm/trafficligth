#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include "TcpSocketServer.h"
#include "Log.h"

TcpSocketServer::TcpSocketServer(const std::string &port):
    Thread(),
    port(port),
    isInited(false)
{

}

TcpSocketServer::~TcpSocketServer()
{
    close(sockfd);
}

bool TcpSocketServer::init()
{
    struct sockaddr_in serv_addr;

    isInited = false;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        Log::err("ERROR opening socket");
        return false;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(port.c_str()));

    while(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        Log::err("ERROR on binding");
        sleep(1);
    }

    listen(sockfd,5);

    isInited = true;
    return true;
}

bool TcpSocketServer::run()
{
    int fd, ret;
    socklen_t address_length;
    struct sockaddr_in cli_addr;

    fd = accept4(sockfd,(struct sockaddr *) &cli_addr, &address_length, 0);
#ifdef DEBUG
        Log::info("accept");
#endif

    bzero(buf,sizeof(buf));
    while( (ret = read(fd, buf, sizeof(buf))) )
    {
        printf("srv: read: %d\n", ret);
#ifdef DEBUG
        Log::info("sock read: %d", ret);
#endif
            if(buf[0] == 0x3A)
            {
                cmd();
                ret = write(fd, buf, 6 + buf[4]);
                printf("srv: write: %d\n", ret);
            }
            else if(buf[0] == 0x3B)
            {
                ret = write(fd, buf, 2);
            }
            else
            {
                ret = write(fd, buf, 6 + buf[4]);
            }

            if(ret <= 0)
            {
                Log::err("socket write error");
            }
    }
    close(fd);
    return true;
    //printf("sock err: %d %s\n", errno, strerror(errno));
    return false;
}

void *TcpSocketServer::threadLoop()
{
    for(;;)
    {
        if(isInited)
        {
            run();
        }
        else
        {
            init();
        }
    }
    return NULL;
}
/*
            char b[2];

            for(int i=0; i<ret/2; i++)
            {
                b[0] = buf[i*2];
                b[1] = buf[i*2+1];
                int num = (int)strtol(b, NULL, 16);
                buf[i] = (unsigned char)num;
            }
*/
