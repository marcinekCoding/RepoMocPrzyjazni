#include <errno.h>
#include <sys/stat.h>

/* Wklej nad main. Tworzy katalog. Gdy pod sciezka jest juz katalog, zwraca 0.
 * Gdy jest tam zwykly plik, zwraca -1. Do podkatalogow gatunku.
 */
int mkdir_jesli_trzeba(const char *sciezka, mode_t tryb)
{
    if (mkdir(sciezka, tryb) == 0)
        return 0;
    if (errno != EEXIST)
        return -1;

    struct stat st;
    if (stat(sciezka, &st) == -1)
        return -1;
    if (!S_ISDIR(st.st_mode)) {
        errno = EEXIST;
        return -1;
    }
    return 0;
}
