#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "Spi.h"
#include "Log.h"

Spi::Spi() :
    mode(SPI_MODE_1),
    bitsPerWord(8),
    speed(1000000),
    spifd(-1)
{
    spiOpen(std::string("/dev/spidev0.0"));
    //microchip init
    microchipInit();
}

Spi::Spi(const std::string &devspi, const std::string &spiMode, const std::string &spiSpeed, const std::string &spiBitsPerWord) :
    spifd(-1)
{
    Log::info("spi: dev: %s mode: %s speed: %s bitsPerWord: %s",
    devspi.c_str(), spiMode.c_str(), spiSpeed.c_str(), spiBitsPerWord.c_str());

    if(spiMode == "SPI_MODE_0")
    {
        mode = SPI_MODE_0;
    }
    else if(spiMode == "SPI_MODE_1")
    {
        mode = SPI_MODE_1;
    }
    else if(spiMode == "SPI_MODE_2")
    {
        mode = SPI_MODE_2;
    }
    else if(spiMode == "SPI_MODE_3")
    {
        mode = SPI_MODE_3;
    }

    speed =  atoi(spiSpeed.c_str());
    bitsPerWord = atoi(spiBitsPerWord.c_str());

    this->spiOpen(devspi);
    //microchip init
    microchipInit();
}

Spi::~Spi(){
    this->spiClose();
}

int Spi::spiOpen(const std::string &devspi){
    int statusVal = -1;
    this->spifd = open(devspi.c_str(), O_RDWR);

    Log::info("open %s",devspi.c_str());

    if(this->spifd < 0){
        fprintf(stderr,"could not open SPI device: %s\n", devspi.c_str());
        exit(1);
    }

    statusVal = ioctl (this->spifd, SPI_IOC_WR_MODE, &(this->mode));
    if(statusVal < 0){
        fprintf(stderr,"Could not set SPIMode (WR)...ioctl fail\n");
        exit(1);
    }

    statusVal = ioctl (this->spifd, SPI_IOC_WR_BITS_PER_WORD, &(this->bitsPerWord));
    if(statusVal < 0) {
      fprintf(stderr,"Could not set SPI bitsPerWord (WR)...ioctl fail\n");
      exit(1);
    }

    statusVal = ioctl (this->spifd, SPI_IOC_WR_MAX_SPEED_HZ, &(this->speed));
    if(statusVal < 0) {
      fprintf(stderr,"Could not set SPI speed (WR)...ioctl fail\n");
      exit(1);
    }

    return statusVal;
}

int Spi::spiClose(){
    int statusVal = -1;
    statusVal = close(this->spifd);
        if(statusVal < 0) {
      perror("Could not close SPI device");
      exit(1);
    }
    return statusVal;
}

void Spi::microchipInit()
{
    SetRegister(IOCON,0x28);    // IOCON: BANK=0, SEQOP=1(Sequential Operation), HAEN=1 (Enable Addressing)

    SetRegister(GPIOA,0x00);    // Reset Output on GPIOA
    SetRegister(GPIOB,0x00);    // Reset Output on GPIOB

    SetRegister(GPINTENA,0xFF);
    SetRegister(GPINTENB,0xFF);

    SetRegister(DEFVALA,0);   // GPIOA As Input
    SetRegister(DEFVALB,0);   // GPIOB As Input

    SetRegister(IODIRA,0xFF);   // GPIOA As Input
    SetRegister(IODIRB,0xFF);   // GPIOB As Input

    //SetRegister(IPOLA,0xFF);    // Polarity A
    //SetRegister(IPOLB,0xFF);    // Polarity B
}

int Spi::SetRegister( unsigned char reg, unsigned char val)
{

    struct spi_ioc_transfer spi;
    unsigned char tx[3] = {0x40, 0, 0};
    unsigned char rx[3] = {0, 0, 0};
    int retVal = -1;

    tx[1] = reg;
    tx[2] = val;

    memset((void *)&spi, 0, sizeof(struct spi_ioc_transfer));

    spi.tx_buf        = (unsigned long)tx; //transmit from tx
    spi.rx_buf        = (unsigned long)rx; //receive into rx
    spi.len           = sizeof(tx);
    spi.delay_usecs   = 0;
    spi.speed_hz      = 0;
    spi.bits_per_word = 0;
    spi.cs_change = 0;

    retVal = ioctl (this->spifd, SPI_IOC_MESSAGE(1), &spi) ;

    if(retVal != sizeof(tx))
    {
        Log::err("Problem transmitting spi data..ioctl");
    }
    //printf("ret: %.2X %.2X %.2X\n", rx[0], rx[1], rx[2]);
    return retVal;
}

unsigned short Spi::GetRegister( unsigned char reg, unsigned short *ret)
{

    struct spi_ioc_transfer spi;
    unsigned char tx[2] = {0x41, 0};
    rx_c rx;
    rx.y = 0;
    int retVal = -1;

    //set register address
    tx[1] = reg;

    memset((void *)&spi, 0, sizeof(struct spi_ioc_transfer));

    spi.tx_buf        = (unsigned long)tx; //transmit from tx
    spi.rx_buf        = (unsigned long)rx.x; //receive into rx
    spi.len           = sizeof(tx);
    spi.delay_usecs   = 0;
    spi.speed_hz      = 0;
    spi.bits_per_word = 0;
    spi.cs_change = 0;

    retVal = ioctl (this->spifd, SPI_IOC_MESSAGE(1), &spi) ;

    if(retVal != sizeof(tx))
    {
        Log::err("spi error");
    }

    *ret = rx.y;

    //printf("ret: %.2X %.2X\n", rx[0], rx[1]);

    return rx.y;
}

unsigned short Spi::GetGPIO()
{
    struct spi_ioc_transfer spi;
    unsigned char tx[4] = {0x41, GPIOA, GPIOB, 0};
    unsigned char rx[4] = {0, 0, 0, 0};
    rx_c r;
    int retVal = -1;

    memset((void *)&spi, 0, sizeof(struct spi_ioc_transfer));

    spi.tx_buf        = (unsigned long)tx; //transmit from tx
    spi.rx_buf        = (unsigned long)rx; //receive into rx
    spi.len           = sizeof(tx);
    spi.delay_usecs   = 0;
    spi.speed_hz      = 0;
    spi.bits_per_word = 0;
    spi.cs_change = 0;

    retVal = ioctl (this->spifd, SPI_IOC_MESSAGE(1), &spi) ;

    if(retVal != sizeof(tx))
    {
        Log::err("spi error");
    }

    r.x[0] = rx[2];
    r.x[1] = rx[3];

    return r.y;
}

const char *Spi::ByteToBinary(unsigned char x, char *b)
{
    b[0] = '\0';

    for (int z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }
    return b;
}

const char *Spi::ShortToBinary(unsigned short x, char *b)
{
    b[0] = '\0';
    while (x)
    {
        strcat(b, (x & 1) ? "1" : "0");
        x >>= 1;
    }
    return b;
}
