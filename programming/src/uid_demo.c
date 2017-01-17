/*
 * The purpose of this program is to demonstrate how UIDs change depending on ownership
 */

#include <stdio.h>

int main()
{
    printf("real UID: %d\n", getuid());
    printf("effective UID: %d\n", geteuid());
}
