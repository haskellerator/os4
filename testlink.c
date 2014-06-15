#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main()
{
	int n;
	char buf[128];
	/*sets the environment for testing . creates a/b/c 
	 * and a link C -> c*/ 

	mkdir("a");
	mkdir("a/d");
	mkdir("a/d/b");
	if (link("/ls","a/d/b/c") < 0 || symlink("c","a/d/b/C") < 0){
		printf(1,"didnt set test evironment\n");
	}
	
	// n = readlink("a/b/C",buf,128);
	// printf(1,"test2 output: %d buf: %s supposed: a/b/c\n", n, buf);
	// n = readlink("/a/b/C",buf,128);
	// printf(1,"test2 output: %d buf: %s supposed: /a/b/c\n", n, buf);
	
	chdir("a/d/b"); // from a/b
	
	n = readlink("C",buf,128);
	printf(1,"test2 output: %d buf: %s supposed: c\n", n, buf);
	// n = readlink("/a/b/C",buf,128);
	// printf(1,"test2 output: %d buf: %s supposed: /a/b/c\n", n, buf);
	exit();
}
