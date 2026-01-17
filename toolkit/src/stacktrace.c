#include <stdint.h>
#include <unistd.h>
#include "write_hex.h"

#define MAX_DEPTH 64
#define STACK_PROX (64 * 1024) // 64 KB

static inline uintptr_t get_rbp(void){
    uintptr_t rbp;
    __asm__("mov %%rbp, %0" : "=r"(rbp));
    return rbp;
}

void stacktrace(void){
  uintptr_t rbp = get_rbp();

  for (int i = 0; i < MAX_DEPTH && rbp; i++){
    uintptr_t next_rbp = *(uintptr_t *)rbp;
    uintptr_t ret = *(uintptr_t *)(rbp + sizeof(uintptr_t));

    if(!next_rbp || !ret) break;
    if (ret >= rbp - STACK_PROX && ret < rbp) break;
    if (next_rbp <= rbp) break;

    write_hex(rbp);
    write_hex(ret);
    
    rbp = next_rbp;
  }
}