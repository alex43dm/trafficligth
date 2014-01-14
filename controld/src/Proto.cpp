#include <stdlib.h>
#include <strings.h>

#include "Proto.h"

Proto::Proto()
{
    buf = (unsigned char*)malloc(BUFSIZE);
    //init pointers
    pStart  = buf;
    pAddr1  = buf + 1;
    pAddr2  = buf + 2;
    pCmd    = buf + 3;
    pLen    = buf + 4;
}

Proto::Proto(const std::string &aFrom, const std::string &aTo)
{
    buf = (unsigned char*)malloc(BUFSIZE);
    //init pointers
    pStart  = buf;
    pAddr1  = buf + 1;
    pAddr2  = buf + 2;
    pCmd    = buf + 3;
    pLen    = buf + 4;
    Addr1 = (unsigned char)atoi(aTo.c_str());
    Addr2 = (unsigned char)atoi(aFrom.c_str());
}

Proto::~Proto()
{
    delete buf;
}

void Proto::sign()
{
    buf[len()-1] = buf[1];
    for(unsigned int i = 2; i < len()-1; i++)
        buf[len()-1] ^= buf[i];
}

bool Proto::signCheck()
{
    unsigned char crc;

    crc = buf[1];
    for(unsigned int i = 2; i < len()-1; i++)
        crc ^= buf[i];

    if(crc != buf[len()-1])
        return false;
    return true;
}

void Proto::head()
{
    bzero(buf, BUFSIZE);
    *pStart = 0x3A;
    *pAddr1 = Addr1;
    *pAddr2 = Addr2;
}


unsigned char *Proto::cmd_1B(unsigned char cmd)
{
    head();
    *pCmd = cmd;
    *pLen = 0;
    sign();
    return buf;
}

unsigned char *Proto::setPhase(int phase)
{
    head();
    *pCmd = 0x4D;
    *pLen = 3;
    buf[5] = phase > 0 && phase < 255 ? (char)phase : 0;
    sign();
    return buf;
}

int Proto::getPhase()//after 0x60
{
    unsigned char ret;
    if( *pCmd == 0xE0 && *pLen == 5 && signCheck())
    {
        ret = buf[6] & 0x0F;
        return ret += 1;
    }
    return -1;
}

unsigned char *Proto::setTime()
{
    time_t now;
    struct tm tmnow;

    time(&now);
    tmnow = *localtime(&now);

    head();
    *pCmd = 0x52;
    *pLen = 7;


    buf[5] = tmnow.tm_sec;
    buf[6] = tmnow.tm_min;
    buf[7] = tmnow.tm_hour;
    buf[8] = tmnow.tm_wday;
    buf[9] = tmnow.tm_mday;
    buf[10] = tmnow.tm_mon;
    buf[11] = tmnow.tm_year & 0x00FF;

    sign();
    return buf;
}

unsigned char *Proto::cmd_Tab(int *val, int len)
{
    bzero(buf, BUFSIZE);

    buf[0] = 0xFF;
    buf[1] = 0x3A;
    buf[2] = 0;
    buf[3] = 0x3E;
    buf[4] = 0x83;
    buf[5] = 0x22;
    buf[6] = 0x34;

    for(int i = 0; i < len; i++)
        buf[7+i] = val[i];

    buf[40] = buf[2];
    for(unsigned int i = 3; i < 40; i++)
        buf[40] ^= buf[i];

    buf[41] = 0xFF;
    return buf;
}

/*
0x52 - УСТАНОВИТЬ ВРЕМЯ
      длина - 7 байт

данные - секунды, минуты, часы, день недели, число, месяц, год.
(в двоично-десятичном виде)

	ОТВЕТ
длина – 0 байт

------------------------------------------------------------------------------------

	0x53 - ПРОЧИТАТЬ ВРЕМЯ
      длина – 0 байт

      ОТВЕТ
длина 7 байт

данные - секунды, минуты, часы, день недели, число, месяц, год.
(в двоично-десятичном виде)

*/

/*
0x60 - ОПРОС ОБЩЕГО СОСТОЯНИЯ
	длина - 0 байт

	ОТВЕТ
	длина - 5 байт

1 байт:
		D7..D6 – тип управления
			0b00 - КУ
0b01 – ДУ
0b10 - ЛУ
0b11 – РУ

		D5..D3 – режим работы
         		0b000 – ОС
0b001 – ЖМ
0b010 – КК
0b011 – СП
0b100 – РСП
0b101 – СФ
0b110, 0b111 – резерв

		D2 - корректировка цикла
1 - начало цикла контроллера не совпадает с заданной временной меткой. Контроллер будет работать по минимальным временам фаз до устранения различия.
0 – коррекция не производится.

		D1 – такт фазы
0 - основной такт
1 – промежуточный такт

		D0 - старший бит номера СП

2 байт:
D7..D4 - номер текущей СП
	0x00..0x1F – номер СП 1..32
D3..D0 - номер текущей фазы
0x0..0xF – номер фазы 1..16

	3 байт:
длительность текущего основного/промежуточного такта в сек

	4 байт:
		D7 - открыта дверь
D6 - конфликт
		D5 - обрыв красного
		D4 - наличие перегоревших ламп
		D3 - нет сети ~220В
		D2 - внутренняя неисправность контроллера
		D1 - состояние ТВП1 (1-кнопка нажата, 0-отжата)
		D0 - состояние ТВП2 (1-кнопка нажата, 0-отжата)

5 байт:
	D0 – признак стартового промтакта (переход из КК в фазу).
Если 1, то идёт промтакт, при чём ранее не было фазы. Такое состояние может быть
при переходе в циклическую программу после ОС или ЖМ.
Бит был введён, при разработке коннектора для чехов.
*/
