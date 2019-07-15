#include<bits/stdc++.h>
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<queue>
using namespace std;


//semaphore to control sleep and wake up
sem_t empty;
sem_t full;
sem_t wt1;
sem_t wt2;
sem_t ch;
queue<int> q;
queue<int> w1;
queue<int> w2;
pthread_mutex_t lock1;
pthread_mutex_t wait1;
pthread_mutex_t wait2;
int vanilla_cake = 1;
int choco_cake = 2;
int buf = 5;
int r;

void init_semaphore()
{
	//srand(time(NULL));
	//int k = (rand()%buf) + 1;
	sem_init(&empty,0,5);
	sem_init(&full,0,0);
	sem_init(&wt1,0,0);
	sem_init(&wt2,0,0);
	//sem_init(&ch,0,2);
	pthread_mutex_init(&lock1,0);
	pthread_mutex_init(&wait1,0);
	pthread_mutex_init(&wait2,0);
}

void * ChefX(void * arg){
	
	while(1){
	//for(int i = 0; i < x; i++){
	sem_wait(&empty);
	sleep(2);
	pthread_mutex_lock(&lock1);		
		
	q.push(vanilla_cake);
	printf("%s : ",(char*)arg);
	printf("ChefX produced vanilla cake %d\n",vanilla_cake);
	printf("Cake box size %d\n",q.size());
		
	pthread_mutex_unlock(&lock1);
	
	sem_post(&full);
	//}
	}

}

void * ChefY(void * arg){
	//char y = *(int*)arg;
	while(1){
	//for(int i = 0; i < y;i++){
	sem_wait(&empty);
	sleep(2);
	pthread_mutex_lock(&lock1);		
		
	q.push(choco_cake);
	printf("%s : ",(char*)arg);
	printf("ChefY produced chocolate cake %d\n",choco_cake);
	printf("Cake box size %d\n",q.size());
		
	pthread_mutex_unlock(&lock1);
	
	sem_post(&full);
	}
	//}

}

void * ChefZ(void * arg){
printf("%s : ",(char*)arg);
	while(1){
	//for(int i=0;i<z;i++)
	//{	
		sem_wait(&full);
		sleep(1);
 		int item = q.front();
		
		
		
		if(item == vanilla_cake){
			//sem_wait(&wt1);
			//sem_post(&wt1);
			//pthread_mutex_lock(&lock);
			pthread_mutex_lock(&lock1);
			pthread_mutex_lock(&wait1);
			q.pop();
		     w1.push(item);
		     printf("ChefZ takes a vanilla cake and puts in box1 %d\n",item);
			//pthread_mutex_unlock(&lock);
			pthread_mutex_unlock(&wait1);
			pthread_mutex_unlock(&lock1);
			sem_post(&wt1);
		}
		else{
			//sem_post(&wt2);
			//pthread_mutex_lock(&lock1);
			pthread_mutex_lock(&lock1);
			pthread_mutex_lock(&wait2);
			q.pop();
		     w2.push(item);
		     printf("ChefZ takes a chocolate cake and puts in box2 %d\n",item);
			//sem_post(&wt2);
			//pthread_mutex_unlock(&lock);
			pthread_mutex_unlock(&wait2);
			pthread_mutex_unlock(&lock1);
			sem_post(&wt2);
		}
			

		
		
		sem_post(&empty);
		//sem_post(&wt2);
	}
//}
}


void *waiter1(void * arg){
	while(1){
	
		sem_wait(&wt1);
		//}
 		sleep(1);
		pthread_mutex_lock(&wait1);
			
		int item = w1.front();
		w1.pop();
		printf("%s : ",(char*)arg);
		printf("Waiter1 takes a vanilla cake %d\n",item);
		
		pthread_mutex_unlock(&wait1);
		
	}




}

void *waiter2(void * arg){
	while(1){
	//for(int i=0;i<z;i++)
	//{	
		//(w1.size() == 0){
		sem_wait(&wt2);
		//}
 		sleep(1);
		pthread_mutex_lock(&wait2);
			
		
		int item = w2.front();
		w2.pop();
		printf("%s : ",(char*)arg);
		printf("Waiter2 takes a chocolate cake %d\n",item);
		
		pthread_mutex_unlock(&wait2);
		
		//if(w2.empty()){
			//sem_post(&empty);
		//}
	}




}




int main(void)
{	
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	pthread_t thread4;
	pthread_t thread5;
	
	init_semaphore();
	
	char * msg1 = "i am chefX";
	char * msg2 = "i am chefY";
	char * msg3 = "i am chefZ";
	char * msg4 = "i am waiter1";
	char * msg5 = "i am waiter2";	
	
	pthread_create(&thread1,NULL,ChefY,(void*)msg2);
	pthread_create(&thread2,NULL,ChefX,(void*)msg1);
	pthread_create(&thread3,NULL,ChefZ,(void*)msg3 );
	
	pthread_create(&thread4,NULL,waiter1,(void*)msg4);
	pthread_create(&thread5,NULL,waiter2,(void*)msg5 );
	//pthread_create(&thread2,NULL,ChefX,(void*)msg1);
	////srand(time(NULL));

	
	while(1);
	return 0;
}
