# Memory Event Tracing (memtrace)

## Overview

This module implements a **low-level memory event tracer** for C programs.

It records a linear history of memory allocation events (`malloc`, `free`,
`realloc`) and exposes them through a simple API for inspection and analysis.

The goal is **observation, not enforcement**:
memtrace does not prevent memory bugs — it provides the raw information
needed to *detect and reason about them*.

---

## Purpose

The memtrace module answers questions such as:

* Has a given allocation been freed since a given logical time?
* Does a pointer still refer to the same allocation?
* Did a `realloc` keep an allocation in place or move it?
* Is a snapshot taken earlier still temporally valid?

It serves as a **foundational layer** for higher-level tools, including:

* Memory snapshots
* Temporal validity checks
* Lifetime and use-after-free analysis
* Debugging and memory inspection utilities

---

## Design

### Event Log

All memory-related operations are recorded as **events** in a fixed-size,
append-only array.

Tracked operations:

* Allocation (`MEMTRACE_MALLOC`)
* Deallocation (`MEMTRACE_FREE`)
* Reallocation:

  * in-place (`MEMTRACE_REALLOC_INPLACE`)
  * moved (`MEMTRACE_REALLOC_MOVE`)
  * failed (`MEMTRACE_REALLOC_FAIL`)

Each event records:

* `ptr` — pointer involved in the operation
* `old_ptr` — previous pointer (for realloc-related events)
* `size` — allocation size (when applicable)
* `time` — logical timestamp
* `alloc_id` — unique allocation identifier
* `generation` — generation counter incremented on realloc moves
* `type` — event type

Events are ordered by a **logical clock**, not wall time.

---

## Allocation Identity Model

Each allocation is identified by:

* **alloc_id** — uniquely identifies an allocation lifetime
* **generation** — increments when a `realloc` causes a move

This allows consumers to detect when memory:

* has been freed
* has been resized
* has moved to a different address
* is no longer the same logical object

---

## API Summary

### Allocation wrappers

```c
void* memtrace_malloc(size_t size);
void* memtrace_realloc(void *ptr, size_t size);
void  memtrace_free(void *ptr);
```

These behave like their standard counterparts while recording events.

---

### Event inspection

```c
const struct memtrace_event* memtrace_last_event(void *ptr);
void memtrace_dump(void);
```

* `memtrace_last_event` returns the most recent event involving a pointer
* `memtrace_dump` prints the full event log

---

### Temporal validity

```c
int memtrace_alloc_unchanged_since(
    uint64_t alloc_id,
    uint64_t generation,
    uint64_t since_time
);
```

Returns non-zero if the allocation has not been freed or moved since
the specified logical time.

---

## Global State

The module maintains the following global state:

* `g_events` — fixed-size event buffer
* `g_event_count` — number of recorded events
* `g_logical_time` — monotonically increasing logical timestamp

No dynamic allocation is performed for the event log itself.

---

## Scope and Limitations

* Single-threaded only
* Fixed maximum number of events
* Linear search for event queries (O(n))
* No enforcement or interception beyond wrapped allocators
* Intended for debugging, inspection, and tooling — not production runtime use

---

## Philosophy

memtrace favors:

* **Explicit state**
* **Simple data structures**
* **Deterministic behavior**
* **Composability with higher-level tools**

It is designed to be small, understandable, and reliable —
a solid base layer rather than a feature-rich framework.

---
