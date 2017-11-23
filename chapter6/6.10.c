#include "stdio.h"
#include "signal.h"

long current;
long max_prime;
long prime_num;

void print_prime()
{
	printf("The numbers of the prime are %ld\n", prime_num);
	printf("Now, the max prime is %ld\n", max_prime);
}

int main()
{	
	int test;
	current   = 2;
	prime_num = 0;
	int test_num  = 0;
	signal(SIGINT, print_prime);
	while(1){
		test_num = 0; 		
		for(test = 2; test <= current; test++){
			if((current % test) == 0)
				test_num++;
		}
		if(test_num == 1){
			max_prime = current;
			prime_num++;
		}
		current++;
		sleep(2);
	}
}
