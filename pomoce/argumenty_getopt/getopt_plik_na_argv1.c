#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Trik z sop-library: plik MUSI byc w argv[1], a opcje sa dopiero za nim:
 *   ./prog baza.bin -s Hist -a "Tytul" 120 -p 10
 * getopt na macOS/BSD nie przeskakuje argumentow pozycyjnych, wiec parsujemy
 * od argv+1. optind liczy sie wtedy wzgledem (argv+1), czyli prawdziwy
 * indeks w argv to optind + 1.
 *
 * Opcja z DWOMA wartosciami (-a tytul rozmiar): pierwszy daje getopt (optarg),
 * drugi bierzesz recznie z argv i sam przesuwasz optind.
 */
void przyklad_plik_na_argv1(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "USAGE: %s plik [-s prefiks] [-a tytul rozmiar] [-p procent]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *plik = argv[1];
    char *prefiks = NULL;
    char *a_tytul = NULL;
    int a_rozmiar = 0;
    int procent = 0;

    int c;
    while ((c = getopt(argc - 1, argv + 1, "s:a:p:")) != -1)
    {
        switch (c)
        {
        case 's':
            prefiks = optarg;
            break;
        case 'a':
            a_tytul = optarg;
            if (optind + 1 >= argc) /* brak drugiej wartosci */
            {
                fprintf(stderr, "-a wymaga: tytul rozmiar\n");
                exit(EXIT_FAILURE);
            }
            a_rozmiar = atoi(argv[optind + 1]); /* (argv+1)[optind] == argv[optind+1] */
            optind++;                           /* przeskocz zabrana wartosc */
            break;
        case 'p':
            procent = atoi(optarg);
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }
    printf("plik=%s prefiks=%s tytul=%s rozmiar=%d procent=%d\n", plik, prefiks ? prefiks : "-",
           a_tytul ? a_tytul : "-", a_rozmiar, procent);
}
