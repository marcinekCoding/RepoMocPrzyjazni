#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

#define BUF_WIAD 1024

/* SPRAWDZONY wzorzec dzielenia strumienia z FIFO na linie zakonczone '\n'.
 * Jedno read() moze dac pol wiadomosci albo kilka naraz — ta funkcja to ogarnia.
 *
 * bufor: osobny bufor na kazdy fd, char bufory[ile_fd][BUF_WIAD]
 * zebrane: osobny licznik na kazdy fd, int zebrane[ile_fd] = {0}
 * obsluz: Twoja funkcja, dostaje JEDNA linie juz z '\0' (bez '\n') + kontekst
 * zwraca: 1 = czytamy dalej, 0 = EOF (zamknij fd i ustaw fds[i].fd = -1)
 *
 * Zasady (bo to sie psulo 3 razy):
 *  1. memcpy nowych danych pod STARY licznik, dopiero potem licznik += n
 *  2. skanuj CALY bufor (moze byc kilka linii naraz)
 *  3. '\n' -> '\0', zamiast strlen na buforze bez zera
 *  4. resztke przesun memmove (nie memcpy — zakresy sie nakladaja)
 */
int dziel_wiadomosci(int fd, char *bufor, int *zebrane, void (*obsluz)(char *linia, void *kontekst),
                     void *kontekst)
{
    int wolne = BUF_WIAD - 1 - *zebrane;
    if (wolne <= 0) /* linia dluzsza niz bufor — wyrzucamy smieci */
    {
        fprintf(stderr, "za dluga wiadomosc, odrzucam\n");
        *zebrane = 0;
        wolne = BUF_WIAD - 1;
    }
    ssize_t n = read(fd, bufor + *zebrane, wolne);
    if (n == 0)
        return 0; /* EOF */
    if (n == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
            return 1; /* chwilowo pusto */
        ERR("read");
    }
    *zebrane += (int)n;

    int start = 0;
    for (int i = 0; i < *zebrane; i++)
    {
        if (bufor[i] == '\n')
        {
            bufor[i] = '\0';
            obsluz(bufor + start, kontekst);
            start = i + 1;
        }
    }
    int reszta = *zebrane - start;
    memmove(bufor, bufor + start, reszta);
    *zebrane = reszta;
    return 1;
}
