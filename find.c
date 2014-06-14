#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"


void print_help() {
	printf(1, "Find\n");
	printf(1, "    This application searches for files, directories and symlinks inside\n");
	printf(1, "    the given path. Command syntax:\n\n");

	printf(1, "    find <path> <options> <predicates>\n\n");

	printf(1, "Options:\n");
	printf(1, "    -follow\n");
	printf(1, "        Dereference symbolic links. If a symbolic link is encountered,\n");
	printf(1, "        apply tests to the target of the link. If a symbolic link\n");
	printf(1, "        points to a directory, then descend into it.\n\n");

	printf(1, "    -help\n");
	printf(1, "        Display this message.\n\n");

	printf(1, "Predicates:\n");
	printf(1, "    -name filename\n");
	printf(1, "        All files named \"filename\" in the given path.\n\n");

	printf(1, "    -size [+/-]n\n");
	printf(1, "        Files of size n (exactly), +n (more than n), -n (less than n).\n\n");

	printf(1, "    -type t\n");
	printf(1, "        Files of type:\n");
	printf(1, "        d        directory\n");
	printf(1, "        f        regular file\n");
	printf(1, "        s        soft (symbolic) link\n");
}

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;
  
  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  
  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), '\0', DIRSIZ-strlen(p));
  return buf;
}


void find(char *path, int follow, int size, int greater, char *name, int type) {
  char buf[128], *p;
  char *temp;
  int fd, display = 1;
  struct dirent de;
  struct stat st;
  
  if((fd = open(path, 0)) < 0){
    printf(2, "find: cannot open %s\n", path);
    return;
  }
  
  if(fstat(fd, &st) < 0){
    printf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
  	// Predicates
  	temp = fmtname(path);
  	if (name && strcmp(name, temp) != 0) {
  		display = 0;
  	}
  	if ((greater > 0 && st.size <= size) ||
  	    (greater < 0 && st.size >= size) ||
  	    (greater == 0 && size >= 0 && st.size != size)) {
  		display = 0;
  	}
  	if (type != 0 && type != T_FILE) {
  		display = 0;
  	}
  	// If predicates passed
 	if (display) {
  		printf(1, "%s\n", path);
  	}

    break;
  
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "find: path too long\n");
      break;
    }

    // Predicates
	temp = fmtname(path);    
  	if (name && strcmp(name, temp) != 0) {
  		display = 0;
  	}
  	if (type != 0 && type != T_DIR) {
  		display = 0;
  	}

  	// Add /
    strcpy(buf, path);
    p = buf+strlen(buf);
    if (*(p-1) != '/') {
    	*p++ = '/';
    }

  	// If predicates passed
  	if (display) {
  		printf(1, "%s\n", path);
  	}

  	// Next
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
		if(de.inum == 0)
			continue;
		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0;
		if(stat2(buf, &st) < 0){
			printf(1, "find: cannot stat %s\n", buf);
			continue;
		}
		if (*p != '.') {
			find(buf, follow, size, greater, name, type); // Recursive call
  		}
    }
    break;

  case T_SYMLINK:
  	printf(1, "Got here, follow is %d\n", follow);
	if (follow) {
		printf(1, "Following\n");
		readlink(path, buf, 128);
		printf(1, "Link contents: %s\n", buf);
		strcpy(path, buf);
		find(path, follow, size, greater, name, type);
	} else {
		temp = fmtname(path);    
	  	if (name && strcmp(name, temp) != 0) {
	  		display = 0;
	  	}
  	  	if ((greater > 0 && st.size <= size) ||
	  	    (greater < 0 && st.size >= size) ||
	  	    (greater == 0 && size >= 0 && st.size != size)) {
	  		display = 0;
	  	}
	  	if (type != 0 && type != T_SYMLINK) {
	  		display = 0;
	  	}  	
  	  	// If predicates passed
 	 	if (display) {
  			printf(1, "%s\n", path);
  		}
  	}
  	break;

  }

  close(fd);
}


int
main(int argc, char *argv[])
{
	int i = 0, follow = 0;
	int size = -1, greater = 0;
	char *path, *name = 0;
	int type = 0;

	if (argc < 2) {
		printf(1, "usage: find <path> <options> <preds>\n");
		exit();
	}

	path = argv[1]; 	// path is a required argument
	i = 2;				// argument index

	if (strcmp(argv[i], "-help") == 0) {	// We've decided that help or follow are exclusive
		print_help();
		exit();
	} else if (strcmp(argv[i], "-follow") == 0) {
		follow = 1;
		i++;
	}

	while (i < argc) {
		if (strcmp(argv[i], "-name") == 0) {
			name = argv[i+1];
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
				type = T_DIR;
			} else if (strcmp(argv[i+1], "f") == 0) {
				type = T_FILE;
			} else if (strcmp(argv[i+1], "s") == 0) {
				type = T_SYMLINK;
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

	find(path, follow, size, greater, name, type);
	
	exit();
}

