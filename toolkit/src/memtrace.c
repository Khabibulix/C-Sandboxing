#include "memtrace.h"
#include <stdlib.h> // malloc, realloc, free
#include <stdio.h>  // printf

struct memtrace_event g_events[MEMTRACE_MAX_EVENTS];
uint64_t g_event_count = 0;
static uint64_t g_next_alloc_id = 1;
uint64_t g_logical_time = 0;


static void log_event(enum memtrace_event_type type, void *ptr, void *old_ptr, size_t size, uint64_t alloc_id, uint64_t generation) {
  if (g_event_count >= MEMTRACE_MAX_EVENTS) return;
  struct memtrace_event *e = &g_events[g_event_count++];
  e->type = type;
  e->ptr = ptr;
  e->old_ptr = old_ptr;
  e->size = size;
  e->time = g_logical_time++;
  e->thread_id = 0;
  e->alloc_id = alloc_id;
  e->generation = generation;
}

void memtrace_dump(void) {
    for (uint64_t i = 0; i < g_event_count; i++) {
      struct memtrace_event *e = &g_events[i];         
      printf(
      "[%lu] type=%d ptr=%p old_ptr=%p size=%zu time=%lu alloc_id=%lu generation=%lu\n", 
      i, e->type, e->ptr, e->old_ptr, e->size, e->time, e->alloc_id, e->generation
    );
    }
}


void* memtrace_malloc(size_t size) {
  void *p = malloc(size);
  uint64_t id = g_next_alloc_id++;

  log_event(MEMTRACE_MALLOC, p, NULL, size, id, 0);
  return p;
}

void memtrace_free(void *ptr) {
  const struct memtrace_event *last = memtrace_last_event(ptr);
  uint64_t id = last ? last->alloc_id : 0;
  uint64_t gen = last ? last->generation : 0;
  log_event(MEMTRACE_FREE, ptr, NULL, 0, id, gen);
  free(ptr);
}

void* memtrace_realloc(void *ptr, size_t size) {
  const struct memtrace_event *last = memtrace_last_event(ptr);
  uint64_t old_id = last ? last->alloc_id : 0;
  uint64_t old_gen = last ? last->generation : 0;

  void *new_ptr = realloc(ptr, size);

  if (!new_ptr && size) {
    log_event(MEMTRACE_REALLOC_FAIL, ptr, ptr, size, old_id, old_gen);
    return NULL;
  }

  if (!ptr) {
    log_event(MEMTRACE_MALLOC, new_ptr, NULL, size, g_next_alloc_id++, 0);
  } else if (new_ptr == ptr) {
    log_event(MEMTRACE_REALLOC_INPLACE, new_ptr, ptr, size, old_id, old_gen);
  } else {
    log_event(MEMTRACE_REALLOC_MOVE, new_ptr, ptr, size, g_next_alloc_id++, old_gen + 1);
  }

  return new_ptr;

 
}

const struct memtrace_event* memtrace_last_event(void *ptr) {
  const struct memtrace_event *last = NULL;
  for (uint64_t i = 0; i < g_event_count; i++) {
    if (g_events[i].ptr == ptr || g_events[i].old_ptr == ptr) last = &g_events[i];
  }
  return last;
}

int memtrace_alloc_unchanged_since(uint64_t alloc_id, uint64_t generation, uint64_t since_time) {
  const struct memtrace_event *last = NULL;
  for (uint64_t i = 0; i < g_event_count; i++) {
    if (g_events[i].alloc_id == alloc_id) {
      last = &g_events[i];
    }
  }

  if (!last) return 0;
  if (last->generation != generation) return 0;
  if (last->type == MEMTRACE_FREE && last->time >= since_time) return 0;

  return 1;
}
