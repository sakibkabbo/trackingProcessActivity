#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

/* initialize the cycle counter */
u_int64_t start = 0;


/* Set *hi and *lo to the high and low order bits of the cycle counter.
 * Implementation requires assembly code to use the rdtsc instruction.
 * NOTE: static keyword is required on inline function declaration to 
 * tell compiler/linker that the function won't be used from any other 
 * compilation unit.
 */
static inline void access_counter(unsigned *hi, unsigned *lo)
{
  asm volatile("rdtsc; movl %%edx, %0; movl %%eax, %1" /* Read cycle counter */
      : "=r" (*hi), "=r" (*lo)                /* and move results to */
      : /* No input */                        /* the two outputs */
      : "%rdx", "%rax"); /* clobbered registers */
}

void start_counter()
{
  unsigned hi, lo;
  access_counter(&hi, &lo);
  start = ((u_int64_t)hi << 32) | lo;
}

u_int64_t get_counter()
{
  unsigned ncyc_hi, ncyc_lo;

  /* Get the cycle counter */
  access_counter(&ncyc_hi, &ncyc_lo);

  return (((u_int64_t)ncyc_hi << 32) | ncyc_lo) - start;
}

u_int64_t inactive_periods(int num, u_int64_t threshold, u_int64_t *samples)
{
  int c = 0;
  int currentlyInactive = 0;
  start_counter();
  u_int64_t currCycles = 0;
  u_int64_t prevCycles;
  while (c != (num * 2) + 1){
    prevCycles = currCycles;
    currCycles = get_counter();
    if (c == 0) {
      samples[c] = currCycles;
      c += 1;
    }
    else if (currCycles - prevCycles > threshold){
      samples[c] = prevCycles;
      samples[c + 1] = currCycles;
      c += 2;
    }
  }
  return samples[0];
}
