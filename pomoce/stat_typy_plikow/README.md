# stat_typy_plikow — stat/lstat, typ pliku, rozmiar, uprawnienia

| plik | kiedy |
|---|---|
| `typy_plikow.c` | `istnieje`, `to_katalog`, `to_zwykly_plik`, `to_link`, `to_fifo`, rozmiar, uprawnienia, wykonywalny |
| `sprawdz_stat.c` | wybor `stat` (za linkiem) / `lstat` (sam link) jednym argumentem |
| `porownaj_rozmiar.c` | czy plik (za linkiem) ma oczekiwany rozmiar — brak / zgodny / inny |

`stat` idzie za linkiem, `lstat` nie. `fstat(fd, &st)` gdy masz juz otwarty fd (np. sprawdzenie `S_ISFIFO`).
`access(path, F_OK) == 0` -> istnieje (ale idzie za linkiem; zepsuty link = "nie istnieje").
