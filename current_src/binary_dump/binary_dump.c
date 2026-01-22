#include <stdio.h>
#include <string.h>
#include "binary_dump.h"

int main(){
    FILE * file;
    struct header h;

    file = fopen("dummy.bin", "rb");
    if (!file) return 1;

    if (fread(&h, sizeof(h), 1, file) != 1) return 1;

    if (memcmp(h.magic, "BDMP", 4) != 0){
        printf("KO\n");
        return 1;
    }

    if (h.version != 1){
        printf("KO\n");
        return 1;
    }

    printf("OK\n");
}

