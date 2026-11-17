#include <string.h>

/* Wklej nad main. Zwraca wskaznik na sama nazwe, w srodku podanego napisu.
 * Tego wskaznika sie nie zwalnia. "library/room/book.txt" -> "book.txt".
 */
const char *nazwa_pliku(const char *sciezka)
{
    const char *slash = strrchr(sciezka, '/');
    if (slash == NULL)
        return sciezka;
    return slash + 1;
}
