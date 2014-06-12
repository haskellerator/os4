#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BUF_SIZE 128

char buf2[512];

void
cat(int fd)
{
  int n;

  while((n = read(fd, buf2, sizeof(buf2))) > 0)
    write(1, buf2, n);
  if(n < 0){
    printf(1, "cat: read error\n");
    exit();
  }
}


int
main(int argc, char *argv[])
{
	if(argc !=3){
		printf(2,"Usage: test2 pathname password\n");
	}

	char * pathname = argv[1];
	char * password = argv[2];
	char buf[BUF_SIZE];
	int pid,unlock, fd, prot,br,unprot;
	
	// locks file with password
	if((prot = fprot(pathname, password)) < 0){
		printf(1,"protection failed: %d\n" ,prot);
		exit();
	} 

	printf(1,"protection successful\n"); 

	if((pid = fork()) == 0){ // child process
		
		// unlocks file
		if((unlock = funlock(pathname,password)) < 0){
			printf(1,"child: unlock failed: %d\n" ,unlock); 
			exit();
		} 

		printf(1,"child: unlock successful\n"); 
		
		// open file to read
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
		if((fd = open(pathname, O_RDONLY)) < 0){
			printf(1,"parent: open failed\n");  // if failed, print failed to open
			exit(); 		
		}	
		printf(1,"parent: open successful\nparent: now cat supposed to fail\n"); 



		// read using cat while still protected
		cat(fd); // supposed to fail

		// unprotect
		if((unprot = funprot(pathname,password)) < 0){
			printf(1,"parent: unprot failed: %d\n" ,unprot); 
			exit();
		} 

		printf(1,"parent: unprotect successful\nparent: now cat supposed to succeed"); 

		cat(fd); // supposed to succeed
	}
	
	printf(1,"Test ended"); 
	exit();
}

