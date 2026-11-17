#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* Otwiera (tworzy, jesli nie ma — wypelnia zerami), sprawdza rozmiar i mapuje do pamieci
 * w JEDNYM wywolaniu. fd zamykany od razu — mapowanie dziala dalej.
 * Zapisy do mapy trafiaja do pliku (MAP_SHARED).
 *
 *   struct komora *mapa = mmap_otworz_plik(argv[1], 16 * sizeof(struct komora));
 *   mapa[3].glod += 5;
 *   ... na koncu:  msync(mapa, rozmiar, MS_SYNC); munmap(mapa, rozmiar);
 *
 * mmap rob ZARAZ po otwarciu, jesli piszesz do mapy w trakcie petli poll.
 */
void *mmap_otworz_plik(const char *sciezka, size_t rozmiar)
{
    int fd = open(sciezka, O_RDWR);
    if (fd == -1)
    {
        fd = open(sciezka, O_RDWR | O_CREAT, 0644);
        if (fd == -1)
            ERR("open");
        if (ftruncate(fd, (off_t)rozmiar)) /* nowy plik = same zera */
            ERR("ftruncate");
    }
    struct stat st;
    if (fstat(fd, &st) == -1)
        ERR("fstat");
    if ((size_t)st.st_size != rozmiar)
    {
        fprintf(stderr, "%s: zly rozmiar pliku\n", sciezka);
        exit(EXIT_FAILURE);
    }
    void *p = mmap(NULL, rozmiar, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED)
        ERR("mmap");
    close(fd);
    return p;
}

/* Mapuje istniejacy plik w CALOSCI (dowolny rozmiar), tylko do odczytu.
 * *rozmiar = dlugosc. Pusty plik -> NULL (mmap nie umie dlugosci 0).
 *   size_t n; char *t = mmap_tylko_odczyt("test.txt", &n);
 *   for (size_t i = 0; i < n; i++) if (t[i] != ' ') putchar(t[i]);
 *   munmap(t, n); */
char *mmap_tylko_odczyt(const char *sciezka, size_t *rozmiar)
{
    int fd = open(sciezka, O_RDONLY);
    if (fd == -1)
        ERR("open");
    struct stat st;
    if (fstat(fd, &st) == -1)
        ERR("fstat");
    *rozmiar = (size_t)st.st_size;
    if (*rozmiar == 0)
    {
        close(fd);
        return NULL;
    }
    char *p = mmap(NULL, *rozmiar, PROT_READ, MAP_PRIVATE, fd, 0);
    if (p == MAP_FAILED)
        ERR("mmap");
    close(fd);
    return p;
}
