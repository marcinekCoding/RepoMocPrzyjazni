#define _XOPEN_SOURCE 700
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* (baza_wiedzy.h) Operacje na calym pliku naraz. Samodzielne — wklej caly plik. */

/* Wczytuje caly plik do pamieci, z '\0' na koncu (trzeba free).
 * NULL gdy pliku nie ma albo to katalog. *rozmiar moze byc NULL. */
char *wczytaj_plik(const char *sciezka, size_t *rozmiar)
{
    int fd = open(sciezka, O_RDONLY);
    if (fd < 0)
    {
        if (errno == ENOENT || errno == ENOTDIR || errno == EISDIR)
            return NULL;
        ERR("open");
    }
    struct stat st;
    if (fstat(fd, &st))
        ERR("fstat");
    if (S_ISDIR(st.st_mode))
    {
        close(fd);
        return NULL;
    }
    char *tresc = malloc(st.st_size + 1);
    if (!tresc)
        ERR("malloc");
    size_t zebrane = 0;
    while (zebrane < (size_t)st.st_size)
    {
        ssize_t n = read(fd, tresc + zebrane, st.st_size - zebrane);
        if (n < 0)
        {
            if (errno == EINTR)
                continue;
            ERR("read");
        }
        if (n == 0)
            break;
        zebrane += n;
    }
    tresc[zebrane] = '\0';
    if (rozmiar)
        *rozmiar = zebrane;
    close(fd);
    return tresc;
}

/* Tworzy/nadpisuje plik z trescia. perm ustawione DOKLADNIE (fchmod omija umask).
 * -1 gdy brak katalogu nadrzednego albo sciezka to katalog. */
int zapisz_plik(const char *sciezka, const char *tresc, mode_t perm)
{
    int fd = open(sciezka, O_WRONLY | O_CREAT | O_TRUNC, perm);
    if (fd < 0)
    {
        if (errno == EISDIR || errno == ENOENT || errno == ENOTDIR)
            return -1;
        ERR("open");
    }
    if (fchmod(fd, perm))
        ERR("fchmod");
    size_t n = strlen(tresc), off = 0;
    while (off < n)
    {
        ssize_t w = write(fd, tresc + off, n - off);
        if (w < 0)
            ERR("write");
        off += w;
    }
    close(fd);
    return 0;
}

/* Dopisuje na koniec (tworzy 0644, jesli nie ma). O_APPEND = zawsze na koniec. */
int dopisz_do_pliku(const char *sciezka, const char *tresc)
{
    int fd = open(sciezka, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
        return -1;
    size_t n = strlen(tresc), off = 0;
    while (off < n)
    {
        ssize_t w = write(fd, tresc + off, n - off);
        if (w < 0)
            ERR("write");
        off += w;
    }
    close(fd);
    return 0;
}

/* Kopiuje plik src -> dst z tymi samymi uprawnieniami. -1 gdy src nie istnieje. */
int kopiuj_plik(const char *src, const char *dst)
{
    int in = open(src, O_RDONLY);
    if (in < 0)
    {
        if (errno == ENOENT)
            return -1;
        ERR("open");
    }
    struct stat st;
    if (fstat(in, &st))
        ERR("fstat");
    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, st.st_mode & 0777);
    if (out < 0)
        ERR("open");
    char buf[4096];
    ssize_t n;
    while ((n = read(in, buf, sizeof(buf))) > 0)
    {
        ssize_t off = 0;
        while (off < n)
        {
            ssize_t w = write(out, buf + off, n - off);
            if (w < 0)
                ERR("write");
            off += w;
        }
    }
    if (n < 0)
        ERR("read");
    close(in);
    close(out);
    return 0;
}
