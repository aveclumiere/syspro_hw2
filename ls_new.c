#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
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

int main(int argc, char* argv[]){
  DIR* dp;
  char* path = ".";
  struct dirent* dir;
  struct stat sb;

  struct group* grp;
  struct passwd* pwd;

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
  }
  qsort(LS_LIST, LS_INDEX, sizeof(struct ls_st), cmpstr);
  for(int i = 0; i < LS_INDEX; i++){
    if (LS_LIST[i].name[0] == '.'){ continue; }
    grp = getgrgid(LS_LIST[i].gid);
    pwd = getpwuid(LS_LIST[i].uid);
    char* t = ctime(&LS_LIST[i].mtime);
    if (t[strlen(t) - 1] == '\n') t[strlen(t) - 1] = '\0';
    printf((S_ISDIR(LS_LIST[i].mode)) ? "d" : "-");
    printf((LS_LIST[i].mode & S_IRUSR) ? "r" : "-");
    printf((LS_LIST[i].mode & S_IWUSR) ? "w" : "-");
    printf((LS_LIST[i].mode & S_IXUSR) ? "x" : "-");
    printf((LS_LIST[i].mode & S_IRGRP) ? "r" : "-");
    printf((LS_LIST[i].mode & S_IWGRP) ? "w" : "-");
    printf((LS_LIST[i].mode & S_IXGRP) ? "x" : "-");
    printf((LS_LIST[i].mode & S_IROTH) ? "r" : "-");
    printf((LS_LIST[i].mode & S_IWOTH) ? "w" : "-");
    printf((LS_LIST[i].mode & S_IXOTH) ? "x" : "-");
    printf("\t");
    printf("%ld\t%s\t%s\t%lld\t%s\t%s\n",
          (long) LS_LIST[i].nlink,
          pwd -> pw_name,
          grp -> gr_name,
          (long long) LS_LIST[i].size,
          t,
          LS_LIST[i].name);
  }
  return 0;
}


int cmpstr(const void* a, const void* b){
  const struct ls_st *ls_a = a;
  const struct ls_st *ls_b = b;
  return strcmp(ls_a -> name, ls_b -> name);
}
