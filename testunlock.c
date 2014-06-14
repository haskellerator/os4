#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"



int
main(int argc, char *argv[])
{
	char *pathname, *password;
	if (argc < 3) {
		printf(1, "usage: testunlock pathname password\n");
		exit();
	}
	pathname = argv[1];
	password = argv[2];


	printf(1, "Attempting to protect %s with password %s\n", pathname, password);
	if (fprot(pathname, password) < 0) {
		printf(1, "fprot failed\n");
		exit();
	} else {
		printf(1, "fprot succeeded\n");
	}


	printf(1, "Attempting to open %s. This should fail.\n", pathname);
	if (open(pathname, O_RDONLY) < 0) {
		printf(1, "open failed as expected\n");
	} else {
		printf(1, "open succeeded while file is protected\n");
		exit();
	}

	funlock(pathname, password);
	printf(1, "Attempting to open %s after unlocking. This should succeed.\n", pathname);
	if (open(pathname, O_RDONLY) < 0) {
		printf(1, "open failed after unlocking\n");
		exit();
	} else {
		printf(1, "open succeeded\n");
	}

	exit();

}

