#define _XOPEN_SOURCE 700
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* (baza_wiedzy.h, lab2 ex2) opendir/readdir/closedir z callbackiem — pomija "." i "..".
 * Dla KAZDEGO wpisu (plik, katalog, link) wola cb(sciezka, nazwa, lstat, dane).
 * glebokosc: 1 = tylko ten katalog, 2 = + podkatalogi, ..., -1 = bez limitu.
 * Linki NIE sa sledzone (lstat), wiec nie ma petli.
 *
 *   void wypisz(const char *p, const char *n, const struct stat *st, void *d)
 *   { if (S_ISREG(st->st_mode)) printf("%s %lld\n", n, (long long)st->st_size); }
 *   dla_kazdego_wpisu(".", 2, wypisz, NULL);
 */
typedef void (*wpis_cb)(const char *sciezka, const char *nazwa, const struct stat *st, void *dane);

void dla_kazdego_wpisu(const char *katalog, int glebokosc, wpis_cb cb, void *dane)
{
    DIR *dir = opendir(katalog);
    if (dir == NULL)
        ERR("opendir");
    struct dirent *e;
    for (errno = 0; (e = readdir(dir)) != NULL; errno = 0)
    {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
            continue;
        char sciezka[4096];
        snprintf(sciezka, sizeof(sciezka), "%s/%s", katalog, e->d_name);
        struct stat st;
        if (lstat(sciezka, &st))
            ERR("lstat");
        cb(sciezka, e->d_name, &st, dane);
        if (S_ISDIR(st.st_mode) && glebokosc != 1)
            dla_kazdego_wpisu(sciezka, glebokosc - 1, cb, dane);
    }
    if (errno != 0)
        ERR("readdir");
    if (closedir(dir))
        ERR("closedir");
}
