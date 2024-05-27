#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ksw2.h"

#ifdef USE_SIMDE
#include <simde/x86/sse2.h>
#else
#include <emmintrin.h>
#endif

#ifdef __GNUC__
#define LIKELY(x) __builtin_expect((x),1)
#define UNLIKELY(x) __builtin_expect((x),0)
#else
#define LIKELY(x) (x)
#define UNLIKELY(x) (x)
#endif

typedef struct {
	int qlen, slen;
	uint8_t shift, mdiff, max, size;
	__m128i *qp, *H0, *H1, *E, *Hmax;
} kswq_t;

/**
 * Initialize the query data structure
 *
 * @param size   Number of bytes used to store a score; valid valures are 1 or 2
 * @param qlen   Length of the query sequence
 * @param query  Query sequence
 * @param m      Size of the alphabet
 * @param mat    Scoring matrix in a one-dimension array
 *
 * @return       Query data structure
 */
void *ksw_ll_qinit(void *km, int size, int qlen, const uint8_t *query, int m, const int8_t *mat)
{
	kswq_t *q;
	int slen, a, tmp, p;

	size = size > 1? 2 : 1;
	p = 8 * (3 - size); // # values per __m128i
	slen = (qlen + p - 1) / p; // segmented length
	q = (kswq_t*)kmalloc(km, sizeof(kswq_t) + 256 + 16 * slen * (m + 4)); // a single block of memory
	q->qp = (__m128i*)(((size_t)q + sizeof(kswq_t) + 15) >> 4 << 4); // align memory
	q->H0 = q->qp + slen * m;
	q->H1 = q->H0 + slen;
	q->E  = q->H1 + slen;
	q->Hmax = q->E + slen;
	q->slen = slen; q->qlen = qlen; q->size = size;
	// compute shift
	tmp = m * m;
	for (a = 0, q->shift = 127, q->mdiff = 0; a < tmp; ++a) { // find the minimum and maximum score
		if (mat[a] < (int8_t)q->shift) q->shift = mat[a];
		if (mat[a] > (int8_t)q->mdiff) q->mdiff = mat[a];
	}
	q->max = q->mdiff;
	q->shift = 256 - q->shift; // NB: q->shift is uint8_t
	q->mdiff += q->shift; // this is the difference between the min and max scores
	// An example: p=8, qlen=19, slen=3 and segmentation:
	//  {{0,3,6,9,12,15,18,-1},{1,4,7,10,13,16,-1,-1},{2,5,8,11,14,17,-1,-1}}
	if (size == 1) {
		int8_t *t = (int8_t*)q->qp;
		for (a = 0; a < m; ++a) {
			int i, k, nlen = slen * p;
			const int8_t *ma = mat + a * m;
			for (i = 0; i < slen; ++i)
				for (k = i; k < nlen; k += slen) // p iterations
					*t++ = (k >= qlen? 0 : ma[query[k]]) + q->shift;
		}
	} else {
		int16_t *t = (int16_t*)q->qp;
		for (a = 0; a < m; ++a) {
			int i, k, nlen = slen * p;
			const int8_t *ma = mat + a * m;
			for (i = 0; i < slen; ++i)
				for (k = i; k < nlen; k += slen) // p iterations
					*t++ = (k >= qlen? 0 : ma[query[k]]);
		}
	}
	return q;
}

int ksw_ll_i16(void *q_, int tlen, const uint8_t *target, int _gapo, int _gape, int *qe, int *te)
{
	kswq_t *q = (kswq_t*)q_;
	int slen, i, gmax = 0, qlen8;
	__m128i zero, gapoe, gape, *H0, *H1, *E, *Hmax;
	uint16_t *H8;

#define __max_8(ret, xx) do { \
		(xx) = simde_mm_max_epi16((xx), simde_mm_srli_si128((xx), 8)); \
		(xx) = simde_mm_max_epi16((xx), simde_mm_srli_si128((xx), 4)); \
		(xx) = simde_mm_max_epi16((xx), simde_mm_srli_si128((xx), 2)); \
		(ret) = simde_mm_extract_epi16((xx), 0); \
	} while (0)

	// initialization
	*qe = *te = -1;
	zero = simde_mm_set1_epi32(0);
	gapoe = simde_mm_set1_epi16(_gapo + _gape);
	gape = simde_mm_set1_epi16(_gape);
	H0 = q->H0; H1 = q->H1; E = q->E; Hmax = q->Hmax;
	slen = q->slen, qlen8 = slen * 8;
	memset(E,    0, slen * sizeof(__m128i));
	memset(H0,   0, slen * sizeof(__m128i));
	memset(Hmax, 0, slen * sizeof(__m128i));
	// the core loop
	for (i = 0; i < tlen; ++i) {
		int j, k, imax;
		__m128i e, h, f = zero, max = zero, *S = q->qp + target[i] * slen; // s is the 1st score vector
		h = simde_mm_load_si128(H0 + slen - 1); // h={2,5,8,11,14,17,-1,-1} in the above example
		h = simde_mm_slli_si128(h, 2);
		for (j = 0; LIKELY(j < slen); ++j) {
			h = simde_mm_adds_epi16(h, *S++);
			e = simde_mm_load_si128(E + j);
			h = simde_mm_max_epi16(h, e);
			h = simde_mm_max_epi16(h, f);
			max = simde_mm_max_epi16(max, h);
			simde_mm_store_si128(H1 + j, h);
			h = simde_mm_subs_epu16(h, gapoe);
			e = simde_mm_subs_epu16(e, gape);
			e = simde_mm_max_epi16(e, h);
			simde_mm_store_si128(E + j, e);
			f = simde_mm_subs_epu16(f, gape);
			f = simde_mm_max_epi16(f, h);
			h = simde_mm_load_si128(H0 + j);
		}
		for (k = 0; LIKELY(k < 8); ++k) {
			f = simde_mm_slli_si128(f, 2);
			for (j = 0; LIKELY(j < slen); ++j) {
				h = simde_mm_load_si128(H1 + j);
				h = simde_mm_max_epi16(h, f);
				simde_mm_store_si128(H1 + j, h);
				h = simde_mm_subs_epu16(h, gapoe);
				f = simde_mm_subs_epu16(f, gape);
				if(UNLIKELY(!_mm_movemask_epi8(simde_mm_cmpgt_epi16(f, h)))) goto end_loop_i16;
			}
		}
end_loop_i16:
		__max_8(imax, max);
		if (imax >= gmax) {
			gmax = imax; *te = i;
			memcpy(Hmax, H1, slen * sizeof(__m128i));
		}
		S = H1; H1 = H0; H0 = S;
	}
	for (i = 0, H8 = (uint16_t*)Hmax; i < qlen8; ++i)
		if ((int)H8[i] == gmax) *qe = i / 8 + i % 8 * slen;
	return gmax;
}
