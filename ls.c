#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char* argv[]){
  DIR *dp;
  struct dirent *dir;
  char *path = ".";

  if((dp = opendir(path)) == NULL){
    perror("Can't open directory");
    exit(1);
  }

  while((dir = readdir(dp)) != NULL){
    if(dir -> d_ino == 0){   /* skip remove file */
      continue;
    }
    (void) printf("%llu\t%lld\t%d\t%hhu\t%s\n", dir -> d_ino, (int long long)dir -> d_off, dir -> d_reclen, dir -> d_type, dir -> d_name);
  }

  (void) closedir(dp);

  exit(0);
}
