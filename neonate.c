#include "headers.h"

void die(const char *s) {
    perror(s);
    return;
}

struct termios orig_termios;


void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
*/
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/**
 * stdout and stdin are buffered we disable buffering on that
 * After entering in raw mode we read characters one by one
 * Up arrow keys and down arrow keys are represented by 3 byte escape codes
 * starting with ascii number 27 i.e. ESC key
 * This way we interpret arrow keys
 * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
 * Backspace move the cursor one control character to the left
 * @return
*/
int neonate(int time_interval) {
    // printf("neonating\n");
    char c;
    time_t start_time, end_time;
    clock_t cpu_start, cpu_end;

    start_time = time(NULL);
    cpu_start = clock();

    ssize_t bytes_read;

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if(fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1) 
    {
        perror(RED"fcntl"reset);
    }

    enableRawMode();

    while (1) {
    
        setbuf(stdout, NULL);

        bytes_read = read(STDIN_FILENO, &c, 1);

        if (bytes_read == 1) 
        {
            if (c == 'x') 
            {
                // printf("IS THAT AN EX?\n");
                break;
            }
        }

        end_time = time(NULL);
        cpu_end = clock();

        time_t t = (int)(difftime(end_time, start_time));

        if(t >= time_interval)
        {
            FILE *fd;
            fd = fopen("/proc/loadavg", "r");
            if(fd == NULL)
            {
                perror(RED"open"reset);
                return 0;
            }


            char buf[1000];
            int pid;
            
            fscanf(fd, "%*f %*f %*f %*s %d", &pid);

            printf("%d\n", pid);

            fclose(fd);

            start_time = time(NULL);
            cpu_start = clock();
        }
    }

    if(fcntl(STDIN_FILENO, F_SETFL, flags) == -1) 
    {
        perror(RED"fcntl"reset);
    }

    disableRawMode();
    return 0;
}