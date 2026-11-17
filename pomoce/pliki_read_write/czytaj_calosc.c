#include <errno.h>
#include <stddef.h>
#include <unistd.h>

/* Wklej nad main. read() w petli, az zbierze dokladnie n bajtow.
 * Koniec pliku w srodku bufora to blad (errno = EIO). Zwraca 0 albo -1.
 */
int czytaj_calosc(int fd, void *buf, size_t n)
{
    char *p = buf;
    size_t zebrane = 0;
    while (zebrane < n) {
        ssize_t chunk = read(fd, p + zebrane, n - zebrane);
        if (chunk < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (chunk == 0) {
            errno = EIO;
            return -1;
        }
        zebrane += (size_t)chunk;
    }
    return 0;
}
