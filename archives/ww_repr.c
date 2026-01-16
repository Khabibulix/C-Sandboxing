
# include <stdio.h>
# include <stdint.h>

static void print_byte_bin(uint8_t b) {
  for (int i = 7; i>= 0; i--){
    printf("%d", (b >> i) & 1);
  }
}

static void dump_word(const char *label, uint64_t w){
  printf("%s\n", label);
  for (int i = 7; i >= 0; i--){
    uint8_t b = (w >> (i * 8)) & 0xFF;
    print_byte_bin(b);
    printf(" ");
  }
  printf("\n\n");
}

int main(void){
  const char s[8] = {'A', 'B', 'C', '\0', 'D', 'E', 'F', 'G'};
  uint64_t word = 0;
  for (int i = 0; i < 8; i++){
    word |= (uint64_t)(unsigned char)s[i] << (i * 8);
  }
  uint64_t minus1 = word - 0x0101010101010101ULL;
  uint64_t notw = ~word;
  uint64_t and1 = minus1 & notw;
  uint64_t mask = and1 & 0x8080808080808080ULL;

  printf("INPUT BYTES\n");
  for (int i = 0; i < 8; i++){
    printf("'%c' 0x%02X ", s[i] ? s[i] : '0', (unsigned char)s[i]);
    print_byte_bin((unsigned char)s[i]);
    printf("\n");
  }
  printf("\n");

  dump_word("WORD", word);
  dump_word("WORD - 0x01", minus1);
  dump_word("~WORD", notw);
  dump_word("(WORD - 1) & ~WORD", and1);
  dump_word("MASK & 0x80", mask);
	
  if (mask) {
    int bit = __builtin_ctzll(mask);
    printf("First MSB set at bit %d -> byte index %d\n", bit, bit / 8);
  } else {
    printf("No zero byte detected\n");
  }
  return 0;
}
