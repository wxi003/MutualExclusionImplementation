#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define withdraw_req 400
#define NUM_THREADS 5

pthread_mutex_t lock;

int shared_balance = 1000;

void  *withdraw_money(void *thread_id);


void *withdraw_money(void *thread_id)
{

long int *temp = (long int *) thread_id;
long int thread_name = *temp;


pthread_mutex_lock(&lock);  // ENTRY

printf("\n >>>>>> ENTERING CRITICAL SECTION: Thread %ld\n", thread_name);
/*****CRITICAL SECTION STARTS ******/
printf("\n ****From withdraw_money thread ID %ld****", thread_name);
printf("\n From Thread %ld current balance is %d", thread_name, shared_balance);
printf("\n Processing Thread ID %ld request for $ %d", thread_name, withdraw_req);


if (shared_balance >= withdraw_req)
{
	printf("\n Withdraw amount of %d is Authorized for Thread %ld", withdraw_req, thread_name);
	printf("\n Thread %ld successfully withdraw $400", thread_name);
	shared_balance = shared_balance - withdraw_req;
	printf("\n Updated balance %d from Thread %ld\n", shared_balance, thread_name);

}

else
	{
	printf("\n Request denied for Thread %ld\n", thread_name);
	}

/***** CRITICAL SECTION ENDS ******/
	
printf("\n LEAVING CRITICAL SECTION: Thread %ld >>>>>>\n", thread_name);

pthread_mutex_unlock(&lock); // EXIT
return NULL;
}



int main(void)
{
    
int i, err_thread;

pthread_t threads[NUM_THREADS];


for (i = 0; i< NUM_THREADS; i++)
  {
    err_thread = pthread_create(&threads[i], NULL, &withdraw_money, &threads[i]); 

if (err_thread != 0)
	printf("\n Error creating thread %d", i);
}


for (i = 0; i< NUM_THREADS; i++)
	pthread_join(threads[i], NULL);

pthread_mutex_destroy(&lock); 

return 0;

}
