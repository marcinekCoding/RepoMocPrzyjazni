#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* SZABLON petli poll po kilku FIFO z argv. Potrzebuje: dziel_wiadomosci.c
 * (i np. otworz_fifo.c). Wklej i zmien obsluz_linie + TIMEOUT_MS.
 *
 *   ./prog plik.bin fifo1 fifo2 fifo3    -> PIERWSZE_FIFO = 2
 */
#define PIERWSZE_FIFO 2
#define TIMEOUT_MS 3000
#define BUF_WIAD 1024

int dziel_wiadomosci(int fd, char *bufor, int *zebrane, void (*obsluz)(char *, void *), void *kontekst);

static void obsluz_linie(char *linia, void *kontekst)
{
    int ktory = *(int *)kontekst;
    printf("[fifo %d] %s\n", ktory, linia);
}

void petla_poll(int argc, char **argv)
{
    int ile = argc - PIERWSZE_FIFO;
    struct pollfd fds[ile];
    char bufory[ile][BUF_WIAD];
    int zebrane[ile];
    memset(zebrane, 0, sizeof(zebrane));

    for (int i = 0; i < ile; i++)
    {
        fds[i].fd = open(argv[i + PIERWSZE_FIFO], O_RDONLY | O_NONBLOCK); /* albo otworz_fifo_*() */
        if (fds[i].fd == -1)
            ERR("open");
        fds[i].events = POLLIN;
    }

    int aktywne = ile;
    while (aktywne > 0)
    {
        int ret = poll(fds, ile, TIMEOUT_MS);
        if (ret == -1)
        {
            if (errno == EINTR)
                continue;
            ERR("poll");
        }
        if (ret == 0)
        {
            printf("Cisza... (timeout)\n");
            continue;
        }
        for (int i = 0; i < ile; i++)
        {
            if (fds[i].fd == -1)
                continue;
            /* POLLHUP tez czytamy: dopiero read()==0 mowi, ze to naprawde koniec */
            if (fds[i].revents & (POLLIN | POLLHUP | POLLERR))
            {
                int ktory = i;
                if (!dziel_wiadomosci(fds[i].fd, bufory[i], &zebrane[i], obsluz_linie, &ktory))
                {
                    printf("%s zamkniete\n", argv[i + PIERWSZE_FIFO]);
                    close(fds[i].fd);
                    fds[i].fd = -1; /* poll ignoruje ujemne fd */
                    aktywne--;
                }
            }
        }
    }
}
