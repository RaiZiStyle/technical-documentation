/*
From https://x.com/raysan5/status/2049451452997742657
Expended to : https://www.computerenhance.com/p/lets-answer-rays-question
Waiting for the part 2 of the video the see the assembly version of the code and why callof add a bunch of more bytes after the requested data. (metadata perhaps ?)
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_STR_SIZE 8

int main() {

    char* str01 = (char*)calloc(MAX_STR_SIZE, sizeof(char));
    char* str02 = (char*)calloc(MAX_STR_SIZE, sizeof(char));

    memcpy(str02, "1234567890ABCDEFGH", MAX_STR_SIZE);

    int cpyLength = snprintf(str01, MAX_STR_SIZE, "%s", str02);

    printf("STR01 [%i]: %s\n", cpyLength, str01);
    printf("STR02 [%i]: %s\n", (int)strlen(str02), str02);

    return 0;
}