#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

// Timer
// call update to get new time, and get difference from previous time

class Timer {
  struct timeval tv, tv_prev;
  unsigned n_dif;

 public:

  inline Timer() {
    update();
    tv_prev = tv;
    n_dif = 0;
  }


  inline void wait_until(unsigned n_usec) {
    if (update() < n_usec)
      usleep(n_usec - n_dif);
  }

  inline void reset(void) {
    gettimeofday(&tv, NULL);
    tv_prev = tv;
    n_dif = 0;
  }

  inline unsigned update(void) {

    gettimeofday(&tv, NULL);

    n_dif = (tv.tv_sec - tv_prev.tv_sec) * 1000000 + 
      tv.tv_usec - tv_prev.tv_usec;

    return n_dif;
  }

  inline unsigned get_dif(void) {
    return n_dif;
  }

  inline void set_dif(unsigned dif) {
    n_dif = dif;
  }

  inline float get_dif_f(void) {
    return (float)n_dif / 1000000.0f;
  }
};

#endif
