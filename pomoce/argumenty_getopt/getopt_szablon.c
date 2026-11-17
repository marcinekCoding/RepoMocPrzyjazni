#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* SZABLON getopt — skopiuj srodek do main.
 * "p:e:d:o" -> -p, -e, -d wymagaja argumentu (dwukropek), -o to sama flaga.
 * Opcje moga sie powtarzac (np. kilka -p) — zbieraj je do tablicy.
 * Po petli argv[optind] to pierwszy argument pozycyjny PO opcjach.
 *
 *   ./prog -p dir1 -p dir2 -e txt -d 3 -o
 */
void przyklad_getopt(int argc, char **argv)
{
    char *katalogi[argc]; /* -p moze wystapic wiele razy */
    int ile_katalogow = 0;
    char *rozszerzenie = NULL;
    int glebokosc = 1;
    int flaga_o = 0;

    int c;
    while ((c = getopt(argc, argv, "p:e:d:o")) != -1)
    {
        switch (c)
        {
        case 'p':
            katalogi[ile_katalogow++] = optarg;
            break;
        case 'e':
            rozszerzenie = optarg;
            break;
        case 'd':
            glebokosc = atoi(optarg); /* bezpieczniej: liczba_z_argumentu.c */
            break;
        case 'o':
            flaga_o = 1;
            break;
        case '?': /* nieznana opcja albo brak argumentu */
        default:
            fprintf(stderr, "USAGE: %s -p DIR [-p DIR ...] [-e EXT] [-d N] [-o]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    if (ile_katalogow == 0) /* wymagana opcja nie podana */
    {
        fprintf(stderr, "brak -p\n");
        exit(EXIT_FAILURE);
    }
    /* argumenty pozycyjne po opcjach: */
    for (int i = optind; i < argc; i++)
        printf("pozycyjny: %s\n", argv[i]);

    (void)rozszerzenie; (void)glebokosc; (void)flaga_o;
}
