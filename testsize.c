#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main()
{
  char buf[512];
  int fd, i, sectors;

  fd = open("output1", O_CREATE | O_WRONLY);
  if(fd < 0){
    printf(2, "test1: cannot open output1 for writing\n");
    exit();
  }

  sectors = 0;
  while(1){  
    *(int*)buf = sectors;
    int cc = write(fd, buf, sizeof(buf));
    if(cc <= 0)
      break;
    sectors++;
	  if (sectors == 12)
		  printf(1, "Finished writing 6KB (direct)\n");
    if (sectors == 140)
      printf(1, "Finished writing 70KB (single indirect)\n");
    if (sectors == 2000)
      printf(1, "Finished writing 1MB\n");
  }

  printf(1, "Finished writing 8MB\n");
  close(fd);
  fd = open("output1", O_RDONLY);
  if(fd < 0){
    printf(2, "test1: cannot re-open output1 for reading\n");
    exit();
  }
  for(i = 0; i < sectors; i++){
    int cc = read(fd, buf, sizeof(buf));
    if(cc <= 0){
      printf(2, "test1: read error at block %d\n", i);
      exit();
    }
    if(*(int*)buf != i){
      printf(2, "big: read the wrong data (%d) for sector %d\n",
             *(int*)buf, i);
      exit();
    }
  }

  exit();
}
