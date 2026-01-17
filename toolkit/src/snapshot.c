#include <stdio.h>
#include <stdlib.h> //malloc
#include <string.h> //memcpy
#include "snapshot.h"
#include "memtrace.h"
#include <stdint.h>
#include <stddef.h>


struct abi_identity {
  void *return_addr;
  void *frame_ptr;
};

struct snapshot{
  void *addr;
  size_t size;
  uint64_t time;
  uint64_t alloc_id;
  uint64_t generation;
  unsigned char *data;
  unsigned char hash;
  struct abi_identity abi;
};


static inline void *abi_caller_frame_ptr(void){return __builtin_frame_address(1);}

static inline void *abi_caller_return_addr(void){return __builtin_return_address(1);}


struct snapshot* snapshot(void *addr, size_t size){
  if (!addr || size == 0) return NULL;


  struct snapshot *snap = malloc(sizeof(struct snapshot));
  if (!snap) return NULL;

  snap->data = malloc(size);
  if (!snap->data){free(snap); return NULL;}

  const struct memtrace_event *before, *after;
  unsigned char *tmp = malloc(size);
  if (!tmp) {free(snap->data); free(snap); return NULL;}

  do {
    before = memtrace_last_event(addr);
    memcpy(tmp, addr, size);
    after = memtrace_last_event(addr);
  } while (!before || !after || before->alloc_id != after->alloc_id || before->generation != after->generation);

  memcpy(snap->data, tmp, size);
  free(tmp);

  snap->addr = addr;
  snap->size = size;
  snap->time = g_logical_time;
  snap->alloc_id = after->alloc_id;
  snap->generation = after->generation;

  unsigned char xor_hash = 0;
  for (size_t i = 0; i < size; i++){
    xor_hash ^= snap->data[i];
  }

  snap->hash = xor_hash;

  snap->abi.frame_ptr = abi_caller_frame_ptr();
  snap->abi.return_addr = abi_caller_return_addr();

  return snap;
  
} 


void free_snapshot(struct snapshot *s){
  if (!s) return;
  free(s->data);
  free(s);
}

int snapshot_is_temporally_valid(const struct snapshot *s){
  if (!s) return 0;

  return memtrace_alloc_unchanged_since(s->alloc_id, s->generation, s->time);
}

void visualize_snapshot(const struct snapshot *snap){
  if (!snap) return;

  unsigned char check = 0;
  for (size_t i = 0; i < snap->size; i++) check ^= ((unsigned char*)snap->data)[i];

  printf("Address : %p\n", snap->addr);
  printf("Size : %zu bytes\n", snap->size);
  printf("Logical Time : %lu\n", snap->time);
  printf("Alloc ID : %lu, Generation : %lu\n", snap->alloc_id, snap->generation);
  printf("Integrity hash : %s\n", (check == snap->hash) ? "OK" : "CORRUPTED");
  printf("Temporal valid : %s\n", snapshot_is_temporally_valid(snap) ? "YES" : "NO");
	
  printf("Bytes : ");
  for (size_t i = 0; i < snap->size; i++){
    printf(" %02X ", snap->data[i]);
    if ((i+1) % 16 == 0) printf("\n");
  }
  printf("\n\n");

}

