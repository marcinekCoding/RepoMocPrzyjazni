#define _XOPEN_SOURCE 700
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* (baza_wiedzy.h) Szybkie pytania o plik. Uzywaja lstat — link to link,
 * NIE plik/katalog, na ktory wskazuje. Zeby isc za linkiem: zamien lstat na stat. */

/* Czy cokolwiek istnieje (takze zepsuty link). Inaczej niz access(F_OK), ktore idzie za linkiem. */
bool istnieje(const char *p)
{
    struct stat st;
    if (lstat(p, &st) == 0)
        return true;
    if (errno == ENOENT || errno == ENOTDIR)
        return false;
    ERR("lstat");
}

static bool ma_typ(const char *p, mode_t typ)
{
    struct stat st;
    return lstat(p, &st) == 0 && (st.st_mode & S_IFMT) == typ;
}

bool to_katalog(const char *p) { return ma_typ(p, S_IFDIR); }
bool to_zwykly_plik(const char *p) { return ma_typ(p, S_IFREG); }
bool to_link(const char *p) { return ma_typ(p, S_IFLNK); }
bool to_fifo(const char *p) { return ma_typ(p, S_IFIFO); }

/* Rozmiar w bajtach, -1 gdy nie ma. */
off_t rozmiar_pliku(const char *p)
{
    struct stat st;
    if (lstat(p, &st) == -1)
        return -1;
    return st.st_size;
}

/* Uprawnienia jako liczba osemkowa (np. 0644): printf("%o", uprawnienia(p)); */
mode_t uprawnienia(const char *p)
{
    struct stat st;
    if (lstat(p, &st) == -1)
        ERR("lstat");
    return st.st_mode & 0777;
}

/* Czy ktos moze to uruchomic (dowolny bit x). */
bool wykonywalny(const char *p)
{
    struct stat st;
    return lstat(p, &st) == 0 && S_ISREG(st.st_mode) && (st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH));
}

/* Nazwa typu do wypisania. */
const char *nazwa_typu(mode_t m)
{
    if (S_ISREG(m)) return "plik";
    if (S_ISDIR(m)) return "katalog";
    if (S_ISLNK(m)) return "link";
    if (S_ISFIFO(m)) return "fifo";
    return "inny";
}
