#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main()
{
	symlink("/ls","blabla");
	char buf[128];
	int n = readlink("/blabla",buf, 82);

	printf(1,"test2 %d %s\n", n, buf);
	exit();
}
