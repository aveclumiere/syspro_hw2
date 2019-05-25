#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

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
      if(dir -> d_ino == 0){   /* skip remove file */
        continue;
      }

      if(lstat(dir -> d_name, &sb) == -1) {
        perror("lstat");
        exit(-1);
      }

      printf("ID of containing device:  [%lx,%lx]\n",
             (long) major(sb.st_dev), (long) minor(sb.st_dev));

      printf("File type:                ");

      switch (sb.st_mode & S_IFMT) {
      case S_IFBLK:  printf("block device\n");            break;
      case S_IFCHR:  printf("character device\n");        break;
      case S_IFDIR:  printf("directory\n");               break;
      case S_IFIFO:  printf("FIFO/pipe\n");               break;
      case S_IFLNK:  printf("symlink\n");                 break;
      case S_IFREG:  printf("regular file\n");            break;
      case S_IFSOCK: printf("socket\n");                  break;
      default:       printf("unknown?\n");                break;
      }

      printf("File name:                %s\n", dir -> d_name);
      printf("I-node number:            %ld\n", (long) sb.st_ino);

      printf("Mode:                     %lo (octal)\n",
             (unsigned long) sb.st_mode);

      printf("Link count:               %ld\n", (long) sb.st_nlink);
      printf("Ownership:                UID=%ld   GID=%ld\n",
             (long) sb.st_uid, (long) sb.st_gid);

      printf("Preferred I/O block size: %ld bytes\n",
             (long) sb.st_blksize);
      printf("File size:                %lld bytes\n",
             (long long) sb.st_size);
      printf("Blocks allocated:         %lld\n",
             (long long) sb.st_blocks);

      printf("Last status change:       %s", ctime(&sb.st_ctime));
      printf("Last file access:         %s", ctime(&sb.st_atime));
      printf("Last file modification:   %s", ctime(&sb.st_mtime));
      printf("--------------------------\n");

    }

  (void) closedir(dp);

  exit(0);
}
