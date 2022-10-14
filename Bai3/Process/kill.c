#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) 
{    
    pid_t child_pid;    
    int status;
    pid_t wait_result;
    child_pid = fork();
    // In child process
    if (child_pid == 0) 
    { 
        printf ("I am a child and my pid = %d\n", getpid());
        sleep(1000);
    }
    //In parent process
    else if (child_pid > 0) 
    {
        printf ("I am the parent and my pid = %d\n", getpid());
        printf ("My child has pid = %d\n", child_pid);
        kill(child_pid, SIGKILL);
        printf ("Killing child process completed.\n");
    }
    //Failed
    else 
    {
        printf ("The fork system call failed to create a new process\n");
        exit(1);
    }
    return 0;
}