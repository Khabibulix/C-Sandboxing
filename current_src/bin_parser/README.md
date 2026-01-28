# Black‑Box Parser Harness — Mini Guide

## Run

```bash
./parser input.bin
echo $?
```

## Oracle

* **Only signal**: exit code
* `0` → input accepted
* `!= 0` → rejection (different code = different check)

## Method

1. Generate input
2. Run parser
3. Read exit code
4. Mutate one thing
5. Observe transition

## Error Code Inference

* Same input class → same code
* One change → different code
* Stable code → stable check

## Rules

* No stdout / stderr
* No source code
* No assumptions
* Oracle only

## Goal

Reach exit code `0` with minimal input.
