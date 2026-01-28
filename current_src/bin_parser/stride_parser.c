#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define CHUNK_SIZE 16

void analyse_with_stride(uint8_t *buf, size_t file_size, size_t stride){
    size_t chunk_start = 0;

    while(chunk_start < file_size){
        printf("Stride %zu, Offset %zu :", stride, chunk_start);

        for (size_t i = 0; i < CHUNK_SIZE && (chunk_start + i) < file_size; i++){
            size_t idx = chunk_start + i * stride;
            
            if (idx < file_size){
                uint8_t oct = buf[idx];
                
                if (oct >= 0x20 && oct <= 0x7E)
                    printf("%c", oct);
                else
                    printf(".");
            } else {
                printf(" ");
            }
        }

        printf("\n");
        chunk_start += CHUNK_SIZE;
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