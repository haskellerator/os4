#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main()
{
	symlink("/ls","blabla");
	symlink("ls","blabla2");

	char buf[128];
	int n = readlink("/blabla2",buf, 82);

	printf(1,"test2 %d %s\n", n, buf);
	// printf(1,"test2\n");
	exit();
}
