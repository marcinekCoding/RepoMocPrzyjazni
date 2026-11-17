#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* Przepisuje wszystko z fd_in do fd_out az do EOF (cat / kopiowanie, warsztaty2 zad1).
 * NIE printf("%s", buf) — buf nie ma '\0' na koncu! Uzywaj write(..., n).
 *   przepisz_do_eof(fd, STDOUT_FILENO);
 */
void przepisz_do_eof(int fd_in, int fd_out)
{
    char buf[4096];
    ssize_t n;
    while ((n = read(fd_in, buf, sizeof(buf))) > 0)
    {
        ssize_t off = 0;
        while (off < n)
        {
            ssize_t w = write(fd_out, buf + off, n - off);
            if (w < 0)
                ERR("write");
            off += w;
        }
    }
    if (n < 0)
        ERR("read");
}
