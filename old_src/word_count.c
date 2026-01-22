#include <stdio.h>
#include <string.h>
#define MAX_WORD_LEN 50
#define MAX_WORDS 1000

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordEntry;

WordEntry words[MAX_WORDS];
int word_count = 0;


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


void update_word_count(const char* word){
    for (int i = 0; i < word_count; i++){
        if (strcmp(words[i].word, word) == 0){
            words[i].count++;
            return;
        }
    }
    if (word_count < MAX_WORDS){
        strcpy(words[word_count].word, word);
        words[word_count].count = 1;
        word_count++;
    } else {
        fprintf(stderr, "Max words is overflowed\n");
    }
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
                update_word_count(buffer);
                index = 0;
                in_word = 0;
            }
        }
    }

    // last word
    if (in_word){
        buffer[index] = '\0';
        update_word_count(buffer);
    }
}

void print_word_counts(){
    for (int i = 0; i < word_count; i++){
        printf("%s : %d\n", words[i].word, words[i].count);
    }
}


int main (void){
    FILE* f = open_file("log.txt");
    if (f == NULL) return 1;

    parse_words(f);
    print_word_counts();

    fclose(f);
}