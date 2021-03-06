
struct file {
  enum { FD_NONE, FD_PIPE, FD_INODE, FD_SYMLINK } type;
  int ref; // reference count
  char readable;
  char writable;
  struct pipe *pipe;
  struct inode *ip;
  uint off;
};


// in-memory copy of an inode
struct inode {
  uint dev;           // Device number
  uint inum;          // Inode number
  int ref;            // Reference count
  int flags;          // I_BUSY, I_VALID

  short type;         // copy of disk inode
  short major;
  short minor;
  short nlink;
  uint size;

  /* task1 changes, added another pointer entry for the
   * second level indirection. from +1 to +2
   */
  uint addrs[NDIRECT+2];  // Data block addresses

  char password[10];  // inode password, task 2
};

#define I_BUSY 0x1
#define I_VALID 0x2

// Task 2
#define I_PROT 0x4

// table mapping major device number to
// device functions
struct devsw {
  int (*read)(struct inode*, char*, int);
  int (*write)(struct inode*, char*, int);
};

extern struct devsw devsw[];

#define CONSOLE 1
