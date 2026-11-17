#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* Zamiana CALEGO napisu na liczbe; konczy program, gdy to nie liczba ("12a", "").
 * baza 10 -> rozmiary, ilosci;  baza 8 -> uprawnienia "0644".
 *
 *   int n = (int)liczba_z_argumentu(optarg, 10);
 *   mode_t perm = (mode_t)liczba_z_argumentu(optarg, 8);
 */
long liczba_z_argumentu(const char *str, int baza)
{
    char *koniec;
    errno = 0;
    long wartosc = strtol(str, &koniec, baza);
    if (errno)
        ERR("strtol");
    if (koniec == str || *koniec != '\0')
    {
        fprintf(stderr, "Niepoprawna liczba: %s\n", str);
        exit(EXIT_FAILURE);
    }
    return wartosc;
}

/* Jak wyzej, ale dodatkowo sprawdza zakres [min, max] (np. n > 0). */
long liczba_w_zakresie(const char *str, long min, long max)
{
    long v = liczba_z_argumentu(str, 10);
    if (v < min || v > max)
    {
        fprintf(stderr, "Liczba %ld poza zakresem [%ld, %ld]\n", v, min, max);
        exit(EXIT_FAILURE);
    }
    return v;
}
