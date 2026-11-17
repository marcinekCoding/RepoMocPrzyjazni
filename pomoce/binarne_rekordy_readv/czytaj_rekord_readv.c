#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

/* Wklej nad main, razem ze struktura. Jeden rekord bazy: 4 bajty rozmiaru
 * i 64 bajty tytulu. tytul[64] jest zawsze zerem.
 * Zwraca 1 (jest rekord), 0 (koniec pliku) albo -1 (blad, urwany rekord).
 */
typedef struct {
    uint32_t rozmiar;
    char tytul[65];
} Rekord68;

int czytaj_rekord_readv(int fd, Rekord68 *rekord)
{
    uint32_t rozmiar = 0;
    char tytul[64];
    memset(tytul, 0, sizeof(tytul));

    struct iovec iov[2];
    iov[0].iov_base = &rozmiar;
    iov[0].iov_len = 4;
    iov[1].iov_base = tytul;
    iov[1].iov_len = 64;

    size_t zebrane = 0;
    int indeks = 0;
    while (zebrane < 68) {
        ssize_t chunk = readv(fd, iov + indeks, 2 - indeks);
        if (chunk < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (chunk == 0) {
            if (zebrane == 0)
                return 0;
            errno = EIO;
            return -1;
        }

        zebrane += (size_t)chunk;
        size_t zostalo = (size_t)chunk;
        while (zostalo > 0 && indeks < 2) {
            if (zostalo < iov[indeks].iov_len) {
                iov[indeks].iov_base = (char *)iov[indeks].iov_base + zostalo;
                iov[indeks].iov_len -= zostalo;
                zostalo = 0;
            } else {
                zostalo -= iov[indeks].iov_len;
                indeks++;
            }
        }
    }

    memset(rekord, 0, sizeof(*rekord));
    rekord->rozmiar = rozmiar;
    memcpy(rekord->tytul, tytul, 64);
    rekord->tytul[64] = '\0';
    return 1;
}
