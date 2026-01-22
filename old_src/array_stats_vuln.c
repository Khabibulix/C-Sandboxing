
#include <stdio.h>
#include <stdlib.h>

int* parse_arguments(int argc, char **argv, int* out_count) {
    int n = argc - 1;
    int *tab = malloc(n * sizeof(int));

    for (int i = 1; i < argc; i++) {
        tab[i - 1] = atoi(argv[i]);
    }

    *out_count = n;
    return tab;
}

int main(int argc, char **argv) {
    int n;
    int *tab = parse_arguments(argc, argv, &n);

    for (int i = 0; i < n; i++) {
        printf("%d\n", tab[i]);
    }

    free(tab);
    return 0;
}
