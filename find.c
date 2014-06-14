#include "types.h"
#include "stat.h"
#include "user.h"

void print_help() {
	printf(1, "This is the help printout.\n");
}



int
main(int argc, char *argv[])
{
	int i = 0, follow = 0;
	int size = 0, greater = 0;
	char *path, *filename = 0;
	enum { NONE, FILE, DIR, SOFT } type;

	if (argc < 2) {
		printf(1, "usage: find <path> <options> <preds>\n");
		exit();
	}

	path = argv[1];
	i = 2;

	if (strcmp(argv[i], "-help") == 0) {
		print_help();
		exit();
	} else if (strcmp(argv[i], "-follow") == 0) {
		follow = 1;
		i++;
	}

	while (i < argc) {
		if (strcmp(argv[i], "-name") == 0) {
			filename = argv[i+1];
			i += 2;
		} else if (strcmp(argv[i], "-size") == 0) {
			if (argv[i+1][0] == '+') {
				greater = 1;
				size = atoi(argv[i+1]+1);
			} else if (argv[i+1][1] == '-') {
				greater = -1;
				size = atoi(argv[i+1]+1);
			} else {
				size = atoi(argv[i+1]);
			}
			i += 2;
		} else if (strcmp(argv[i], "-type") == 0) {
			if (strcmp(argv[i+1], "d") == 0) {
				type = DIR;
			} else if (strcmp(argv[i+1], "f") == 0) {
				type = FILE;
			} else if (strcmp(argv[i+1], "s") == 0) {
				type = SOFT;
			} else {
				printf(2, "find error: unknown type.\n");
				exit();
			}
			i += 2;
		} else {
			printf(2, "find error: unknown option or predicate.\n");
			exit();
		}
	}

	// Printout
	printf(1, "find: %s\n", path);

	if (follow) {
		printf(1, "Follow detected.\n");
	}

	if (filename) {
		printf(1, "Name: %s\n", filename);
	}

	if (size) {
		printf(1, "Size: ");
		if (greater > 0) {
			printf(1, "+");
		} else if (greater < 0) {
			printf(1, "-");
		}
		printf(1, "%d\n", size);
	}

	if (type) {
		printf(1, "Type: %d\n", type);
	}
	
	exit();
}

