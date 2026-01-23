#include <stdio.h>
#include <string.h>
#include "binary_dump.h"

int main(){
    FILE * file;
    struct header h;
    char name[256];
    int index = 0;


    file = fopen("dummy.bin", "rb");
    if (!file) return 1;

    if (fread(&h, sizeof(h), 1, file) != 1) return 1;


    if (memcmp(h.magic, "BDMP", 4) != 0){
        printf("KO\n");
        return 1;
    }

    if (h.version != 1){
        printf("KO\n");
        return 1;
    }

    while (1){
        int c = fgetc(file);
        if (c == EOF) return 1;
        name[index++] = (char)c;
        if (c == '\0') break;
    }

    printf("ASCII_NAME is %s\n",name);
}

