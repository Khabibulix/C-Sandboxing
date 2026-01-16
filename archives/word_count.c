#include <stdio.h>

FILE* open_file(const char* filename){
    FILE *fp = fopen(filename, "r");

    if (fp == NULL){
        fprintf(stderr, "File unopenable.\n");
        return NULL;
    } 

    return fp;
}


int main (void){
    FILE* f = open_file("log.txt");
    if (f == NULL) return 1;

    fclose(f);
}