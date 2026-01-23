#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "binary_dump.h"

int main(){
    FILE * file;
    struct header h;
    char name[256];
    size_t max = sizeof(name) - 1;
    size_t index = 0;
    int c;

    file = fopen("dummy.bin", "rb");
    if (!file) return 1;

    //Validate header struct
    if (fread(&h, sizeof(h), 1, file) != 1) return 1;
    if (memcmp(h.magic, "BDMP", 4) != 0){printf("KO\n"); return 1;}
    if (h.version != 1){printf("KO\n"); return 1;}

    while (index < max){
        c = fgetc(file);
        if (c == EOF) return 1;
        name[index++] = (char)c;
        if (c == '\0') break;
    }

    if (index == max && name[index-1] != '\0') return 1;
    name[index] = '\0';

    printf("ASCII_NAME is %s\n",name);

    // Remaining bytes -> long remaining
    long current_pos = ftell(file);
    if (current_pos < 0) return 1;
    if (fseek(file, 0, SEEK_END) != 0) return 1;
    long end_pos = ftell(file);
    if (end_pos < 0) return 1;
    if (fseek(file,current_pos, SEEK_SET) != 0) return 1;
    long remaining = end_pos - current_pos;
    if (h.payload_size > remaining) return 1;

    // Validate payload
    unsigned char *payload = malloc(h.payload_size);
    if (!payload) return 1;
    if (fread(payload, 1, h.payload_size, file) != h.payload_size){
        free(payload);
        return 1;
    } 

    // Verify and display payload
    printf("Payload hex: ");
    for(int i = 0; i < h.payload_size; i++){
        printf("%02X ", payload[i]);
    }
    printf("\n");

    free(payload);


}

