#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#include <stdlib.h>
#include <string.h>

/* Wklej nad main. Dzieli napis po spacjach. *ile dostaje liczbe kawalkow.
 * Wynik to tablica napisow do oddania w zwolnij_napisy. Przy bledzie NULL.
 * Kawalkow moze byc co najwyzej 256.
 */
char **podziel_napis(const char *tekst, int *ile)
{
    char *kopia = strdup(tekst);
    if (kopia == NULL)
        return NULL;

    char *tymczasowe[256];
    *ile = 0;
    char *save = NULL;
    for (char *tok = strtok_r(kopia, " ", &save); tok != NULL; tok = strtok_r(NULL, " ", &save)) {
        if (*ile >= 256) {
            for (int i = 0; i < *ile; i++)
                free(tymczasowe[i]);
            free(kopia);
            return NULL;
        }
        tymczasowe[*ile] = strdup(tok);
        if (tymczasowe[*ile] == NULL) {
            for (int i = 0; i < *ile; i++)
                free(tymczasowe[i]);
            free(kopia);
            return NULL;
        }
        (*ile)++;
    }

    char **wynik = malloc(sizeof(char *) * (size_t)(*ile > 0 ? *ile : 1));
    if (wynik == NULL) {
        for (int i = 0; i < *ile; i++)
            free(tymczasowe[i]);
        free(kopia);
        return NULL;
    }
    for (int i = 0; i < *ile; i++)
        wynik[i] = tymczasowe[i];

    free(kopia);
    return wynik;
}
