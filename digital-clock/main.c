#include <stdio.h>      // For printf, getchar
#include <time.h>       // For time and localtime
#include <stdlib.h>     // For system()

// Platform-specific headers and functions
#ifdef _WIN32
    #include <conio.h>  // For getch, kbhit on Windows
    #include <windows.h> // For Sleep on Windows

    // Clears the console screen on Windows
    void clear_screen() {
        system("cls");
    }

    // Sleep for given milliseconds (Windows)
    void sleep_ms(int milliseconds) {
        Sleep(milliseconds);
    }

#else
    // For Linux / macOS
    #include <unistd.h>     // For usleep
    #include <termios.h>    // For terminal I/O
    #include <fcntl.h>      // For file control options

    // Clears the console screen on Unix
    void clear_screen() {
        system("clear");
    }

    // Sleep for given milliseconds (Unix)
    void sleep_ms(int milliseconds) {
        usleep(milliseconds * 1000); // usleep takes microseconds
    }

    // Check if a key was pressed (non-blocking)
    int kbhit() {
        struct termios oldt, newt;
        int ch;
        int oldf;

        tcgetattr(STDIN_FILENO, &oldt);             // Save terminal settings
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);           // Disable canonical mode & echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);    // Apply new settings
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);    // Restore old settings
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        if (ch != EOF) {
            ungetc(ch, stdin); // Put the char back
            return 1;
        }

        return 0;
    }

    // Read a character without waiting for Enter key
    int getch() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); // Turn off canonical and echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore
        return ch;
    }
#endif

int is24Hour = 1; // Global flag to toggle between 12-hour and 24-hour formats

// Function to display the current time on the console
void print_time(int blink) {
    time_t now;               // Variable to hold current system time
    struct tm *t;             // Struct to break down time into components
    char ampm[3] = "";        // AM/PM string for 12-hour format

    time(&now);               // Get current time
    t = localtime(&now);      // Convert to local time structure

    int hour = t->tm_hour;

    // If 12-hour mode is enabled
    if (!is24Hour) {
        if (hour == 0)
            hour = 12;
        else if (hour > 12)
            hour -= 12;

        snprintf(ampm, sizeof(ampm), t->tm_hour >= 12 ? "PM" : "AM");
    }

    // Display clock interface
    printf("\n\n\t\t+--------------------------+\n");
    printf("\t\t|     DIGITAL CLOCK        |\n");
    printf("\t\t+--------------------------+\n\n");

    // Print current time in hh:mm:ss format
    printf("\t\t      %02d%c%02d%c%02d %s\n",
        hour,
        blink ? ':' : ' ',  // Blinking colon every second
        t->tm_min,
        blink ? ':' : ' ',
        t->tm_sec,
        is24Hour ? "" : ampm
    );

    // User controls
    printf("\n\t[H] Toggle 12/24 Hour   [Q] Quit\n");
}

// Entry point of the program
int main() {
    int blink = 1;   // Toggle colon blinking effect
    char key;        // To capture user keypresses

    while (1) {
        clear_screen();       // Clear screen every second
        print_time(blink);    // Print time
        blink = !blink;       // Toggle blinking colon

        sleep_ms(1000);       // Wait for 1 second

        // Check if user pressed a key
        if (kbhit()) {
            key = getch();    // Get the pressed key
            if (key == 'q' || key == 'Q')
                break;        // Exit loop if 'q' is pressed
            else if (key == 'h' || key == 'H')
                is24Hour = !is24Hour;  // Toggle 12/24 hour mode
        }
    }

    clear_screen(); // Final clear
    printf("Clock closed. Goodbye!\n");
    return 0;
}
