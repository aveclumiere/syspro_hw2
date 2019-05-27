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


void ls_recursive(char* path);
int cmpstr(const void* a, const void* b);

int main(int argc, char* argv[]){
  char* initial_path = ".";
  ls_recursive(initial_path);
  return 0;
}

void ls_recursive(char* path){
  DIR* dp;
  struct dirent* dir;
  struct stat sb;

  struct group* grp;
  struct passwd* pwd;

  struct ls_st ls_list[2000];
  int ls_index = 0;

  char dir_list[2000][250];
  int dir_index = 0;

  int blk_cnt = 0;

  if((dp = opendir(path)) == NULL){
      perror("Can't open directory");
      exit(1);
  }

  while((dir = readdir(dp)) != NULL){
    char* f_name = malloc(strlen(dir -> d_name) + strlen(path) + 2);
    strcpy(f_name, path);
    strcat(f_name, "/");
    strcat(f_name, dir -> d_name);

    if(dir -> d_ino == 0){ continue; }

    if(lstat(f_name, &sb) == -1){
      perror("lstat");
      exit(-1);
    }
    free(f_name);

    struct ls_st ls;
    ls.mode = sb.st_mode;
    ls.nlink = sb.st_nlink;
    ls.uid = sb.st_uid;
    ls.gid = sb.st_gid;
    ls.size = sb.st_size;
    ls.mtime = sb.st_mtime;
    strcpy(ls.name, dir -> d_name);
    ls_list[ls_index] = ls;
    ls_index++;

    if (ls.name[0] != '.'){
      blk_cnt += ((int) ((double) ls.size / 4096.0) * 4);
      if(((int) ls.size % 4096) != 0){ blk_cnt += 4; }
    }
  }

  qsort(ls_list, ls_index, sizeof(struct ls_st), cmpstr);

  printf("%s:\n", path);
  printf("total %d\n", blk_cnt);
  int i;
  for(i = 0; i < ls_index; i++){
    if (ls_list[i].name[0] == '.'){ continue; }
    if (S_ISDIR(ls_list[i].mode)){ strcpy(dir_list[dir_index++], ls_list[i].name); }

    grp = getgrgid(ls_list[i].gid);
    pwd = getpwuid(ls_list[i].uid);

    char* t = ctime(&ls_list[i].mtime);
    if (t[strlen(t) - 1] == '\n') t[strlen(t) - 1] = '\0';

    printf((S_ISDIR(ls_list[i].mode)) ? "d" : "-");
    printf((ls_list[i].mode & S_IRUSR) ? "r" : "-");
    printf((ls_list[i].mode & S_IWUSR) ? "w" : "-");
    printf((ls_list[i].mode & S_IXUSR) ? "x" : "-");
    printf((ls_list[i].mode & S_IRGRP) ? "r" : "-");
    printf((ls_list[i].mode & S_IWGRP) ? "w" : "-");
    printf((ls_list[i].mode & S_IXGRP) ? "x" : "-");
    printf((ls_list[i].mode & S_IROTH) ? "r" : "-");
    printf((ls_list[i].mode & S_IWOTH) ? "w" : "-");
    printf((ls_list[i].mode & S_IXOTH) ? "x" : "-");
    printf("\t");

    printf("%ld\t%s\t%s\t%lld\t%s\t%s\n",
          (long) ls_list[i].nlink,
          pwd -> pw_name,
          grp -> gr_name,
          (long long) ls_list[i].size,
          t,
          ls_list[i].name);
  }
  printf("\n");

  if(dir_index != 0){
    int i;
    for(i = 0; i < dir_index; i++){
      char* recursive_path = malloc(strlen(path) + strlen(dir_list[i]) + 2);
      strcpy(recursive_path, path);
      strcat(recursive_path, "/");
      strcat(recursive_path, dir_list[i]);
      ls_recursive(recursive_path);
      free(recursive_path);
    }
  }
}

int cmpstr(const void* a, const void* b){
  const struct ls_st *ls_a = a;
  const struct ls_st *ls_b = b;
  return strcmp(ls_a -> name, ls_b -> name);
}
