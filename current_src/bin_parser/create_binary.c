#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*0  -> accepté
23 -> fichier trop court
3  -> header invalide
7  -> taille trop grande
1  -> payload tronqué
14 -> logique interne rejet
*/

void create_binary(const char *filename){
    FILE *f = fopen(filename, "wb");
    if (!f){
        printf("Creation failed.");
        return;
    }


    uint32_t payload_size = 16;
    uint32_t real_size = payload_size + 64;

    uint8_t *payload = malloc(real_size);

    memset(payload, 0x41, real_size);
    payload[real_size - 1] = 0x42;

    fwrite(payload, 1, real_size, f);

    fclose(f);
}
     

int main(){
    
}

