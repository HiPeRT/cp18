/*
 * pthread_create demo 
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

pthread_t main_id;
int pippo = 5;

void *body(void *arg)
{
  int p = *(int *)arg;
  pthread_t mythread_id;

  printf("mythread: parameter=%d\n", p);

  mythread_id = pthread_self();

  printf("mythread: main_id==mythread_id:%d\n", 
	 pthread_equal(main_id, mythread_id) );

  return (void *)5678;
}

int main()
{
  pthread_attr_t myattr;
  pthread_t thethread;
  int err;
  int parameter;
  void *returnvalue;

  parameter = 1234;

  /* initializes the thread attribute */
  pthread_attr_init(&myattr);


  puts("main: before pthread_create\n");
  main_id = pthread_self();


  /* creation and activation of the new thread */
  err = pthread_create(&thethread, &myattr, body, (void *)&parameter);

  puts("main: after pthread_create\n");

  /* the thread attribute is no more needed */
  pthread_attr_destroy(&myattr);

  /* wait the end of the thread we just created */
  pthread_join(thethread, &returnvalue);

  printf("main: returnvalue is %d\n", (int)returnvalue);

  return 0;
}













