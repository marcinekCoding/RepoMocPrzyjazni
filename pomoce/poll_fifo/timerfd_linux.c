/* timerfd — zegar jako zwykly fd, ktory wrzucasz do tablicy poll obok FIFO.
 * TYLKO LINUX (na macOS nie ma sys/timerfd.h — tam uzyj timeoutu w poll).
 *
 *   int tfd = utworz_timer(2000, 1000);   // pierwszy za 2s, potem co 1s
 *   fds[ile].fd = tfd; fds[ile].events = POLLIN;
 *   ... gdy fds[ile].revents & POLLIN:
 *   uint64_t ile_razy = odbierz_timer(tfd);   // KONIECZNIE przeczytaj, inaczej poll wraca w kolko
 */
#ifdef __linux__
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timerfd.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* co_ms = 0 -> timer jednorazowy */
int utworz_timer(int pierwszy_ms, int co_ms)
{
    int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (tfd == -1)
        ERR("timerfd_create");
    struct itimerspec t = {
        .it_value = {.tv_sec = pierwszy_ms / 1000, .tv_nsec = (pierwszy_ms % 1000) * 1000000L},
        .it_interval = {.tv_sec = co_ms / 1000, .tv_nsec = (co_ms % 1000) * 1000000L},
    };
    if (timerfd_settime(tfd, 0, &t, NULL) == -1)
        ERR("timerfd_settime");
    return tfd;
}

/* zwraca, ile razy timer tyknal od ostatniego odczytu */
uint64_t odbierz_timer(int tfd)
{
    uint64_t ile = 0;
    if (read(tfd, &ile, sizeof(ile)) != sizeof(ile))
        return 0;
    return ile;
}
#endif
