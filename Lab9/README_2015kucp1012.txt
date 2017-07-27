ANSWER 1.

1. malloc()

	malloc stands for "memory allocation".
	The function malloc() reserves a block of memory of specified size and return a pointer of type void which can be casted into pointer of any form.

	Syntax of malloc()

		ptr = (cast-type*) malloc(byte-size)

	Here, ptr is pointer of cast-type. The malloc() function returns a pointer to an area of memory with size of byte size. If the space is insufficient, allocation fails and returns NULL pointer.

	ptr = (int*) malloc(100 * sizeof(int));
	This statement will allocate either 200 or 400 according to size of int 2 or 4 bytes respectively and the pointer points to the address of first byte of memory.

	The malloc() function allocates size bytes and returns a pointer to the allocated memory. The memory is not initialized.  If size is  0,  then malloc()  returns either NULL, or a unique pointer value.

	Usage of malloc in a C program :

		#include <stdlib.h>

		int main(int argc, const char *argv[]) {

		  typedef struct {
		    char *name;
		    char *sex;
		    int age;
		  } student;


		  //Now I can do two things
		  student p;

		  //or
		  student *ptr = (student *)malloc(sizeof(student));

		  return 0;
		}

	As said, it is dynamic allocation which means you allocate the memory at run time. For example when you don't know the amount of memory during compile time then we use malloc to dynamically allocate memory. 

2. calloc()

	calloc stands for "clear allocation".

	A diffrerence between calloc() and malloc() is that calloc() zero-initializes the buffer, while malloc() leaves the memory uninitialized.

	Another difference is that in operating systems with optimistic memory allocation, like Linux, the pointer returned by malloc isn't backed by real memory until the program actually touches it.
	calloc does indeed touch the memory (it writes zeroes on it) and thus you'll be sure the OS is backing the allocation with actual RAM (or swap). This is also why it is slower than malloc (not only does it have to zero it, the OS must also find a suitable memory area by possibly swapping out other processes)

	Syntax of calloc()

		ptr = (cast-type*)calloc(n, element-size);

	This statement will allocate contiguous space in memory for an array of n elements. For example:

	ptr = (float*) calloc(25, sizeof(float));
	This statement allocates contiguous space in memory for an array of 25 elements each of size of float, i.e, 4 bytes.

3. realloc()

	realloc () function modifies the allocated memory size by malloc () and calloc () functions to new size.
	If enough space doesn’t exist in memory of current block to extend, new block is allocated for the full size of reallocation, then copies the existing data to new block and then frees the old block

	Syntax of realloc()

		ptr = realloc(ptr, newsize);

	Here, ptr is reallocated with size of newsize.

	Usage of realloc()

		#include <stdio.h>
		#include <stdlib.h>

		int main()
		{
		    int *ptr, i , n1, n2;
		    printf("Enter size of array: ");
		    scanf("%d", &n1);

		    ptr = (int*) malloc(n1 * sizeof(int));

		    printf("Address of previously allocated memory: ");
		    for(i = 0; i < n1; ++i)
		         printf("%u\t",ptr + i);

		    printf("\nEnter new size of array: ");
		    scanf("%d", &n2);
		    ptr = realloc(ptr, n2);
		    for(i = 0; i < n2; ++i)
		         printf("%u\t", ptr + i);
		    return 0;
		}


4. free()

	Dynamically allocated memory created with either calloc() or malloc() doesn't get freed on its own. We must explicitly use free() to release the space.If the allocated memory is not freed then there might be  a problem of Memory Leakage.

	Syntax of free()

	free(ptr);

	Usage of free():

		#include <stdio.h>
		#include <stdlib.h>

		int main()
		{
		    int num, i, *ptr, sum = 0;

		    printf("Enter number of elements: ");
		    scanf("%d", &num);

		    ptr = (int*) malloc(num * sizeof(int));  //memory allocated using malloc
		    if(ptr == NULL)                     
		    {
		        printf("Error! memory not allocated.");
		        exit(0);
		    }

		    printf("Enter elements of array: ");
		    for(i = 0; i < num; ++i)
		    {
		        scanf("%d", ptr + i);
		        sum += *(ptr + i);
		    }

		    printf("Sum = %d", sum);
		    free(ptr);
		    return 0;
		}


5. alloca()

	alloca() allocates memory from Stack rather than heap which is case in malloc(). So, when I return from the routine the memory is freed. So, actually this solves my problem of freeing up of dynamically allocated memory. Freeing of memory allocated through malloc() is a major headache and if somehow missed leads to all sorts memory problems.

	RETURN VALUE : The alloca() function returns a pointer to the beginning of the allocated space. If the allocation causes stack overflow, program behaviour is undefined.

5. longjmp() and  setjmp() 

	Syntax:

		void longjmp(jmp_buf env, int val);
		int setjmp(jmp_buf env);

	The functions described above are used for performing "nonlocal gotos": transferring execution from one function to
	a predetermined location in another function.  The setjmp() function dynamically establishes the target to which
	control will later be transferred, and longjmp() performs the transfer of execution.

	The setjmp() function saves various information about the calling environment (typically, the stack pointer, the
	instruction pointer, possibly the values of other registers and the signal mask) in the buffer env for later use by
	longjmp().  In this case, setjmp() returns 0.

	The longjmp() function uses the information saved in env to transfer control back to the point where setjmp() was
	called and to restore ("rewind") the stack to its state at the time of the setjmp() call. In addition, and depending
	on the implementation (see NOTES), the values of some other registers and the process signal mask may be restored to
	their state at the time of the setjmp() call.

	Following a successful longjmp(), execution continues as if setjmp() had returned for a second time.  This "fake"
	return can be distinguished from a true setjmp() call because the "fake" return returns the value provided in val.
	If the programmer mistakenly passes the value 0 in val, the "fake" return will instead return 1.

6. brk()

	Syntax :

		int brk(void *addr);

	brk() change the location of the program break, which defines the end of the process's data segment (i.e., the
	program break is the first location after the end of the uninitialized data segment).  Increasing the program break
	has the effect of allocating memory to the process; decreasing the break deallocates memory.

	brk() sets the end of the data segment to the value specified by addr, when that value is reasonable, the system has
	enough memory, and the process does not exceed its maximum data size


7. mmap

	Syntax:
	      void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);

	mmap() creates a new mapping in the virtual address space of the calling process.  The starting address for the new
	mapping is specified in addr.  The length argument specifies the length of the mapping.

	If addr is NULL, then the kernel chooses the address at which to create the mapping; this is the most portable
	method of creating a new mapping.  If addr is not NULL, then the kernel takes it as a hint about where to place the
	mapping; on Linux, the mapping will be created at a nearby page boundary.  The address of the new mapping is
	returned as the result of the call.

7. munmap

	Syntax:
		int munmap(void *addr, size_t len); 

	The munmap() function shall remove any mappings for those entire pages containing any part of the address space of the process starting at addr and continuing for len bytes. Further references to these pages shall result in the generation of a SIGSEGV signal to the process. If there are no mappings in the specified address range, then munmap() has no effect.


malloc implementation :

Simplistically malloc and free work like this:

malloc provides access to a process's heap. The heap is a construct in the C core library (commonly libc) that allows objects to obtain exclusive access to some space on the process's heap.

Each allocation on the heap is called a heap cell. This typically consists of a header that hold information on the size of the cell as well as a pointer to the next heap cell. This makes a heap effectively a linked list.

When one starts a process, the heap contains a single cell that contains all the heap space assigned on startup. This cell exists on the heap's free list.

When one calls malloc, memory is taken from the large heap cell, which is returned by malloc. The rest is formed into a new heap cell that consists of all the rest of the memory.

When one frees memory, the heap cell is added to the end of the heap's free list. Subsequent mallocs walk the free list looking for a cell of suitable size.

As can be expected the heap can get fragmented and the heap manager may from time to time, try to merge adjacent heap cells.

When there is no memory left on the free list for a desired allocation, malloc calls brk or sbrk which are the system calls requesting more memory pages from the operating system.

Now there are a few modification to optimize heap operations.

For large memory allocations (typically > 512 bytes, the heap manager may go straight to the OS and allocate a full memory page.
The heap may specify a minimum size of allocation to prevent large amounts of fragmentation.
The heap may also divide itself into bins one for small allocations and one for larger allocations to make larger allocations quicker.
There are also clever mechanisms for optimizing multi-threaded heap allocation.

Some precautions while using these functions are:
1) Always check if the call was successful.

2) Type cast the pointer properly.

3) If allocating memory for a string, don't forget to allocate , one extra byte for the null terminator of the string.

4) Don't forget to free the memory!

5) We can also use some tools like valgrind to check for memory leaks.

ANSWER 2. 

Memory Leak : In computer science, a memory leak is a type of resource leak that occurs when a computer program incorrectly manages memory allocations in such a 
			  way that memory which is no longer needed is not released. In object-oriented programming, a memory leak may happen when an object is stored in memory but cannot be accessed by the running code. 

In simple terms, memory leak occurs when programmers create a memory in heap and forget to delete it.
Memory leaks are particularly serious issues for programs like daemons and servers which by definition never terminate.

An example of memory leak : 

	#include <stdlib.h>
	 
	void f()
	{
	   int *ptr = (int *) malloc(sizeof(int));
	 
	   /* Do some work */
	 
	   return; /* Return without freeing ptr. Hence there is a memory leak.*/
	}

To avoid memory leaks, memory allocated on heap should always be freed when no longer needed.

	#include <stdlib.h>;
	 
	void f()
	{
	   int *ptr = (int *) malloc(sizeof(int));
	 
	   /* Do some work */
	 
	   free(ptr); // Since this pointer ptr is released, there is no memory leak.
	   return;
	}

An example of large memory allocation without freeing the memory.

    #include<stdio.h>
    #include<stdlib.h>
    
    int main()
    {
		int *p;
		while(1)
		{
			p = (int *) malloc(1000000000 * sizeof(int));
			if(!p) // Printing output if no memory is available.
			{
				printf("No avilable memory\n");
				break;
			}
			else // If memory is allocated then prompt the user.
			{
				printf("Memory Allocated\n");
			}
		}
		
	}
	
	Output : Initially the memory will be allocated then after some time memory will be exhausted and the program will print "No memory available"
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
		|
		|
		|
		|
		|
		|
		|
		|
		|
		|
		|
		.
		.
		.
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	Memory Allocated
	No avilable memory
  

Ways to avoid Memory leaks in C are : 

   1. Always write “free” just after “malloc”.

   2. Never work with the allocated pointer. Use a copy!
   	  For example consider the following code : 
   	  	int *p_allocated = (int*) malloc ( sizeof(int) * n );
		int *p_copy = p_allocated;
		// do your stuff with p_copy, not with p_allocated!
		// e.g.:
		while (n--) { *p_copy++ = n; }
		...
		free (p_allocated)

	 Thus we are safe from [accidentally] changing the pointer and returning the wrong address over to free. You can freely play with the p_copy pointer, like in the example above.

	3.Be careful when using realloc(). Do not use the same pointer for input & output parameters.

	4. can use tools such as valgrind to check for memory leaks. It will show all the memory that are not freed on termination of the program.
  
  
ANSWER 3.


The following is the code segment which finds out address of stack, heap and code segment.

	#include <stdio.h>
	#include <stdlib.h>

	extern char etext, edata, end;

	int main() {
	    
		printf("The ending address of heap is : %x",sbrk(0));
		printf("First address past:\n");
		printf("Program text (etext)      %10p\n", &etext);
		printf("Initialized data (edata)  %10p\n", &edata);
		printf("Uninitialized data (end)  %10p\n", &end);

	    exit(EXIT_SUCCESS);
	}

Output :

	Terminal 1

		The ending address of heap is :  0x8c8c000
		First address past:
		Program text (etext)       0x80485a8
		Initialized data (edata)   0x804a028
		Uninitialized data (end)   0x804a02c

	Terminal 2

		The ending address of heap is :  0x833a000
		First address past:
		Program text (etext)       0x80485a8
		Initialized data (edata)   0x804a028
		Uninitialized data (end)   0x804a02c

	Termainl 3

		The ending address of heap is :  0x8620000
		First address past:
		Program text (etext)       0x80485a8
		Initialized data (edata)   0x804a028
		Uninitialized data (end)   0x804a02c


Another program:


  #include <stdio.h>
  #include <malloc.h>     /* for definition of ptrdiff_t on GLIBC */
  #include <unistd.h>
  #include <alloca.h>     /* for demonstration only */

  extern void afunc(void);    /* a function for showing stack growth */

  int bss_var;            /* auto init to 0, should be in BSS */
  int data_var = 42;      /* init to nonzero, should be data */

  int main(int argc, char **argv) /* arguments aren't used */
  {
      char *p, *b, *nb;

      printf("Text Locations:\n");
      printf("\tAddress of main: %p\n", main);
      printf("\tAddress of afunc: %p\n", afunc);

      printf("Stack Locations:\n");
      afunc();

      p = (char *) alloca(32);
      if (p != NULL) {
          printf("\tStart of alloca()'ed array: %p\n", p);
          printf("\tEnd of alloca()'ed array: %p\n", p + 31);
      }

      printf("Data Locations:\n");
      printf("\tAddress of data_var: %p\n", & data_var);

      printf("BSS Locations:\n");
      printf("\tAddress of bss_var: %p\n", & bss_var);

      b = sbrk((ptrdiff_t) 32);   /* grow address space */
      nb = sbrk((ptrdiff_t) 0);
      printf("Heap Locations:\n");
      printf("\tInitial end of heap: %p\n", b);
      printf("\tNew end of heap: %p\n", nb);

      b = sbrk((ptrdiff_t) -16);  /* shrink it */
      nb = sbrk((ptrdiff_t) 0);
      printf("\tFinal end of heap: %p\n", nb);
  }

  void afunc(void)
  {
      static int level = 0;       /* recursion level */
      auto int stack_var;        /* automatic variable, on stack */

      if (++level == 3)           /* avoid infinite recursion */
          return;

      printf("\tStack level %d: address of stack_var: %p\n",
              level, & stack_var);
      afunc();                    /* recursive call */
  }

  Output:

  Terminal 1:
  		Text Locations:
		    Address of main: 0x80484cb
		    Address of afunc: 0x8048689
		Stack Locations:
		    Stack level 1: address of stack_var: 0xbfa68cf8
		    Stack level 2: address of stack_var: 0xbfa68cd8
		    Start of alloca()'ed array: 0xbfa68ce0
		    End of alloca()'ed array: 0xbfa68cff
		Data Locations:
		    Address of data_var: 0x804a028
		BSS Locations:
		    Address of bss_var: 0x804a034
		Heap Locations:
		    Initial end of heap: 0x97ac000
		    New end of heap: 0x97ac020
		    Final end of heap: 0x97ac010
  Terminal 2:

  		Text Locations:
		    Address of main: 0x80484cb
		    Address of afunc: 0x8048689
		Stack Locations:
		    Stack level 1: address of stack_var: 0xbfae04f8
		    Stack level 2: address of stack_var: 0xbfae04d8
		    Start of alloca()'ed array: 0xbfae04e0
		    End of alloca()'ed array: 0xbfae04ff
		Data Locations:
		    Address of data_var: 0x804a028
		BSS Locations:
		    Address of bss_var: 0x804a034
		Heap Locations:
		    Initial end of heap: 0x8f8b000
		    New end of heap: 0x8f8b020
		    Final end of heap: 0x8f8b010

  Terminal 3:

		Text Locations:
		    Address of main: 0x80484cb
		    Address of afunc: 0x804866d
		Stack Locations:
		    Stack level 1: address of stack_var: 0xbf881b08
		    Stack level 2: address of stack_var: 0xbf881ae8
		    Start of alloca()'ed array: 0xbf881af0
		    End of alloca()'ed array: 0xbf881b0f
		Data Locations:
		    Address of data_var: 0x804a028
		BSS Locations:
		    Address of bss_var: 0x804a034
		Heap Locations:
		    Initial end of heap: 0x8087000
		    New end of heap: 0x8087020
		    Final end of heap: 0x8087010

Conclusion:

The address of the Program text,Initialized data and Uninitialized data reamins the same while the address of the end of the heap changes when executed from different terminals. Address of the main remains the same while the address of the function afunc changes.


ANSWER 4.

The following is the code with an integer variable x and a pointer to that varibale. It algo prints out a string massage "Hello OS" on the screen.

	#include<stdio.h>

	int main()
	{
	    int x = 5;
	    int *p = &x;
	    printf("Hello OS\n");

	    return    0;                                                                                                                                                                                                
	}

The following is the output we get after disassembling the above program:

Dump of assembler code for function main:
   0x0804846b <+0>:    lea    0x4(%esp),%ecx
   0x0804846f <+4>:    and    $0xfffffff0,%esp
   0x08048472 <+7>:    pushl  -0x4(%ecx)
   0x08048475 <+10>:    push   %ebp
   0x08048476 <+11>:    mov    %esp,%ebp
   0x08048478 <+13>:    push   %ecx
   0x08048479 <+14>:    sub    $0x14,%esp
   0x0804847c <+17>:    mov    %gs:0x14,%eax
   0x08048482 <+23>:    mov    %eax,-0xc(%ebp)
   0x08048485 <+26>:    xor    %eax,%eax
   0x08048487 <+28>:    movl   $0x5,-0x14(%ebp)
   0x0804848e <+35>:    lea    -0x14(%ebp),%eax
   0x08048491 <+38>:    mov    %eax,-0x10(%ebp)
   0x08048494 <+41>:    sub    $0xc,%esp
   0x08048497 <+44>:    push   $0x8048550
   0x0804849c <+49>:    call   0x8048340 <puts@plt>
   0x080484a1 <+54>:    add    $0x10,%esp
   0x080484a4 <+57>:    mov    $0x0,%eax
   0x080484a9 <+62>:    mov    -0xc(%ebp),%edx
   0x080484ac <+65>:    xor    %gs:0x14,%edx
   0x080484b3 <+72>:    je     0x80484ba <main+79>
   0x080484b5 <+74>:    call   0x8048330 <__stack_chk_fail@plt>


	 
