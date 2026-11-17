#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

#define SZEROKOSC 64 /* 63 znaki + '\n' */

/* (sop-cake) Plik = LINIE stalej szerokosci (np. 32 linie po 64 bajty, dopelnione spacjami).
 * Wpisuje tekst do linii nr `nr`: pozycja = nr * SZEROKOSC (pwrite, nie trzeba lseek).
 * Tekst dluzszy niz 63 jest obcinany, krotszy dopelniany spacjami, na koncu '\n'.
 * Do utworzenia pliku: otworz_lub_utworz(path, 32*64, ' ') (wtedy bez '\n' — albo
 * wpisz wszystkie linie pusta tresc: for (i..32) wpisz_linie(fd, i, "", 0);)
 */
void wpisz_linie(int fd, int nr, const char *tekst, size_t dlugosc)
{
    char linia[SZEROKOSC];
    memset(linia, ' ', SZEROKOSC);
    if (dlugosc > SZEROKOSC - 1)
        dlugosc = SZEROKOSC - 1;
    memcpy(linia, tekst, dlugosc);
    linia[SZEROKOSC - 1] = '\n';
    if (pwrite(fd, linia, SZEROKOSC, (off_t)nr * SZEROKOSC) != SZEROKOSC)
        ERR("pwrite");
}

/* Czyta linie nr `nr` do buf (min. SZEROKOSC bajtow), obcina spacje z konca, dodaje '\0'.
 * Zwraca dlugosc tekstu (0 = linia pusta). */
int czytaj_linie(int fd, int nr, char *buf)
{
    if (pread(fd, buf, SZEROKOSC, (off_t)nr * SZEROKOSC) != SZEROKOSC)
        ERR("pread");
    int n = SZEROKOSC - 1;
    while (n > 0 && (buf[n - 1] == ' ' || buf[n - 1] == '\0'))
        n--;
    buf[n] = '\0';
    return n;
}
