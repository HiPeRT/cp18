/*
 * Linux FIFO/RR scheduler demo
 * 
 * This demo creates a few tasks scheduled with the SCHED_FIFO or the
 * SCHED_RR scheduler
 *
 * Copyright (C) 2002 by Paolo Gai
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

/* for sched_param.c */
#include <sched.h> 

void *low(void *arg)
{
  printf("LOW priority thread!!!\n");
  return NULL;
}

void *medium(void *arg)
{
  int i,j;

  for (i=0; i<300; i++) {
    for (j=0; j<1000000; j++) ;
    printf((char *)arg);
  }

  return NULL;
}

void my_create(int policy)
{
  pthread_t th1, th2, th3;
  pthread_attr_t medium_attr, low_attr;
  struct sched_param medium_policy, low_policy;

  pthread_attr_init(&medium_attr);
  pthread_attr_setschedpolicy(&medium_attr, policy);
  medium_policy.sched_priority = 2;
  pthread_attr_setschedparam(&medium_attr, &medium_policy);  
  // NTPL defaults to INHERIT_SCHED!!!
  pthread_attr_setinheritsched(&medium_attr, PTHREAD_EXPLICIT_SCHED);

  pthread_attr_init(&low_attr);
  pthread_attr_setschedpolicy(&low_attr, SCHED_FIFO);
  low_policy.sched_priority = 1;
  pthread_attr_setschedparam(&low_attr, &low_policy);  
  // NTPL defaults to INHERIT_SCHED!!!
  pthread_attr_setinheritsched(&low_attr, PTHREAD_EXPLICIT_SCHED);

  pthread_create(&th1, &medium_attr, medium, (char *)".");
  pthread_create(&th2, &medium_attr, medium, (char *)"#");
  pthread_create(&th3, &low_attr, low, (char *)"-");
  
  pthread_attr_destroy(&medium_attr);
  pthread_attr_destroy(&low_attr);

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  pthread_join(th3, NULL);
}


void *high(void *arg)
{
  /* first experiment:
     - two medium priority thread scheduled with RR
     - one low priority thread scheduled with FIFO
  */
  my_create(SCHED_RR);

  /* second experiment:
     - two medium priority thread scheduled with FIFO
     - one low priority thread scheduled with FIFO
  */
  my_create(SCHED_FIFO);

  return NULL;

}

int main()
{
  pthread_t mythread;
  pthread_attr_t myattr;
  struct sched_param myparam;

  int err;
  int parameter;
  void *returnvalue;

  /* initializes the thread attribute */
  pthread_attr_init(&myattr);
  pthread_attr_setschedpolicy(&myattr, SCHED_FIFO);
  
  // NTPL defaults to INHERIT_SCHED!!!
  pthread_attr_setinheritsched(&myattr, PTHREAD_EXPLICIT_SCHED);

  myparam.sched_priority = 3;
  pthread_attr_setschedparam(&myattr, &myparam);

  err = pthread_create(&mythread, &myattr, high, (void *)&parameter);

  if (err) {
    perror("ERROR");
    exit(1);
  }

  pthread_attr_destroy(&myattr);

  /* wait the end of the thread we just created */
  pthread_join(mythread, &returnvalue);

  return 0;
}

