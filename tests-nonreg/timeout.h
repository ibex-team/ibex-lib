#ifndef __TIMEOUT__H
#define __TIMEOUT__H

/* returns 0 if f is executed in less than nsecs,
 * 1 otherwise. */
int timeout(void (*f)(void), int nsecs);

#endif
