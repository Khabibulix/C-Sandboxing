#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void analyse_with_stride(uint8_t *buf, size_t file_size, size_t stride){
    size_t cursor = 0;

    while(cursor < file_size){
        printf("Stride %zu, Offset %zu :", stride, cursor);
        printf("%02X\n", buf[cursor]);

        cursor += stride;
    }
}

int main(){


    FILE *f = fopen("dummy.bin", "rb");
    if (!f) return 1;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    
    uint8_t *buf = malloc(size);
    if (!buf) return 2;

    if (fread(buf, 1, size, f) != size){free(buf); fclose(f); return 3;}
    fclose(f);

    size_t strides[] = {2, 4, 8};

    for (size_t i = 0; i < sizeof(strides)/sizeof(strides[0]); i++){
        analyse_with_stride(buf, size, strides[i]);
    }


    free(buf);
    return 0;
}