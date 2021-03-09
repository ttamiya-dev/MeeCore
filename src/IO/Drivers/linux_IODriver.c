#ifdef LINUX

#include "IODriver.h"
#include <stdio.h>
#include "assert.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>

// Code from
// https://stackoverflow.com/questions/448944/c-non-blocking-keyboard-input
struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

/* Initialize IO Driver */
STATUS ioDriver_Initialize(void)
{
    set_conio_terminal_mode();
    return SUCCESS;
}

/* Write a single char to output */
STATUS ioDriver_WriteChar(const char ch)
{
    STATUS ret = ERROR;
    char success = putchar(ch);
    if (success != EOF)
        ret = SUCCESS;
    return ret;
}

/* Get a single char. Returns ERROR if empty */
STATUS ioDriver_GetChar(char* ch)
{
    STATUS ret = ERROR;
    assert(ch != NULL);

    if (kbhit())
    {
        *ch = getch();
        if (*ch == 'z')
            reset_terminal_mode();

        ret = SUCCESS;
    }

    return ret;
}

#endif /* LINUX */

