#include <unistd.h>
#include <stdlib.h>

#include "Thread.h"

Thread::Thread() :
    fExit(false)
{
    m_mtx = new pthread_mutex_t;
	pthread_mutex_init(m_mtx, NULL);
    m_cond = new pthread_cond_t;
	pthread_cond_init(m_cond, NULL);
}
//###########################################################################
Thread::~Thread()
{
    pthread_mutex_destroy(m_mtx);
    pthread_cond_destroy(m_cond);
    delete m_cond;
    delete m_mtx;
    pthread_exit(NULL);
}
//###########################################################################
bool Thread::start()
{
    pthread_attr_t attr, *pattr = &attr;
//    int stack_size;
//    sched_param schedparam;
//    schedparam.sched_priority = 3;

    pthread_attr_init(pattr);

    //pthread_attr_setdetachstate(pattr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setdetachstate(pattr, PTHREAD_CREATE_DETACHED);

    //pthread_attr_setinheritsched(pattr, PTHREAD_EXPLICIT_SCHED);
    //pthread_attr_setschedpolicy(pattr, SCHED_RR);
    //pthread_attr_setschedparam(pattr, &schedparam);

//    posix_memalign(&sp, sysconf(_SC_PAGESIZE), stack_size);
 //   pthread_attr_setstack(pattr, sp, stack_size);

    pthread_create(&m_threadId, pattr, internalThreadFunc, this);

    pthread_attr_destroy(pattr);
    return true;
}
//###########################################################################
void Thread::waitExit()
{
      (void) pthread_join(m_threadId, NULL);
}
//###########################################################################
void Thread::lock(bool getSignal)
{
    pthread_mutex_lock(m_mtx);
    if(getSignal)
        pthread_cond_wait(m_cond, m_mtx);
}
//###########################################################################
void Thread::unlock(bool sendSignal)
{
    if(sendSignal)
        pthread_cond_signal(m_cond);
    pthread_mutex_unlock(m_mtx);
}
