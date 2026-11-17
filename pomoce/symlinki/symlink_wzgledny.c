#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* Wklej nad main RAZEM z wzgledna_sciezka (ten plik ja wola).
 * Tworzy link "nazwa_linku" w katalog_linku. Cel jest wzgledny.
 * nazwa_linku nie moze zawierac '/'. Gdy nazwa juz jest, zwraca -1 i EEXIST.
 */
char *wzgledna_sciezka(const char *z_katalogu, const char *do_pliku);

int symlink_wzgledny(const char *katalog_linku, const char *cel, const char *nazwa_linku)
{
    if (katalog_linku == NULL || cel == NULL || nazwa_linku == NULL || nazwa_linku[0] == '\0' ||
        strchr(nazwa_linku, '/') != NULL) {
        errno = EINVAL;
        return -1;
    }

    char *wzgledna = wzgledna_sciezka(katalog_linku, cel);
    if (wzgledna == NULL)
        return -1;

    int katalog_fd = open(katalog_linku, O_RDONLY | O_DIRECTORY);
    if (katalog_fd == -1) {
        int zachowany = errno;
        free(wzgledna);
        errno = zachowany;
        return -1;
    }

    int rc = symlinkat(wzgledna, katalog_fd, nazwa_linku);
    int zachowany = errno;
    close(katalog_fd);
    free(wzgledna);
    errno = zachowany;
    return rc;
}
