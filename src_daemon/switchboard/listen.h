#ifndef LISTEN_INCLUDED
#define LISTEN_INCLUDED

extern int Listener_inet_start  (void);
extern int Listener_unix_start  (char *path);
extern int Listener_stop        (int fd);

#endif

