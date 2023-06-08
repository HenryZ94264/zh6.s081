#include "kernel/types.h"
#include "kernel/syscall.h"
#include "user/user.h"

// If the user forgets to pass an argument, sleep should print an error message.
// The command-line argument is passed as a string; you can convert it to an integer using atoi (see user/ulib.c).
// Use the system call sleep.
// See kernel/sysproc.c for the xv6 kernel code that implements the sleep system call (look for sys_sleep), user/user.h for the C definition of sleep callable from a user program, and user/usys.S for the assembler code that jumps from user code into the kernel for sleep.
// Make sure main calls exit() in order to exit your program.

int main(int argc, char* argv[]){
    
    if(argc < 1 || argc > 2){
        fprintf(2, "Usage: sleep... \n");
        exit(1);
    }

    int tick = atoi(argv[1]);
    sleep(tick);

    exit(0);

}