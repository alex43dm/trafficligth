#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <unistd.h>

#include <stdio.h>

#include "Serial.h"
#include "Log.h"

Serial::Serial(const std::string &port, const std::string &speed):
    port(port),
    speed(speed)
{
    //Log::info("openning port: '%s' speed: '%s'", port.c_str(), speed.c_str());
    fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(!fd)
    {
        Log::err("error open port: '%s'", port.c_str());
        ::exit(1);
    }

    ibuf = (char*)malloc(1024);

    tcgetattr(fd, &oldOptions);
}

Serial::~Serial()
{
    Log::err("Serial::~Serial port: '%s' speed: '%s'", port.c_str(), speed.c_str());
    free((void*)ibuf);
    tcsetattr(fd, TCSANOW, &oldOptions);
    close(fd);
}

bool Serial::Config()
{
    memset((void*)&options, 0x00, sizeof(options));

    if( speed == "115200")
    {
        cfsetispeed(&options, B115200);
        cfsetospeed(&options, B115200);
    }
    else
    {
        cfsetispeed(&options, B1200);
        cfsetospeed(&options, B1200);
    }

    options.c_cflag |= (CLOCAL | CREAD); //Ignore modem status lines | Enable receiver
    options.c_cflag |= PARENB; //Parity enable
    options.c_cflag |= PARODD; //Odd parity else even
    options.c_cflag |= CSTOPB; //Send two stop bits else one
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8; //Number of bits per byte CS5 CS6 CS7 CS8

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    options.c_cc[VMIN]      =   1;// read doesn't block
    options.c_cc[VTIME]     =   0;// 0.5 seconds read timeout

    tcflush(fd , TCIFLUSH);

    if(tcsetattr(fd, TCSANOW, &options) == -1)
    {
        Log::err("tcsetattr error port: '%s'", port.c_str());
        ::exit(1);
        //return false;
    }

    return true;
}
//91.222.250.108
ssize_t Serial::SendCmd(unsigned char *ioBuff, int bufLength, int msgLength)
{
    printf("send  cmd: %X %d ", ioBuff[3], ioBuff[5] );
    int cbreak;
    ssize_t rlen = write(fd, (const void*)ioBuff, msgLength);
    if((size_t)rlen != (size_t)msgLength)
    {
        msgLength = 0;
        Log::err("write error: %ld port: '%s' speed: '%s'\n", rlen, port.c_str(), speed.c_str());
        return rlen;
    }
    return rlen;

    bzero(ibuf,sizeof(ibuf));
    for( cbreak = 0, rlen = 0; rlen <= msgLength && cbreak < 10; cbreak++ )
    {
        rlen = rlen + read(fd, ibuf+rlen, sizeof(ibuf)-rlen);
        usleep(1000);
    }


    return rlen;
}

ssize_t Serial::SendCmdWO(unsigned char *ioBuff, int msgLength)
{
    ssize_t rlen = write(fd, (const void*)ioBuff, msgLength);
    if((size_t)rlen != (size_t)msgLength)
    {
        msgLength = 0;
        Log::err("write error: ret: %ld  port: '%s' speed: '%s'",  rlen, port.c_str(), speed.c_str());
        return rlen;
    }

    return rlen;
}
