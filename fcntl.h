#define O_RDONLY  0x000
#define O_WRONLY  0x001
#define O_RDWR    0x002
#define O_NOREF	  0x004 // task1 addition, when flag is off, we dereference the last link in path.
#define O_CREATE  0x200
