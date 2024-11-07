This C program develops a mutual exclusion algorithm for a process synchronization problem. Ensure that the mutual exclusion algorithm guarantees only
one process access to the critical section portion of the code at a given time.The program uses mutual exclusion/semaphore related C function calls.

Description of the problem:  
Assume that there are a set of n bank accounts (n ≥ 1) shared by a set of x clients (x ≥ 1). Clients
can perform two different types of transactions with each bank account: deposit and withdraw
funds. If a particular transaction results in a negative account balance, the transaction should be
ignored (i.e., an account balance should never be less than 0).  

Example and structure of the input file:  
In the following example, there are two bank accounts (A1 and A2) shared by a total of ten clients
(C1 to C10). The clients are allowed to deposit money into both accounts and withdraw money
from both accounts. The initial balances of the accounts are specified in the input file.   

An input file is provided below for illustrative purposes.  
A1 balance 7000  
A2 balance 4500  
C1 deposit A1 100 withdraw A2 500  
C2 withdraw A1 2500  
...  
...  
C9 withdraw A1 1000 withdraw A2 500  
C10 deposit A1 50 deposit A2 200 

Illustration:  
(i) A1 balance 7000  
The above line specifies the initial balance of account #1 as $7000  
(ii) C1 deposit A1 100 withdraw A2 500  
The above line specifies the operations performed by client #1. client #1 deposits $100 into account  
#1, then withdraws $500 from Account #2.  

  
For every client, the program uses a separate thread. The transactions for a unique client should be
handled by a distinct thread. For appropriate synchronization among the threads (i.e., critical
sections are protected against random access by the threads) you must use mutual exclusion-related
system functions (such as pthread_mutex_lock(), pthread_mutex_unlock() etc. which will be
discussed in the lecture #14). As part of your output (i) you must show the number of Accounts and
Clients (as per the input file data), and (ii) you must output the balances of each bank account after
all the transactions have been performed.   
For example, the output looks like:  
No. of Accounts: 2  
No. of Clients: 2  
A1 balance 500  
A2 balance 300
