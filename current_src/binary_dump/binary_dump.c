#include <stdio.h>

int main(){
    FILE *file;
    size_t bytes_read;
    size_t offset = 0;
    file = fopen("sample.txt", "rb");
    if (!file) return 1;

    char buffer[100];

    while((bytes_read = fread(buffer, 1, 16, file)) > 0){
        printf("%08zx ", offset);
        for(size_t i = 0; i < bytes_read; i++){
            printf("%02x ", buffer[i]);
        }
        printf("\n");
        offset += bytes_read;
    }
}

