#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(){
    FILE *f = fopen("dummy.bin", "rb");
    if (!f) return 1;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    uint8_t *buf = malloc(size);
    if (!buf) return 2;

    fread(buf, 1, size, f);
    fclose(f);

    size_t cursor = 0;

    while (cursor < 16){
         uint8_t oct = buf[cursor];
        printf("offset %zu : value 0x%02X\n", cursor, oct);
        cursor++;
    }

    free(buf);
    return 0;
}