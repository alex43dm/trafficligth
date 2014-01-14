#ifndef SHMEM_H
#define SHMEM_H

#include <sys/shm.h>

class ShMem
{
    public:
        ShMem(bool);
        virtual ~ShMem();
        bool write(unsigned short val);
        unsigned short read();
    protected:
    private:
        bool isInited;
        int shmid;
        key_t key;
        unsigned short *shm;
};

#endif // SHMEM_H
