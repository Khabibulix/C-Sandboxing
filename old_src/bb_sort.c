#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_SIZE 100


void swap(int arr[], int i, int j){
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void bubble_sort(int array[], int size){
  for (int i = 0; i < size-1; i++){
    int swapped = 0;
    for (int j = 0; j < size- i -1; j++){
        if (array[j] > array[j+1]){
            swap(array, j, j+1);
            swapped = 1;
            for (int k = 0; k < size; k++) printf("%d ", array[k]);
            printf("\n");
        }
    }
    if (!swapped) break;
  }
}


int main(int argc, char *argv[]){
    int array[MAX_SIZE];
    int size;

    if (argc > 1) {
        size = argc - 1;
        for (int i = 0; i < size; i++){
            array[i] = atoi(argv[i+1]);
        }
    } else {
        int default_array[] = {5, 2, 9, 1, 5, 6};
        size = 6;
        for (int i = 0; i < size; i++){
            array[i] = default_array[i];
        }
    }


    bubble_sort(array, size);
    
    for (int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}