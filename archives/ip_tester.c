# include <stdio.h>
# include <string.h>

int is_valid_ipv4(const char *ip)
{

  if (!ip || !*ip) return 0;
	
    int seg = 0;

    while (*ip) {
      int num = 0;
      int digits = 0;

      if (*ip == '0' && ip[1] >= '0' && ip[1] <= '9') return 0;
      
      while (*ip >= '0' && *ip <= '9') {
        int digit = *ip - '0';
	num = num * 10 + digit;
        if (num > 255 || digits > 3 ) return 0;
        digits++;
        ip++;
      }
      
      if (digits == 0) return 0;

      seg++;

      if (*ip == '.') ip++;
      else break;
    }

    return seg == 4 && *ip == '\0';
}

const char *tests[] = {
    "01.02.03.004", "000.000.000.000", "001.1.1.1", "192.168.1.1hello","10.0.0.1/24","8.8.8.8???",
    "192.168.1.1 ", " 192.168.1.1", "192.168.1.1\n", "192.168.1.1\033", "256.1.1.1", "1.2.3.4.",
    "192.168.1.1","8.8.8.8",NULL
};

int main(void) {
  for (int i = 0; tests[i]; i++) {
    printf("%s -> %s\n", tests[i], is_valid_ipv4(tests[i]) ? "OK" : "NOPE");
  }
}
