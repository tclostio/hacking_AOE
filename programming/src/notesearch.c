#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "hacking.h"

#define FILENAME "/tmp/notes"

int print_notes(int, int, char *);
int find_user_note(int, int);
int search_note(char *, char *);
void fatal(char *);

int main(int argc, char *argv[])
{
    int userid, printing=1, fd;
    char searchstring[100];

    if(argc > 1)
        strcpy(searchstring, argv[1]);
    else
        searchstring[0] = 0; // if there is no arg, there is no search

    userid = getuid();
    fd = open(FILENAME, O_RDONLY);
    if(fd == -1)
        fatal("in main() while opening file for reading.");

    while(printing)
        printing = print_notes(fd, userid, searchstring);
    printf("--------[END OF NOTE DATA]--------\n");
    close(fd);
}

// A funtion to print the notes for a given UID that match an optional search string.
int print_notes(int fd, int uid, char *searchstring)
{
    int note_length;
    char byte = 0, note_buffer[100];

    note_length = find_user_note(fd, uid);
    if(note_length == -1)
        return 0; // if the end of the file is reached, return 0

    read(fd, note_buffer, note_length); // read the data
    note_buffer[note_length] = 0; // terminate the string

    if(search_note(note_buffer, searchstring))
        printf(note_buffer);
    return 1;
}

// A function to find the next note for a given UID
int find_user_note(int fd, int user_uid)
{
    int note_uid = -1;
    unsigned char byte;
    int length;

    while(note_uid != user_uid) {
        if(read(fd, &note_uid, 4) != 4)
            return -1;
        if(read(fd, &byte, 1) != 1) // read the newline seperator
            return -1;

        byte = length = 0;
        while(byte != '\n') {
            if(read(fd, &byte, 1) != 1) // read a single byte
                return -1;
            length++;
        }
    }
    lseek(fd, length * -1, SEEK_CUR); // rewind file reading by length bytes

    printf("[DEBUG] found a %d byte note for user id %d\n", length, note_uid);
    return length;
}

// A function to search a note for a given keyword
int search_note(char *note, char *keyword)
{
    int i, keyword_length, match = 0;

    keyword_length = strlen(keyword);
    if(keyword_length == 0) // if there is no search string
        return 1;           // always match

    for(i=0; i < strlen(note); i++) { // iterate over bytes in note
        if(note[i] == keyword[match])
            match++;
        else {
            if(note[i] == keyword[0])
                match = 1;
            else
                match = 0;
        }
        if(match == keyword_length) // if there is a full match
            return 1;
    }
    return 0; // else return 0 for no matches
}


