# poll_fifo — poll, FIFO, nieblokujace czytanie, dzielenie strumienia na wiadomosci

| plik | kiedy |
|---|---|
| `otworz_fifo.c` | otwarcie FIFO: `O_RDWR` (nie blokuje, brak EOF) / czytelnik `O_RDONLY` / pisarz z ponawianiem (ENXIO) |
| `ustaw_nonblock.c` | wlacz `O_NONBLOCK` na juz otwartym fd (np. stdin) |
| `dziel_wiadomosci.c` | **strumien -> linie `\n`**, osobny bufor na fd; EAGAIN/EOF obsluzone |
| `petla_poll.c` | **szablon calej petli poll** po kilku FIFO z argv (timeout, zamykanie, `fd = -1`) |
| `multiplex_read_lines.c` | awaryjnie: cala petla poll+bufor+split w jednym wywolaniu z callbackiem |
| `rekordy_binarne_z_fifo.c` | przez FIFO leca binarne rekordy (np. 2x uint32), a nie tekst |
| `poll_stdin_timeout.c` | czekaj na wpisanie z klawiatury max N ms |
| `timerfd_linux.c` | zegar jako fd w poll (tylko Linux) |

Wazne: `poll` zwraca `0` = timeout, `-1` + `EINTR` = powtorz. Zamkniety fd -> `fds[i].fd = -1`.
Testowanie z terminala: `mkfifo f1; ./prog plik f1 &` i w drugim oknie `echo "07:ok" > f1`.
