#include "timeout.h"
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>
#include <errno.h>

static pthread_t thread;
sem_t my_sem;
static void sig_alarm(int signo)
{
  pthread_cancel(thread);
}


static void (*func)(void);

static void* process(void*) {
  func();
  sem_post(&my_sem);
  return NULL;
}

int timeout(void (*f)(void), int nsecs) {
  
  func = f;

  struct timespec my_timeout;
  struct timeval now;

  gettimeofday(&now,NULL);

  my_timeout.tv_sec = now.tv_sec + nsecs;
  my_timeout.tv_nsec = now.tv_usec * 1000;

  pthread_create(&thread, NULL, process, NULL );
 
  int ret = sem_timedwait(&my_sem,&my_timeout);

  if ((ret==-1)&&(errno==ETIMEDOUT)) {
    pthread_cancel(thread);
    return 1;
  }
  else {
    pthread_join(thread,NULL);
    return 0;
  }
}
