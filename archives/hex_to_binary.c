#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void decimal_to_binary(int nb, char* binary){
    for (int i = 7; i >= 0; i--){
        if (nb % 2 == 1){
            binary[i] = '1';
        }  else {
            binary[i] = '0';
        }
        nb = nb / 2;
    }
    binary[8] = '\0';
}

int main(void){
    srand(time(NULL));
    const char *hex_bin[16] = {
    "0000","0001","0010","0011",
    "0100","0101","0110","0111",
    "1000","1001","1010","1011",
    "1100","1101","1110","1111"
    };

    for(int i = 0; i < 16; i++){
        printf("%X:%s  ", i, hex_bin[i]);
        if(i == 7) printf("\n");
    }
    printf("\n\n");


    
    while(1){
        char response[9];
        char result_to_binary[9];
        int result = rand() % 256;
        decimal_to_binary(result, result_to_binary);
        printf("%02X >> ",result);

        scanf("%8s", response);

        if (strcmp(response, result_to_binary) == 0){
            printf("Correct\n");
        } else {
            printf("False, correct response is %.4s %.4s\n", result_to_binary, result_to_binary + 4);
        }
    }        
    return 0;
}
