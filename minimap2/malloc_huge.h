#ifndef MALLOC_HUGE_H
#define MALLOC_HUGE_H

#include <sys/mman.h>
#include <stdio.h>
#include <signal.h>
typedef unsigned long UINT;

typedef enum
{
  PAGE_SIZE_4K = 0,
  PAGE_SIZE_2M = 1,
  PAGE_SIZE_1G = 2
}page_size_t;

static inline void * malloc_huge_page ( size_t size, page_size_t page_size )
{
  void * ptr = NULL;
  UINT ps = 0;
  if ( page_size == PAGE_SIZE_4K )
  {
    ps = 1UL << 12;
  }
  else if ( page_size == PAGE_SIZE_2M )
  {
    ps = 1UL << 21;
  }
  else if ( page_size == PAGE_SIZE_1G )
  {
    ps = 1UL << 30;
  }
  else
  {
    printf ( "invalid page size enumerator %d\n", page_size );
    exit ( -1 );
  }
  if ( posix_memalign ( &ptr, ps, size ) != 0 )
  {
    printf ( "posix memalign failed ps = %ld, size = %ld\n", ps, size );
    raise(SIGTRAP);
    exit ( -1 );
  }
  madvise ( ptr, size, MADV_HUGEPAGE );
  return ptr;
}
#endif
