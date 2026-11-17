#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* Wlacza O_NONBLOCK na juz otwartym fd (np. STDIN_FILENO).
 * Potem read() bez danych zwraca -1 i errno == EAGAIN zamiast czekac.
 * UWAGA: open(STDIN_FILENO, ...) to blad — stdin juz jest otwarty, uzyj tej funkcji. */
void ustaw_nonblock(int fd)
{
    int flagi = fcntl(fd, F_GETFL);
    if (flagi == -1)
        ERR("fcntl");
    if (fcntl(fd, F_SETFL, flagi | O_NONBLOCK) == -1)
        ERR("fcntl");
}
