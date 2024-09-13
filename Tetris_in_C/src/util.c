#define _POSIX_C_SOURCE 199309L

#include <time.h>

void sleep_milli(int miliseconds){
    struct timespec ts;
    ts.tv_sec =0;
    ts.tv_nsec = miliseconds * 1000 * 1000;
    nanosleep(&ts,NULL);
}