#include <errno.h>
#include <stddef.h>
#include <unistd.h>

/* Wklej nad main. write() w petli, az zejdzie caly bufor. Powtarza po EINTR.
 * Zwraca 0 albo -1. Do niskopoziomowego zapisu, zamiast jednego write().
 */
int zapisz_calosc(int fd, const void *buf, size_t n)
{
    const char *p = buf;
    while (n > 0) {
        ssize_t zapisane = write(fd, p, n);
        if (zapisane < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (zapisane == 0) {
            errno = EIO;
            return -1;
        }
        p += zapisane;
        n -= (size_t)zapisane;
    }
    return 0;
}
