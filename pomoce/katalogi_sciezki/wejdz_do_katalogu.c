#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

/* Wklej nad main. Zapamietuje cwd w *poprzedni (trzeba oddac do wroc_do_katalogu)
 * i robi chdir. Zwraca 0 albo -1.
 */
int wejdz_do_katalogu(const char *sciezka, char **poprzedni)
{
    char *tu = getcwd(NULL, 0);
    if (tu == NULL)
        return -1;
    if (chdir(sciezka) == -1) {
        int zachowany = errno;
        free(tu);
        errno = zachowany;
        return -1;
    }
    *poprzedni = tu;
    return 0;
}
