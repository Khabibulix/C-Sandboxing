#include <stdio.h>
#include <string.h>

#define PASSWORD "letmein"

int check_password(const char *input) {
    if (strcmp(input, PASSWORD) == 0) {
        return 1;
    }
    return 0;
}

int main(void) {
    char buf[64];

    printf("Password: ");
    if (!fgets(buf, sizeof(buf), stdin)) {
        return 1;
    }

    // remove newline if present
    buf[strcspn(buf, "\n")] = '\0';

    if (check_password(buf)) {
        puts("Access granted");
    } else {
        puts("Access denied");
    }

    return 0;
}
