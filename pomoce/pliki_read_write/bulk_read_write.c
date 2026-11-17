#include <errno.h>
#include <unistd.h>

/* (baza_wiedzy.h) read/write, ktore nie odpuszczaja: powtarzaja po czesciowym
 * odczycie/zapisie i po EINTR. Zwracaja liczbe bajtow albo -1 (errno ustawione).
 * bulk_read moze zwrocic mniej niz count tylko przy EOF (0 = od razu EOF).
 * (wersje zwracajace 0/-1 i traktujace EOF jako blad: czytaj_calosc.c / zapisz_calosc.c)
 */
ssize_t bulk_read(int fd, char *buf, size_t count)
{
    ssize_t len = 0;
    while (count > 0)
    {
        ssize_t c = read(fd, buf, count);
        if (c < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (c == 0)
            break;
        buf += c;
        len += c;
        count -= c;
    }
    return len;
}

ssize_t bulk_write(int fd, const char *buf, size_t count)
{
    ssize_t len = 0;
    while (count > 0)
    {
        ssize_t c = write(fd, buf, count);
        if (c < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }
        buf += c;
        len += c;
        count -= c;
    }
    return len;
}
