#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Wklej nad main. Szuka w pliku linii "klucz:wartosc".
 * Zwraca nowy napis (trzeba free) albo NULL, gdy klucza nie ma.
 * Linia bez ':' jest pomijana. Pierwsze trafienie wygrywa.
 *
 * char *author = meta_pole(argv[1], "author");
 * printf("author: %s\n", author ? author : "missing!");
 * free(author);
 */
char *meta_pole(const char *sciezka, const char *klucz)
{
    FILE *plik = fopen(sciezka, "r");
    if (plik == NULL)
        return NULL;

    char *linia = NULL;
    size_t pojemnosc = 0;
    char *wynik = NULL;

    while (getline(&linia, &pojemnosc, plik) != -1) {
        char *dwukropek = strchr(linia, ':');
        if (dwukropek == NULL || dwukropek == linia)
            continue;

        *dwukropek = '\0';
        if (strcmp(linia, klucz) != 0)
            continue;

        char *wartosc = dwukropek + 1;
        size_t n = strlen(wartosc);
        while (n > 0 && (wartosc[n - 1] == '\n' || wartosc[n - 1] == '\r'))
            wartosc[--n] = '\0';

        wynik = strdup(wartosc);
        break;
    }

    free(linia);
    fclose(plik);
    return wynik;
}
