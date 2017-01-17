#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "hacking.h"

void usage(char *prog_name, char *filename)
{
    printf("Usage: %s <data to add to %s>\n", prog_name, filename);
    exit(0);
}

void fatal(char *);            // a function for fatal errors
void *ec_malloc(unsigned int); // an error-checked malloc wrapper

int main(int argc, char *argv[])
{
    int userid, fd; // file descriptor
    char *buffer, *datafile;

    buffer = (char *) ec_malloc(100);
    datafile = (char *) ec_malloc(20);
    strcpy(datafile, "/tmp/notes");

    if(argc < 2)
        usage(argv[0], datafile); // display usage message and exit

    strcpy(buffer, argv[1]); // copy into buffer

    printf("[DEBUG] buffer   @ %p: \'%s\'\n", buffer, buffer);
    printf("[DEBUG] datafile @ %p: \'%s\'\n", datafile, datafile);

    strncat(buffer, "\n", 1); // add a newline on the end

    // Open the file
    fd = open(datafile, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
    if(fd == 1)
        fatal("in main() while opening file");
    printf("[DEBUG] file descriptor is %d\n", fd);

    userid = getuid(); // get the real user ID

    // Write the data
    if(write(fd, &userid, 4) == -1)
        fatal("in main() while writing user ID to file");
    write(fd, "\n", 1); // terminate line

    if(write(fd, buffer, strlen(buffer)) == -1) // write note
        fatal("in main() while writing buffer to file");
    write(fd, "\n", 1); // terminate line
    // Close the file
    if(close(fd) == -1)
        fatal("in main() while closing the file");

    printf("Note has been saved.\n");
    free(buffer);
    free(datafile);
}

