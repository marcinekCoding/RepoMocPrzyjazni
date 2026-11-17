#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

/* Wklej nad main. open() z dokladnymi bitami uprawnien. Na czas wywolania
 * umask jest 0, potem wraca. Bez tego umask scina bity i plik nie ma mode
 * z polecenia. flagi np. O_WRONLY | O_CREAT | O_TRUNC, perm np. 0640.
 */
int otworz_z_uprawnieniami(const char *sciezka, int flagi, mode_t perm)
{
    mode_t poprzednia = umask(0);
    int fd = open(sciezka, flagi, perm);
    int zachowany = errno;
    umask(poprzednia);
    errno = zachowany;
    return fd;
}
