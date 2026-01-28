#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define FILE_SIZE 1024 * 1024
#define STRIDE_PATTERN_INTERVAL 4


void create_obscure_binary(const char *filename){
    FILE *f = fopen(filename, "wb");
    if (!f) {
        printf("Failed file creation");
        return;
    }

    uint8_t *buf = malloc(FILE_SIZE);
    if (!buf) {
        fclose(f);
        printf("Mem alloc failed.\n");
        return;
    }

    size_t i;

    memset(buf, 0x00, FILE_SIZE);

    for (i = 0; i < FILE_SIZE; i++){
        if (i % STRIDE_PATTERN_INTERVAL == 0){
            buf[i] = 'A' + (i / STRIDE_PATTERN_INTERVAL) % 26;
        } else {
            buf[i] = 0xFF;
        }
    }

    fwrite(buf, 1, FILE_SIZE, f);
    fclose(f);
    free(buf);
    printf("Created.\n");

}

int main(){
    create_obscure_binary("obs_dummy.bin");
    return 0;
}

