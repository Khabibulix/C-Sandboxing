# 📄 **Mini Binary Fuzzer – Quick Documentation**

## 🎯 **Objective**

To **find a crash** or an **interesting return code (ideally `1`)** in `parser.exe` by mutating a valid binary file (`dummy.bin`), and then reinjecting the "live" mutations into the cycle.

---

## 🧠 **General Principle (Poor Man Fuzzing Guided by Return Code)**

We are working **without internal feedback** from the parser, only using:

* The **return code** (`$?`)
* The **parser behavior**

The cycle is as follows:

```
valid seed
   ↓
mutations
   ↓
parser.exe
   ↓
filtered by return code
   ↓
keepers (interesting inputs)
   ↺
```

---

## 📁 **Directory Structure**

```
corpus/
├── seeds/     # Starting seeds (e.g., dummy.bin)
├── keepers/   # Files that "survive" (ret != boring)
├── crashes/   # ret=1 or actual crashes
└── tmp/       # Temporary mutations
```

---

## 🧩 **Files**

### `dummy.bin`

* **Valid** file accepted by the parser (`ret=0`)
* Used as the **initial seed**
* Example content:

```
42 44 4d 50 01 00 4c 00 01 00 00 00 ...
```

👉 Recognized header → stable base for mutation

---

### `mutate.exe`

* **C program**
* Takes a valid seed
* Generates **N mutations**
* Mutations:

  * Bit flips
  * Byte overwrite
  * Insert/delete bytes
  * Numeric value corruption
  * Internal section permutations (violent mutations)

Usage:

```bash
./mutate.exe <seed> <output_dir> <count>
```

---

### `parser.exe`

* **Target binary**
* Returns:

  * `0` → OK / boring
  * `14` → common error
  * `1` → 🎯 jackpot / crash
  * others → interesting inputs

---

### `auto_generate.sh` (the core script)

This script automates everything.

### Logic:

1. Takes files from:

   * `keepers/`
   * `seeds/`
2. Mutates them
3. Tests each mutation
4. Sorts them based on return code

---

## 🔁 **Full Automated Cycle**

### 1️⃣ **Initial Seed**

```bash
cp dummy.bin corpus/seeds/
```

---

### 2️⃣ **Run the Fuzzer**

```bash
./auto_generate.sh
```

---

### 3️⃣ **Automatic Filtering**

| Return Code   | Action                                |
| ------------- | ------------------------------------- |
| `0, 3, 7, 23` | deleted                               |
| `14`          | ignored or kept depending on strategy |
| `1`           | 🚨 moved to `crashes/` + stop         |
| others        | moved to `keepers/`                   |

---

## 🧬 **Why This Works (Quick Overview)**

* Mutations **break internal invariants**.
* The parser:

  * often validates the header
  * fails further down (sizes, offsets, structures)
* By reinjecting the **keepers**, you explore:

  * deeper code paths
  * unstable internal states

👉 Even without coverage, this is a **guided exploration based on the program's behavior**.

---

## 🧨 **Why You See a Lot of `14`**

This is expected:

* `14` = early error / safeguard
* The parser rejects before reaching the more interesting sections.

