#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#include <ftw.h>

/* Wklej caly plik nad main. nftw nie umie przyjac Twoich danych, dlatego
 * adapter jest w tym samym pliku — kopiujesz go razem z przejdz_nftw.
 * Do callbacka trafiaja tylko zwykle pliki. Symlinki sa pomijane (FTW_PHYS).
 * Nie wolaj przejdz_nftw z wnetrza callbacka.
 */
typedef int (*plik_nftw_cb)(const char *sciezka, const char *nazwa, const struct stat *st, void *dane);

static plik_nftw_cb nftw_cb;
static void *nftw_dane;

static int nftw_adapter(const char *path, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if (typeflag != FTW_F)
        return 0;
    return nftw_cb(path, path + ftwbuf->base, sb, nftw_dane);
}

int przejdz_nftw(const char *katalog, plik_nftw_cb cb, void *dane)
{
    nftw_cb = cb;
    nftw_dane = dane;
    return nftw(katalog, nftw_adapter, 32, FTW_PHYS);
}
