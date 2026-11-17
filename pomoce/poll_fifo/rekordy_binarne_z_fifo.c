#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

#define ROZMIAR_REKORDU 8 /* np. dwa uint32_t */
#define BUF_REK 1024

/* (sop-ratking compute_bytes_all) Gdy przez FIFO leca BINARNE rekordy stalej dlugosci,
 * a nie linie tekstu. Zbiera bajty az bedzie pelny rekord, potem wola obsluz(a, b).
 * bufor/zebrane: osobne dla kazdego fd (jak w dziel_wiadomosci).
 * zwraca 1 = dalej, 0 = EOF.
 */
int czytaj_rekordy(int fd, char *bufor, int *zebrane, void (*obsluz)(uint32_t a, uint32_t b))
{
    ssize_t n = read(fd, bufor + *zebrane, BUF_REK - *zebrane);
    if (n == 0)
        return 0;
    if (n == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
            return 1;
        ERR("read");
    }
    *zebrane += (int)n;

    int start = 0;
    while (*zebrane - start >= ROZMIAR_REKORDU)
    {
        uint32_t a, b;
        memcpy(&a, bufor + start, 4); /* memcpy, nie rzutowanie — bezpieczne wyrownanie */
        memcpy(&b, bufor + start + 4, 4);
        obsluz(a, b);
        start += ROZMIAR_REKORDU;
    }
    memmove(bufor, bufor + start, *zebrane - start);
    *zebrane -= start;
    return 1;
}
