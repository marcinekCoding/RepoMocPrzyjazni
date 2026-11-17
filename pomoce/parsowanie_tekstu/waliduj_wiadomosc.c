#include <stdio.h>
#include <string.h>

/* Parsowanie wiadomosci z FIFO w stylu labow. linia jest juz bez '\n' i z '\0'.
 *
 * 1) Format "NN:tresc" (sop-merge, sop-cake): dwie cyfry + dwukropek.
 *    Zwraca numer NN w zakresie [min,max] albo -1. *tresc -> za dwukropkiem.
 *
 *    const char *t; int nr = numer_i_tresc(linia, 0, 15, &t);
 *    if (nr < 0) printf("zla wiadomosc: %s\n", linia);
 */
int numer_i_tresc(const char *linia, int min, int max, const char **tresc)
{
    if (strlen(linia) < 3 || linia[0] < '0' || linia[0] > '9' || linia[1] < '0' || linia[1] > '9' ||
        linia[2] != ':')
        return -1;
    int nr = (linia[0] - '0') * 10 + (linia[1] - '0');
    if (nr < min || nr > max)
        return -1;
    if (tresc)
        *tresc = linia + 3;
    return nr;
}

/* 2) Format "X:a,b" (sop-ratking: "G:12,340", "P:...", "A:...").
 *    Litera typu + ':' + dwie liczby rozdzielone przecinkiem.
 *    Zwraca litere typu, albo 0 gdy format zly. a/b ustawiane tylko dla poprawnych.
 *    Liczb moze nie byc (np. "P:") — wtedy *ile_liczb = 0.
 */
char typ_i_dwie_liczby(const char *linia, long *a, long *b, int *ile_liczb)
{
    *ile_liczb = 0;
    if (linia[0] == '\0' || linia[1] != ':')
        return 0;
    char typ = linia[0];
    const char *p = linia + 2;
    if (*p == '\0')
        return typ;

    char reszta;
    int n = sscanf(p, "%ld,%ld%c", a, b, &reszta); /* %c wylapie smieci na koncu */
    if (n == 2)
    {
        *ile_liczb = 2;
        return typ;
    }
    n = sscanf(p, "%ld%c", a, &reszta);
    if (n == 1)
    {
        *ile_liczb = 1;
        return typ;
    }
    return 0;
}

/* Przyklad obslugi:
 *   long a, b; int k;
 *   switch (typ_i_dwie_liczby(linia, &a, &b, &k)) {
 *   case 'G': if (k != 2 || a > 15) goto zle; komory[a].glod += b; break;
 *   case 'P': puts("Raport P"); break;
 *   default: zle: fprintf(stderr, "Szczur bredzi: %s\n", linia);
 *   }
 */
