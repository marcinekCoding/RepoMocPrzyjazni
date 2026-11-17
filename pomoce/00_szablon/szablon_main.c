/* SZABLON NOWEGO ZADANIA — skopiuj caly plik jako main.c i dopisuj.
 * Komplet naglowkow (nie trzeba myslec, ktore potrzebne) + ERR + usage + main.
 * Kompilacja: gcc -Wall -Wextra -o prog main.c   (dodaj -lm, jesli uzywasz math.h)
 */
#define _XOPEN_SOURCE 700 /* MUSI byc przed includami: strdup, strtok_r, nftw, getline, realpath */

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <ftw.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* Wypisuje jak uzywac programu i konczy. Wolaj, gdy argumenty sa zle. */
void usage(const char *pname)
{
    fprintf(stderr, "USAGE: %s <argumenty>\n", pname);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    /* ZAWSZE najpierw sprawdz argc, zanim dotkniesz argv[1], argv[2]... */
    if (argc < 2)
        usage(argv[0]);

    /* ... tu Twoj kod ... */

    return EXIT_SUCCESS;
}
