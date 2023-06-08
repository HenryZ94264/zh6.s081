#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1

void prime(int* );

int main(int argc, char* argv[]){

    int p[2];
    pipe(p);

    // child process
    if(fork() == 0) {
        close(p[WR]);
        prime(p);
        close(p[RD]);
        exit(0);
    } else {    // parent process
        close(p[RD]);
        for(int i = 2; i <= 35; ++i){
            write(p[WR], &i, sizeof(int));
        }
        close(p[WR]);   // 关闭写端告知后面进程没有内容可写
    }
    wait(0);
    exit(0);
}

void prime(int* left_pipe){
    // read from left
    // close(left_pipe[WR]);
    int p;
    int tmp = read(left_pipe[RD], &p, sizeof(int));
    if(tmp == 0){  // 函数出口，表示已经读不出数据
        close(left_pipe[RD]);
        exit(0);
    }
    fprintf(1, "prime %d\n", p);
    

    // create new pipe
    int right_pipe[2];
    pipe(right_pipe);
    int n;

    // subprocess
    if(fork() > 0){
        close(right_pipe[WR]);
        prime(right_pipe);
        close(right_pipe[RD]);
        exit(0);
    } else {    // parent process
        close(right_pipe[RD]);
        while(read(left_pipe[RD], &n, sizeof(int)) == sizeof(int)){
            if(n % p != 0){
                write(right_pipe[WR], &n, sizeof(int));
            }
        }
        close(right_pipe[WR]);  // 关闭写端，停止读端阻塞
        wait(0);    // wait等待返回
    }

    exit(0);
}