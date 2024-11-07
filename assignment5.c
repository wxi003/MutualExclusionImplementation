#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
   This program illustrates the mutual exclusion algorithm for a process synchronization problem.
*/

// gcc assignment5.c -o assignment5.out
//./assignment5.out

typedef struct
{
    pthread_mutex_t lock;
    int balance;
} BankAccounts;

// use dynamic memory
BankAccounts *accounts;

int numAccounts = 0;
int numClients = 0;

void *clientOperations(void *args)
{
    // casting the args pointer to an int * pointer;
    // dereference the pointer to get the actual integer value.
    int clientId = *((int *)args);

    FILE *file = fopen("assignment_5_input.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        pthread_exit(NULL);
    }

    char operation[1000];
    while (fgets(operation, sizeof(operation), file))
    {   
        if (operation[0] == 'C')
        {
            int client;
            char action[10];
            char account[10];
            int amount;
            int offset = 0;
            char *data = operation;

            // %n: an optional specifier to store the number of characters scanned so far
            sscanf(data, "C%d %s %s %d%n", &client, action, account, &amount, &offset);
            data += offset;
            if (client == clientId)
            {
                while (1)
                {
                    pthread_mutex_lock(&accounts[account[1] - '0' - 1].lock);
                    if (strcmp(action, "deposit") == 0)
                    {   

                        //'0': This is the character literal representing the digit 0. 
                        // In ASCII, the characters '0' to '9' are represented by consecutive integer values. 
                        // Subtracting '0' from a character digit will give you the corresponding integer value.
                        accounts[account[1] - '0' - 1].balance += amount;
                    }
                    else if (strcmp(action, "withdraw") == 0)
                    {
                        if (accounts[account[1] - '0' - 1].balance >= amount)
                        {
                            accounts[account[1] - '0' - 1].balance -= amount;
                        }
                    }
                    pthread_mutex_unlock(&accounts[account[1] - '0' - 1].lock);

                    // 'operation + offset' points to the location in the operation string after the offset characters.
                    // It allows sscanf to continue reading from the position where the previous sscanf call ended.
                    if (sscanf(data, "%s %s %d%n", action, account, &amount, &offset) != 3)
                    {
                        break;
                    }
                    else
                    {
                        data += offset;
                    }
                }
            }
        }
    }

    fclose(file);
    free(args);
    pthread_exit(NULL);
}

int main(void)
{
    FILE *fptr;
    int i, err_thread;

    int balances[100];
    char readString[1000];
    char balance[10];

    fptr = fopen("assignment_5_input.txt", "r");

    // count the number of accounts and clients
    // store the balances
    while (fgets(readString, sizeof(readString), fptr))
    {
        if (readString[0] == 'A')
        {
            numAccounts++;

            sscanf(readString, "A%*s balance %d", &balances[numAccounts - 1]);
        }
        if (readString[0] == 'C')
        {
            numClients++;
        }
    }

    // move the file read pointer
    rewind(fptr);
    fclose(fptr);

    accounts = malloc(numAccounts * sizeof(BankAccounts));

    // initialize account balances and locks
    for (i = 0; i < numAccounts; ++i)
    {
        accounts[i].balance = balances[i];
        pthread_mutex_init(&accounts[i].lock, NULL);
    }

    pthread_t threads[numClients];

    // create threads for each client
    for (i = 0; i < numClients; i++)
    {
        int *args = malloc(sizeof(int));
        *args = i + 1;
        err_thread = pthread_create(&threads[i], NULL, &clientOperations, args);

        if (err_thread != 0)
            printf("\n Error creating thread %d", i);
    }
    // make sure all thread have already run
    for (i = 0; i < numClients; i++)
        pthread_join(threads[i], NULL);

    printf("No. of Accounts: %d\n", numAccounts);
    printf("No. of Clients: %d\n", numClients);

    for (i = 0; i < numAccounts; i++)
    {
        pthread_mutex_destroy(&accounts[i].lock);
        printf("A%d balance %d\n", i + 1, accounts[i].balance);
    }

    free(accounts);
    return 0;
}
