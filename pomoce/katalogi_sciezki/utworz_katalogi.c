#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* Wklej nad main. Tworzy cala sciezke katalogow, razem z ostatnim czlonem.
 * Istniejacy katalog nie jest bledem. "a/b/c" tworzy a, a/b i a/b/c.
 */
int utworz_katalogi(const char *sciezka)
{
    char *kopia = strdup(sciezka);
    if (kopia == NULL)
        return -1;

    for (char *p = kopia + 1; *p != '\0'; p++) {
        if (*p != '/')
            continue;
        *p = '\0';
        if (mkdir(kopia, 0755) != 0) {
            if (errno != EEXIST) {
                free(kopia);
                return -1;
            }
            struct stat st;
            if (stat(kopia, &st) == -1 || !S_ISDIR(st.st_mode)) {
                free(kopia);
                return -1;
            }
        }
        *p = '/';
    }

    if (mkdir(kopia, 0755) != 0) {
        if (errno != EEXIST) {
            free(kopia);
            return -1;
        }
        struct stat st;
        if (stat(kopia, &st) == -1 || !S_ISDIR(st.st_mode)) {
            free(kopia);
            return -1;
        }
    }

    free(kopia);
    return 0;
}
