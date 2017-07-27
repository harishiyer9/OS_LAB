#include<stdio.h>
#include "awesome.h"
#include<kernel/console.h>
#include<random.h>
#include "threads/thread.h"

void SelectionSort(void)
{
	int a[10],i,j,min_idx,temp;;

	random_bytes(a,10*sizeof(int));	//To generate random numbers
	
	for(i=0;i<10;i++)              //To make the numbers small
		a[i] /= 10000000;

	//Selection sort to sort the numbers.
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

	//To display the sorted array
	printf("The numbers after sorting are: ");
	for(i=0;i<10;i++)
		printf("\n%d",a[i]);
	printf("\n");
	thread_yield();
}

void test_hello(void)
{
	printf("\nHello World\n");
	thread_create("sort",63,SelectionSort,NULL);
}

