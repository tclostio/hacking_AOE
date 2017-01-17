#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *errorchecked_malloc(unsigned int); // function prototype

int main(int argc, char *argv[])
{
    char *char_ptr;
    int *int_ptr;
    int mem_size;

    if (argc < 2)
        mem_size = 50;
    else
        mem_size = atoi(argv[1]);

    printf("\t[+] Allocating %d bytes of memory on the head for char_ptr\n", mem_size);
    char_ptr = (char *) errorchecked_malloc(mem_size); // allocating heap memory

    strcpy(char_ptr, "This memory is located on the heap.");
    printf("char_ptr (%p) --> '%s'\n", char_ptr, char_ptr);

    printf("\t[+] Allocating 12 bytes of memory on the heap for int_ptr\n");
    int_ptr = (int *) errorchecked_malloc(12); // allocating heap memory again

    *int_ptr = 31337; // put this value where int_ptr is pointing
    printf("int_ptr (%p) --> %d\n", int_ptr, *int_ptr);

    printf("\t[-] Freeing char_ptr's heap memory...\n");
    free(char_ptr); // free heap memory

    printf("\t[+] Allocating another 15 bytes for char_ptr\n");
    char_ptr = (char *) errorchecked_malloc(15); // allocate some more heap memory

    strcpy(char_ptr, "new memory");
    printf("char_ptr (%p) --> '%s'\n", char_ptr, char_ptr);

    printf("\t[-] Freeing int_ptr's heap memory...\n");
    free(int_ptr);
    printf("\t[-] Freeing char_ptr's heap memory...\n");
    free(char_ptr);
}

void *errorchecked_malloc(unsigned int size)
{
    void *ptr;
    ptr = malloc(size);
    if(ptr == NULL) {
        fprintf(stderr, "ERROR: Could not allocate heap memory.\n");
        exit(-1);
    }
    return ptr;
}
