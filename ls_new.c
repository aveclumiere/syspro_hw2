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
  time_t          mtime;
  char            name[250];
};

struct ls_st LS_LIST[2000];
int LS_INDEX = 0;

int cmpstr(const void* a, const void* b);
int cmpls(struct ls_st a, struct ls_st b);

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
    strcpy(ls.name, dir -> d_name);
    LS_LIST[LS_INDEX] = ls;
    LS_INDEX++;
    // printf("%lld\t%ld\t%ld\t%ld\t%lld\t%s\t%s\n",
    //       (long long) ls.mode,
    //       (long) ls.nlink,
    //       (long) ls.uid,
    //       (long) ls.gid,
    //       (long long) ls.size,
    //       ctime(&ls.mtime),
    //       ls.name);
  }
  qsort(LS_LIST, LS_INDEX, sizeof(ls_st), cmpls);
  for(int i = 0; i < LS_INDEX; i++){
    printf("%lld\t%ld\t%ld\t%ld\t%lld\t%s\t%s\n",
          (long long) LS_LIST[i].mode,
          (long) LS_LIST[i].nlink,
          (long) LS_LIST[i].uid,
          (long) LS_LIST[i].gid,
          (long long) LS_LIST[i].size,
          ctime(&LS_LIST[i].mtime),
          LS_LIST[i].name);
  }
  return 0;
}

int cmpls(struct ls_st a, struct ls_st b){
  return cmpstr(a.name, b.name);
}

int cmpstr(const void* a, const void* b){
  const char* aa = *(const char**)a;
  const char* bb = *(const char**)b;
  return strcmp(aa, bb);
}
