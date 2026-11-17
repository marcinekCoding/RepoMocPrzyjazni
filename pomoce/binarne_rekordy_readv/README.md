# binarne_rekordy_readv — pliki binarne z rekordami stalej dlugosci

| plik | kiedy |
|---|---|
| `rekordy_stalej_dlugosci.c` | baza `uint32 + 64 B tytulu`: czytaj rekord i, wczytaj wszystkie, zmien pole, dopisz rekord (`readv`/`writev`/`pread`/`pwrite`) |
| `czytaj_rekord_readv.c` | czytanie rekordu 68 B po kolei `readv` z obsluga czesciowych odczytow |

Pokrewne: `mmap/mmap_struktura.c` (to samo przez mmap), `pliki_read_write/otworz_lub_utworz_o_rozmiarze.c` (`% REKORD`),
`poll_fifo/rekordy_binarne_z_fifo.c` (rekordy binarne przychodzace przez FIFO).
Liczby w pliku binarnym zapisuj jako `uint32_t` (`write(fd, &x, 4)`), NIE jako tekst ASCII.
