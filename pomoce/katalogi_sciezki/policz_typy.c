#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

/* Wklej nad main. Liczy zawartosc jednego katalogu, bez schodzenia w glab.
 * Pomija "." i "..". Zwraca 0 albo -1.
 */
int policz_typy(const char *katalog, int *zwykle, int *katalogi, int *linki)
{
    *zwykle = 0;
    *katalogi = 0;
    *linki = 0;

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

        if (S_ISREG(st.st_mode))
            (*zwykle)++;
        else if (S_ISDIR(st.st_mode))
            (*katalogi)++;
        else if (S_ISLNK(st.st_mode))
            (*linki)++;
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
