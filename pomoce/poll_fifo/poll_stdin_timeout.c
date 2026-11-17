#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* (warsztaty2/poll_test_moj.c) Czeka na wpisanie czegos na stdin, ale max timeout_ms.
 * Zwraca liczbe wczytanych bajtow (bufor zakonczony '\0'), 0 = timeout, -1 = EOF (Ctrl+D).
 *
 *   char buf[128];
 *   printf("podaj numer: "); fflush(stdout);
 *   int n = czytaj_stdin_z_timeoutem(buf, sizeof(buf), 3000);
 *   if (n == 0) puts("za wolno!");
 */
int czytaj_stdin_z_timeoutem(char *buf, size_t rozmiar, int timeout_ms)
{
    struct pollfd p = {.fd = STDIN_FILENO, .events = POLLIN};
    int ret;
    do
        ret = poll(&p, 1, timeout_ms);
    while (ret == -1 && errno == EINTR);
    if (ret == -1)
        ERR("poll");
    if (ret == 0)
        return 0;
    ssize_t n = read(STDIN_FILENO, buf, rozmiar - 1);
    if (n < 0)
        ERR("read");
    if (n == 0)
        return -1;
    buf[n] = '\0';
    return (int)n;
}
