#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/stat.h>

/* Wklej nad main. Patrzy przez stat, wiec idzie za symlinkiem (rozmiar ksiazki,
 * a nie dlugosc samego linku).
 *  1  rozmiar sie zgadza
 *  0  pliku nie ma
 * -2  plik jest, rozmiar inny (*faktyczny ustawione, moze byc NULL)
 * -1  inny blad
 */
int porownaj_rozmiar(const char *sciezka, uint32_t oczekiwany, off_t *faktyczny)
{
    struct stat st;
    if (stat(sciezka, &st) == -1) {
        if (errno == ENOENT)
            return 0;
        return -1;
    }
    if (faktyczny != NULL)
        *faktyczny = st.st_size;
    if (st.st_size != (off_t)oczekiwany)
        return -2;
    return 1;
}
