#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define ERR_FILE_OPEN           2
#define ERR_HEADER_TRUNC        23
#define ERR_HEADER_INVALID      3
#define ERR_PAYLOAD_TRUNC       1
#define ERR_PAYLOAD_TOO_BIG     7
#define ERR_INVALID_FILE        14

struct header {
    uint8_t magic[4];
    uint8_t version;
    uint8_t flags[1];
    uint16_t header_size;
    uint32_t payload_size;
    uint8_t ascii[64];
};

long get_file_size(const char *file_name){
    FILE *f = fopen(file_name, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fclose(f);
    return size;
}

int main(){
    FILE * file;
    struct header h;
    size_t MAX_PAYLOAD_SIZE = 1024 * 1024; // 1 MB
    long file_size = get_file_size("dummy.bin");
    int offset = 0;

    file = fopen("dummy.bin", "rb");
    if (!file) return ERR_FILE_OPEN;

    if (offset + sizeof(h) > file_size) return ERR_HEADER_TRUNC;
    if (fread(&h, sizeof(h), 1, file) != 1) return ERR_HEADER_INVALID;
    if (memcmp(h.magic, "BDMP", 4) != 0){printf("KO\n"); return ERR_HEADER_INVALID;}
    if (h.version != 1){printf("KO\n"); return ERR_HEADER_INVALID;}
    if (h.payload_size > MAX_PAYLOAD_SIZE) return ERR_PAYLOAD_TOO_BIG;
    if (sizeof(h) + h.payload_size != file_size) return ERR_INVALID_FILE;
    if (h.header_size != sizeof(h)) return ERR_HEADER_INVALID;

    offset += sizeof(h);


    uint8_t ascii_buff[65];
    memcpy(ascii_buff, h.ascii, 64);
    ascii_buff[64] = '\0';

    printf("ASCII_file_NAME is %s\n",ascii_buff);

    long remaining = file_size - offset;
    if (h.payload_size > remaining) return ERR_PAYLOAD_TRUNC;


    unsigned char *payload;    
    payload = malloc(h.payload_size);
    if (!payload) return ERR_PAYLOAD_TRUNC;


    if (fread(payload, 1, h.payload_size, file) != h.payload_size){
        free(payload);
        return ERR_PAYLOAD_TRUNC;
    } 
    offset += h.payload_size;
    
    int payload_is_empty = 1;
    for (int i = 0; i < h.payload_size; i++){
        if (payload[i] != 0){
            payload_is_empty = 0;
            break;
        }
    }
    if (payload_is_empty) return ERR_INVALID_FILE;

    // Verify and display payload
    printf("Payload hex: ");
    for(int i = 0; i < h.payload_size; i++){
        printf("%02X ", payload[i]);
    }
    printf("\n");

    free(payload);

}

