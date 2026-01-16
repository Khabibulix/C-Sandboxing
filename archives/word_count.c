#include <stdio.h>
#define MAX_WORD_LEN 50


FILE* open_file(const char* filename){
    FILE *fp = fopen(filename, "r");

    if (fp == NULL){
        fprintf(stderr, "File unopenable.\n");
        return NULL;
    } 

    return fp;
}

int is_valid_char(int c){
    return ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z')||
            (c >= '0' && c <= '9'));
}

void parse_words(FILE* f){
    char buffer[MAX_WORD_LEN];
    int index = 0;
    int in_word = 0;

    int c;
    while((c = fgetc(f)) != EOF){
        if(is_valid_char(c)){
            if (index < MAX_WORD_LEN - 1) buffer[index++] = c;
            in_word = 1;
        } else {
            if (in_word){
                buffer[index] = '\0';
                printf("Word detected : %s\n", buffer);
                index = 0;
                in_word = 0;
            }
        }
    }

    // last word
    if (in_word){
        buffer[index] = '\0';
        printf("Word detected : %s\n", buffer);
    }
}


int main (void){
    FILE* f = open_file("log.txt");
    if (f == NULL) return 1;

    parse_words(f);

    fclose(f);
}