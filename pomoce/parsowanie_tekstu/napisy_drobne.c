#include <ctype.h>
#include <stdbool.h>
#include <string.h>

/* Drobiazgi do tekstu. Wszystkie dzialaja W MIEJSCU albo zwracaja wskaznik do srodka
 * napisu (nic nie alokuja, nic nie trzeba zwalniac). */

/* Usuwa '\n' i '\r' z konca — ZAWSZE po getline()/fgets(). */
void usun_nowa_linie(char *s)
{
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r'))
        s[--n] = '\0';
}

/* Obcina biale znaki z obu stron. Zwraca wskaznik na pierwszy niebialy znak. */
char *przytnij(char *s)
{
    while (isspace((unsigned char)*s))
        s++;
    size_t n = strlen(s);
    while (n > 0 && isspace((unsigned char)s[n - 1]))
        s[--n] = '\0';
    return s;
}

/* true, gdy s zaczyna sie od prefiksu. */
bool zaczyna_sie_od(const char *s, const char *prefiks)
{
    return strncmp(s, prefiks, strlen(prefiks)) == 0;
}

/* true, gdy s konczy sie sufiksem (np. ".txt"). */
bool konczy_sie_na(const char *s, const char *sufiks)
{
    size_t ls = strlen(s), lx = strlen(sufiks);
    return ls >= lx && strcmp(s + ls - lx, sufiks) == 0;
}

/* Ile razy znak c wystepuje w s. */
int policz_znak(const char *s, char c)
{
    int n = 0;
    for (; *s; s++)
        if (*s == c)
            n++;
    return n;
}

/* Dlugosc tekstu w buforze stalej szerokosci, bez spacji/'\0' na koncu
 * (linie 64-bajtowe dopelnione spacjami, tytul 64B dopelniony zerami).
 *   write(STDOUT_FILENO, linia, dlugosc_bez_dopelnienia(linia, 63));   */
size_t dlugosc_bez_dopelnienia(const char *buf, size_t szerokosc)
{
    while (szerokosc > 0 && (buf[szerokosc - 1] == ' ' || buf[szerokosc - 1] == '\0'))
        szerokosc--;
    return szerokosc;
}
