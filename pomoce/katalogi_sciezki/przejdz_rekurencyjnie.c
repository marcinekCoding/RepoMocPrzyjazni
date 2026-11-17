#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

/* Wklej nad main, razem z typedefem. Schodzi w katalogi. Do callbacka trafiaja
 * tylko zwykle pliki. Symlinkow nie otwiera. Callback zwraca 0, zeby isc dalej.
 * Sciezka dluzsza niz 4095 znakow konczy przejscie bledem.
 */
typedef int (*plik_rek_cb)(const char *sciezka, const char *nazwa, const struct stat *st, void *dane);

int przejdz_rekurencyjnie(const char *katalog, plik_rek_cb cb, void *dane)
{
    DIR *dir = opendir(katalog);
    if (dir == NULL)
        return -1;

    errno = 0;
    struct dirent *wpis;
    while ((wpis = readdir(dir)) != NULL) {
        if (strcmp(wpis->d_name, ".") == 0 || strcmp(wpis->d_name, "..") == 0)
            continue;

        char sciezka[4096];
        int n = snprintf(sciezka, sizeof(sciezka), "%s/%s", katalog, wpis->d_name);
        if (n < 0 || (size_t)n >= sizeof(sciezka)) {
            closedir(dir);
            errno = ENAMETOOLONG;
            return -1;
        }

        struct stat st;
        if (lstat(sciezka, &st) == -1) {
            closedir(dir);
            return -1;
        }

        int rc = 0;
        if (S_ISREG(st.st_mode))
            rc = cb(sciezka, wpis->d_name, &st, dane);
        else if (S_ISDIR(st.st_mode))
            rc = przejdz_rekurencyjnie(sciezka, cb, dane);

        if (rc != 0) {
            closedir(dir);
            return rc;
        }
        errno = 0;
    }

    int blad = errno;
    closedir(dir);
    if (blad != 0) {
        errno = blad;
        return -1;
    }
    return 0;
}
