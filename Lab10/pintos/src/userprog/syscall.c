#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <user/syscall.h>
#include "devices/input.h"
#include "devices/shutdown.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/interrupt.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"

/*struct lock defined in synch.h
	struct lock 
	  {
	    struct thread *holder;       Thread holding lock (for debugging). 
	    struct semaphore semaphore;  Binary semaphore controlling access. 
	  };
*/

struct lock filesys_lock;

static void syscall_handler (struct intr_frame *);

struct process_file {
  struct file *file;
  int fd;
  struct list_elem elem;
};

/*
   Registers internal interrupt VEC_NO to invoke HANDLER, which
   is named NAME for debugging purposes.  The interrupt handler
   will be invoked with interrupt status LEVEL.
   It is defined in threads/interrrupt.c
*/
void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

/*
   It is used to extract arguments from the stack. These arguments are used in the system calls.
   The arguments are stored in arg array.
*/
void get_arg (struct intr_frame *f, int *arg, int n)
{
  int i;
  int *ptr;
  for (i = 0; i < n; i++)
    {
      ptr = (int *) f->esp + i + 1;
      arg[i] = *ptr;
    }
}


int process_add_file (struct file *f)
{
  struct process_file *pf = malloc(sizeof(struct process_file));
  pf->file = f;
  pf->fd = thread_current()->fd;
  thread_current()->fd++;
  list_push_back(&thread_current()->file_list, &pf->elem);
  return pf->fd;
}

struct file* process_get_file (int fd)
{
  struct thread *t = thread_current();
  struct list_elem *e;

  for (e = list_begin (&t->file_list); e != list_end (&t->file_list);e = list_next (e))
        {
          struct process_file *pf = list_entry (e, struct process_file, elem);
          if (fd == pf->fd)
	    {
	      return pf->file;
	    }
        }
  return NULL;
}

/*
   Terminates Pintos by calling shutdown_power_off() (declared in 'devices/shutdown.h').
*/
void halt (void)
{
  printf("Reached\n");
  shutdown_power_off();
}

/*
   Terminates the current user program, returning status to the kernel. Conventionally, a status of 0 indicates
   success and nonzero values indicate errors.
*/
void exit (int status)
{
  struct thread *cur = thread_current();
  if (thread_alive(cur->parent))
    {
      cur->cp->status = status;
    }
  printf ("%s: exit(%d)\n", cur->name, status);
  thread_exit();
}


int exec (const char *cmd_line) {
  tid_t tid = process_execute (cmd_line);
  if (tid == TID_ERROR)
    return -1;
  return tid;
}

/*
   Creates a new file called file initially initial size bytes in size. Returns true if successful, false otherwise.
*/
bool create (const char *file, unsigned initial_size)
{
  bool success = filesys_create(file, initial_size);//filesys_create defined in filesys.c
  if(success == 1)
  {
     printf("File has been created\n");
  }
  return success;
}


/*
   Deletes the file called 'file'. Returns true if successful, false otherwise.
*/
bool remove (const char *file)
{
  bool success = filesys_remove(file);//filesys_remove defined in filesys.c
  if(success == 1)
  {
     printf("The file has been removed\n");
  }
  return success;
}

/*
   Opens the file called 'file'. Returns a nonnegative integer handle called a "file descriptor" (fd), or -1 if the file
   could not be opened. 
*/
int open (const char *file)
{
  struct file *f = filesys_open(file);//filesys_open defined in filesys.c
  if (!f)
    {
      printf("No such file exists.\n");
      return;
    }
  int fd = process_add_file(f);//process_add_file is defined above in this file
  return fd;
}

/*
   Returns the size, in bytes, of the file open as fd.
*/
int filesize (int fd)
{
  struct file *f = process_get_file(fd);
  if (!f)
    {
      return;
    }
  int size = file_length(f); // file _length defined in file.c
  return size;
}

/*
  Reads size bytes from the file open as fd into buffer. Returns the number of bytes actually read (0 at end of
  file), or - 1 if the file could not be read (due to a condition other than end of file). fd 0 reads from the keyboard
  using input_getc().
*/
int read (int fd, void *buffer, unsigned size)
{
  if (fd == STDIN_FILENO) // STD_FILENO is declared in stdio.h
    {
      unsigned i;
      uint8_t* local_buffer = (uint8_t *) buffer;
      for (i = 0; i < size; i++)
	{
	  local_buffer[i] = input_getc();
	}
      return size;
    }

  struct file *f = process_get_file(fd);
  if (!f)
    {
      return;
    }
  int bytes = file_read(f, buffer, size);//file_read is defined in file.c in filesys directory.
  return bytes;
}

/*
   Writes size bytes from buffer to the open file fd. Returns the number of bytes actually written, which may be
   less than size if some bytes could not be written.
*/
int write (int fd, const void *buffer, unsigned size)
{
  if (fd == STDOUT_FILENO)
    {
      putbuf(buffer, size);
      return size;
    }
  struct file *f = process_get_file(fd);
  if (!f)
    {
      return;
    }
  int bytes = file_write(f, buffer, size);//file_write is defined in file.c in filesys directory.
  return bytes;
}

/*
   Changes the next byte to be read or written in open file fd to position, expressed in bytes from the beginning
   of the file. (Thus, a position of 0 is the file’s start.)
*/
void seek (int fd, unsigned position)
{
  struct file *f = process_get_file(fd);
  if (!f)
    {
      return;
    }
  file_seek(f, position); //file_seek is defined in file.c in filesys directory.
}

/*
   Returns the position of the next byte to be read or written in open file fd, expressed in bytes from the
   beginning of the file.
*/
unsigned tell (int fd)
{
  struct file *f = process_get_file(fd);
  if (!f)
    {
      return;
    }
  off_t offset = file_tell(f); //file_tell is defined in file.c in filesys directory.
  return offset;
}

/*
   Closes file descriptor fd. Exiting or terminating a process implicitly closes all its open file descriptors.
*/

void close (int fd)
{
  process_close_file(fd);
}

/*
   If process pid is still alive, waits until it dies. It returns the status that pid passed to exit, or -1 if pid was
   terminated by the kernel (e.g. killed due to an exception).
*/
int wait(pid_t pid)
{
   return process_wait(pid);//defined in userprog/process.c
}


static void
syscall_handler (struct intr_frame *f)// UNUSED) 
{
  int arg[5];
  switch (* (int *) f->esp)
    {
	    case SYS_HALT:
	      {
		halt(); 
		break;
	      }
	    case SYS_EXIT:
	      {
		get_arg(f, &arg[0], 1);
		exit(arg[0]);
		break;
	      }
	   case SYS_EXEC:
	      {
		get_arg(f, &arg[0], 1);
		f->eax = exec((const char *) arg[0]); 
		break;
	      }
	    case SYS_WAIT:
	      {
		get_arg(f, &arg[0], 1);
		f->eax = wait(arg[0]);
		break;
	      }
	    case SYS_CREATE:
	      {
		get_arg(f, &arg[0], 2);
		f->eax = create((const char *)arg[0], (unsigned) arg[1]);
		break;
	      }
	    case SYS_REMOVE:
	      {
		get_arg(f, &arg[0], 1);
		f->eax = remove((const char *) arg[0]);
		break;
	      }
	    case SYS_OPEN:
	      {
		get_arg(f, &arg[0], 1);
		f->eax = open((const char *) arg[0]);
		break; 		
	     }
	    case SYS_FILESIZE:
	      {
		get_arg(f, &arg[0], 1);
		f->eax = filesize(arg[0]);
		break;
	      }
	   case SYS_READ:
	      {
		get_arg(f, &arg[0], 3);
		f->eax = read(arg[0], (void *) arg[1], (unsigned) arg[2]);
		break;
	     }
	   case SYS_WRITE:
	      { 
		get_arg(f, &arg[0], 3);
		f->eax = write(arg[0], (const void *) arg[1],(unsigned) arg[2]);
		break;
	     }
	    case SYS_SEEK:
	      {
		get_arg(f, &arg[0], 2);
		seek(arg[0], (unsigned) arg[1]);
		break;
	      } 
	    case SYS_TELL:
	      { 
		get_arg(f, &arg[0], 1);
		f->eax = tell(arg[0]);
		break;
	      }
	    case SYS_CLOSE:
	      { 
		get_arg(f, &arg[0], 1);
		close(arg[0]);
		break;
	      }
    }
  thread_exit ();
}



void process_close_file (int fd)
{
  struct thread *t = thread_current();
  struct list_elem *next, *e = list_begin(&t->file_list);

  while (e != list_end (&t->file_list))
    {
      next = list_next(e);
      struct process_file *pf = list_entry (e, struct process_file, elem);
      if (fd == pf->fd || fd == -1)
	{
	  file_close(pf->file);//defined in filesys/sile.c
	  list_remove(&pf->elem);
	  free(pf);
	  if (fd != -1)
	    {
	      return;
	    }
	}
      e = next;
    }
}

struct child_process* add_child_process (int pid)
{
  struct child_process* cp = malloc(sizeof(struct child_process));
  cp->pid = pid;
  cp->load = NOT_LOADED;
  cp->wait = false;
  cp->exit = false;
  lock_init(&cp->wait_lock);
  list_push_back(&thread_current()->child_list,&cp->elem);
  return cp;
}


