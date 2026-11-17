#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* (lab2 zad3) Tworzy plik o rozmiarze `rozmiar`: same zera + 10% losowych liter A-Z
 * w losowych miejscach. Uprawnienia DOKLADNIE `perm` (np. 0640) — fchmod omija umask.
 * Na poczatku main: srand(time(NULL));
 *
 *   ./prog -n plik -p 0640 -s 1000   ->  utworz_losowy_plik(n, 1000, 0640);
 * Jesli plik ma byc zawsze swiezy: if (unlink(n) && errno != ENOENT) ERR("unlink");
 */
void utworz_losowy_plik(const char *sciezka, off_t rozmiar, mode_t perm)
{
    int fd = open(sciezka, O_WRONLY | O_CREAT | O_TRUNC, perm);
    if (fd < 0)
        ERR("open");
    if (fchmod(fd, perm))
        ERR("fchmod");
    if (ftruncate(fd, rozmiar)) /* plik od razu ma rozmiar, wypelniony zerami */
        ERR("ftruncate");
    for (off_t i = 0; i < rozmiar / 10; i++)
    {
        char c = 'A' + (char)(i % ('Z' - 'A' + 1));
        if (pwrite(fd, &c, 1, rand() % rozmiar) != 1)
            ERR("pwrite");
    }
    close(fd);
}
