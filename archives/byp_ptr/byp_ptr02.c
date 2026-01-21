struct auth {
    int ok;
    void (*cb)();
};

void granted() { puts("Access granted"); }
void denied()  { puts("Access denied"); }

int main() {
    struct auth a = {0, denied};
    if (a.ok)
        a.cb();
}
