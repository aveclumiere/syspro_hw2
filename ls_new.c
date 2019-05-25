#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ls_st {
  mode_t          mode;
  nlink_t         nlink;
  uid_t           uid;
  gid_t           gid;
  off_t           size;
  struct timespec mtime;
  char            name[250];
};


int main(int argc, char* argv[]){
  DIR* dp;
  char* path = ".";
  struct dirent* dir;
  struct stat sb;

  if((dp = opendir(path)) == NULL){
      perror("Can't open directory");
      exit(1);
  }

  while((dir = readdir(dp)) != NULL){
    if(dir -> d_ino == 0){ continue; }

    if(lstat(dir -> d_name, &sb) == -1){
      perror("lstat");
      exit(-1);
    }

    struct ls_st ls;
    ls.mode = sb.st_mode;
    ls.nlink = sb.st_nlink;
    ls.uid = sb.st_uid;
    ls.gid = sb.st_gid;
    ls.size = sb.st_size;
    ls.mtime = sb.st_mtime;
    strcpy(ls.name, dir.d_name);
  }
}
