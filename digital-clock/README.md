# 🕒 Digital Clock in C (Console)

A real-time digital clock built in pure C for terminal use. Includes keyboard shortcuts, blinking display, and 12/24-hour format switching.

## 🚀 Features
- Real-time clock
- 12/24 hour toggle (press `H`)
- Exit any time (press `Q`)
- Blinking colon
- Clean terminal output
- Cross-platform (Windows/Linux)

## 🧠 Concepts Used
- `time.h` for system time
- `conio.h` (Windows) / `termios.h` (Linux) for key handling
- Loops, system calls
- Conditional compilation for cross-platform support

## 🛠️ Compile & Run

### On Windows:
```bash
gcc digital_clock.c -o digital_clock
./digital_clock
```
### On Linux/macOS:
```bash
gcc digital_clock.c -o digital_clock
./digital_clock
```

## 💡 Author
Made with ❤️ in C by Achal C.