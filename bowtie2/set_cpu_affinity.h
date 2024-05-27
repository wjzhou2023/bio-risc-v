#ifndef SET_AFFINITY_H
#define SET_AFFINITY_H

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <pthread.h>

static inline void set_thread_affinity(int core) {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(core,&set);
    if (pthread_setaffinity_np(pthread_self(),sizeof(set),&set) == -1) {
        printf("set thread%d to core:%d failed\n",(int)pthread_self(),core);
        exit(-1);
    };
}

#endif