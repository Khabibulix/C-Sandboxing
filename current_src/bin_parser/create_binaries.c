#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define OUTDIR "cases"

void ensure_dir(void){
  if (mkdir(OUTDIR, 0755) && errno != EEXIST){
    perror("mkdir");
    exit(1);
  }
}

void create_binary(
  const char *filename,
  uint16_t header_size,
  uint32_t payload_size,
  uint8_t fill){

    FILE *f = fopen(filename, "wb");
    if (!f) return;

    //HEADER
    uint8_t header[76];
    memset(header, 0, sizeof(header));
    memcpy(header + 0x00, "BDMP", 4);
    header[0x04] = 0x01;
    header[0x05] = 0x00;
    memcpy(header + 0x06, &header_size, 2);
    memcpy(header + 0x08, &payload_size, 4);
    memset(header + 0x0C, fill, 64);

    //PAYLOAD
    if (payload_size > 0){
	uint8_t *payload = malloc(payload_size);
	memset(payload, fill, payload_size);
	fwrite(payload, 1, payload_size, f);
	free(payload);
    }
    

    fwrite(header, 1, header_size, f);

    fclose(f);
}
     

int main(){
    char name[64];
    ensure_dir();    

    for (uint16_t hs = 0; hs <= 100; hs++){
	snprintf(name, sizeof(name), OUTDIR "/case_hs_%u.bin", hs);
	create_binary(name, hs, 1, 0x42);
    }

    for (uint32_t ps = 0; ps <= 100; ps++){
	snprintf(name, sizeof(name), OUTDIR "/case_ps%u.bin", ps);
	create_binary(name, 76, ps, 0x41);
    }

    for (uint8_t b = 0; b < 0xFF; b++ ){
	snprintf(name, sizeof(name), OUTDIR "/case_fill_%02x.bin", b);
	create_binary(name, 76, 1, b);

    }

}

