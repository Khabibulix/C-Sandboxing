#include <stdio.h>

char *extract_ip(char *s) {
    char *p = s;

    while (*p) {
        if (*p >= '0' && *p <= '9' && (p == s || (p[-1]< '0' || p[-1] > '9'))) {
            char *start = p;
            int num = 0, dots = 0;
            char *q = p;

            while (*q && dots <= 3) {
                if (*q >= '0' && *q <= '9') {
                    num = num * 10 + (*q - '0');
                    if (num > 255) break;
                    q++;
                } else if (*q == '.') {
                    dots++;
                    num = 0;
                    q++;
                } else {
                    break;
                }
            }

            if (dots == 3 && (q == p || (*q != '.' && *q != '\0'))) {
                printf("%.*s\n", (int)(q - start), start);
            }
        }
        p++;
    }

    return NULL;
}


int main(void){
    char *line = "Jan 31 05:12:37 debian sshd[600]: Connection closed by 203.0.113.200 port 44055 [preauth]";
    extract_ip(line);
}
