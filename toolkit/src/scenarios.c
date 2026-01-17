#include "scenarios.h"


unsigned int logical_time = 0;



// Only address is lying
void invoke_scenario_one(void){
  void *p = memtrace_malloc(32);

  memset(p, 0xAA, 32);
  struct snapshot *snap1 = snapshot(p, 32);
  memtrace_free(p);  
  void *q = memtrace_malloc(32);
  struct snapshot *snap2 = snapshot(q, 32);
  printf("p = %p\nq = %p\n", p, q);

  visualize_snapshot(snap1);
  visualize_snapshot(snap2);
  free_snapshot(snap1);
  free_snapshot(snap2);
}


//Memory state != Logic state
void invoke_scenario_two(void){
  void *p = memtrace_malloc(16);
  memset(p, 0xAA, 16);
  struct snapshot *snap1 = snapshot(p, 16);
  memtrace_free(p);
  struct snapshot *snap2 = snapshot(p, 16);
  printf("p = %p\n", p);
  
  visualize_snapshot(snap1);
  visualize_snapshot(snap2);
  memtrace_dump();
  free_snapshot(snap1);
  free_snapshot(snap2);

}

// Prove that realloc can move stuff
void invoke_scenario_three(void){
  void *p = memtrace_malloc(16);
  memset(p, 0xAA, 16);
  struct snapshot *snap1 = snapshot(p, 17);
  void *p2 = memtrace_realloc(p, 17);
  struct snapshot *snap2 = snapshot(p2, 17);

  visualize_snapshot(snap1);
  visualize_snapshot(snap2);
  memtrace_dump();
  free_snapshot(snap1);
  free_snapshot(snap2);
}


void invoke_scenario_four(void){
    void *p = memtrace_malloc(16);
    memset(p, 0xAA, 16);
    struct snapshot *snap1 = snapshot(p, 16);
    void *p2 = memtrace_realloc(p, 32);
    memset(p2, 0xBB, 32);
    struct snapshot *snap2 = snapshot(p2, 32);
    printf("p  = %p\np2 = %p\n", p, p2);
    
    visualize_snapshot(snap1);
    visualize_snapshot(snap2);
    memtrace_dump();
    free_snapshot(snap1);
    free_snapshot(snap2);
    memtrace_free(p2);
}

void invoke_scenario_five(void) {

    char *p = memtrace_malloc(32);
    for (size_t i = 0; i < 32; i++) p[i] = (unsigned char)0xAA;

    struct snapshot *s1 = snapshot(p, 32);
    visualize_snapshot(s1);

    p = memtrace_realloc(p, 64);
    for (size_t i = 32; i < 64; i++) p[i] = (unsigned char)0xBB;

    visualize_snapshot(s1);


    struct snapshot *s2 = snapshot(p, 32);
    memtrace_free(p);

    visualize_snapshot(s2);

    free_snapshot(s1);
    free_snapshot(s2);
}

