#ifndef DIO_H
#define DIO_H

#include <string>

#include "Spi.h"

class Dio
{
    public:
        std::string spiDev;
        std::string spiMode;
        std::string spiSpeed;
        std::string spiBitsPerWord;
        std::string period;

        Dio();
        virtual ~Dio();
        int loop();
    protected:
    private:
        Spi *spi;
};

#endif // DIO_H
