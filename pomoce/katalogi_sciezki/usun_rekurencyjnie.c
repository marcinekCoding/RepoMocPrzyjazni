#define _XOPEN_SOURCE 700
#include <errno.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* (baza_wiedzy.h) Jak `rm -rf`: usuwa plik, link (nie cel) albo cale drzewo katalogow.
 * FTW_DEPTH = najpierw zawartosc, potem katalog; FTW_PHYS = nie wchodz w linki.
 * Zwraca 0, -1 gdy sciezki nie ma. */
static int usun_cb(const char *path, const struct stat *s, int typ, struct FTW *f)
{
    (void)s; (void)typ; (void)f;
    if (remove(path))
        ERR("remove");
    return 0;
}

int usun_rekurencyjnie(const char *sciezka)
{
    struct stat st;
    if (lstat(sciezka, &st) == -1)
    {
        if (errno == ENOENT)
            return -1;
        ERR("lstat");
    }
    if (nftw(sciezka, usun_cb, 20, FTW_DEPTH | FTW_PHYS))
        ERR("nftw");
    return 0;
}
