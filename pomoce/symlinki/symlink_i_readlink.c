#define _XOPEN_SOURCE 700
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* (baza_wiedzy.h, lab2 ex4) Tworzenie i czytanie linkow symbolicznych.
 *
 * WAZNE: wzgledny cel linku jest liczony od KATALOGU LINKU, nie od cwd!
 *   link index/by-genre/fantasy/Hobbit -> "../../by-title/Hobbit"
 * Najprosciej: cel bezwzgledny (realpath) albo symlink_wzgledny.c.
 */

/* Tworzy link `link` -> `cel`. nadpisz=true usuwa istniejacy wpis.
 * Zwraca 0, -1 gdy juz istnieje i nadpisz=false. */
int utworz_symlink(const char *cel, const char *link, bool nadpisz)
{
    if (symlink(cel, link) == 0)
        return 0;
    if (errno != EEXIST)
        ERR("symlink");
    if (!nadpisz)
        return -1;
    if (unlink(link))
        ERR("unlink");
    if (symlink(cel, link))
        ERR("symlink");
    return 0;
}

/* Gdzie wskazuje link (trzeba free). NULL gdy to nie link albo go nie ma.
 * readlink NIE dopisuje '\0' — robimy to sami. */
char *gdzie_wskazuje(const char *link)
{
    char buf[PATH_MAX];
    ssize_t n = readlink(link, buf, sizeof(buf) - 1);
    if (n < 0)
    {
        if (errno == ENOENT || errno == EINVAL || errno == ENOTDIR)
            return NULL;
        ERR("readlink");
    }
    buf[n] = '\0';
    return strdup(buf);
}

/* Link z celem BEZWZGLEDNYM (dziala niezaleznie od tego, gdzie lezy link).
 * cel musi istniec. */
void symlink_bezwzgledny(const char *cel, const char *link)
{
    char abs[PATH_MAX];
    if (realpath(cel, abs) == NULL)
        ERR("realpath");
    if (symlink(abs, link) == -1)
        ERR("symlink");
}
