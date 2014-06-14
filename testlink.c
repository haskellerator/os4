#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main()
{
	symlink("/blabla2","blabla");
	symlink("/blabla3","blabla2");
	symlink("/blabla2","blabla3");

	char buf[128];
	int n = readlink("/blabla2",buf, 82);

	printf(1,"test2 %d %s\n", n, buf);
	// printf(1,"test2\n");
	exit();
}
