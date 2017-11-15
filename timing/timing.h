/* Smalltime support for timing programs. Use as follows:

   timing_start();
   do_loads_of_stuff();
   timing_end();
   printf("Elapsed microsec: %g\n", timing_diff());
*/


static struct timeval tstart, tend;

void timing_start() {
  gettimeofday(&tstart, 0);
}

void timing_end() {
  gettimeofday(&tend, 0);
}

/* From: https://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html */
int
timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

double timing_diff() {
  struct timeval tdiff;

  timeval_subtract(&tdiff, &tend, &tstart);

# ifdef DEBUG
  printf("timing_diff: %d.%6.6d - %d.%6.6d = %d.%6.6d\n",
         (int)tend.tv_sec, (int)tend.tv_usec,
         (int)tstart.tv_sec, (int)tstart.tv_usec,
         (int)tdiff.tv_sec, (int)tdiff.tv_usec);
# endif
  return (double)tdiff.tv_sec * 1000000 + tdiff.tv_usec;
}
