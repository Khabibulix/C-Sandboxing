# Pointer Overwrite with Radare2

## Goal

The goal of this exercise is to understand and perform a **pointer overwrite** in a C binary in order to **redirect control flow**, using **radare2**.

This is a **static patch**: the binary on disk is modified, without changing the source code.

---

## Prerequisites

- Linux x86-64
- `radare2` installed
- A binary compiled **without PIE** and **without stack protection**

Compile using:

```bash
gcc -no-pie -fno-stack-protector test.c -o test
