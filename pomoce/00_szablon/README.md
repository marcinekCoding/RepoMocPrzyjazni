# 00_szablon

- `szablon_main.c` — pusty program: wszystkie naglowki, `ERR`, `usage`, sprawdzenie `argc`. Start kazdego zadania.
- `sciagawka_pelna.md` — Twoja pelna sciagawka SOP1 (kopia z `lab2_punktowane/sop1_sciagawka.md`), z checklista bledow.

**Checklist przed oddaniem** (z ktorej najczesciej sie wykladales):
- bufor null-terminowany przed `strlen()` / `%s`?
- po `close(fds[i].fd)` ustawiasz `fds[i].fd = -1`?
- sprawdzasz wynik `read`/`write`/`open`/`mmap`?
- `munmap` + `close` na koncu?
- `argc` sprawdzone na poczatku `main`?
