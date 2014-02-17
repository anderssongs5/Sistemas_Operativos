#include "account.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NAME    "QUIZ"
#define OUTPUT_NAME "Output.txt"

struct threads_info{
    struct account *accounts;
    pthread_mutex_t *mutexes;
} attr;

int threadNum, amountTime, amountAccounts;
double initialValue;
struct timespec ts;

void *useBank(void* attr);

int extractParameters(int argc, char* argv[]);
    
void createAccountsAndMutex(struct account *accounts, pthread_mutex_t *mutex,
    int size, double initialValue);

int main(int argc, char* argv[]) {
	FILE* output = fopen(OUTPUT_NAME, "a+");
	
    if (output == NULL) {
        fprintf(stdout, "%s: The file: \"%s\" can not be opened.\n", NAME,
            OUTPUT_NAME);

        return (EXIT_FAILURE);
    }
	
    if (extractParameters(argc, argv) == 0) {
        fprintf(stderr, "%s: The params amount is not valid. Amount: %d.\n",
            NAME, argc);

        return (EXIT_FAILURE);
    }

    struct account *accounts = (struct account *) malloc(sizeof(struct account)* amountAccounts);
    pthread_mutex_t *mutexes = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * amountAccounts);
    
    ts.tv_sec = 0;
    ts.tv_nsec = 100000000;
    
    createAccountsAndMutex(accounts, mutexes, amountAccounts, initialValue);

    attr.accounts = accounts;
    attr.mutexes = mutexes;

    pthread_t threads[threadNum];
    int i;
    for (i = 0; i < threadNum; i++) {
        pthread_create(&threads[i], NULL, &useBank, &attr);
    }

    for(i = 0; i < threadNum; i++){
        pthread_join(threads[i], NULL);
    }

    double totalMoney = 0.0;
    struct account *p = accounts;
    int counter = 0;
    for (; counter < amountAccounts; counter++) {
        printf("ID: %ld  - Money: %f\n", p->id, p->money);
        totalMoney += p->money;
        p++;
    }
    
    fprintf(output, "Total_money %f", totalMoney);
    fflush(output);

    free(accounts);
    free(mutexes);

	fclose(output);

    return (EXIT_SUCCESS);
}

void createAccountsAndMutex(struct account *accounts, pthread_mutex_t *mutexes,
    int size, double initialValue) {
    long j;
    for(j = 1; j <= size; j++) {
        accounts->id = j;
        accounts->money = initialValue;

        accounts++;
        pthread_mutex_init(mutexes, NULL);
        mutexes++;
    }
}

int extractParameters(int argc, char* argv[]) {
    if (argc == 5) {
        threadNum = atoi(argv[1]);
        amountTime = atoi(argv[2]);
        amountAccounts = atoi(argv[3]);
        initialValue = atof(argv[4]);
        
        /*printf("Thread Num: %d\n", threadNum);
        printf("Amount Time: %d\n", amountTime);
        printf("Amount Accounts: %d\n", amountAccounts);
        printf("Initial Value: %f\n", initialValue);
        
        for(;;);*/

        return (1);
    }

    return (0);
}

void generateRand(int indexes[]) {
    do {
        indexes[0] = (int)(rand() % amountAccounts);
        indexes[1] = (int)(rand() % amountAccounts);
    } while(indexes[0] == indexes[1]);
}

void *useBank(void* attr) {
    struct threads_info *a = (struct threads_info *) attr;
    int indexes[] = {0, 0};

    srand(time(NULL));
    int timeCounter;
    for (timeCounter = 1; timeCounter <= amountTime; timeCounter++){
        generateRand(indexes);

        struct account *ac1 = a->accounts + indexes[0];
        struct account *ac2 = a->accounts + indexes[1];
        
        pthread_mutex_t *mut1 = a->mutexes + indexes[0];
		pthread_mutex_t *mut2 = a->mutexes + indexes[1];
		
		//printf("ID: %ld  - Money: %f\n", ac1->id, ac1->money);
		//printf("ID: %ld  - Money: %f\n", ac2->id, ac2->money);
		
		int r1 = pthread_mutex_trylock(mut1);
		int r2 = pthread_mutex_trylock(mut2);

		if ((r1 == 0) && (r2 == 0)) {
			double amount = rand() % ((int) ac1->money);
			int h = transfer(ac1, ac2, amount);
			
			//printf("ID: %ld  - Money: %f\n", ac1->id, ac1->money);
			//printf("ID: %ld  - Money: %f\n\n", ac2->id, ac2->cmoney);
			
			pthread_mutex_unlock(mut2);
			pthread_mutex_unlock(mut1);
		} else {
			if (r1 == 0) {
				pthread_mutex_unlock(mut1);
			} 
			
			if(r2 == 0){
				pthread_mutex_unlock(mut2);
			}
		}
        
        nanosleep(&ts, NULL);
    }

    return NULL;
}
