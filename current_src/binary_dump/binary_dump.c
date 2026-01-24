#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "binary_dump.h"

int main(){
    FILE * file;
    struct header h;
    size_t MAX_PAYLOAD_SIZE = 1024 * 1024; // 1 MB

    file = fopen("dummy.bin", "rb");
    if (!file) return 2;

    //Validate header struct
    if (fread(&h, sizeof(h), 1, file) != 1) return 3;
    if (memcmp(h.magic, "BDMP", 4) != 0){printf("KO\n"); return 3;}
    if (h.version != 1){printf("KO\n"); return 3;}

    uint8_t ascii_buff[65];
    memcpy(ascii_buff, h.ascii, 64);
    ascii_buff[64] = '\0';

    printf("ASCII_file_NAME is %s\n",ascii_buff);

    // Remaining bytes -> long remaining
    long current_pos = ftell(file);
    if (current_pos < 0) return 6;
    if (fseek(file, 0, SEEK_END) != 0) return 6;
    long end_pos = ftell(file);
    if (end_pos < 0) return 6;
    if (fseek(file,current_pos, SEEK_SET) != 0) return 6;
    long remaining = end_pos - current_pos;
    if (h.payload_size > remaining) return 1;


    unsigned char *payload;
    // Validate payload
    if (h.payload_size <= MAX_PAYLOAD_SIZE && h.payload_size <= (size_t)remaining){
        payload = malloc(h.payload_size);
        if (!payload) return 1;

    } else {
        return 7;
    }

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

