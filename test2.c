#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main()
{
	symlink("hahaha","blabla");
	readlink("what","the", 82);
	exit();
}
