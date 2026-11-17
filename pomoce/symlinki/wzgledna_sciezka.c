#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#include <errno.h>
#include <stdlib.h>
#include <string.h>

/* Wklej nad main. Sciezka z katalogu do istniejacego pliku, np. "../../library/a/book".
 * Oba argumenty musza istniec. Wynik trzeba free(). Przy bledzie NULL.
 */
char *wzgledna_sciezka(const char *z_katalogu, const char *do_pliku)
{
    char *skad = realpath(z_katalogu, NULL);
    char *dokad = realpath(do_pliku, NULL);
    if (skad == NULL || dokad == NULL) {
        int zachowany = errno;
        free(skad);
        free(dokad);
        errno = zachowany;
        return NULL;
    }

    char *czesci_skad[128];
    char *czesci_dokad[128];
    int ile_skad = 0;
    int ile_dokad = 0;
    char *save = NULL;

    for (char *tok = strtok_r(skad, "/", &save); tok != NULL; tok = strtok_r(NULL, "/", &save)) {
        if (ile_skad >= 128) {
            free(skad);
            free(dokad);
            errno = ENAMETOOLONG;
            return NULL;
        }
        czesci_skad[ile_skad++] = tok;
    }

    save = NULL;
    for (char *tok = strtok_r(dokad, "/", &save); tok != NULL; tok = strtok_r(NULL, "/", &save)) {
        if (ile_dokad >= 128) {
            free(skad);
            free(dokad);
            errno = ENAMETOOLONG;
            return NULL;
        }
        czesci_dokad[ile_dokad++] = tok;
    }

    int wspolne = 0;
    while (wspolne < ile_skad && wspolne < ile_dokad &&
           strcmp(czesci_skad[wspolne], czesci_dokad[wspolne]) == 0)
        wspolne++;

    size_t pojemnosc = 2;
    for (int i = wspolne; i < ile_skad; i++)
        pojemnosc += 3;
    for (int i = wspolne; i < ile_dokad; i++)
        pojemnosc += strlen(czesci_dokad[i]) + 1;

    char *wynik = malloc(pojemnosc);
    if (wynik == NULL) {
        free(skad);
        free(dokad);
        return NULL;
    }

    char *p = wynik;
    int cos_jest = 0;
    for (int i = wspolne; i < ile_skad; i++) {
        if (cos_jest)
            *p++ = '/';
        *p++ = '.';
        *p++ = '.';
        cos_jest = 1;
    }
    for (int i = wspolne; i < ile_dokad; i++) {
        if (cos_jest)
            *p++ = '/';
        size_t len = strlen(czesci_dokad[i]);
        memcpy(p, czesci_dokad[i], len);
        p += len;
        cos_jest = 1;
    }
    if (!cos_jest)
        *p++ = '.';
    *p = '\0';

    free(skad);
    free(dokad);
    return wynik;
}
