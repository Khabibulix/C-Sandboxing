#include <unistd.h>
#include <stdint.h>

void write_hex(uintptr_t val){
    char buf[19]; // 0x + 16 hex digits +\n
    char *p = buf + sizeof(buf) - 1;
    *p-- = '\n';

    for (int i = 0; i < 16; i++){
        int nib = val & 0xF;
        buf[i] = (nib < 10 ? '0' + nib : 'a' + nib - 10);
        val >>= 4;
    }

    buf[0] = '0';
    buf[1] = 'x';
    write(1, buf, sizeof(buf));
}