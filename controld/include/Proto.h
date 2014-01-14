#ifndef PROTO_H
#define PROTO_H

#include <string>

#define BUFSIZE 255
#define STARTCHAR 0x3A

class Proto
{
    public:
        Proto();
        Proto(const std::string &, const std::string &);
        virtual ~Proto();
        inline void SetRequest(){ *pCmd |= 0x80; }
        inline void SetAddrTo(unsigned char a){ Addr1 = a; }
        inline void SetAddrFrom(unsigned char a){ Addr2 = a; }
        inline size_t len(){ return 5 + *pLen + 1; }
        inline unsigned char *cmd(){ return buf; }

        unsigned char *cmd_1B(unsigned char cmd);
        unsigned char *setPhase(int phase);
        bool signCheck();
        int getPhase();
        unsigned char *setTime();
        unsigned char *cmd_Tab(int *val, int len);

    protected:
    private:
        unsigned char *buf,
                        *pStart,
                        *pAddr1,
                        *pAddr2,
                        *pCmd,
                        *pLen,
                        Addr1,
                        Addr2;
        void sign();
        void head();
};

#endif // PROTO_H
