#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MaxItems 10 // Maximum items a producer can produce or a consumer can consume
#define BufferSize 10// Size of the buffer

// 0 pastry | 1 meat | 2 cheesecake | 3 cabbage
sem_t emptyPastry;                  // Number of empty slots in buffer
sem_t emptyMeat;
sem_t emptyCheescake;
sem_t emptyCabbage;
sem_t fullPastry;                   // Number of slots filled
sem_t fullMeat;
sem_t fullCheescake;
sem_t fullCabbage;
int in[4] = {0, 0, 0, 0};           // index at which producer will put the next data
int buffer[4][BufferSize];          //arrays of buffers
pthread_mutex_t mutexPastry;
pthread_mutex_t mutexMeat;
pthread_mutex_t mutexCheescake;
pthread_mutex_t mutexCabbage;



void *producer(void *pno)
{
    char item;
    if ( *((int *)pno) == 0)
    {
        while(2)
        {
            item = 'P';
            sem_wait(&emptyPastry);             // decrement empty slots
            pthread_mutex_lock(&mutexPastry);
            buffer[0][in[0]] = item;            // put item into given buffer
            printf("Producer %d: Insert Item 🥟 at %d\n", *((int *)pno),in[0]);
            in[0] = (in[0]+1)%BufferSize;
            sleep(1);
            pthread_mutex_unlock(&mutexPastry);
            sem_post(&fullPastry);             // increments number of slots filled
        }
    }
    else if ( *((int *)pno) == 1)
    {
        while (1)
        {
            item = 'M';
            sem_wait(&emptyMeat);
            pthread_mutex_lock(&mutexMeat);
            buffer[1][in[1]] = item;
            printf("Producer %d: Insert Item 🥩 at %d\n", *((int *)pno), in[1]);
            in[1] = (in[1]+1)%BufferSize;
            sleep(1);

            pthread_mutex_unlock(&mutexMeat);
            sem_post(&fullMeat);
        }
    }
    else if ( *((int *)pno) == 2)
    {
        while (1)
        {
            item = 'H';
            sem_wait(&emptyCheescake);
            pthread_mutex_lock(&mutexCheescake);
            buffer[2][in[2]] = item;
            printf("Producer %d: Insert Item 🧀 at %d\n", *((int *)pno), in[2]);
            in[2] = (in[2]+1)%BufferSize;
            sleep(1);

            pthread_mutex_unlock(&mutexCheescake);
            sem_post(&fullCheescake);
        }
    }
    else if ( *((int *)pno) == 3)
    {
        while (1)
        {
            item = 'A';
            sem_wait(&emptyCabbage);
            pthread_mutex_lock(&mutexCabbage);
            buffer[3][in[3]] = item;
            printf("Producer %d: Insert Item 🥬 at %d\n", *((int *)pno), in[3]);
            in[3] = (in[3]+1)%BufferSize;
            sleep(1);

            pthread_mutex_unlock(&mutexCabbage);
            sem_post(&fullCabbage);
        }
    }
}
// 🥟 🥩 🧀 🥬
void *consumer(void *cno)
{
    if ( *((int *)cno) == 0)
    {
        while(1)
        {
            sem_wait(&fullMeat);
            sem_wait(&fullPastry);

            printf("Client %d: Made delicious 🥟 with 🥩\n", *((int *)cno));
            sleep(1);


            sem_post(&emptyMeat);
            sem_post(&emptyPastry);
        }
    }
    else if ( *((int *)cno) == 1)
    {
        while(1)
        {
            sem_wait(&fullCheescake);
            sem_wait(&fullPastry);

            printf("Client %d: Made delicious 🥟 with 🧀\n", *((int *)cno));
            sleep(1);


            sem_post(&emptyCheescake);
            sem_post(&emptyPastry);
        }
    }
    else if ( *((int *)cno) == 2)
    {
        while(1)
        {
            sem_wait(&fullCabbage);
            sem_wait(&fullPastry);

            printf("Client %d: Made delicious 🥟 with 🥬\n", *((int *)cno));
            sleep(1);


            sem_post(&emptyCabbage);
            sem_post(&emptyPastry);
        }
    }
}
int main(int argc, char** argv)
{
    int index[4] = {0, 1, 2, 3};
    // Getting data
    int pastryProd, meatProd, meatCons, cheeseProd, cheeseCons, cabbageProd, cabbageCons;
    if (argc != 8)
    {
		printf("\nUsage: ./name pastryProd meatProd meatCons cheeseProd cheeseCons cabbageProd cabbageCons");
		return 1;
	}
    pastryProd = atoi(argv[1]);
    meatProd = atoi(argv[2]);
    meatCons = atoi(argv[3]);
    cheeseProd = atoi(argv[4]);
    cheeseCons = atoi(argv[5]);
    cabbageProd = atoi(argv[6]);
    cabbageCons = atoi(argv[7]);
    // got it
    int prodArray[4] = {pastryProd, meatProd, cheeseProd, cabbageProd};
    int consArray[3] = {meatCons, cheeseCons, cabbageCons};

    int totalProd = 0;
    for (int i = 0; i < 4; ++i)
        totalProd += prodArray[i];

    int totalCons = 0;
    for (int i = 0; i < 4; ++i)
        totalProd += prodArray[i];

    // thread init
    pthread_t pro[totalProd], con[totalCons];
    pthread_mutex_init(&mutexPastry, NULL);
    pthread_mutex_init(&mutexMeat, NULL);
    pthread_mutex_init(&mutexCheescake, NULL);
    pthread_mutex_init(&mutexCabbage, NULL);


    sem_init(&emptyPastry, 0, BufferSize);
    sem_init(&emptyMeat, 0, BufferSize);
    sem_init(&emptyCheescake, 0, BufferSize);
    sem_init(&emptyCabbage, 0, BufferSize);

    sem_init(&fullPastry,0,0);
    sem_init(&fullMeat,0,0);
    sem_init(&fullCheescake,0,0);
    sem_init(&fullCabbage,0,0);


    for(int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < prodArray[i]; ++j)
        {
            pthread_create(&pro[i], NULL, (void *)producer, (void *)&index[i]);
        }
    }

    for(int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < consArray[i]; ++j)
        {
            pthread_create(&con[i], NULL, (void *)consumer, (void *)&index[i]);
        }
    }

    for(int i = 0; i < totalProd; ++i) {
        pthread_join(pro[i], NULL);
    }
    for(int i = 0; i < totalCons; ++i) {
        pthread_join(con[i], NULL);
    }

    return 0;
}