#include <stdio.h>
#include <stdlib.h>
#include <threads/malloc.h>
#include "awesome.h"
#include <kernel/console.h>
#include <random.h>
#include <devices/timer.h>
#include "lib/user/syscall.c"
#include "threads/thread.h"
#include "devices/shutdown.h"

/*
int a[10];

To display the sorted array and to print the priority of the thread.			
void test_print(void *aux)
{
	
	struct thread *CurrentThread = thread_current();

	int i;			
	printf("Priority of the third thread is : %d\n",CurrentThread->priority);
	printf("The numbers after sorting are:\n");
	for(i=0;i<10;i++)
        {
		printf("%d\n",a[i]);
	}
	printf("\n");	
	
}

 To sort the array and to print the ID of the current thread.
void test_sort(void *aux)
{
	int i,min_idx,j,temp;
	struct thread *CurrentThread = thread_current();
	printf("Id of the second thread is : %d\n",CurrentThread->tid);
	
	for(i=0;i<10-1;i++)
	{
		min_idx=i;
		for(j=i+1;j<10;j++)
		{
		    if(a[j]<a[min_idx])
		    {
			min_idx=j;
		    }
		}
		temp=a[min_idx];
		a[min_idx]=a[i];
		a[i]=temp;
	}
	
	//Create third thread that displays the sorted array and the thread's id.
	thread_create("DisplayNumbers",61,test_print,a);
	
}

// To generate random numbers and to display the current threads name.
void test_generate(void *aux )
{
	
	struct thread *CurrentThread = thread_current();
	// To print the current threads's name.
	printf("Name of the first thread is : %s\n",CurrentThread->name);

	int i;

	random_bytes(a,10*sizeof(int));	//To generate random numbers
	
	for(i=0;i<10;i++) 
	{
                //To make the numbers small
		a[i] /= 10000000;
	}
	thread_create("SortNumbers",62,test_sort,a);	
	
}
*/
void test_hello(void)
{
	printf("\nHello World\n");
	halt();
	
	//Creates thread to genrate random numbers and sort them in array
	//thread_create("CreateRandomNumbers",63,test_generate,NULL);
	
	/*thread_yield();
	thread_yield();
	thread_yield();*/
}

