#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

/* Wklej nad main. Wraca do katalogu zapamietanego przez wejdz_do_katalogu
 * i zwalnia ten napis. Zwraca 0 albo -1.
 */
int wroc_do_katalogu(char *poprzedni)
{
    int rc = chdir(poprzedni);
    int zachowany = errno;
    free(poprzedni);
    errno = zachowany;
    return rc;
}
