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
    size_t current_allocated_size = size;

    struct annotations *annotation_ptr = malloc(size * sizeof(struct annotations));
    if (!annotation_ptr) return 1;



    while (cursor < size){
        uint8_t oct = buf[cursor];

        if (cursor >= current_allocated_size){
            size_t new_size = current_allocated_size * 2;
            annotation_ptr = realloc(annotation_ptr, new_size * sizeof(struct annotations));
            if (!annotation_ptr) exit(1);
            current_allocated_size = new_size;
        }

        annotation_ptr[cursor].offset = cursor;
        annotation_ptr[cursor].value = oct;
        annotation_ptr[cursor].comment[0] = '\0';

        if (oct >= 0x20 && oct <= 0x7E){
            strcpy(annotation_ptr[cursor].comment, "ASCII printable");
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
    free(annotation_ptr);
    return 0;
}