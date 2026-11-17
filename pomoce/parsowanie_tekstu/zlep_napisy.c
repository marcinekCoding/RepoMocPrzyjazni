#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

/* Wklej nad main. Skleja n napisow w nowy (trzeba free). Zaden nie moze byc NULL.
 * char *p = zlep_napisy(3, "index/", "by-genre/", gatunek);
 */
char *zlep_napisy(int n, ...)
{
    va_list args;
    size_t dlugosc = 0;

    va_start(args, n);
    for (int i = 0; i < n; i++)
        dlugosc += strlen(va_arg(args, char *));
    va_end(args);

    char *wynik = malloc(dlugosc + 1);
    if (wynik == NULL)
        return NULL;
    wynik[0] = '\0';

    va_start(args, n);
    for (int i = 0; i < n; i++)
        strcat(wynik, va_arg(args, char *));
    va_end(args);
    return wynik;
}
