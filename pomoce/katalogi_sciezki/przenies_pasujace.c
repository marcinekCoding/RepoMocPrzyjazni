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

/* (lab2 bin_dir_test) Przenosi (rename) zwykle pliki z katalogu `skad` do `dokad`,
 * jesli pasuje(nazwa, st) zwraca 1. Katalog docelowy jest tworzony, jesli go nie ma.
 * Zwraca liczbe przeniesionych plikow.
 *
 *   int wykonywalny(const char *n, const struct stat *st)
 *   { return (st->st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0; }
 *   przenies_pasujace(".", "bin", wykonywalny);
 * UWAGA: rename dziala tylko w obrebie jednego systemu plikow (inaczej EXDEV).
 */
int przenies_pasujace(const char *skad, const char *dokad, int (*pasuje)(const char *nazwa, const struct stat *st))
{
    if (mkdir(dokad, 0755) == -1 && errno != EEXIST)
        ERR("mkdir");
    DIR *dir = opendir(skad);
    if (dir == NULL)
        ERR("opendir");
    int ile = 0;
    struct dirent *e;
    for (errno = 0; (e = readdir(dir)) != NULL; errno = 0)
    {
        char z[4096], do_[4096];
        snprintf(z, sizeof(z), "%s/%s", skad, e->d_name);
        struct stat st;
        if (lstat(z, &st) || !S_ISREG(st.st_mode) || !pasuje(e->d_name, &st))
            continue;
        snprintf(do_, sizeof(do_), "%s/%s", dokad, e->d_name);
        if (rename(z, do_))
            ERR("rename");
        ile++;
    }
    if (errno)
        ERR("readdir");
    closedir(dir);
    return ile;
}
