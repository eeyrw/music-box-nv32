#include  <errno.h>
#include  <stdio.h>
#include  <sys/stat.h>
#include  <sys/unistd.h>
#include "common.h"
#include "uart.h"

#undef errno
extern int errno;

/******************************************************************************
 *
 ******************************************************************************/

void stdio_setup(void)
{
    // Turn off buffers, so I/O occurs immediately
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

/******************************************************************************
 *
 ******************************************************************************/

int _read(int file, char *data, int len)
{
    int bytes_read;

    if (file != STDIN_FILENO)
    {
        errno = EBADF;
        return -1;
    }

    for (bytes_read = 0; bytes_read < len; bytes_read++)
    {
        *data = (char) UART_GetChar(TERM_PORT);
        data++;
    }

    return bytes_read;
}

/******************************************************************************
 *
 ******************************************************************************/

int _write(int file, char *data, int len)
{
    int bytes_written;

    if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
    {
        errno = EBADF;
        return -1;
    }

    for (bytes_written = 0; bytes_written < len; bytes_written++)
    {
        UART_PutChar(TERM_PORT,*data);
        data++;
    }

    return bytes_written;
}

/******************************************************************************
 *
 ******************************************************************************/

int _close(int file)
{
    return -1;
}

/******************************************************************************
 *
 ******************************************************************************/

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

/******************************************************************************
 *
 ******************************************************************************/

int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

/******************************************************************************
 *
 ******************************************************************************/

int _isatty(int file)
{
    if ((file == STDOUT_FILENO) ||
        (file == STDIN_FILENO) ||
        (file == STDERR_FILENO))
    {
        return 1;
    }

    errno = EBADF;
    return 0;
}