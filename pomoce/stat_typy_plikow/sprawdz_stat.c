#include <sys/stat.h>

/* Wklej nad main. Wypelnia *st.
 * idz_za_linkiem = 1 oznacza stat (rozmiar pliku, na ktory wskazuje link).
 * idz_za_linkiem = 0 oznacza lstat (sam link albo sam plik, bez schodzenia).
 * Zwraca 0 albo -1. Brak pliku: -1 i errno == ENOENT.
 *
 * if (sprawdz_stat(path, &st, 0) == 0 && S_ISREG(st.st_mode)) { ... }
 */
int sprawdz_stat(const char *sciezka, struct stat *st, int idz_za_linkiem)
{
    if (idz_za_linkiem)
        return stat(sciezka, st);
    return lstat(sciezka, st);
}
