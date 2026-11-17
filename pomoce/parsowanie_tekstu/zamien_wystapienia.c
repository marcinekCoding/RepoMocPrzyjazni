#include <stddef.h>
#include <string.h>

/* Zamienia w buforze (np. zmapowanym plikiem) kazde wystapienie `stare` na `nowe`.
 * WAZNE: stare i nowe musza miec TA SAMA dlugosc (plik nie zmienia rozmiaru).
 * Bufor nie musi miec '\0' (dziala na bajtach). Zwraca liczbe zamian.
 *
 *   int ile = zamien_wystapienia(mapa, 2048, "raw~berries", "raspberries");
 */
int zamien_wystapienia(char *buf, size_t rozmiar, const char *stare, const char *nowe)
{
    size_t len = strlen(stare);
    if (len == 0 || len != strlen(nowe))
        return -1;
    int zamian = 0;
    size_t i = 0;
    while (i + len <= rozmiar) /* nie wychodz za bufor przy memcmp */
    {
        if (memcmp(buf + i, stare, len) == 0)
        {
            memcpy(buf + i, nowe, len);
            zamian++;
            i += len;
        }
        else
            i++;
    }
    return zamian;
}

/* Szuka pierwszego wystapienia wzorca w buforze bez '\0' (jak strstr dla bajtow).
 * Zwraca indeks albo -1. */
long znajdz_w_buforze(const char *buf, size_t rozmiar, const char *wzorzec)
{
    size_t len = strlen(wzorzec);
    for (size_t i = 0; len && i + len <= rozmiar; i++)
        if (memcmp(buf + i, wzorzec, len) == 0)
            return (long)i;
    return -1;
}
