#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BUF_SIZE 128


void
cat(int fd)
{
  char buf[BUF_SIZE];
  int n;

  while((n = read(fd, buf, sizeof(buf))) > 0)
    write(1, buf, n);
  if(n < 0){
    printf(1, "cat: read error\n");
  }
}


int
main(int argc, char *argv[])
{
	if(argc !=3){
		printf(2,"Usage: test2 pathname password\n");
		exit();
	}

	char * pathname = argv[1];
	char * password = argv[2];
	char buf[BUF_SIZE];
	int pid,unlock, fd, prot,br,unprot;
	
	// locks file with password

	printf(1, "Attempting to protect %s with password %s\n", pathname, password);
	if((prot = fprot(pathname, password)) < 0){
		printf(1,"protection failed: %d\n" ,prot);
		exit();
	} 

	printf(1,"protection successful\n"); 

	if((pid = fork()) == 0){ // child process
		printf(1, "Child attempting to unlock %s\n", pathname);
		// unlocks file
		if((unlock = funlock(pathname,password)) < 0){
			printf(1,"child: unlock failed: %d\n" ,unlock); 
			exit();
		} 
		printf(1,"child: unlock successful\n"); 
		
		// open file to read
		printf(1, "Child attempting to open unlocked file %s\n", pathname);		
		if((fd = open(pathname, O_RDONLY)) < 0){
			printf(1,"child: open failed\n"); 
			exit(); 		
		}	

		printf(1,"child: open successful\nchild: file contents are:\n"); 

		// reads contents.
		while((br = read(fd, buf, BUF_SIZE)) > 0){
			printf(1,"%s",buf); // prints to screen
		}	
			
		printf(1,"\nchild: finished reading\n");
		
		// close file
		if(close(fd) < 0){
			printf(1,"child: close failed\n"); 
			exit(); 		
		}	

		printf(1,"\nchild: closed file\n");
		exit();
		
	} else { // parent process
		wait(); // wait for child

		// open file
		if((fd = open(pathname, O_RDONLY)) >= 0){
			printf(1,"parent: open successful\n"); 
			exit(); 		
		}	

		printf(1,"parent: open failed\nparent: now cat supposed to fail\n"); 

		// read using cat while still protected
		cat(fd); // supposed to fail

		// unprotect
		if((unprot = funprot(pathname,password)) < 0){
			printf(1,"parent: unprot failed: %d\n" ,unprot); 
			exit();
		} 

		printf(1,"parent: unprotect successful\nparent: now cat supposed to succeed\n"); 

		cat(fd); // supposed to succeed
	}
	
	printf(1,"Test Passed\n"); 
	exit();
}

