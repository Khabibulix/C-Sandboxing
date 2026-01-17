# stacktrace.cc

## Overview

This file implements a **low-level, optimized stack walker** using the x86-64 frame pointer (RBP) chain.  
Frames are captured in **batch** in a contiguous array, storing both the saved RBP and return addresses for complete observation.

---

## Guarantees

- Captures all stack frames up to `MAX_DEPTH`, as long as the RBP chain is intact.
- Prevents crashes via minimal validation (`saved_rbp != 0`, `ret_addr != 0`).
- Provides a full batch of frames for later analysis or display.

---

## Limitations

- Only works if the code is compiled **without `-fomit-frame-pointer`**.
- Corrupted stacks or aggressive inlining may cause missing frames.
- Maximum depth is limited to `MAX_DEPTH`.
- Local variables are not captured; only frame pointers and return addresses.

---

## Key Structures

- **`uintptr_t frames[MAX_DEPTH * 2]`** – batch storage of `[saved_rbp, ret_addr]` pairs.
- **`int frame_count`** – number of stored words (2 per frame).

No intermediate `struct` is used for each frame, for **performance and minimal abstraction**.

---

## Functions

### `stacktrace(void)`

- Walks the stack using RBP pointer chasing.
- Stores `[saved_rbp, ret_addr]` pairs in `frames[]`.
- Prints all captured frames in batch.
- Validates pointers minimally to prevent crashes.
