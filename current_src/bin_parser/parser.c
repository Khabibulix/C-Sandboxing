#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(){


    FILE *f = fopen("dummy.bin", "rb");
    if (!f) return 1;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    
    uint8_t *buf = malloc(size);
    if (!buf) return 2;

    if (fread(buf, 1, size, f) != size){
        free(buf);
        fclose(f);
        return 3;
    }
    fclose(f);


    size_t cursor = 0;
    size_t start_offset = 0;
    char ascii_seq_temp[1024] = "";
    size_t ascii_index = 0;
    uint8_t repeat_value = 0;
    size_t repeat_start = 0;
    size_t repeat_len = 0;
    const size_t MIN_REPEAT = 4;


    while (cursor < size){
        uint8_t oct = buf[cursor];
        

        if (oct >= 0x20 && oct <= 0x7E){
            if (ascii_index == 0) start_offset = cursor;
            if (ascii_index < sizeof(ascii_seq_temp) - 1){
                ascii_seq_temp[ascii_index++] = oct;
                ascii_seq_temp[ascii_index] = '\0';
            }


        } else {
            if (ascii_index > 0){
                printf("Offset %zu-%zu : ASCII = %s\n", start_offset, cursor-1, ascii_seq_temp );
                ascii_index = 0;
                ascii_seq_temp[0] = '\0';
            }
        }

        if (repeat_len == 0){
            repeat_value = oct;
            repeat_start = cursor;
            repeat_len = 1;
        } else if (oct == repeat_value){
            repeat_len++;
        } else {
            if (repeat_len >= MIN_REPEAT){
                printf("Offset %zu-%zu : Repeat for %zu char: 0x%02X\n", repeat_start, cursor - 1, repeat_len, repeat_value);
            }
            repeat_value = oct;
            repeat_start = cursor;
            repeat_len = 1;
        }


        cursor++;
    }

    if (ascii_index > 0) {
        printf("Offset %zu-%zu : ASCII = %s\n", start_offset, cursor - 1, ascii_seq_temp);
    }
    if (repeat_len >= MIN_REPEAT) {
        printf("Offset %zu-%zu : Repeat for %zu char: 0x%02X\n", repeat_start, cursor - 1, repeat_len, repeat_value);
    }


    free(buf);
    return 0;
}