#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Dzieli napis po DOWOLNYM ze znakow w separatory (" \t", ":", ",", ";").
 * Bez limitu 256 (tablica rosnie). *ile = liczba kawalkow.
 * Wynik zwolnij przez zwolnij_napisy (zwolnij_napisy.c). Przy bledzie NULL.
 *
 *   int n; char **cz = dziel_po_separatorach("a:b,,c", ":,", &n);  // "a" "b" "c"
 */
char **dziel_po_separatorach(const char *tekst, const char *separatory, int *ile)
{
    char *kopia = strdup(tekst);
    if (kopia == NULL)
        return NULL;
    int pojemnosc = 8;
    char **wynik = malloc(sizeof(char *) * pojemnosc);
    if (wynik == NULL)
    {
        free(kopia);
        return NULL;
    }
    *ile = 0;
    char *save = NULL;
    for (char *tok = strtok_r(kopia, separatory, &save); tok; tok = strtok_r(NULL, separatory, &save))
    {
        if (*ile == pojemnosc)
        {
            pojemnosc *= 2;
            char **nowy = realloc(wynik, sizeof(char *) * pojemnosc);
            if (nowy == NULL)
                break;
            wynik = nowy;
        }
        wynik[(*ile)++] = strdup(tok);
    }
    free(kopia);
    return wynik;
}
