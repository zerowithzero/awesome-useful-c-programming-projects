# 🧮 CLI Calculator in C

A modular command-line calculator in C featuring:
- Arithmetic operations: `+`, `-`, `*`, `/`, `%`
- Square root and power calculations
- Input validation
- Operation logging with timestamp
- View & clear history from terminal

---

## 📦 Features

- Modularized: Split into `main.c`, `calc.c`, `calc.h`
- Logs saved in `calc_history.txt`
- Options:
  - [1-7] Perform operations
  - [8] Clear History
  - [9] View History
  - [0] Exit

---

## ⚙️ How to Compile and Run

### 🔧 Compile

```bash
gcc main.c -o calculator
