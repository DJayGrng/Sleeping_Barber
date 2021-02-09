#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

// function for client thread
void *client(void *param);

// function for barber thread
void *barber(void *param);

// The maximum time that one haircut takes
const int HAIRCUT_TIME=3;


int numchair,maxchair,customer;


pthread_mutex_t cchair_mutex;
pthread_cond_t cchair_cond;

int main() 
{
	pthread_t barberid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int runTime;
	int clients,i;

	//Initialize mutex and cond variables
	
	pthread_mutex_init(&cchair_mutex,NULL);
	pthread_cond_init(&cchair_cond,NULL);

	printf("Enter the maximum number of chairs in the barber shop: ");
	scanf("%d",&maxchair);

	
	numchair=maxchair;

	printf("Enter the total number of customers that will visit the shop: ");
	scanf("%i",&customer);
	
	//Initialize client threads
	pthread_t clientids[customer];

	printf("Enter time for which barber shop will remain open: ");
	scanf("%i",&runTime);

	printf("\nBarber has opened shop for %i seconds.\n\n",runTime);

	
	
	/* 3. Create barber thread. */
	pthread_create(&barberid, &attr, barber, NULL);

	// 4. Create client threads.  
	
	for (i = 0; i < customer; )
	{
		if( rand() % 4 == 0)
		{
			pthread_create(&clientids[i], &attr, client, NULL);
			i++;
		}

	}
	// 5. Sleep. 

	sleep(runTime);

	/* 6. Exit.  */

	printf("Barber Closed Shop !!! \n");

	// 7. Destroy Everything
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&cchair_mutex);
	pthread_cond_destroy(&cchair_cond);
	exit(0);
}

void *barber(void *param) 
{
	while(1)
	{
		
		// lock available chairs
		pthread_mutex_lock(&cchair_mutex);

		// if shop is empty wait for customer
		if(numchair == maxchair)

		{
			printf("No Customers in Shop. Barber is sleeping till customer arrives.\n\n");
			pthread_cond_wait(&cchair_cond,&cchair_mutex);
			printf("Customer woke barber up !!\n");
			pthread_mutex_unlock(&cchair_mutex);
		}

		// if shop is not empty take one customer and increase available chairs
		else
		{
			
			printf("Barber took a client\n");

			numchair+=1;
			printf("Available Chairs:%i\n", numchair);
			pthread_mutex_unlock(&cchair_mutex);	

		printf("Barber is cutting hair now which will take %i seconds\n\n",HAIRCUT_TIME);
			sleep(HAIRCUT_TIME);
		}

	}
}

void *client(void *param)
{
	// sleep for a random time so customers arrive at different times
	sleep((rand()%20)+8);

	pthread_mutex_lock(&cchair_mutex);	

	printf("Customer arrived\n");

	if(numchair == maxchair)
	{
		pthread_cond_signal(&cchair_cond);
	}



	if (numchair <= 0)
	{
		printf("Shop is Full. Customer Left !!\n\n");
		pthread_mutex_unlock(&cchair_mutex);
		pthread_exit(NULL);
	}
	
	// Decrease chairs if customer comes
	numchair-=1;

	printf("Available Chairs:%i\n\n", numchair);




	// Unlock chair
	pthread_mutex_unlock(&cchair_mutex);

	pthread_exit(0);

}
  
