/* multiplex_read_lines — CALA petla poll + buforowanie + dzielenie na linie w jednym wywolaniu.
 * (z Twojej sciagawki, sekcja "funkcje awaryjne").
 * fds: JUZ otwarte deskryptory z O_NONBLOCK; n: ile; timeout_ms: -1 = czekaj w nieskonczonosc
 * handler: wolany raz na kazda pelna linie (indeks fd, linia bez '\n')
 * zwraca 0 gdy wszystkie fd daly EOF, -1 gdy timeout/blad.
 *
 *   void moj_handler(int ktory, char *linia) { printf("%d: %s\n", ktory, linia); }
 *   int fds[argc-2];
 *   for (int i = 0; i < argc-2; i++) fds[i] = open(argv[i+2], O_RDONLY | O_NONBLOCK);
 *   multiplex_read_lines(fds, argc-2, 4000, moj_handler);
 *
 * Potrzebujesz w handlerze np. mapy z mmap? Zrob ja zmienna globalna.
 * UWAGA: z O_RDWR na FIFO nigdy nie bedzie EOF -> wyjdzie dopiero przez timeout.
 */
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

// fds: tablica JUZ otwartych deskryptorow (musza miec O_NONBLOCK!)
// n: ile fd w tablicy
// timeout_ms: po ilu ms braku JAKIEJKOLWIEK aktywnosci funkcja sie podda (-1 = czekaj wiecznie)
// handler: wywolywany raz na kazda kompletna linie; dostaje indeks fd w tablicy i tresc linii
// zwraca: 0 gdy wszystkie fd daly EOF, -1 gdy wyszlo przez timeout
int multiplex_read_lines(int* fds, int n, int timeout_ms,
                          void (*handler)(int ktory_fd, char* linia))
{
    struct pollfd pfds[n];
    char** bufory = malloc(n * sizeof(char*));
    int* dlugosc = calloc(n, sizeof(int));
    int pojemnosc = 4096;
    for (int i = 0; i < n; i++)
    {
        pfds[i].fd = fds[i];
        pfds[i].events = POLLIN;
        bufory[i] = malloc(pojemnosc);
    }

    int aktywne = n;
    int wynik = 0;
    while (aktywne > 0)
    {
        int ret = poll(pfds, n, timeout_ms);
        if (ret == -1)
        {
            if (errno == EINTR) continue;
            wynik = -1;
            break;
        }
        if (ret == 0) { wynik = -1; break; }

        for (int i = 0; i < n; i++)
        {
            if (pfds[i].fd == -1) continue;
            if (!(pfds[i].revents & (POLLIN | POLLHUP))) continue;

            char tmp[1024];
            int r = read(pfds[i].fd, tmp, sizeof(tmp));
            if (r == 0 || (r == -1 && errno != EAGAIN))
            {
                pfds[i].fd = -1;
                aktywne--;
                continue;
            }
            if (r == -1) continue;   // EAGAIN - nic teraz nie ma, nie blad

            while (dlugosc[i] + r > pojemnosc)
            {
                pojemnosc *= 2;
                bufory[i] = realloc(bufory[i], pojemnosc);
            }
            memcpy(bufory[i] + dlugosc[i], tmp, r);
            dlugosc[i] += r;

            int start = 0;
            for (int j = 0; j < dlugosc[i]; j++)
            {
                if (bufory[i][j] == '\n')
                {
                    bufory[i][j] = '\0';
                    handler(i, bufory[i] + start);
                    start = j + 1;
                }
            }
            int reszta = dlugosc[i] - start;
            memmove(bufory[i], bufory[i] + start, reszta);
            dlugosc[i] = reszta;
        }
    }

    for (int i = 0; i < n; i++) free(bufory[i]);
    free(bufory);
    free(dlugosc);
    return wynik;
}
