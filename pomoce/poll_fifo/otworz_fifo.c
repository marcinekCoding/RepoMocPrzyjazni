#define _XOPEN_SOURCE 700
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* 3 sposoby otwarcia FIFO. Kazdy sprawdza, ze to NAPRAWDE FIFO (S_ISFIFO).
 * Tworzenie FIFO z terminala:  mkfifo f1 f2;   z kodu:  mkfifo("f1", 0666);
 */

static void upewnij_sie_ze_fifo(int fd, const char *sciezka)
{
    struct stat st;
    if (fstat(fd, &st) == -1)
        ERR("fstat");
    if (!S_ISFIFO(st.st_mode))
    {
        fprintf(stderr, "%s nie jest FIFO\n", sciezka);
        exit(EXIT_FAILURE);
    }
}

/* A) "nigdy sie nie blokuj" — O_RDWR. Najczestsze na labach.
 *    Minus: read() NIGDY nie da EOF (sami jestesmy pisarzem), wiec koniec
 *    rozpoznajesz innym sposobem (timeout, wiadomosc "koniec"). */
int otworz_fifo_rdwr(const char *sciezka)
{
    int fd = open(sciezka, O_RDWR | O_NONBLOCK);
    if (fd == -1)
        ERR("open fifo");
    upewnij_sie_ze_fifo(fd, sciezka);
    return fd;
}

/* B) prawdziwy czytelnik — O_RDONLY | O_NONBLOCK (open sie nie blokuje).
 *    read(): n>0 dane | n==0 EOF (wszyscy pisarze odeszli -> zamknij fd)
 *            n==-1 && errno==EAGAIN -> chwilowo pusto, NIE zamykaj. */
int otworz_fifo_czytelnik(const char *sciezka)
{
    int fd = open(sciezka, O_RDONLY | O_NONBLOCK);
    if (fd == -1)
        ERR("open fifo");
    upewnij_sie_ze_fifo(fd, sciezka);
    return fd;
}

/* C) pisarz — gdy czytelnika jeszcze nie ma, open(O_WRONLY|O_NONBLOCK) daje ENXIO.
 *    Probuje max_prob razy co odstep_ms. Ustaw tez signal(SIGPIPE, SIG_IGN),
 *    zeby write() do porzuconego FIFO dal EPIPE zamiast zabic proces. */
int otworz_fifo_pisarz(const char *sciezka, int max_prob, int odstep_ms)
{
    for (int i = 0; i < max_prob; i++)
    {
        int fd = open(sciezka, O_WRONLY | O_NONBLOCK);
        if (fd != -1)
        {
            upewnij_sie_ze_fifo(fd, sciezka);
            return fd;
        }
        if (errno != ENXIO)
            ERR("open fifo");
        usleep(odstep_ms * 1000);
    }
    fprintf(stderr, "nikt nie czyta z %s\n", sciezka);
    exit(EXIT_FAILURE);
}
