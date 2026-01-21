#include <stdio.h>

void ok()   { puts("Access granted"); }
void nope() { puts("Access denied"); }

void (*auth_cb)();

int main() {
    auth_cb = nope;
    auth_cb();
}
