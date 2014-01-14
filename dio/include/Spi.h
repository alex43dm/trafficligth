#ifndef MCP3008SPI_H
#define MCP3008SPI_H

#include <string>

#include <linux/spi/spidev.h>

#define IOCON 0x0A
#define IODIRA 0x00
#define IODIRB 0x01
#define GPIOA 0x12
#define GPIOB 0x13
#define GPPUA 0x0C
#define GPPUB 0x0D
#define IPOLA 0x02
#define IPOLB 0x03
#define DEFVALA 0x06
#define DEFVALB 0x07
#define GPINTENA 0x04
#define GPINTENB 0x05

class Spi
{
    public:
        typedef union
        {
            unsigned char x[2];
            unsigned short y;
        } rx_c;

        Spi();
        Spi(const std::string &devspi, const std::string &spiMode, const std::string &spiSpeed, const std::string &spibitsPerWord);
        virtual ~Spi();
        int SetRegister( unsigned char reg, unsigned char data);
        unsigned short GetRegister( unsigned char reg, unsigned short *data);
        unsigned short GetGPIO();
        static const char *ByteToBinary(unsigned char x, char *);
        static const char *ShortToBinary(unsigned short x, char *b);

    protected:
    private:
        unsigned char mode;
        unsigned char bitsPerWord;
        unsigned int speed;
        int spifd;

        int spiOpen(const std::string &devspi);
        int spiClose();
        void microchipInit();
};

#endif // MCP3008SPI_H
