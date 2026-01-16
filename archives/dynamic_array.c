#include <stdio.h>
#include <stdlib.h> // malloc, free

int main(){
    int size;

    printf("Type the size of the array:\n");
    scanf("%d", &size);

    int* array = malloc(size * sizeof(int));

    if (array == NULL){
        fprintf(stderr, "ERROR: shitty allocation.\n");
        return 1;
    }
    
    for (int i = 0; i < size; i++){
        array[i] = i * i;
    }

    for (int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");

    
    int new_size = 2 * size;
    array = realloc(array, new_size * sizeof(int));
    if (array == NULL){
        fprintf(stderr, "ERROR: shitty allocation.\n");
        return 1;
    }

    for (int i = 0; i < new_size; i++){
        array[i] = i * i;
    }

    for (int i = 0; i < new_size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");

    free(array);

}