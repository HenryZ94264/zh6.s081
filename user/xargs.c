#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

// xargs: transform standard input to command line args

void run(char* command, char** args){
    if(fork() == 0){
        exec(command, args);
        exit(0);
    } 
    return;
}

int main(int argc, char* argv[]){

    char line[1024];
    char* params[MAXARG];
    int args_index = 0;
    int i;

    for(i = 1; i < argc; ++i) {
        params[args_index++] = argv[i];     // 先把右边除了xargs以外的参数输入进来
    }

    char* p = line, *last_p = line;
    int p_index = args_index;  // 开始
    while(read(0, p, 1) != 0){
        if(*p == ' ' || *p == '\n'){
            *p = '\0';
            params[p_index++] = last_p;
            last_p = p+1;
            
            if(*p == '\n'){
                params[p_index] = 0;
                run(argv[1], params);
                p_index = args_index;
            }
        }
        p++;
    }

    if(p_index != args_index){
        *p = 0;
        params[p_index++] = last_p;
        params[p_index] = 0;
        run(argv[1], params);
    }
    while(wait(0) != -1) {};

    exit(0);
}
