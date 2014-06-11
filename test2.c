#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main()
{
	symlink("/ls","blabla");
	readlink("what","the", 82);
	exit();
}
