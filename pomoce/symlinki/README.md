# symlinki

| plik | kiedy |
|---|---|
| `symlink_i_readlink.c` | utworz link (z nadpisaniem), odczytaj cel (`readlink` + `'\0'`), link z celem bezwzglednym (`realpath`) |
| `wzgledna_sciezka.c` | sciezka wzgledna z katalogu A do pliku B (`../../library/x`) |
| `symlink_wzgledny.c` | link z celem wzglednym liczonym poprawnie od katalogu linku (wymaga `wzgledna_sciezka.c`) |

Sprawdzanie, czy cos jest linkiem: `lstat` (nie `stat`!) + `S_ISLNK` — patrz `stat_typy_plikow/`.
Rozmiar pliku, na ktory wskazuje link: `stat` (idzie za linkiem).
