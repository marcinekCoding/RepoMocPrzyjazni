#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* (sop-ratking) mmap pliku jako TABLICA STRUKTUR — zamiast liczyc offsety recznie.
 * Plik 128 B = 16 komor x (2 x uint32_t).
 *
 * Gdy w strukturze mieszasz typy (char + uint32), dodaj __attribute__((packed)),
 * a rozmiar sprawdz _Static_assert — kompilator sam powie, ze cos nie pasuje.
 */
struct komora
{
    uint32_t odwiedzono;
    uint32_t glod;
};
_Static_assert(sizeof(struct komora) == 8, "zly rozmiar struktury - sprawdz padding");

#define ILE_KOMOR 16

void przyklad_mmap_struktura(int fd) /* fd z otworz_lub_utworz(path, 128, 0) */
{
    size_t rozmiar = ILE_KOMOR * sizeof(struct komora);
    struct komora *mapa = mmap(NULL, rozmiar, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapa == MAP_FAILED)
        ERR("mmap");

    mapa[3].glod += 10; /* zwykly dostep do pola */
    mapa[3].odwiedzono++;

    for (int i = 0; i < ILE_KOMOR; i++)
        printf("Komora %02d: glod=%u\n", i, mapa[i].glod);

    if (msync(mapa, rozmiar, MS_SYNC)) /* wymus zapis na dysk (przed munmap) */
        ERR("msync");
    if (munmap(mapa, rozmiar))
        ERR("munmap");
}
