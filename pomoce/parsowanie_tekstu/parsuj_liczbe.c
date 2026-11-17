#include <stdbool.h>
#include <string.h>

/* Liczba z FRAGMENTU tekstu [start, koniec) — bez atoi na buforze bez '\0'.
 * Zwraca -1, gdy jest cos innego niz cyfra albo fragment pusty.
 *   int nr = parsuj_liczbe(linia, 0, 2);   // "07:abc" -> 7
 */
int parsuj_liczbe(const char *s, int start, int koniec)
{
    if (start >= koniec)
        return -1;
    int wynik = 0;
    for (int i = start; i < koniec; i++)
    {
        if (s[i] < '0' || s[i] > '9')
            return -1;
        wynik = wynik * 10 + (s[i] - '0');
    }
    return wynik;
}

/* Czyta liczbe od *pos az do pierwszego nie-cyfry, przesuwa *pos za nia.
 * Dobre do "G:12,340": czytaj, sprawdz separator, czytaj dalej.
 * Zwraca -1, gdy pod *pos nie ma cyfry. */
long czytaj_liczbe_od(const char *s, int *pos)
{
    if (s[*pos] < '0' || s[*pos] > '9')
        return -1;
    long w = 0;
    while (s[*pos] >= '0' && s[*pos] <= '9')
        w = w * 10 + (s[(*pos)++] - '0');
    return w;
}

/* true, gdy CALY napis to cyfry (i nie jest pusty). */
bool same_cyfry(const char *s)
{
    if (*s == '\0')
        return false;
    for (; *s; s++)
        if (*s < '0' || *s > '9')
            return false;
    return true;
}
