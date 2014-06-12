#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
	if(argc !=3){
		printf(2,"Usage: test2 pathname password\n");
	}

	char * pathname = argv[1];
	char * password = argv[2];

	int prot = fprot(pathname, password); // locks file with password
	printf(1,"prot result: %d\n" ,prot); 
	int pid;

	if((pid = fork()) == 0){ // child process

	}
   
	int unprot = funprot("tal","zelig");    
	int unlock = funlock("bulbul","akabulbul");
	printf(1,"prot %d, unprot %d, unlock %d\n",prot,unprot,unlock);
	exit();
}

