#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <string.h>

/* Dzieli "klucz<SEP>wartosc" na dwa nowe napisy (oba trzeba free).
 * SEP moze miec kilka znakow: "==", ":", "=". Spacje/taby na poczatku sa pomijane,
 * '\n' z konca wartosci usuwany. Zwraca 0, albo -1 gdy brak SEP / pusty klucz.
 *
 *   char *pakiet, *wersja;
 *   if (podziel_klucz_wartosc("numpy==1.2.3", "==", &pakiet, &wersja) == 0) {...}
 *   if (podziel_klucz_wartosc("title:Histories\n", ":", &k, &v) == 0) {...}
 */
int podziel_klucz_wartosc(const char *linia, const char *sep, char **klucz, char **wartosc)
{
    while (*linia == ' ' || *linia == '\t')
        linia++;
    const char *gdzie = strstr(linia, sep);
    if (gdzie == NULL || gdzie == linia)
        return -1;

    size_t dl_klucza = (size_t)(gdzie - linia);
    *klucz = malloc(dl_klucza + 1);
    *wartosc = strdup(gdzie + strlen(sep));
    if (*klucz == NULL || *wartosc == NULL)
    {
        free(*klucz);
        free(*wartosc);
        return -1;
    }
    memcpy(*klucz, linia, dl_klucza);
    (*klucz)[dl_klucza] = '\0';

    size_t n = strlen(*wartosc);
    while (n > 0 && ((*wartosc)[n - 1] == '\n' || (*wartosc)[n - 1] == '\r'))
        (*wartosc)[--n] = '\0';
    return 0;
}
