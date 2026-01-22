#include <stdio.h>
#include <limits.h> //INT_MAX
#include <stdlib.h> //malloc, free

int tests_failed = 0;

#define ASSERT(expr) do { if (!(expr)) tests_failed++; } while (0)

/* Funcs */
int is_valid_integer(char* string){
    if (!string || string[0] == '\0') return 0;
    if (string[0] == '+' || string[0] == '-') string += 1;

    for (int i = 0; string[i] != '\0'; i++){
        if (string[i] < '0' || string[i] > '9') return 0;
    }
    return 1;
}


int string_to_int(char* string){
    int value = 0;
    int sign = 1;

    if (string[0] == '-'){
        sign = -1;
        string += 1;
    } else if (string[0] == '+'){
        string += 1;
    }
    
    for (int i = 0; string[i] != '\0'; i++){
        if (value > (INT_MAX - (string[i] - '0')) / 10) return 0;
        value = value * 10 + (string[i] - '0');
    }

    return value * sign;  
}

int* parse_arguments(int argc, char **argv, int* out_count){
    if (argc < 2){
        return NULL;
    }

    int n = argc -  1;
    int* tab = malloc(n * sizeof(int));

    for (int i = 1; i < argc; i++){
        char* current_string = argv[i];
        if (!is_valid_integer(current_string)){
            free(tab);
            return NULL;
        }

        int value = string_to_int(current_string); 
        tab[i - 1] = value;
    }
    *out_count = n;
    return tab;

}

int compute_sum(int* arr, int size){
    int sum = 0;

    for (int i = 0; i < size; i++){
        sum += arr[i];
    }

    return sum;
}

/* Tests */

void test_is_valid_integer(void){
    ASSERT(is_valid_integer("123") == 1);
    ASSERT(is_valid_integer("-42") == 1);
    ASSERT(is_valid_integer("+7") == 1);
    ASSERT(is_valid_integer("") == 0);
    ASSERT(is_valid_integer("12a") == 0);
    ASSERT(is_valid_integer("--5") == 0);
}

void test_string_to_int(void){
    ASSERT(string_to_int("123") == 123);
    ASSERT(string_to_int("-42") == -42);
    ASSERT(string_to_int("+7") == 7);
    ASSERT(string_to_int("0") == 0);
}

void test_parse_arguments(void){
    char *argv[] = {"prog", "10", "-5", "3"};
    int argc = 4;
    int n;

    int *tab = parse_arguments(argc, argv, &n);
    ASSERT(tab != NULL);
    ASSERT(n == 3);
    ASSERT(tab[0] == 10);
    ASSERT(tab[1] == -5);
    ASSERT(tab[2] == 3);
    free(tab);
}

void test_parse_arguments_invalid(void)
{
    char *argv[] = { "prog", "10", "abc", "3" };
    int argc = 4;
    int n;

    int *tab = parse_arguments(argc, argv, &n);
    ASSERT(tab == NULL);
}

void test_compute_sum(void)
{
    int arr[] = { 1, -2, 3, 4 };
    int sum = compute_sum(arr, 4);
    ASSERT(sum == 6);
}

int main(int argc, char **argv){
    test_is_valid_integer();
    test_string_to_int();
    test_parse_arguments();
    test_parse_arguments_invalid();
    test_compute_sum();

    if (tests_failed == 0){
        printf("Tests OK\n");
    } else {
        return 1;
    }

    return 0;
}