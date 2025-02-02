#include <stdio.h>
#include <pthread.h>

int max = 10000000; 
volatile int balance = 0; // shared global variable, volatile prevents optimization 
void *mythread(void *arg) { 
 int i; // stack (private per thread) 
 for (i = 0; i < max; i++) { 
 balance = balance + 1; // shared: only one 
 } 
 return NULL; 
} 

int main() {
 pthread_t p1, p2; 
 pthread_create(&p1, NULL, mythread, NULL); 
 pthread_create(&p2, NULL, mythread, NULL); 
 pthread_join(p1, NULL); // wait for threads to finish 
 pthread_join(p2, NULL); 
 printf("main: done\n [balance: %d]\n [should: %d]\n", balance, max*2);
 return 0;
}