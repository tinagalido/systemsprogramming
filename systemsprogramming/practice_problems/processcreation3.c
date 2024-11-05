/**
 * The goal is for a parent process to create three child process
 * The parent will print after creating each child
 * When the first child process is created, it will print & sleep for 10 seconds, then print it's exit
 * The second child will do the same but for 5 seconds
 * The third child will also do the same but for 2 seconds
 * Check the output for these 3 conditions:
 * (a) Parent does not wait for the child processes
 * (b) Parent waits for the children using the wait system call
 * (c) Parent waits for the children using the waitpid systsem call, with pid being set to x,y & z
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char* argv[])
{

    
}