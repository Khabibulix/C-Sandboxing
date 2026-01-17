#ifndef MEMTRACE_H
#define MEMTRACE_H
#define MEMTRACE_MAX_EVENTS 65536

#include <stddef.h>
#include <stdint.h>

enum memtrace_event_type {
  MEMTRACE_MALLOC,
  MEMTRACE_REALLOC_MOVE,
  MEMTRACE_REALLOC_FAIL,
  MEMTRACE_REALLOC_INPLACE,
  MEMTRACE_FREE
};

struct memtrace_event {
  enum memtrace_event_type type;
  void *ptr;
  void *old_ptr;
  size_t size;
  uint64_t time;
  uint32_t thread_id;
  uint64_t alloc_id;
  uint64_t generation;
};


extern uint64_t g_logical_time;
extern struct memtrace_event g_events[MEMTRACE_MAX_EVENTS];
extern struct memtrace_state g_state;
extern uint64_t g_event_count;


int memtrace_alloc_unchanged_since(uint64_t alloc_id, uint64_t generation, uint64_t since_time);

const struct memtrace_event* memtrace_last_event(void *ptr);

void ensure_state_initialized(void);
void* memtrace_malloc(size_t size);
void* memtrace_realloc(void *ptr, size_t size);
void memtrace_free(void *ptr);

void memtrace_dump(void);


#endif //MEMTRACE_H