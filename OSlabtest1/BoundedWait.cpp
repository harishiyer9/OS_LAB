#include<iostream>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

bool test_and_set(bool *target) {
    bool rv = *target;
    *target = true;
    return rv;
}

int main()
{
	int n=10;bool waiting[10]; int i=0,j;
	bool lock;
	do {
		waiting[i] = true;
		while (waiting[i] && test_and_set(&lock)) ;
		waiting[i] = false;

		/* critical section */

		j = (i + 1) % n;
		while ((j != i) && !waiting[j]) 
		    j = (j + 1) % n; 
		if (j == i) 
		    lock = false; 
		else
		    waiting[j] = false;
		printf("Thread %d acquiring lock\n",j);
		printf("Thread %d in critical section\n",j);
		printf("Thread %d releasing lock\n",j);

		/* remainder section */		
		i++;
		if(i==10)
		{
			i=0;
		}
	} 
	while (true);

}
