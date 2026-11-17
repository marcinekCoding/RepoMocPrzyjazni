#include <stdlib.h>
#include <string.h>

/* Wklej nad main. Skleja dwa kawalki sciezki. Wynik trzeba free().
 * char *p = dolacz_sciezki("index/by-genre", gatunek);
 */
char *dolacz_sciezki(const char *path1, const char *path2)
{
    size_t l1 = strlen(path1);
    size_t l2 = strlen(path2);
    int dokleic_slash = (l1 > 0 && path1[l1 - 1] != '/');

    char *wynik = malloc(l1 + l2 + (dokleic_slash ? 2 : 1));
    if (wynik == NULL)
        return NULL;

    memcpy(wynik, path1, l1);
    size_t pozycja = l1;
    if (dokleic_slash)
        wynik[pozycja++] = '/';
    memcpy(wynik + pozycja, path2, l2 + 1);
    return wynik;
}
