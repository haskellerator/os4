#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"



int
main(int argc, char *argv[])
{
	
	printf(1, "Locking mkdir...\n");
	fprot("mkdir", "abc");
	
	exit();

}

