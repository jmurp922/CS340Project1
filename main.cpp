#include <iostream>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string>
#include <cstring>
#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1
int main() {
    pid_t pid;
    int fd[2];
    int userEntry;
    char read_syscall[BUFFER_SIZE];
    //write_syscall[BUFFER_SIZE];
    std::cout << "What system call do you want to execute?"
    << std::endl << "1 : ls"
    << std::endl << "2 : ps"
    << std::endl << "3 : whoami"
    << std::endl << "4 : hostname"
    << std::endl << "5 : date "
    << std::endl;
    std::cin >> userEntry;
    while (userEntry < 1 || userEntry > 5) {
        std::cout << "Invalid entry, try again :  " << std::endl;
        std:: cin >> userEntry;
    }
    /* Create the pipe for information to be transferred through. */
    if (pipe(fd) < 0) {
        /* Print the Error and Quit if unable to be opened */
        exit(1);
    }
    /* fork a child process */
    pid = fork();
    if (pid < 0) {
        /* ERROR OCCURED, RETURN */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid> 0) {
        /* PARENT PROCESS */
        /* Parent will wait for the child to complete */
        wait(NULL);
        /* Close the write end of the pipe since the parent in this problem reads and does not write. */
        close(fd[WRITE_END]);
        /* Now read what the child wrote to the pipe from the read end of the pipe */
        read(fd[READ_END], read_syscall, BUFFER_SIZE);
        /* Close the read end of the pipe since the information has been extracted*/
        close(fd[READ_END]);
        /* Get Parent Time */
        struct timeval current;
        gettimeofday(&current, NULL);
        long parentSeconds = current.tv_usec;
        std::cout << "The parent executed at : " << parentSeconds << " microseconds." << std::endl;
        long childSeconds;
        /* sscanf is called to turn the character array into a number that is able to be operated on since
          in its current state is not a number and is needed to find the difference between the parent time
          and child time to derive the elapsed time  */
        sscanf(read_syscall, "%d", &childSeconds);
        long elapstedTime = parentSeconds - childSeconds;
        std::cout << "The total elapsed time is : " << elapstedTime << " microseconds." <<std::endl;
        //printf("Parent Complete \n");
    }
    else {
        /* CHILD PROCESS */
        /* Get Child Time */
        struct timeval current;
        gettimeofday(&current, NULL);
        long childSeconds = current.tv_usec;
        /*I'm not really sure about how the write call works, but from the format that was
        given in the textbook, it seems to me that you need to put a char array in because of the strlen function call
        So I looked up what I should do to convert a floating point number into a string and found out that the std library
        supports toString, and then converting that into a character array through the cstring library */
        std::cout << "The child executed at : " << childSeconds << " microseconds." << std::endl;
        std::string tempTime = std::to_string(childSeconds);
        char timeArray[tempTime.length()];
        strcpy(timeArray, tempTime.c_str());
        /* Close the read end of the pipe since this is a unidirectional pipe and the child writes the time */
        close(fd[READ_END]);
        /* Write to the pipe for the parent to read */
        write(fd[WRITE_END], timeArray, strlen(timeArray) + 1);
        /* Now we close the write end of the pipe since we're done writing */
        close(fd[WRITE_END]);
        /* Now execute the user inputted system call after taking the time and making it into a character array */
        if (userEntry == 1) {
            std::cout << "User entered command is : ls" <<std::endl << "The output from the syscall is  : ";
            execlp("ls", "ls", NULL);
           }
           else if (userEntry == 2) {
               std::cout << "User entered command is : ps" <<std::endl << "The output from the syscall is  : ";
               execlp("ps", "ps", NULL);
           }
           else if (userEntry == 3) {
               std::cout << "User entered command is : whoami" <<std::endl << "The output from the syscall is  : ";
               execlp("whoami", "whoami", NULL);
           }
           else if (userEntry == 4) {
             std::cout << "User entered command is : hostname " <<std::endl << "The output from the syscall is  : ";
             execlp("hostname", "hostname", NULL);
           }
           else {
               std::cout << "User entered command is : date \n" <<std::endl << "The output from the syscall is  : ";
               execlp("date", "date", NULL);
           }
        
    }
    return 0;
}
