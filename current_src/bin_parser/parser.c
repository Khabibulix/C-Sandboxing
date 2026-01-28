#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct annotations {
    size_t offset;
    uint8_t value;
    char comment[64];
};

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
    size_t start_offset = 0;
    char ascii_seq_temp[64] = "";
    size_t ascii_index = 0;

    //TODO: Replace by dynamic struct for bigger files
    struct annotations annotation[size];

    while (cursor < size){
        uint8_t oct = buf[cursor];

        annotation[cursor].offset = cursor;
        annotation[cursor].value = oct;
        annotation[cursor].comment[0] = '\0';

        if (oct >= 0x20 && oct <= 0x7E){
            strcpy(annotation[cursor].comment, "ASCII printable");
            if (ascii_index == 0) start_offset = cursor;
            ascii_seq_temp[ascii_index++] = oct;
            ascii_seq_temp[ascii_index] = '\0';

        } else {
            if (ascii_index > 0){
                printf("Offset %zu-%zu : ASCII = %s\n", start_offset, cursor-1, ascii_seq_temp );
                ascii_index = 0;
                ascii_seq_temp[0] = '\0';
            }
        }
        cursor++;
    }

    free(buf);
    return 0;
}