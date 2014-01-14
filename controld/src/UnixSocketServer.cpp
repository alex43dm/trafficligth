#include <unistd.h>

#include "Log.h"
#include "UnixSocketServer.h"

UnixSocketServer::UnixSocketServer(const std::string &_path):
    path(_path)
{
    socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
    if(socket_fd < 0)
    {
        printf("socket() failed\n");
        exit(-1);
    }

    /* start with a clean address structure */
    memset(&address, 0, sizeof(struct sockaddr_un));

    address.sun_family = AF_UNIX;
    snprintf(address.sun_path, path.size(), "%s", path.c_str());
    unlink(address.sun_path);

    if(bind(socket_fd,
            (struct sockaddr *) &address,
            sizeof(struct sockaddr_un)) != 0)
    {
        printf("bind() failed\n");
    }

    if(listen(socket_fd, 5) != 0)
    {
        printf("listen() failed\n");
    }
}

UnixSocketServer::~UnixSocketServer()
{
    close(socket_fd);
}

void UnixSocketServer::run(Serial *com)
{
    int connection_fd, ret;
    socklen_t address_length;
    unsigned char buf[255], *p;

    bzero(buf,sizeof(buf));
    p = buf;

    while((connection_fd = accept(socket_fd,
                                  (struct sockaddr *) &address,
                                  &address_length)) > -1)
    {
        ret = read(connection_fd,p,5);
        if(ret != 5)
        {
            close(connection_fd);
            continue;
        }
        ret = read(connection_fd, p+5, buf[4]+1);
        if(ret != buf[4]+1)
        {
            close(connection_fd);
            continue;
        }
        if( com->SendCmd(buf, sizeof(buf), 5+buf[4]+1) )
        {
            ret = write(connection_fd,buf, 5+buf[4]+1);
        }

        close(connection_fd);
    }
}
