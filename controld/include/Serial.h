#ifndef SERIAL_H
#define SERIAL_H

#include <string>
#include <termios.h>

class Serial
{
    public:
        std::string port;
        std::string speed;

        Serial(const std::string &port, const std::string &speed);
        virtual ~Serial();
        bool Config();
        ssize_t SendCmd(unsigned char*ioBuff, int bufLength, int msgLength);
        ssize_t SendCmdWO(unsigned char *ioBuff, int msgLength);
    protected:
    private:
        int fd;
        struct termios options;
        struct termios oldOptions;
        char *ibuf;
//        char *obuf;
};

#endif // SERIAL_H
