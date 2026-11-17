#include <string.h>

/* Wklej nad main. Obcina napis do max_bajtow bajtow (nie znakow).
 * s musi dawac sie pisac, czyli pochodzic z malloc albo strdup.
 * NULL przechodzi bez zmian. Przyklad: obetnij_do_bajtow(tytul, 64);
 */
char *obetnij_do_bajtow(char *s, size_t max_bajtow)
{
    if (s == NULL)
        return NULL;
    if (strlen(s) > max_bajtow)
        s[max_bajtow] = '\0';
    return s;
}
