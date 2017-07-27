Answer 1.

Linux adopts a common paging model that fits both 32-bit and 64-bit architectures
Up to version 2.6.10, the Linux paging model consisted of three paginglevels. Starting with version 2.6.11, a four-level paging model has been adopted.

Thefour types of page tables are called:

	1. Page Global Directory

	2. Page Upper Directory

	3. Page Middle Directory

	4. Page Table
	
Reference : Understanding the Linux Kernel, 3rd Edition by Marco Cesati, Daniel P. Bovet
            Chapter 2, Section 2.5
            
          
          
Answer 2.

I used the following program to visualize the memory layout:
	
	#include <stdio.h>
	#include <stdlib.h>
	#include <iostream>
	#include <fstream>
	#include <unistd.h>

	using namespace std;

	int count = 0;

	void swap(int* a,int left,int right)
	{
	  int temp;
	  temp = a[left];
	  a[left] = a[right];
	  a[right] = temp;
	}

	int partition(int* a,int low,int high)
	{
	  int key;
	  key = a[high];
	  int i,j;
	  i = low-1;
	  for(j=low;j<high;j++)
	   {
		  if(a[j] < key)
		  {
		   i++;
		   swap(a,i,j);
		   count++;
		  }  
	   } 
	   swap(a,i+1,high);
	   return i+1;
	}

	void QuickSort(int* a,int low,int high)
	{
	  int pivot;
	  if(high > low)
	  {
		 pivot = partition(a,low,high);
		 QuickSort(a,low,pivot-1);
		 QuickSort(a,pivot+1,high);
	  }
	}

	void printarray(int* a,int n)
	{
	  int i;
	  for(i=0;i<n;i++)
		printf("%d ", a[i]);
	  printf("\n");  
	}

	int main()
	{
	   cout<<"The pid is : "<<getpid();
	   int n,i;
	   cout<<"Enter the number of elements\n";
	   cin>>n;
	   int *a = new int[n];
	   for(i=0;i<n;i++)
	   {
	   		cin>>a[i];
	   }
	   QuickSort(a,0,n-1);
	   for(i=0;i<n;i++)
	   {
	   		cout<<a[i]<<"\n";
	   }	
	   return 0; 
	}
	
The output I got after running the command cat /proc/4259/maps is(since the pid of the process is 4259) : 

	00400000-00401000 r-xp 00000000 08:05 5245222                            /home/mnit/Desktop/a.out
	00601000-00602000 r--p 00001000 08:05 5245222                            /home/mnit/Desktop/a.out
	00602000-00603000 rw-p 00002000 08:05 5245222                            /home/mnit/Desktop/a.out
	010c8000-010e9000 rw-p 00000000 00:00 0                                  [heap]
	7f7de1226000-7f7de123c000 r-xp 00000000 08:05 3670207                    /lib/x86_64-linux-gnu/libgcc_s.so.1
	7f7de123c000-7f7de143b000 ---p 00016000 08:05 3670207                    /lib/x86_64-linux-gnu/libgcc_s.so.1
	7f7de143b000-7f7de143c000 rw-p 00015000 08:05 3670207                    /lib/x86_64-linux-gnu/libgcc_s.so.1
	7f7de143c000-7f7de1541000 r-xp 00000000 08:05 3670212                    /lib/x86_64-linux-gnu/libm-2.19.so
	7f7de1541000-7f7de1740000 ---p 00105000 08:05 3670212                    /lib/x86_64-linux-gnu/libm-2.19.so
	7f7de1740000-7f7de1741000 r--p 00104000 08:05 3670212                    /lib/x86_64-linux-gnu/libm-2.19.so
	7f7de1741000-7f7de1742000 rw-p 00105000 08:05 3670212                    /lib/x86_64-linux-gnu/libm-2.19.so
	7f7de1742000-7f7de18fc000 r-xp 00000000 08:05 3670227                    /lib/x86_64-linux-gnu/libc-2.19.so
	7f7de18fc000-7f7de1afc000 ---p 001ba000 08:05 3670227                    /lib/x86_64-linux-gnu/libc-2.19.so
	7f7de1afc000-7f7de1b00000 r--p 001ba000 08:05 3670227                    /lib/x86_64-linux-gnu/libc-2.19.so
	7f7de1b00000-7f7de1b02000 rw-p 001be000 08:05 3670227                    /lib/x86_64-linux-gnu/libc-2.19.so
	7f7de1b02000-7f7de1b07000 rw-p 00000000 00:00 0 
	7f7de1b07000-7f7de1bed000 r-xp 00000000 08:06 5899209                    /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.19
	7f7de1bed000-7f7de1dec000 ---p 000e6000 08:06 5899209                    /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.19
	7f7de1dec000-7f7de1df4000 r--p 000e5000 08:06 5899209                    /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.19
	7f7de1df4000-7f7de1df6000 rw-p 000ed000 08:06 5899209                    /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.19
	7f7de1df6000-7f7de1e0b000 rw-p 00000000 00:00 0 
	7f7de1e0b000-7f7de1e2e000 r-xp 00000000 08:05 3670220                    /lib/x86_64-linux-gnu/ld-2.19.so
	7f7de2006000-7f7de200b000 rw-p 00000000 00:00 0 
	7f7de2029000-7f7de202d000 rw-p 00000000 00:00 0 
	7f7de202d000-7f7de202e000 r--p 00022000 08:05 3670220                    /lib/x86_64-linux-gnu/ld-2.19.so
	7f7de202e000-7f7de202f000 rw-p 00023000 08:05 3670220                    /lib/x86_64-linux-gnu/ld-2.19.so
	7f7de202f000-7f7de2030000 rw-p 00000000 00:00 0 
	7fff11464000-7fff11485000 rw-p 00000000 00:00 0                          [stack]
	7fff115e8000-7fff115ea000 r-xp 00000000 00:00 0                          [vdso]
	ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]


By the output we can clearly visulaize the memory layout of my program. The output is shown from lower memory address to higher memory address, which is why heap is shown at top and stack at 
bottom. 

In the output :

The first segment: "Text Segment".

The second segment: "Data Segment".

The third segment: "BSS Segment".

Here the program size is 4KB because the page size of the Linux OS is set to 4 KiB by default. This means the minimum addressable memory segment is 4 KiB.

The sudden jump/change in the adresses is due to the implementation of malloc.There is roughly a massive step of 127 Tebibytes.
Linux's memory map reserves the first 0000000000000000 - 00007fffffffffff as user space memory.
It appears that these regions are almost at the very bottom of the 128 TiB range reserved for user space memory. Considering that there's a 127 TiB gap, this basically means that our malloc uses the user space range 0000000000000000 - 00007fffffffffff from both ends. From the low end, it grows the heap upwards (upwards in address numbers). While at the high end, it grows the stack downwards (downwards in address numbers).

At the same time, the stack is actually a fixed section of memory, so it cannot actually grow as much as the heap. On the high end, but lower than the stack, we see lots of memory regions assigned for the shared libraries and anonymous buffers which are probably used by the shared libraries.

We can also view the shared libraries that are being used by the executable. This determines which shared libraries will also be loaded into memory at startup. 