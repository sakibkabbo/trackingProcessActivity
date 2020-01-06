#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "tsc.h"

/*
takes two arguments:
   - first is the number of inactive periods
   - second is the threshold (in cycles)
*/
int main(int argc, char **argv) {
  int num = atoi(argv[1]);
  u_int64_t threshold = atoi(argv[2]);
  u_int64_t *samples = malloc(sizeof(u_int64_t) * (2 * num + 1));

  struct timespec start_experiment, end_experiment;
  
  clock_gettime(CLOCK_MONOTONIC, &start_experiment);
  inactive_periods(num, threshold, samples);
  clock_gettime(CLOCK_MONOTONIC, &end_experiment);
  
  long diff_nsec = end_experiment.tv_nsec - start_experiment.tv_nsec;

  double cycles_per_msec = (double) samples[num] / ((double)diff_nsec / 1000000);

  FILE *f1 = fopen("inactive_periods.dat", "w");
  FILE *f2 = fopen("inactive_periods_detailed.dat", "w");

  int isActive = 1;
  u_int64_t cycle_diff;
  double msec_duration;
  for (int i = 0; i < (num * 2); i++){
    if (isActive) {
      cycle_diff = samples[i + 1] - samples[i];
      msec_duration = (double) cycle_diff / cycles_per_msec;
      printf("Active %d: start at %lu, duration %lu cycles (%lf ms)\n", i / 2, samples[i], cycle_diff, msec_duration);
      isActive = !isActive;
    } else {
      cycle_diff = samples[i + 1] - samples[i];
      msec_duration = (double) cycle_diff / cycles_per_msec;
      printf("Inactive %d: start at %lu, duration %lu cycles (%lf ms)\n", i / 2, samples[i], cycle_diff, msec_duration);
      isActive = !isActive;
    }
  }

/*  for (int i = 0; i < (num * 2); i += 2){
    if (i > 1) {
      fprintf(f1, "set object %d rect from %lf, 1 to %lf, 2 fc rgb \"blue\" fs solid\n", i + 1, (double) (samples[i] - samples[1]) / cycles_per_msec, (double) (samples[i+1] - samples[1]) / cycles_per_msec);
    }
    if (i < 10) {
      fprintf(f2, "set object %d rect from %lf, 1 to %lf, 2 fc rgb \"blue\" fs solid\n", i + 1, (double) samples[i] / cycles_per_msec, (double) samples[i+1] / cycles_per_msec);
    }
  }
  for (int i = 1; i < (num * 2); i += 2){
    if (i > 1) {
      fprintf(f1, "set object %d rect from %lf, 1 to %lf, 2 fc rgb \"red\" fs solid\n", i + 1, (double) (samples[i] - samples[1]) / cycles_per_msec, (double) (samples[i+1] - samples[1]) / cycles_per_msec);
    }
    if (i < 10) {
      fprintf(f2, "set object %d rect from %lf, 1 to %lf, 2 fc rgb \"red\" fs solid\n", i + 1, (double) samples[i] / cycles_per_msec, (double) samples[i+1] / cycles_per_msec);
    }
  }

  fprintf(f1, "plot [0:%d] [0:3] 0\n", (int) ((samples[num * 2] - samples[1]) / cycles_per_msec) + 1);
  fprintf(f2, "plot [0:%d] [0:3] 0\n", (int) (samples[10] / cycles_per_msec) + 1);

  fclose(f1);
  fclose(f2); */
  
}
