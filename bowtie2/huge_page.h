#ifndef HUGE_PAGE_MALLOC
#define HUGE_PAGE_MALLOC

#include <sys/mman.h>
#include <stdio.h>

typedef enum
{
  PAGE_SIZE_4K = 0,
  PAGE_SIZE_2M = 1,
  PAGE_SIZE_1G = 2
}page_size_t;

static inline void *malloc_huge_page(size_t size,page_size_t page_size){
    void *ptr = NULL;
    int ps = 0;
    switch (page_size) {
      case PAGE_SIZE_4K:
        ps = 1 << 12;
        break;
      case PAGE_SIZE_2M:
        ps = 1 << 21;
        break;
      case PAGE_SIZE_1G:
        ps = 1 << 30;
        break;
      default:
        printf("invalid page size enumerator:%d\n",page_size);
        exit(-1);
    }
    
    if(posix_memalign(&ptr,ps,size)!=0){
      printf("posix memalign failed\n");
      exit(-1);
    }
    
    madvise(ptr,size,MADV_HUGEPAGE);
    return ptr;
}
#endif