#include "ShMem.h"
#include "Log.h"

ShMem::ShMem(bool RW):
    isInited(false),
    key(3232)
{
    if(RW)
    {
        shmid = shmget(key, sizeof(unsigned short), IPC_CREAT | 0666);
    }
    else
    {
        shmid = shmget(key,  sizeof(unsigned short), 0666);
    }

    if(shmid < 0)
    {
        Log::err("cann't create shared memory");
    }
    else
    {
        if((shm = (short unsigned *)shmat(shmid, NULL, 0)) == (unsigned short *) -1)
        {
            Log::err("shared memory attach error");
        }
        else
        {
            isInited = true;
        }
    }
}

ShMem::~ShMem()
{
    //dtor
}

bool ShMem::write(unsigned short val)
{
    if(!isInited)
        return false;

    *shm = val;
    return true;
}

unsigned short ShMem::read()
{
    if(!isInited)
        return -1;

    return *shm;
}

