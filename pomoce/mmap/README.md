# mmap — plik jako pamiec

| plik | kiedy |
|---|---|
| `mmap_otworz_plik.c` | otworz/utworz + sprawdz rozmiar + zmapuj RW jednym wywolaniem; zmapuj caly plik tylko do odczytu |
| `mmap_struktura.c` | plik jako tablica `struct` (`mapa[i].pole`), `_Static_assert`, `msync`, `munmap` |
| `dump_hex.c` | podglad surowych bajtow bufora/mapy przy debugowaniu |

Pokrewne: `parsowanie_tekstu/zamien_wystapienia.c` (zamiana tekstu w zmapowanym pliku),
`napisy_drobne.c -> dlugosc_bez_dopelnienia` (wypisanie linii 64 B bez spacji).
Pamietaj: `mmap` na pliku o rozmiarze 0 nie dziala; po `mmap` mozna `close(fd)`.
