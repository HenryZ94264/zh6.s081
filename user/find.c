#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// 1. open file and get file descriptor
// 2. fstat: load file stat of fd to st
// 3. check type of st

void find(const char* path, const char* filename){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    
    if(fstat(fd, &st) < 0){     // failed return -1
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_DIR:  // if is a directory
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);  
        p = buf+strlen(buf);
        *p++ = '/';     // p point to the end of buf
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0 || strcmp(de.name, "..") == 0 || strcmp(de.name,".") == 0)
                continue;
            memmove(p, de.name, DIRSIZ);    // copy name to buf
            p[DIRSIZ] = 0;
            
            find(buf, filename);    // recursive
        }
        break;

    // if fd is a file, compare the end of path and filename
    case T_FILE:
        if(strcmp(filename, path + strlen(path) - strlen(filename)) == 0){
            printf("%s\n", path);
        }
        break;
    }
    close(fd);
}


int main(int argc, char* argv[]){

    if(argc != 3){
        fprintf(2, "Usage: find <PATH> <FILENAME> \n");
    }
    else {
        find(argv[1], argv[2]);
    }

    exit(0);
}