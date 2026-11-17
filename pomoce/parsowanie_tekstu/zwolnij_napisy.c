#include <stdlib.h>

/* Wklej nad main. Zwalnia tablice z podziel_napis. */
void zwolnij_napisy(char **napisy, int ile)
{
    for (int i = 0; i < ile; i++)
        free(napisy[i]);
    free(napisy);
}
