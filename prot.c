#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"



int
main(int argc, char *argv[])
{
	if (argc < 4 || (strcmp(argv[1], "p") != 0 && strcmp(argv[1], "u") != 0)) {
		printf(1, "usage: prot [p|u] pathname password\n");
		exit();
	}

	if (strcmp(argv[1], "p") == 0) {
		printf(1, "Locking %s with %s...\n", argv[2], argv[3]);
		fprot(argv[2], argv[3]);
	} else if (strcmp(argv[1], "u") == 0) {
		printf(1, "Unlocking %s with %s...\n", argv[2], argv[3]);
		funprot(argv[2], argv[3]);
	}
	exit();

}

