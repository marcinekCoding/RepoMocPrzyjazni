#include <string.h>

/* Wklej nad main. Tekst za ostatnia kropka. Brak kropki albo nazwa typu
 * ".gitignore" daje pusty napis "". Wyniku sie nie zwalnia.
 */
const char *rozszerzenie(const char *nazwa)
{
    const char *kropka = strrchr(nazwa, '.');
    if (kropka == NULL || kropka == nazwa)
        return "";
    return kropka + 1;
}
