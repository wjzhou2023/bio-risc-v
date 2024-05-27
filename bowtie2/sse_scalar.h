#ifndef SCALAR_H
#define SCALAR_H

#include<assert.h>
#include<stdint.h>
#include<string.h>
#include<riscv_vector.h>

typedef union m128i {
    uint8_t     u8[16];
    uint16_t    u16[8];
    int8_t      i8[16];
    int16_t     i16[8];
    int32_t     i32[4];
    uint64_t    u64[2];
} __m128i;

static inline __m128i _mm_adds_epi16(__m128i a,__m128i b) {
    for (int i = 0 ;i < 8 ;++i) {
        int32_t aa = a.i16[i];
        aa += b.i16[i];

        a.i16[i] = aa > INT16_MAX ? INT16_MAX : (aa < INT16_MIN ? INT16_MIN : aa);
    }
    return a;
}

static inline __m128i _mm_adds_epu8(__m128i a,__m128i b) {
    for (int i = 0;i < 16 ;++i) {
        uint8_t aa = a.u8[i] + b.u8[i];
        a.u8[i] = aa | -(aa < a.u8[i]);
    }
    return a;
}

static inline __m128i _mm_cmpeq_epi16(__m128i a,__m128i b) {
    for (int i = 0;i < 8 ;++i) {
        a.i16[i] = (a.i16[i] == b.i16[i]) ? 0xffffffff : 0; 
    }
    return a;
}


static inline __m128i _mm_cmpeq_epi8(__m128i a,__m128i b) {
    for (int i = 0;i < 16 ;++i) {
        a.i8[i] = (a.i8[i] == b.i8[i]) ? 0xffff : 0; 
    }
    return a;
}

static inline __m128i _mm_cmpgt_epi16(__m128i a,__m128i b) {
    for (int i = 0;i < 8 ;++i) {
        a.i16[i] = (a.i16[i] > b.i16[i]) ? 0xffffffff : 0; 
    }
    return a;
}

static inline __m128i _mm_cmpgt_epi8(__m128i a,__m128i b) {
    for (int i = 0;i < 16 ;++i) {
        a.i8[i] = (a.i8[i] > b.i8[i]) ? 0xffff : 0; 
    }
    return a;
}

static inline __m128i _mm_cmplt_epi16(__m128i a,__m128i b) {
    for (int i = 0;i < 8 ;++i) {
        a.i16[i] = (a.i16[i] < b.i16[i]) ? 0xffffffff : 0; 
    }
    return a;
}

static inline __m128i _mm_cmplt_epu8(__m128i a,__m128i b) {
    for (int i = 0;i < 16 ;++i) {
        a.u8[i] = (a.u8[i] < b.u8[i]) ? 0xffff : 0; 
    }
    return a;
}

static inline int _mm_extract_epi16(__m128i a,const int imm8) {
    return a.u16[imm8 & 0x7];
}

static inline __m128i _mm_insert_epi16(__m128i a,int i,int imm8) {
    a.i16[imm8 & 0x7] = i & 0xffffffff;
    return a;
}

static inline __m128i _mm_load_si128(__m128i const *mem_addr) {
    __m128i r;
    memcpy(&r,mem_addr,sizeof(r));
    return r;
}

static inline __m128i _mm_max_epi16(__m128i a,__m128i b) {
    for(int i = 0;i < 8 ;++i) {
        if(b.i16[i] > a.i16[i]) {
            a.i16[i] = b.i16[i];
        }
    }
    return a;
}

static inline __m128i _mm_max_epu8(__m128i a,__m128i b) {
    for(int i = 0;i < 16 ;++i) {
        if(b.u8[i] > a.u8[i]) {
            a.u8[i] = b.u8[i];
        }
    }
    return a;
}

static inline int _mm_movemask_epi8(__m128i a) {
    int ret = 0;
    for(int i = 15 ;i >= 0;--i) {
        ret |= ((a.u8[15 - i] >> 7) << (15 - i));
    }
    return ret;
}

static inline __m128i _mm_or_si128(__m128i a,__m128i b) {
    a.u64[0] |= b.u64[0];
    a.u64[1] |= b.u64[1];
    return a;
}

static inline __m128i _mm_setzero_si128() {
    __m128i r;
    r.u64[0] = 0;
    r.u64[1] = 0;
    return r;
}

static inline __m128i _mm_slli_epi16(__m128i a,int imm8) {
    if (imm8 < 16) {
        for (int i = 0 ;i < 8;++i) {
            a.i16[i] <<= imm8;
        }
    } else {
        memset(&a,0,sizeof(a));
    }
    return a;
}

static inline __m128i _mm_slli_si128(__m128i a, int n) {
	int i;
	memmove(&a.u8[n], &a.u8[0], 16 - n);
	for (i = 0; i < n; i++) a.u8[i] = 0;
	return a;
}

static inline __m128i _mm_srli_epi16(__m128i a,int imm8) {
    if (imm8 < 16) {
        for (int i = 0;i < 8;++i) {
            a.i16[i] >>= imm8;
        }
    } else {
        memset(&a,0,sizeof(a));
    }
    return a;
}

static inline __m128i _mm_srli_epu8(__m128i a ,int imm8) {
        if (imm8 < 8) {
        for (int i = 0;i < 16;++i) {
            a.u8[i] >>= imm8;
        }
    } else {
        memset(&a,0,sizeof(a));
    }
    return a;
}

static inline __m128i _mm_srli_si128(__m128i a, int n) {
	int i;
	memmove(&a.u8[0], &a.u8[n], 16 - n);
	for (i = 16 - n; i < n; i++) a.u8[i] = 0;
	return a;
}

static inline void _mm_store_si128(__m128i *ptr, __m128i a) { memcpy(ptr, &a, sizeof a); }

static inline __m128i _mm_subs_epu8(__m128i a, __m128i b) {
 	int i;
 	for (i = 0; i < 16; i++) {
         uint8_t aa = a.u8[i] - b.u8[i];
         aa &= -(aa <= a.u8[i]);
          		a.u8[i] = aa;
 	}
 	return a;
 }


static inline __m128i _mm_subs_epi16(__m128i a, __m128i b) {
	for (int i = 0; i < 8; i++) {
		int32_t aa = a.i16[i];
		aa -= b.i16[i];
		a.i16[i] = aa > INT16_MAX ? INT16_MAX : (aa < INT16_MIN ? INT16_MIN : aa);
	}
	return a;
}

static inline __m128i _mm_xor_si128(__m128i a,__m128i b) {
    a.u64[0] ^= b.u64[0];
    a.u64[1] ^= b.u64[1];
    return a;
}

static inline __m128i _mm_set1_epi16(int imm16) {
    __m128i r;
    for (int i = 0;i < 8 ;++i) {
        r.i16[i] = imm16;
    }
    return r;
}

#endif