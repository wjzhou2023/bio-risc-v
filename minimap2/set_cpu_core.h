#define _GNU_SOURCE

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
//#include <stdio.h>  
//#include <string.h>  
//#include <stdlib.h>  
//#include <stdint.h>  
//#include <sched.h> 
//#include <pthread.h>
typedef unsigned long UINT;

static inline void set_cpu_core (int core)
{
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(core, &mask);
  if (pthread_setaffinity_np(pthread_self(), sizeof (mask),  &mask ) == -1 )
  {
    printf ("set thread %lu to core %lu failed\n", (UINT)pthread_self(), (UINT)core );
    exit (-1);
  }
}
