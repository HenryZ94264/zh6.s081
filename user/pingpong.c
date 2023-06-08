#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Write a program that uses UNIX system calls to ''ping-pong'' 
// a byte between two processes over a pair of pipes, one for each direction. 
// The parent should send a byte to the child; 
// the child should print "<pid>: received ping", 
// where <pid> is its process ID, write the byte on the pipe to the parent, and exit; 
// the parent should read the byte from the child, print "<pid>: received pong", and exit. 
// Your solution should be in the file user/pingpong.c.

#define RD 0 // read end of pipe
#define WR 1 // write end of pipe

int main(int argc, char* argv[]){

    if(argc != 1){
        fprintf(2, "Usage ping-pong...");
        exit(1);
    }

    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);
    char buf = '1';
    int pid = fork();


    // child process
    if(pid == 0) {
        close(p1[WR]);
        close(p2[RD]);

        char* buf;
        if(read(p1[RD], &buf, sizeof(char)) != sizeof(char)){
            fprintf(2, "child read error\n");
            close(p1[RD]);
            close(p2[WR]);
            exit(1);
        } else {
            fprintf(1, "%d: received ping\n", getpid());
        }

        // send pong to parent 
        if(write(p2[WR], &buf, sizeof(buf)) != sizeof(buf)){
            fprintf(2, "child write error\n");
            close(p1[RD]);
            close(p2[WR]);
            exit(1);
        } 
        close(p1[RD]);
        close(p2[WR]);
    } else {    // parent process
        // send ping to child process
        close(p1[RD]);
        close(p2[WR]);
        if(write(p1[WR], &buf, sizeof(char)) != sizeof(char)){
            fprintf(2, "parent write error\n");
            close(p1[WR]);
            close(p2[RD]);
            exit(1);
        } 
        if(read(p2[RD], &buf, sizeof(char)) != sizeof(char)){
            fprintf(2, "parent read error\n");
            close(p1[WR]);
            close(p2[RD]);
            exit(1);
        } else {
            fprintf(1, "%d: received pong\n", getpid());
        }
        close(p1[WR]);
        close(p2[RD]);
    }

    exit(0);
}